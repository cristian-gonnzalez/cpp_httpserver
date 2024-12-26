#pragma once
#include <string>
#include <cstdint>


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
            virtual void write(LogLevel level, const std::string file, const int line, const uint64_t id, const std::string& message) = 0;
    };

}