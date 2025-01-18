#pragma once

#include <string>
#include "logger.h"


namespace app::log 
{
    class CLogger : public Logger
    {
        public:
            CLogger();
            ~CLogger();

            void write(const std::string& message);
    };
}

