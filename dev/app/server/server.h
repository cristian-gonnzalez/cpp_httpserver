#pragma once

#include <string>
#include <memory>
#include "session.h"
#include "protocol_handler.h"
#include "exception.h"


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


class ServerExeption : public server::Exception
{
    public:
        ServerExeption(std::string msg)
        : server::Exception(msg)
        {
        }
};