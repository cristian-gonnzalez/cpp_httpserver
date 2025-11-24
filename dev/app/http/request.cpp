#include "request.h"

#include <sstream>
#include "log_director.h"

using namespace http;

Request::Request(std::string_view method,
                 std::string_view target,
                 std::string_view http_version,
                 std::string_view body)
: Message(http_version, body), _method(method), _target(target)
{
    app_info << "Request::Request()" << std::endl;
}

Request::~Request()
{
    app_info << "Request::~Request()" << std::endl;
}

std::string Request::to_str()
{
    std::ostringstream ss;

    ss << _method << " " << _target << " " << _http_version << "\r\n"
       << "Content-Type: application/json\r\n"
       << "Content-Length: " << _body.size() << "\r\n"
       << "\r\n"
       << _body;

    return ss.str();
}
