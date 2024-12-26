#include "protocol_handler.h"
#include "rest.h"
#include <iostream>

ProtocolHandler::ProtocolHandler()
: m_protocols{}
{
    m_protocols.push_back(new http::protocols::Rest());
}

ProtocolHandler::~ProtocolHandler()
{
    for( auto& p:m_protocols)
    {
        delete p;
    }
}

http::Response ProtocolHandler::handle( std::string in )
{
    http::Response resp{};
    for( auto& p:m_protocols)
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

    throw http::protocols::ProtocolException( "No protocol can handle the reqest");

    return resp;
}