#include <iostream>
#include "Logger.h"

Logger logger(Logger::LogLevel::Debug);

Logger::Logger(LogLevel logLevel)
    : logLevel(logLevel) {
}

Logger::LogLevel Logger::getLogLevel() const {
    return this->logLevel;
}

void Logger::setLogLevel(LogLevel logLevel) {
    this->logLevel = logLevel;
}

std::ostream& Logger::makeLogObject(LogLevel logLevel) {
    if (logLevel <= getLogLevel()) {
        auto it = this->handlers.find(logLevel);
        if (it != std::cend(this->handlers)) {
            auto handler = it->second;
            if (handler) {
                return handler->getStream();
            }
        }

        return standardLogLevelHandler(logLevel);
    }

    return nullLogHandler.getStream();
}

Logger::LogObject& Logger::standardLogLevelHandler(LogLevel logLevel) const {
    switch (logLevel) {
        case LogLevel::Error:
        case LogLevel::Fatal:
            return std::cerr;
        case LogLevel::Info:
        case LogLevel::Warning:
        default:
            return std::cout;
    }
}

void Logger::setHandler(LogLevel logLevel, LogHandlerHolder handler) {
    this->handlers[logLevel] = handler;
}

Logger::LogObject& LOG(Logger& logger, Logger::LogLevel logLevel) {
    return logger.makeLogObject(logLevel);
}
