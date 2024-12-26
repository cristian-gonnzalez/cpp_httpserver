#pragma once

#include <string>
#include <string.h>
#include "exception.h"

class Session
{
  public:
     Session(int socket, std::string host);
     ~Session();
     std::string read(int timeout/*=0*/);
     int write(std::string msg);

  private:
    int m_socket;
    std::string m_host;

    int poll( short events, int msecs_timeout );
};


class SessionException : public server::Exception
{
    public:
        SessionException(std::string msg)
        : server::Exception(msg)
        {
        }
};


