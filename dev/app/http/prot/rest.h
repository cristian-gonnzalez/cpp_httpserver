#pragma once

#include "protocol.h"
#include "request.h"


namespace http::protocols {


    class Rest : public http::protocols::Protocol
    {
        private:
            Request serialize_request( std::string &str );

        public:
            Rest();
            ~Rest();

            http::Response handle( std::string in );
    };

}