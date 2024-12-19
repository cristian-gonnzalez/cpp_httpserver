#pragma once

#include "protocol.h"
#include "request.h"


namespace http::protocols {


    class Rest : public http::protocols::Protocol
    {
        private:

        public:
            Rest();
            ~Rest();

            Request request_load( std::string &str );
            http::Response handle( std::string in );
    };

}