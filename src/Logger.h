#pragma once

#include <ostream>

class Logger {
public:
    enum class LogLevel {
        None = 0,
        Info,
        Warning,
        Error,
        Fatal
    };

    using LogObject = std::ostream;
public:
    Logger(LogLevel logLevel = LogLevel::None);

    LogLevel getLogLevel() const;
    void setLogLevel(LogLevel logLevel);

    LogObject& makeLogObject(LogLevel logLevel);
private:
    LogLevel logLevel;
};

Logger::LogObject& LOG(Logger& logger, Logger::LogLevel logLevel);

static Logger logger;
