#pragma once
#include "logger.h"

#include <atomic>
#include <chrono>
#include <list>
#include <memory>
#include <mutex>
#include <source_location>
#include <sstream>
#include <string>
#include <vector>

namespace app::log
{
    enum class LogLevel : int
    {
        normal = 0,
        debug  = 1,
        info   = 2,
        warn   = 3,
        error  = 4,
        fatal  = 5
    };

    class LogBuffer
    {
        public:
            // Construct a temporary LogBuffer for a given level.
            explicit LogBuffer(LogLevel level);

            // Movable (temporaries must be movable), non-copyable.
            LogBuffer(LogBuffer&&) noexcept;
            LogBuffer& operator=(LogBuffer&&) noexcept;
            LogBuffer(const LogBuffer&) = delete;
            LogBuffer& operator=(const LogBuffer&) = delete;

            ~LogBuffer();

            // Generic streaming
            template<typename T>
            LogBuffer& operator<<(const T& v)
            {
                std::ostringstream ss;
                ss << v;
                _buf << ss.str();
                return *this;
            }

            // source_location specialization
            LogBuffer& operator<<(const std::source_location& loc);

            // C-string specialization (we append, but we do NOT auto-flush on embedded '\n')
            LogBuffer& operator<<(const char* s)
            {
                _buf << s;
                return *this;
            }

            // Manipulator (e.g. std::endl)
            using Manip = std::basic_ostream<char, std::char_traits<char>>& (*)(std::basic_ostream<char, std::char_traits<char>>&);

            LogBuffer& operator<<(Manip manip);

            // Explicit flush
            void flush();

            LogLevel level() const noexcept { return _level; }

        private:
            void emit_to_director();

            LogLevel                _level;
            std::ostringstream      _buf{};
            std::source_location    _location{ std::source_location::current() };
            bool                    _flushed{ false };
    };

    class LogDirector
    {
        public:
            static LogDirector* get();

            LogDirector(const LogDirector&) = delete;
            LogDirector& operator=(const LogDirector&) = delete;

            void add(std::shared_ptr<Logger> logger);
            void set_level(LogLevel level) noexcept;
            LogLevel get_level() const noexcept;

            // Called by LogBuffer to write fully-formed string.
            void write(LogLevel level, const std::string& message);

        private:
            LogDirector() = default;
            ~LogDirector() = default;

            mutable std::mutex                     _mutex{};
            LogLevel                               _level{ LogLevel::normal };
            std::vector< std::shared_ptr<Logger> > _loggers{};
    };

    // Keep macros working with temporary LogBuffer creation.
    #define app_wrapper(level) (app::log::LogBuffer(app::log::LogLevel::level) << "[ " << #level << " ] " << std::source_location::current())
    #define app_log    app_wrapper(normal)
    #define app_debug  app_wrapper(debug)
    #define app_info   app_wrapper(info)
    #define app_warn   app_wrapper(warn)
    #define app_error  app_wrapper(error)
    #define app_fatal  app_wrapper(fatal)

} // namespace app::log