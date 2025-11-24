#include "connection.h"

#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>

Connection::Connection(int socket, std::string host)
: _socket{socket}, _host{host}
{
  std::cout << "Calling Connection\n";

  if (pipe(_awakening_pipe) < 0)
      throw ConnectionException( "Error creating pipe");
}

Connection::~Connection()
{
  std::cout << "Calling ~Connection\n";

  interrup();

  if( _socket >= 0)
    close(_socket);
  if (_awakening_pipe[0] >= 0) 
    close(_awakening_pipe[0]);
  if (_awakening_pipe[1] >= 0) 
    close(_awakening_pipe[1]);
}

Connection::Connection( Connection&& rvalue ) noexcept
: _socket{ rvalue._socket }, _host{ std::move(rvalue._host) }
{
  _awakening_pipe[0] = rvalue._awakening_pipe[0];
  _awakening_pipe[1] = rvalue._awakening_pipe[1];

  rvalue._socket = -1;
  rvalue._awakening_pipe[0] = -1;
  rvalue._awakening_pipe[1] = -1;
}

Connection& Connection::operator=( Connection&& rvalue ) noexcept
{
    if(this == &rvalue) 
      return *this;

    _socket = rvalue._socket;
    _host = std::move(rvalue._host);
    _awakening_pipe[0] = rvalue._awakening_pipe[0];
    _awakening_pipe[1] = rvalue._awakening_pipe[1];

    rvalue._socket = -1;
    rvalue._awakening_pipe[0] = -1;
    rvalue._awakening_pipe[1] = -1;

    return *this;
}

void Connection::interrup()
{
  const char signal = 1;
  if (_awakening_pipe[1] >= 0)
    ::write(_awakening_pipe[1], &signal, 1);
}

int Connection::poll( short events, int msecs_timeout )
{
  struct pollfd pfds[2] = {
    { .fd = _socket, .events = events, .revents = 0},
    { .fd = _awakening_pipe[0], .events = POLLIN, .revents = 0}
  };

  size_t n{ ( sizeof( pfds ) / sizeof( ( pfds )[0] ) ) };

  int ret{ ::poll( pfds, n, msecs_timeout ) };
  if( ret > 0 )
  {
    if ( (pfds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) != 0 )
        return -1;

    if( ( pfds[0].revents & events ) != 0 )
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
      throw ConnectionException("Awakening event received");
    }
  }

  return ret; // 0 timeout, -1 error
}

std::string Connection::read(int timeout/*=0*/)
{
  std::string resp;

  int r{ poll(POLLIN, timeout) };
  if( r <= 0)
      return {};

  constexpr int data_len{1024};
  std::string accumulator;
  while (true)
  {
      char data[data_len];
      ssize_t n = ::read(_socket, data, data_len);
      if (n < 0)
      {
          if (errno == EINTR) continue;
          throw ConnectionException("Error in read");
      }
      if (n == 0) 
        break; // EOF / socket closed by peer
      
      accumulator.append(data, static_cast<size_t>(n));
      if (static_cast<size_t>(n) < data_len) 
          break; // likely no more data immediately available
      // otherwise loop to read more
  }

  return accumulator;
}

int Connection::write(const std::string& msg)
{
  int r{ poll(POLLOUT, 0) };
  if( r <= 0)
    return 0;

  ssize_t bytes_send = ::send(_socket, msg.c_str(), msg.size(), 0);
  if (bytes_send < 0)
  {
    throw ConnectionException("Error in send");
  }

  return static_cast<int>(bytes_send);
}
