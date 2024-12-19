#pragma once

#include <string>

namespace http
{
    class Message
    {
        protected:
            std::string m_http_version;
            std::string m_body;

            // Construnctor is protected because this class is used only as interface
            Message(std::string http_version, std::string body);

        public:
            
            virtual ~Message();
            virtual std::string to_str() = 0;
    };  
}
