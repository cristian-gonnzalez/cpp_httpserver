#pragma once
#include "command.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <iostream>

class CommandAdd : public Command
{
    public:
        ~CommandAdd() override = default;

        std::string execute(const std::string& input) override
        {
            // Pretend JSON format {"a":1,"b":2}
            int a = 0, b = 0;

            std::string s{input};
            s.erase( std::remove_if(s.begin(), s.end(),
                                    [](unsigned char c) 
                                    { 
                                        return std::isspace(c); 
                                    }),
                      s.end() );
            sscanf(s.c_str(), "{\"a\":%d,\"b\":%d}", &a, &b);            
            int result = a + b;

            std::ostringstream out;
            out << "{ \"result\": " << result << " }";

            return out.str();
        }
};