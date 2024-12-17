#pragma once

#include <string>
#include <string.h>

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
    bool m_runnig;

    int poll( short events, int msecs_timeout );
};


class SessionException : public std::exception
{
    private:
      std::string m_msg;
      int  m_errno;

    public:
        SessionException(std::string msg, int _errno)
        : m_errno(_errno)
        {
          m_msg = msg;
          m_msg.append(" ");
          m_msg.append(strerror(_errno));
        }

        const char* what() const throw() { return m_msg.c_str(); }    
};
