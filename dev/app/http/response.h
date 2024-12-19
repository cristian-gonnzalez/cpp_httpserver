#pragma once

#include "message.h"
#include <string>


namespace http
{
    class Response : public Message
    {
        public:
            Response();
            Response(std::string http_version, std::string body);
            ~Response();
            std::string to_str();
    };  
}

