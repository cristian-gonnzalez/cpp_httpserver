#pragma once

#include "response.h"
#include <string>
#include <string.h>
#include "exception.h"

namespace http::protocols {

    class Protocol
    {
        protected:
            // Construnctor is protected because this class is used only as interface
            Protocol();

        public:
            virtual ~Protocol();
            virtual http::Response handle( std::string in ) = 0;
    };

    class ProtocolException : public server::Exception
    {
        public:
            ProtocolException(std::string msg)
            : server::Exception(msg)
            {
            }
    };
}