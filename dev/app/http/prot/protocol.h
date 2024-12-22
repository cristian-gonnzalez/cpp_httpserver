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
            // NOTE: as one of the method is a pure virtual funtion, this class is abstract (interface) so 
            // this means no object can instanciate from this constructor. Anyway, I added this
            // to make clear that I dont want anyone to use this class directly
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
