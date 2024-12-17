#pragma once

#include <string>
#include <memory>
#include "session.h"


class Server
{       
    private:
        int m_socket;
        int m_port;
        bool m_running;
        int m_awakening_pipe[2];

        struct sockaddr_in build_addr_in();
        int poll( int msecs_timeout );
        void bind();
        std::unique_ptr<Session> accept(int timeout);

    public:
        Server(int port);
        ~Server();

        void run();
        void stop();
};


class ServerExeption : public std::exception
{
    private:
        std::string m_msg;
        int  m_errno;

    public:
        ServerExeption(std::string msg, int _errno)
        : m_errno(_errno)
        {
          m_msg = msg;
          m_msg.append(" ");
          m_msg.append(strerror(_errno));
        }

        const char* what() const throw() { return m_msg.c_str(); }    
};
