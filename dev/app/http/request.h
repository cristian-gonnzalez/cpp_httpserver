#pragma once

#include "message.h"
#include <string>

namespace http
{
    class Request : public Message
    {
        std::string _method;
        std::string _target;

    public:
        Request(std::string_view method,
                std::string_view target,
                std::string_view http_version,
                std::string_view body);

        ~Request() override;

        std::string to_str() override;
    };
}