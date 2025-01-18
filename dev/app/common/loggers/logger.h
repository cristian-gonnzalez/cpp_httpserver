#pragma once
#include <string>

namespace app::log 
{    
    class Logger
    {
        protected:
            Logger();
            
        public:
            virtual ~Logger();
            
            virtual void write(const std::string& message) = 0;
    };
}