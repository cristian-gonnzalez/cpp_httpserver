#pragma once

#include <string>
#include <vector>
#include <memory>
#include "protocol.h"
#include "response.h"

class ProtocolHandler
{
    private:
        std::vector< std::unique_ptr< http::protocols::Protocol > > _protocols{};

    public:
        ProtocolHandler();
        ~ProtocolHandler() = default;

        http::Response handle( std::string in );
};
