#include "response.h"
#include "log_director.h"

using namespace http;

Response::Response( std::string http_version, std::string body)
: Message{ http_version, body }
{
  INFO("Calling Response( args )");
}

Response::Response( )
: Message{ "HTTP/1.1", "{}" }
{
  INFO("Calling Response");
}

Response::~Response()
{
  INFO("Calling ~Response");
}

std::string Response::to_str()
{
    std::string r{ "HTTP/1.1 200 OK\n"
                   "Content-Length: 2\n"
                   "Content-Type: application/json\n"
                   "{}"};
    return r;
}