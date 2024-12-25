#include "rest.h"
#include <iostream>
#include <sstream>
#include "log_director.h"

using namespace http::protocols;

Rest::Rest()
{
}

Rest::~Rest()
{
}

http::Request Rest::request_load( std::string &str )
{
    std::string method, target, version, body;

    std::stringstream ss{str};
    ss >> method;
    if(method != "POST" && method != "GET")
    {
        throw http::protocols::ProtocolException( "{Rest} HTT Request format error: the method '" + method + "' is not recognized");
    }

    ss >> target;
    ss >> version;

    std::string key{"Content-Length:"};
    std::size_t found = str.find(key);
    if ( found == std::string::npos )
    {
        throw http::protocols::ProtocolException( "{Rest} HTT Request format error: 'Content-Length' not found");
    }

    str = str.substr( found + key.size() );
    found = str.find("{");
    if ( found == std::string::npos )
    {
        throw http::protocols::ProtocolException( "{Rest} HTT Request format error: '{' not found");
    }

    body = str.substr( found );
    
    return http::Request{method, target, version, body};
}


http::Response Rest::handle( std::string in )
{
    DEBUG( "Handling message with Rest" );

    try
    {
        Request req{ request_load(in) };
        // TODO: handle request here
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw e;
    }
    
    Response r{ "HTTP/1.1", "{}" };
 
    DEBUG( "Rest could handle the message" );   
    return r;
}