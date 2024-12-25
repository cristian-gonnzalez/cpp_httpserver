#include "log_director.h"
#include <thread>
#include <cstdint>
#include <sstream>
#include <iostream>

/** Enables inner logging. */
#ifndef LOG_DIRECTOR_LOG
#  define LOG_DIRECTOR_LOG 0
#endif

using namespace app::log;

std::mutex LogDirector::m_mutex;

LogDirector::LogDirector()
{   
#if LOG_DIRECTOR_LOG
    std::cout << "calling LogDirector\n";
#endif
}

LogDirector::~LogDirector()
{
#if LOG_DIRECTOR_LOG
    std::cout << "calling ~LogDirector\n";
#endif
}

LogDirector& LogDirector::get()
{
    static LogDirector instance;
	return instance;
}

void LogDirector::add( std::shared_ptr<app::log::Logger> logger )
{
    m_loggers.push_back( logger );
}

void LogDirector::write(app::log::LogLevel level, const std::string& file, const int line, const std::string& message)
{   
   std::stringstream ss;
   ss << std::this_thread::get_id(); 
   uint64_t id = std::stoull(ss.str());

    for(auto logger:m_loggers)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        logger->write(level, file, line, id, message );
    }
}