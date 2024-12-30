#pragma once

#include "logger.h"
#include <list>
#include <string>
#include <memory>
#include <mutex>
#include <source_location>


/**
 *  Useful Macros
 */
#define LOG_WRAPPER( level, msg )   \
    app::log::LogDirector::get().write( app::log::LogLevel::level, msg )

#define LOG( msg )   LOG_WRAPPER( normal, msg )
#define DEBUG( msg ) LOG_WRAPPER( debug, msg )
#define INFO( msg )  LOG_WRAPPER( info, msg )
#define WARN( msg )  LOG_WRAPPER( warning, msg )
#define ERROR( msg ) LOG_WRAPPER( error, msg )


namespace app::log 
{
    class LogDirector
    {
        private:
            /** Singlentons variables. */   
            static std::mutex m_mutex;        
            
            std::list< std::shared_ptr<app::log::Logger> > m_loggers;
            
        protected:
            /** Singlentons constructor should not be accesible. */
            LogDirector();
        
        public:  
            ~LogDirector();
            /** Singletons should not have copy constructor. */
            LogDirector(LogDirector &other) = delete;
            /** Singletons should not be assignable. */
            void operator=(const LogDirector &) = delete;
            /** Singlentons unique instance */
            static LogDirector& get();
        
            void add(std::shared_ptr<app::log::Logger> logger);    
            void write(app::log::LogLevel level, 
                       const std::string& message,
                       const std::source_location location = std::source_location::current());
    };
}