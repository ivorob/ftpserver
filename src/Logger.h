#pragma once

#include <ostream>
#include <memory>
#include <unordered_map>

#include "LogHandler.h"

class Logger {
public:
    enum class LogLevel {
        None = 0,
        Fatal = 1,
        Error = 2,
        Info = 3,
        Warning = 4,
        Debug = 5
    };

    using LogObject = std::ostream;
    using LogHandlerHolder = std::shared_ptr<LogHandler>;
public:
    Logger(LogLevel logLevel = LogLevel::None);

    LogLevel getLogLevel() const;
    void setLogLevel(LogLevel logLevel);

    LogObject& makeLogObject(LogLevel logLevel);

    void setHandler(LogLevel logLevel, LogHandlerHolder handler);
private:
    LogObject& standardLogLevelHandler(LogLevel logLevel) const;
private:
    LogLevel logLevel;
    std::unordered_map<LogLevel, LogHandlerHolder> handlers;
};

Logger::LogObject& LOG(Logger& logger, Logger::LogLevel logLevel);

static Logger logger;
