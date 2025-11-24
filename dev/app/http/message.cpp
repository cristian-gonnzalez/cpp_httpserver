#include "message.h"

using namespace http;


Message::Message(std::string_view http_version, std::string_view body)
: _http_version(http_version), _body(body)
{
}
