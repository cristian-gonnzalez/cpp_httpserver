#pragma once

#include <memory>
#include <vector>
#include <deque>
#include <optional>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>

#include "exception.h"
#include "connection.h"

class ServerExeption;

class Server
{
    public:
        explicit Server(int port, size_t worker_num = std::thread::hardware_concurrency());
        virtual ~Server();

        Server(const Server& other) = delete;
        Server& operator=(const Server& other) = delete;

        void run();
        void stop();

    private:
        int                 _socket{-1};
        int                 _port{0};
        std::atomic<bool>   _running{false};
        int                 _awakening_pipe[2]{-1,-1};

        int                 poll(int msecs_timeout);
        void                bind();
        std::shared_ptr<Connection> accept();
        std::shared_ptr<Connection> wait_connection(int timeout_ms = 0);

        // task queue + thread pool
        std::deque<std::shared_ptr<Connection>>   _queue{};
        
        size_t                                    _worker_num{0};
        std::vector<std::thread>                  _workers{};
        std::mutex                                _queue_mtx{};
        std::condition_variable                   _queue_cv{};

        void start_tp( size_t worker_num );
};

class ServerExeption : public server::Exception
{
    public:
        explicit ServerExeption(const std::string& msg)
        : server::Exception(msg)
        {
        }
};

class TimeoutError : public server::Exception
{
    public:
        explicit TimeoutError(const std::string& msg)
        : server::Exception(msg)
        {
        }
};

