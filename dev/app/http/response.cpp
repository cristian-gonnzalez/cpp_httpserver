#include "response.h"
#include "log_director.h"

using namespace http;

Response::Response( std::string http_version, std::string body)
: Message{ http_version, body }
{  
  app_info << "Calling Response( args )\n";
}

Response::Response( )
: Message{ "HTTP/1.1", "{}" }
{
  app_info << "Calling Response\n";
}

Response::~Response()
{
  app_info << "Calling ~Response\n";
}

std::string Response::to_str()
{
    std::string r{ "HTTP/1.1 200 OK\n"
                   "Content-Length: 2\n"
                   "Content-Type: application/json\n"
                   "{}"};
    return r;
}