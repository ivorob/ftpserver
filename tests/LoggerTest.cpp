#include <gtest/gtest.h>

#include "Logger.h"
#include "ScopedStreamRedirector.h"

TEST(LoggerTest, log_level_by_default_isnot_set) {
    // Arrange
    Logger logger;

    // Act
    auto logLevel = logger.getLogLevel();

    // Assert
    ASSERT_EQ(Logger::LogLevel::None, logLevel);
}

TEST(LoggerTest, log_level_can_be_set_in_constructor) {
    // Arrange

    // Act
    Logger logger(Logger::LogLevel::Error);

    // Assert
    ASSERT_EQ(Logger::LogLevel::Error, logger.getLogLevel());
}

TEST(LoggerTest, log_level_can_be_changed) {
    // Arrange
    Logger logger(Logger::LogLevel::Info);

    // Act
    logger.setLogLevel(Logger::LogLevel::Warning);

    // Assert
    ASSERT_EQ(Logger::LogLevel::Warning, logger.getLogLevel());
}

TEST(LoggerTest, error_log_message_is_printed_to_standard_error_output) {
    // Arrange
    std::stringstream output;
    ScopedStreamRedirector streamRedirector(std::cerr, output);

    Logger logger(Logger::LogLevel::Error);
    auto& logMessage = logger.makeLogObject(Logger::LogLevel::Error);

    // Act
    logMessage << "Some error: " << 123;

    // Assert
    ASSERT_EQ(output.str(), "Some error: 123");
}

TEST(LoggerTest, simplify_logging) {
    // Arrange
    std::stringstream output;
    ScopedStreamRedirector streamRedirector(std::cerr, output);

    Logger logger(Logger::LogLevel::Error);

    // Act
    LOG(logger, Logger::LogLevel::Error) << "New message" << " to error log!";

    // Assert
    ASSERT_EQ(output.str(), "New message to error log!");
}
