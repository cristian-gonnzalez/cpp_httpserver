#include "server.h"
#include <iostream>
#include <chrono>
#include <string.h>
#include <vector>
#include <errno.h>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <poll.h>

#include <syncstream>
#include "log_director.h"


Server::Server(int port)
: m_socket{-1}, m_port{port}, m_running{false}
{  
    INFO("Calling constructor Server( args )");
    if( (m_socket = socket(AF_INET, SOCK_STREAM, 0) ) < 0)
    {
        throw ServerExeption( "Error in socket");
    }
 
    int on{ 1 };   // no funciona
  	if (setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
    {
        throw ServerExeption( "Error in setsockopt");
    }

    if (pipe(m_awakening_pipe) < 0) 
    { 
        throw ServerExeption( "Error in pipe");
    } 	
}

Server::~Server()
{
  INFO("Calling ~Server");
  // RAII: we must close all resources adquired in the constructor
  stop();
  close(m_socket);
  close(m_awakening_pipe[0]);
  close(m_awakening_pipe[1]);
}


void handle_session( std::unique_ptr<Session> s )
{  
  std::stringstream ss;
  ss << std::this_thread::get_id();
  uint64_t id = std::stoull(ss.str());

  LOG( "Session " + std::to_string( id ) );

  std::string str_in{ s->read(0) };
  DEBUG("mesasge in: \n\n" + str_in + "\n" );
 
  try
  {
      ProtocolHandler prot_handler{};

      http::Response resp = prot_handler.handle( str_in );
      std::string str_out = resp.to_str();
      DEBUG("mesasge out: \n\n" + str_out + "\n" );
      s->write( str_out );
  }
  catch(const std::exception& e)
  {
    ERROR( e.what() );
  }
}


void Server::run()
{
  bind();
  listen(m_socket, 10);

  std::vector<std::jthread> workers;

  m_running = true;
  while(m_running) 
  {
    int r{ poll(0) }; 
    if( r < 0)
    {
      throw ServerExeption( "Error in poll");
    }
    // 1 means a new connection was made. other value could an awakeking event or timeout
    if( r == 1 ) 
    {
      workers.push_back(  std::jthread( handle_session, accept(0) ));
    }
    // awake event
    if( r == 2 ) 
    {
      //TOD: do sth here to close session
    }
  }
}


void Server::stop()
{
  // writes an awakeing event
  write(m_awakening_pipe[1], "1", 1); 
  m_running = false;
}

struct sockaddr_in Server::build_addr_in()
{  
  struct sockaddr_in addr_in = {
    .sin_family = AF_INET,
    .sin_port = htons( m_port ),
    .sin_addr = { .s_addr = INADDR_ANY },
  };

  memset(&(addr_in.sin_zero), 0, 8);
  
  return addr_in;
}

void Server::bind()
{  
  struct sockaddr_in addr_in{ build_addr_in() };

  if( (::bind(m_socket, (struct sockaddr *)&addr_in, sizeof(addr_in))) < 0)
  { 
    throw new ServerExeption( "Error in bind");
  }
}


int Server::poll( int msecs_timeout )
{
  struct pollfd pfds[2] = {
    { .fd = m_socket, .events = POLLIN, .revents = 0},
    { .fd = m_awakening_pipe[0], .events = POLLIN, .revents = 0}
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
      ssize_t bytes_count{ ::read( m_awakening_pipe[0], buf, sizeof( buf ) ) };
      if( bytes_count )
      {
        INFO("Awakening event received");
      }
      return 2;
    }
  }
  /* there were no events nor any error, so it must have timed out. */
  else if( ret == 0 )
  {
    return 0;
  }

  return -1;
}


std::unique_ptr<Session> Server::accept(int timeout /*= 0*/)
{
  struct sockaddr_in addr_in;
	socklen_t addr_len { sizeof(addr_in) };

	int session_id;
	if( (session_id = ::accept(m_socket, (struct sockaddr *)&addr_in, &addr_len)) == -1)
	{
	  throw new ServerExeption( "Error in accept");
	}
	
	std::string host{ inet_ntoa(addr_in.sin_addr) };
	return std::make_unique<Session>(session_id, host);
}

