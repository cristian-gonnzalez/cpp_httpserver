#pragma once

#include "message.h"
#include <string>

namespace http
{
    class Response : public Message
    {
    public:
        Response(std::string_view http_version = "HTTP/1.1",
                 std::string_view body = "{}");

        ~Response() override;

        std::string to_str() override;
    };
}
