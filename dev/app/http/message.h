#pragma once
#include <string>
#include <string_view>

namespace http
{
    class Message
    {
        public:
            virtual ~Message() = default;               
            virtual std::string to_str() = 0; 
            
        protected:
            std::string _http_version;
            std::string _body;

            // protected: base is an interface
            Message(std::string_view http_version, std::string_view body);
    };
}



