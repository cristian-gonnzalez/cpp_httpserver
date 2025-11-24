#include "response.h"
#include "log_director.h"
#include <sstream>

using namespace http;

Response::Response(std::string_view http_version,
                   std::string_view body)
: Message(http_version, body)
{
    app_info << "Response::Response()" << std::endl;
}

Response::~Response()
{
    app_info << "Response::~Response()" << std::endl;
}

std::string Response::to_str()
{
    std::ostringstream ss;

    ss << _http_version << " 200 OK\r\n"
       << "Content-Type: application/json\r\n"
       << "Content-Length: " << _body.size() << "\r\n"
       << "\r\n"
       << _body;

    return ss.str();
}
