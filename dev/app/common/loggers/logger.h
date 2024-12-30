#pragma once
#include <string>
#include <cstdint>
#include <source_location>


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
    
    class Logger
    {
        protected:
            LogLevel m_level;

            Logger();
            std::string level_tostr(LogLevel level);
        
        public:
            virtual ~Logger();
            void set_level( LogLevel level );
            LogLevel get_level() const;
            
            virtual void write(LogLevel level, 
                               const uint64_t id, 
                               const std::string& message,
                               const std::source_location location = std::source_location::current()) = 0;
    };

}