#include <iostream>
#include <csignal> 
#include <thread> 
#include <memory>
#include <mutex>
#include "server.h"
#include "session.h"
#include "logger.h"
#include "console_logger.h"
#include "log_director.h"

std::mutex gbl_mutex;

void signal_handler(int signal_num) 
{ 
    gbl_mutex.unlock();
    DEBUG( "Signal "+ std::to_string(signal_num) + " recived\n" );
} 

void run_server( std::shared_ptr<Server> serv )
{
    LOG( "Running server" );
    serv->run();
    LOG( "Stoping server" );   
}

void log_init()
{
    std::shared_ptr<app::log::Logger> console_logger{ std::make_shared<app::log::CLogger>() };
    console_logger->set_level( app::log::LogLevel::error );
    
    app::log::LogDirector& log_director = app::log::LogDirector::get();
    log_director.add( console_logger );
}
 

int main()
{
    signal(SIGINT, signal_handler);

    log_init();

    int port{ 8080 };
    auto serv_ptr{ std::make_shared<Server>(port) };

    try
    {
        std::jthread worker( run_server, serv_ptr );

        // take the mutex twice to wait until a signal is received.
        // In this way, we dont need a loop and do not consume 
        gbl_mutex.lock();
        gbl_mutex.lock();
        DEBUG( "Stop server" );
        serv_ptr->stop();
    }
    catch(const ServerExeption& e)
    {
        ERROR( e.what() );
    }
    catch(const SessionException& e2)
    {
        ERROR( e2.what() );
    }
    catch(const std::exception& e3)
    {
        ERROR( e3.what() );
    }

    return 0;
}
