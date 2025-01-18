#include "log_director.h"

/** Enables inner logging. */
#ifndef LOG_DIRECTOR_LOG
#  define LOG_DIRECTOR_LOG 0
#endif

using namespace app::log;

std::mutex LogDirector::m_mutex;

LogDirector::LogDirector()
: m_loggers{}
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

LogDirector* LogDirector::get()
{
    static LogDirector instance;
	return &instance;
}

void LogDirector::add( std::shared_ptr<app::log::Logger> logger )
{
    m_loggers.push_back( logger );
}


void LogDirector::set_level( const LogLevel level )
{
    m_level = level;
}

LogLevel LogDirector::get_level() const 
{
    return m_level;
}

void LogDirector::write(const LogBuffer& buf)
{   
    if( buf.get_level() >= m_level)
        return;

    for(auto logger:m_loggers)
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        logger->write(buf.to_string());
    }
}


LogBuffer::LogBuffer(LogLevel level, LogDirector* director)
: m_director{director}, m_level{level}, m_buffer{""}
{
}

LogBuffer::LogBuffer(const LogBuffer& other)
: m_director{other.m_director}, m_level{other.m_level}, m_buffer{other.m_buffer}
{
}

LogBuffer& LogBuffer::operator=(const LogBuffer& other)
{
    m_director = other.m_director;
    m_level = other.m_level;
    m_buffer = other.m_buffer;

    return *this;
}

LogLevel LogBuffer::get_level() const
{
    return m_level;
}

std::string LogBuffer::to_string() const
{
    return m_buffer;
}

LogBuffer& LogBuffer::operator<<(const std::source_location& location)
{         
    time_t now{ time(0) };
    tm* timeinfo{ localtime(&now) };
    char timestamp[20];
    strftime( timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", timeinfo );

    std::stringstream ss;    
    ss << location.file_name() << ':'
       << location.line() << ':'
       << location.column() 
       << " [" << location.function_name() << "]:"
       << " [" << timestamp << "]"
       << " (" << std::this_thread::get_id() << "): ";
    
    m_buffer += ss.str();

    return *this;
}

LogBuffer& LogBuffer::operator<<(const char* cstr)
{         
    std::string s{cstr};
    m_buffer += s;
    
    if(s.find("\n") != std::string::npos  )
    {
        m_director->write( *this );
        m_buffer.clear();
    }

    return *this;
}

LogBuffer& LogBuffer::operator<<(
            std::basic_ostream<char, std::char_traits<char>>& (*func) (std::basic_ostream<char, std::char_traits<char>>&) )
{
    m_buffer += "\n";

    m_director->write( *this );
    m_buffer.clear();

    return *this;                
}
