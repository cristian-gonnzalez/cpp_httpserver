#pragma once
#include <string>


class Command
{
    public:
        virtual ~Command() = default;

        // TODO: return JSON result
        virtual std::string execute(const std::string& input) = 0;    
};