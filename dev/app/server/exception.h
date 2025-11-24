#pragma once
#include <string>
#include <cstring>
#include <cerrno>
#include <exception>

namespace server {

class Exception : public std::exception {

    public:
        explicit Exception(std::string msg) {
            _msg = "{ error_msg: '" + msg + "', errno: '" + std::string(strerror(errno)) + "' }";
        }

        const char* what() const noexcept override { return _msg.c_str(); }

    private:
        std::string _msg{};

};

} // namespace server