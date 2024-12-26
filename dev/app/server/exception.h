#pragma once
#include <string>
#include <string.h>

namespace server 
{
    class Exception : public std::exception
    {
        private:
            std::string m_msg;
        
        public:
            Exception(std::string msg)
            : m_msg()
            {
                m_msg.append("{ error_msg: '");
                m_msg.append(msg);
                m_msg.append(".', errno: '");
                m_msg.append(strerror(errno));
                m_msg.append("' }");
            }

            const char* what() const throw() { return m_msg.c_str(); }    
    };
}