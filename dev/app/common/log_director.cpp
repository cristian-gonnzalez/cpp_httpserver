#include "log_director.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <thread>

using namespace app::log;

LogDirector* LogDirector::get()
{
    static LogDirector instance;
    return &instance;
}

void LogDirector::add(std::shared_ptr<Logger> logger)
{
    std::lock_guard lock(_mutex);
    _loggers.push_back(std::move(logger));
}

void LogDirector::set_level(LogLevel level) noexcept
{
    std::lock_guard lock(_mutex);
    _level = level;
}

LogLevel LogDirector::get_level() const noexcept
{
    std::lock_guard lock(_mutex);
    return _level;
}

void LogDirector::write(LogLevel level, const std::string& message)
{
    // Only log messages with severity >= configured level.
    // Higher enum value == more severe.
    {
        std::lock_guard lock(_mutex);
        if (static_cast<int>(level) >= static_cast<int>(_level))
            return;

        // Copy loggers under lock to avoid iterator invalidation if someone calls add() concurrently.
        auto loggers_copy = _loggers;

        // Unlock while performing IO? No — keep the lock while writing to guarantee that the full message
        // appears in each logger contiguously and that two messages from different threads don't interleave.
        // So continue holding lock and write to each logger.
        for (auto& logger : loggers_copy)
        {
            if (logger)
            {
                try
                {
                    logger->write(message);
                }
                catch (...)
                {
                    // Swallow exceptions from loggers to avoid crashing the application.
                    // In production you might want to record the error in a fallback sink.
                }
            }
        }
    }
}


LogBuffer::LogBuffer(LogLevel level)
    : _level(level)
{
    // Initial buffer left empty. Source location will be provided by operator<< macro.
}

LogBuffer::LogBuffer(LogBuffer&& other) noexcept
    : _level(other._level),
      _buf(),
      _location(other._location),
      _flushed(other._flushed)
{
    _buf << other._buf.str();
    other._flushed = true;
}

LogBuffer& LogBuffer::operator=(LogBuffer&& other) noexcept
{
    if (this != &other)
    {
        _level = other._level;
        _buf.str("");
        _buf.clear();
        _buf << other._buf.str();
        _location = other._location;
        _flushed = other._flushed;
        other._flushed = true;
    }
    return *this;
}

LogBuffer::~LogBuffer()
{
    if (!_flushed && _buf.str().size() > 0)
    {
        emit_to_director();
    }
}

LogBuffer& LogBuffer::operator<<(const std::source_location& loc)
{
    _location = loc;

    // prepend source information and timestamp
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    tm local_tm;

    localtime_r(&t, &local_tm);

    std::ostringstream ss;
    ss << loc.file_name() << ":" << loc.line() << ":" << loc.column()
       << " [" << loc.function_name() << "] "
       << "[" << std::put_time(&local_tm, "%Y-%m-%d %H:%M:%S") << "] "
       << "(" << std::this_thread::get_id() << "): ";

    _buf << ss.str();
    return *this;
}

LogBuffer& LogBuffer::operator<<(Manip manip)
{
    // Common manip: std::endl will insert newline and flush.
    // Insert newline (we don't inspect the manip, we assume it's newline-like).
    _buf << std::endl;
    emit_to_director();
    _flushed = true;
    return *this;
}

void LogBuffer::flush()
{
    if (!_flushed && _buf.str().size() > 0)
    {
        emit_to_director();
        _flushed = true;
    }
}

void LogBuffer::emit_to_director()
{
    // Build final message: prefix (already added by source_location operator) + buffer content.
    const std::string message = _buf.str();

    // Use singleton director
    LogDirector* director = LogDirector::get();
    if (director)
    {
        director->write(_level, message);
    }
}
