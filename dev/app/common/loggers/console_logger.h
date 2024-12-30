#pragma once

#include <iostream>
#include <string>
#include <source_location>
#include "logger.h"


namespace app::log 
{
    class CLogger : public Logger
    {
        public:
            CLogger();
            ~CLogger();

            void write(LogLevel level,  
                       const uint64_t id, 
                       const std::string& message,
                       const std::source_location location = std::source_location::current());
    };
}

