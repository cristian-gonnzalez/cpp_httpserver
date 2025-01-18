#include "session.h"
#include "log_director.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <poll.h>

Session::Session(int socket, std::string host)
: m_socket{socket}, m_host{host}
{
  app_info << "Calling Session\n";
}

Session::~Session()
{
  app_info << "Calling ~Session\n";
  close(m_socket);
}


int Session::poll( short events, int msecs_timeout )
{
  struct pollfd pfd = { .fd = m_socket, .events = events, .revents = 0};
  
  int ret{ ::poll( &pfd, 1, msecs_timeout ) };
  if( ret > 0 )
  {
    if( ( pfd.revents & events ) != 0 )
        return 1;
  }
  /* there were no events nor any error, so it must have timed out. */
  else if( ret == 0 )
  {
    return 0;
  }

  return -1;
}

std::string Session::read(int timeout/*=0*/)
{
  std::string resp;
  int size{1024};
  char temp[size];

  int r{ poll(POLLIN, timeout) };
  if( r > 0)
  {
    memset(temp, 0, size);
    ssize_t nbytes{ ::read(m_socket, temp, size) };
    if (nbytes <= 0 )
    {
      throw new SessionException("error in read");
    }
    resp.append( temp );
  }
  
  return resp;
}

int Session::write(std::string msg)
{
  int r{ poll(POLLOUT, 0) };
  if( r > 0)
  {
    r = ::send(m_socket, msg.c_str(), msg.size(), 0);
    if (r < 0)
    {
      throw new SessionException("error in send");
    }
  }

  return r;
}

