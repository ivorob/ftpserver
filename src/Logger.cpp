#include <iostream>
#include "Logger.h"

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
    switch (logLevel) {
        case LogLevel::Error:
        case LogLevel::Fatal:
            return std::cerr;
        case LogLevel::None:
        case LogLevel::Info:
        case LogLevel::Warning:
        default:
            return std::cout;
    }
}

Logger::LogObject& LOG(Logger& logger, Logger::LogLevel logLevel) {
    return logger.makeLogObject(logLevel);
}
