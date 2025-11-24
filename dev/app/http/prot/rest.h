#pragma once

#include "protocol.h"
#include "request.h"

namespace http::protocols {

    class Rest : public Protocol
    {
        private:
            http::Request parse_request(const std::string& raw);

        public:
            Rest() = default;
            ~Rest() override = default;

            http::Response handle(std::string in) override;
    };
}
