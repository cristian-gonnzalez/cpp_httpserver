#pragma once

#include <iostream>
#include <string>
#include "logger.h"


namespace app::log 
{
    class CLogger : public Logger
    {
        public:
            CLogger();
            ~CLogger();

            void write(LogLevel level, const std::string file, const int line, const uint64_t id, const std::string& message);
    };
}

