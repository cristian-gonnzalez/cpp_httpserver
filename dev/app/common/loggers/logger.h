#pragma once
#include <string>

namespace app::log 
{    
    class Logger
    {
        protected:
            Logger() = default;
            
        public:
            virtual ~Logger() = default;
            
            virtual void write(const std::string& message) = 0;
    };
}