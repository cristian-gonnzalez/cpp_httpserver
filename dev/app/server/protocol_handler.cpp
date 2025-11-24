#include "protocol_handler.h"
#include "rest.h"
#include <iostream>

ProtocolHandler::ProtocolHandler()
{
    _protocols.emplace_back(std::make_unique<http::protocols::Rest>());
}

http::Response ProtocolHandler::handle( std::string in )
{
    http::Response resp{};
    for( auto& p : _protocols)
    {
        try
        {
            resp = p->handle( in );
            return resp;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    throw http::protocols::ProtocolException( "No protocol can handle the request");
}
