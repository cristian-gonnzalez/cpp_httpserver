#pragma once

#include "logger.h"
#include <thread>
#include <list>
#include <string>
#include <memory>
#include <mutex>
#include <source_location>
#include <iostream>
#include <sstream>


/**
 *  Useful macros
 */
#define app_wrapper( level )  \
    app::log::_##level << "[ " << #level << " ]" << std::source_location::current() 

#define app_log    app_wrapper( normal ) 
#define app_debug  app_wrapper( debug ) 
#define app_info   app_wrapper( info ) 
#define app_warn   app_wrapper( warn ) 
#define app_error  app_wrapper( error ) 
#define app_fatal  app_wrapper( fatal ) 


namespace app::log 
{
    enum class LogLevel 
    { 
        normal, 
        debug, 
        info, 
        warning, 
        error, 
        fatal
    };

    class LogBuffer;

    class LogDirector
    {
        private:
            /** Singlentons variables. */   
            static std::mutex m_mutex;  
            LogLevel m_level{LogLevel::normal};      
            
            std::list< std::shared_ptr<app::log::Logger> > m_loggers;
            
        protected:
            /** Singlentons constructor should not be accesible. */
            LogDirector();
        
        public:             
            ~LogDirector();
            /** Singletons should not have copy constructor. */
            LogDirector(const LogDirector &other) = delete;
            /** Singletons should not be assignable. */
            void operator=(const LogDirector &) = delete;
            /** Singlentons unique instance */
            static LogDirector* get();
        
            void add(std::shared_ptr<app::log::Logger> logger);    

            void set_level( const LogLevel level );
            LogLevel get_level() const;
 
            void write(const LogBuffer& buf);
    };   

    /** LogBuffer stores the message until a 'std::endl' or '\n' is read. Then calls LogDirector 
     *  to writes at the different loggers
     */
    class LogBuffer
    {
        private:
            // Note:
            //      This member variable is a pointer so we have a 'have a' relationship 
            //      but in this case, it is an agregate (it is not a composition), this 
            //      means the lifetime does not depend of this object.
            LogDirector* m_director{nullptr};
            LogLevel     m_level{LogLevel::normal};
            std::string  m_buffer;

        public:
            // Constructor
            LogBuffer(LogLevel level, LogDirector* director);
            // Copy constructor
            LogBuffer(const LogBuffer& other);
            // overload operator =
            LogBuffer& operator=(const LogBuffer& other);

            LogLevel get_level() const;

            std::string to_string() const;

            // formatted output
            template<typename T> 
            LogBuffer& operator<<(const T& value );

            // Specialization of std::source_location for formatted output
            LogBuffer& operator<<(const std::source_location& location);

            // Specialization of literal string for formatted output
            LogBuffer& operator<<(const char* cstr);
 
            // This overload is used to implement output I/O manipulators such as std::endl
            LogBuffer& operator<<(
                       std::basic_ostream<char, std::char_traits<char>>& (*func) (std::basic_ostream<char, std::char_traits<char>>&) );
    };

    // explicitly instantiate the template, and its member definitions 
    // Otherwise, it will not generate the version we need when we call it from main
    template<typename T> 
    LogBuffer& LogBuffer::operator<<(const T& value )
    {
        std::stringstream ss;
        ss << value;

        m_buffer += ss.str();

        return *this;
    }   
    
    /**
     * Global logger objects initializations
     */
    inline LogBuffer  _normal ( LogLevel::normal,  LogDirector::get() ); 
    inline LogBuffer  _debug  ( LogLevel::debug,   LogDirector::get() ); 
    inline LogBuffer  _info   ( LogLevel::info,    LogDirector::get() );
    inline LogBuffer  _warning( LogLevel::warning, LogDirector::get() );
    inline LogBuffer  _error  ( LogLevel::error,   LogDirector::get() ); 
    inline LogBuffer  _fatal  ( LogLevel::fatal,   LogDirector::get() );
}