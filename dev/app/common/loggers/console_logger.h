#pragma once

#include <string>
#include "logger.h"


namespace app::log 
{
    class CLogger : public Logger
    {
        public:
            CLogger() = default;
            ~CLogger() override = default;

            void write(const std::string& message) override;
    };
}

