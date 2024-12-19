#pragma once

#include "message.h"
#include <string>

namespace http
{
    class Request : public Message
    {
        private:
            std::string m_method;
            std::string m_target;
        public:
            Request(std::string method, std::string target, std::string http_version, std::string body);
            ~Request();
            std::string to_str();
    };  
}

