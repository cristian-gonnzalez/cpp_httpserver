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


// global mutex to wait meanwhile the app is running
std::mutex gbl_mutex;

void signal_handler(int signal_num) 
{ 
    // release the global mutex to finalize the app
    gbl_mutex.unlock();
    app_debug << "Signal " << signal_num << " received\n";
} 


void log_init()
{
    std::shared_ptr<app::log::Logger> console_logger{ std::make_shared<app::log::CLogger>() };
    app::log::LogDirector* log_director = app::log::LogDirector::get();
    log_director->add( console_logger );
    log_director->set_level(app::log::LogLevel::fatal);
}

int main()
{
    signal(SIGINT, signal_handler);

    log_init();

    int port{ 8080 };
    auto serv_ptr{ std::make_shared<Server>(port) };

    try
    {
        std::jthread worker( [](std::shared_ptr<Server> serv) 
                             {                               
                                app_log << "Running server\n";
                                serv->run();
                                app_log << "Stoping server\n";
                             }, 
                             serv_ptr );

        // takes the mutex twice to wait until a signal is received.
        // In this way, we dont need a loop and do not consume 
        gbl_mutex.lock();
        gbl_mutex.lock();
        app_debug << "Stop server\n";
        serv_ptr->stop();
    }
    catch(const ServerExeption& e)
    {
        app_error << e.what() << std::endl;
    }
    catch(const SessionException& e2)
    {
        app_error << e2.what() << std::endl;
    }
    catch(const std::exception& e3)
    {
        app_error << e3.what() << std::endl;
    }

    return 0;
}
