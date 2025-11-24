#pragma once
#include "command.h"


#include <sstream>

class CommandAdd : public Command
{
    public:
        ~CommandAdd() override = default;

        std::string execute(const std::string& input) override
        {
            // Pretend JSON format {"a":1,"b":2}
            int a = 0, b = 0;
            sscanf(input.c_str(), "{\"a\":%d,\"b\":%d}", &a, &b);

            int result = a + b;

            std::ostringstream out;
            out << "{ \"result\": " << result << " }";

            return out.str();
        }
};