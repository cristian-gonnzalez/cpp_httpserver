#include "logger.h"

using namespace app::log;


Logger::Logger()
: m_level( LogLevel::normal )
{
}

Logger::~Logger() 
{
}

void Logger::set_level( LogLevel level )
{
    m_level = level;
}

LogLevel Logger::get_level() const 
{
    return m_level;
}


std::string Logger::level_tostr(LogLevel level)
{
    switch (level) 
    {
        case LogLevel::normal:
            return "[  normal ]";
        case LogLevel::debug:
            return "[  debug  ]";
        case LogLevel::info:
            return "[  info   ]";
        case LogLevel::warning:
            return "[ warning ]";
        case LogLevel::error:
            return "[  error  ]";
        case LogLevel::fatal:
            return "[  fatal  ]";
        default:
            return "[ unknown ]";
    }
}
