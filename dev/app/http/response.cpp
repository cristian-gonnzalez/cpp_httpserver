#include "response.h"

using namespace http;

Response::Response( std::string http_version, std::string body)
: Message( http_version, body )
{
}

Response::Response( )
: Message( "HTTP/1.1", "{}" )
{
}

Response::~Response()
{
}

std::string Response::to_str()
{
    std::string r = "HTTP/1.1 200 OK"
                    "Content-Length: 2"
                    "Content-Type: application/json\n"
                    "{}";
    return r;
}