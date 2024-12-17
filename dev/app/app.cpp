#include <iostream>
#include <csignal> 
#include <thread> 
#include <memory>
#include <mutex>
#include "server.h"
#include "session.h"

std::mutex gbl_mutex;

void signal_handler(int signal_num) 
{ 
     gbl_mutex.unlock();
    std::cout << "Signal " << signal_num << " recived\n";
} 

void run_server( std::shared_ptr<Server> serv )
{
    std::cout << "Running server\n";
    serv->run();
    std::cout << "Stopping server\n";    
}

int main()
{
    signal(SIGINT, signal_handler); 

    int port = 8080;
    auto serv_ptr = std::make_shared<Server>(port);

    try
    {
        std::jthread worker( run_server, serv_ptr );

        // take the mutex twice to wait until a signal is received.
        // In this way, we dont need a loop and do not consume 
        gbl_mutex.lock();
        gbl_mutex.lock();
        std::cout << "Stop server\n";
        serv_ptr->stop();
    }
    catch(const ServerExeption& e)
    {
        std::cerr << e.what() << '\n';
    }
    catch(const SessionException& e2)
    {
        std::cerr << e2.what() << '\n';
    }

    return 0;
}