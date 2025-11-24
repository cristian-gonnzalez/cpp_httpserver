#include "rest.h"

#include <iostream>
#include <sstream>

#include "log_director.h"

using namespace http::protocols;

static std::string trim(const std::string& s)
{
    auto start = s.find_first_not_of(" \t\r\n");
    auto end   = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

http::Request Rest::parse_request(const std::string& raw)
{
    // Split headers vs. body
    const std::string delim = "\r\n\r\n";
    size_t pos = raw.find(delim);
    if (pos == std::string::npos)
        throw ProtocolException("Malformed HTTP: missing header/body separator");

    std::string header_block = raw.substr(0, pos);
    std::string body = raw.substr(pos + delim.size());

    // Parse request line: METHOD TARGET VERSION
    std::istringstream header_stream(header_block);
    std::string request_line;
    if (!std::getline(header_stream, request_line))
        throw ProtocolException("Malformed HTTP: empty request line");

    request_line = trim(request_line);

    std::istringstream rl(request_line);
    std::string method, target, version;

    rl >> method >> target >> version;
    if (method.empty() || target.empty() || version.empty())
        throw ProtocolException("Malformed HTTP: invalid request line");

    if (method != "POST" && method != "GET")
        throw ProtocolException("Unsupported method: " + method);

    // Parse Content-Length (optional for GET)
    std::string header;
    size_t content_length = 0;

    while (std::getline(header_stream, header))
    {
        header = trim(header);

        if (header.rfind("Content-Length:", 0) == 0)
        {
            std::string num = header.substr(strlen("Content-Length:"));
            num = trim(num);
            content_length = std::stoul(num);
        }
    }

    if (method == "POST" && body.size() != content_length)
        throw ProtocolException("Content-Length mismatch");

    return http::Request(method, target, version, body);
}

http::Response Rest::handle(std::string in)
{
    app_debug << "Handling message with Rest" << std::endl;

    try
    {
        http::Request req = parse_request(in);
        // TODO: dispatch your "routes" here
    }
    catch (const std::exception& e)
    {
        app_error << e.what() << std::endl;
        throw; // correct rethrow
    }

    app_debug << "Rest handled the message OK" << std::endl;

    return http::Response("HTTP/1.1", "{}");
}
