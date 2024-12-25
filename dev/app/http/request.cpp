#include "request.h"
#include <sstream>
#include "log_director.h"

using namespace http;


Request::Request( std::string method, std::string target, std::string http_version, std::string body)
: Message{ http_version, body }, m_method{method}, m_target{target}
{
  INFO("Calling Request( args )");
}

Request::~Request()
{
  INFO("Calling ~Request");
}

std::string Request::to_str()
{
    std::string r;
    std::stringstream ss{};

    ss << m_method << " "
       << m_target << " "
       << m_http_version << " \n"
       << "Content-Type: application/json\n"
       << "Content-Length: " << m_body.size() << " \n"
       << m_body << "\n";

    ss >> r;

    return r;
}