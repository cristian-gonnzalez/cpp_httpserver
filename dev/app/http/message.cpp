#include "message.h"

using namespace http;

Message::Message(std::string http_version, std::string body)
: m_http_version{http_version}, m_body{body}
{
}

Message::~Message()
{
}
