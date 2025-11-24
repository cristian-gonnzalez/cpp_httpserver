#include "server.h"
#include <iostream>
#include <cstring>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include <syncstream>
#include "log_director.h"
#include "protocol_handler.h"


static void handle_session(std::shared_ptr<Connection> conn)
{
  app_log << "Connection " << std::this_thread::get_id() << " : " << *conn << std::endl;

  std::string str_in{ conn->read(0) };
  app_debug << "message in: \n\n" << str_in << std::endl;

  try
  {
      ProtocolHandler prot_handler{};
      http::Response resp = prot_handler.handle( str_in );
      std::string str_out = resp.to_str();

      app_debug << "message out: \n\n" << str_out << std::endl;
      conn->write( str_out );
  }
  catch(const std::exception& e)
  {
    app_error << e.what() << std::endl;
  }
}

Server::Server(int port, size_t worker_num )
: _port{port}
{  
    if( _socket = socket(AF_INET, SOCK_STREAM, 0); 
        _socket < 0)
        throw ServerExeption( "Error creating socket");
    
    int on{ 1 };   // no funciona
  	if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
        throw ServerExeption( "Error in setsockopt(SO_REUSEADDR)");
    
    if (pipe(_awakening_pipe) < 0) 
        throw ServerExeption( "Error creating pipe");
    
    bind();
    if (listen(_socket, 10) < 0)
        throw ServerExeption("Error in listen()");

    start_thread_pool( worker_num );
}


Server::~Server()
{
  app_log << "Calling ~Server\n";

  // RAII: we must close all resources adquired in the constructor
  stop();
  if( _socket >= 0)
    close(_socket);
  if (_awakening_pipe[0] >= 0) 
    close(_awakening_pipe[0]);
  if (_awakening_pipe[1] >= 0) 
    close(_awakening_pipe[1]);
}


void Server::start_thread_pool( size_t worker_num )
{
    // start worker threads (thread pool)
    size_t n = (worker_num == 0 ? 1 : worker_num);
    for(size_t i=0; i<n ; ++i)
    {
        _workers.emplace_back( [this]()
                                {
                                    while(true)
                                    {
                                        std::shared_ptr<Connection> task;
                                        {
                                            std::unique_lock lock( _queue_mtx );
                                            _queue_cv.wait( lock, 
                                                            [this]()
                                                            { 
                                                              return !_queue.empty() || !_running.load(); 
                                                            } );
  
                                            if ( !_running.load() && _queue.empty() )
                                                return;
  
                                            task = std::move(_queue.front());
                                            _queue.pop_front();
                                        }

                                        try 
                                        { 
                                          handle_session(task); 
                                        }
                                        catch(const std::exception& e) 
                                        { 
                                          app_error << "Thread-pool exception: " << e.what() << std::endl; 
                                        }
                                    }
                                });
    }
}

void Server::stop()
{
  _running = false;

  // wake accept/poll loop
  const char signal = 1;
  if (_awakening_pipe[1] >= 0)
    ::write(_awakening_pipe[1], &signal, 1);

  // notify worker threads (in case they are waiting on empty queue)
  _queue_cv.notify_all();


  for(auto& w: _workers)
  {
      if (w.joinable())
          w.join();
  }

  _workers.clear();
}


void Server::bind()
{  
  struct sockaddr_in addr_in = {
    .sin_family = AF_INET,
    .sin_port = htons( _port ),
    .sin_addr = { .s_addr = INADDR_ANY },
  };

  memset(&(addr_in.sin_zero), 0, 8);
  
  if( (::bind(_socket, reinterpret_cast<sockaddr*>(&addr_in), sizeof(addr_in))) < 0)
    throw ServerExeption( "Error in bind");
}



int Server::poll( int msecs_timeout )
{
  struct pollfd pfds[2] = {
    { .fd = _socket, .events = POLLIN, .revents = 0},
    { .fd = _awakening_pipe[0], .events = POLLIN, .revents = 0}
  };

  size_t n{ ( sizeof( pfds ) / sizeof( ( pfds )[0] ) ) };

  int ret{ ::poll( pfds, n, msecs_timeout ) };
  if( ret > 0 )
  {
    if( ( pfds[0].revents & POLLIN ) != 0 )
        return 1;

    if( ( pfds[1].revents & POLLIN ) != 0 )
    {
      /* consume the awakening event and then use the bytes_count varibale. Otherwise the compiler will
       * complain about un-used variable */
      char buf[50];
      ssize_t bytes_count{ ::read( _awakening_pipe[0], buf, sizeof( buf ) ) };
      if( bytes_count )
      {
        std::cout << "Awakening event received\n";
      }
      return 2;
    }
  }
  /* there were no events nor any error, so it must have timed out. */
  return ret; // 0 timeout, -1 error
}


std::shared_ptr<Connection> Server::accept()
{
  struct sockaddr_in addr_in{};
  socklen_t addr_len { sizeof(addr_in) };

  int conn_id;
  if( (conn_id = ::accept(_socket, reinterpret_cast<sockaddr*>(&addr_in), &addr_len)) == -1)
    throw ServerExeption( "Error accepting new connection");

  std::string host{ inet_ntoa(addr_in.sin_addr) };
  return std::make_shared<Connection>(conn_id, host);
}


std::shared_ptr<Connection> Server::wait_connection(int timeout_ms)
{
  int r{ poll(timeout_ms) }; 
  if( r < 0)
    throw ServerExeption( "Error polling");
  
    // 1 means a new connection was made. Other value could an awakeking event or timeout
  if( r == 1 ) 
    return { accept() };
  
  // awake event
  if( r == 2 ) 
      throw ServerExeption( "Awakened event");

  throw TimeoutError( "Time-out");
}


void Server::run()
{
    std::cout << "Running server\n";
    _running = true;

    while( _running )
    {
        try
        {
            auto conn = wait_connection(1000);
  
            // enqueue connection for processing by a worker
            {
                std::lock_guard lock(_queue_mtx);
                _queue.emplace_back(conn);
            }
            _queue_cv.notify_one();
        }
        catch(const TimeoutError& e)
        {
            continue;
        }
        catch(const ServerExeption& e)
        {
            // awakened or other poll error; if _running is false we should exit
            if (!_running) 
                break;
            app_error << "Server::run poll error: " << e.what() << std::endl;
        }
    }

    std::cout << "Server stopped...\n";
  }




