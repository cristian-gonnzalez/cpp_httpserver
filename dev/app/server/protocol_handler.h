#pragma once

#include <string>
#include <vector>
#include "protocol.h"
#include "response.h"

class ProtocolHandler
{
    private:
        std::vector<http::protocols::Protocol *> protocols;

    public:
        ProtocolHandler();
        ~ProtocolHandler();

        // TODO: change return type to http::response
        http::Response handle( std::string in );
};


