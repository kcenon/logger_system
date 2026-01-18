/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file ilogger_interface_test.cpp
 * @brief Unit tests for ILogger interface implementation
 * @author 🍀☀🌕🌥 🌊
 * @since 2.0.0
 *
 * @details This file contains unit tests to verify that the logger class
 * properly implements the common::interfaces::ILogger interface.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>

#include <memory>
#include <sstream>

namespace kcenon::logger::test {

namespace ci = common::interfaces;

/**
 * @brief Test fixture for ILogger interface tests
 */
class ILoggerInterfaceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a logger in sync mode for predictable testing
        logger_ = std::make_shared<logger>(false, 1024);
        logger_->add_writer(std::make_unique<console_writer>(false));
        logger_->start();
    }

    void TearDown() override {
        if (logger_) {
            logger_->stop();
        }
    }

    std::shared_ptr<logger> logger_;
};

/**
 * @brief Verify that logger can be used as ILogger interface pointer
 */
TEST_F(ILoggerInterfaceTest, CanBeUsedAsILoggerPointer) {
    // logger should be convertible to ILogger*
    ci::ILogger* ilogger = logger_.get();
    ASSERT_NE(ilogger, nullptr);
}

/**
 * @brief Verify that logger can be used as ILogger shared_ptr
 */
TEST_F(ILoggerInterfaceTest, CanBeUsedAsILoggerSharedPtr) {
    // logger shared_ptr should be convertible to ILogger shared_ptr
    std::shared_ptr<ci::ILogger> ilogger = logger_;
    ASSERT_NE(ilogger, nullptr);
}

/**
 * @brief Test log method with common::interfaces::log_level
 */
TEST_F(ILoggerInterfaceTest, LogWithCommonLogLevel) {
    ci::ILogger* ilogger = logger_.get();

    // Should compile and work with common::interfaces::log_level
    auto result = ilogger->log(ci::log_level::info, std::string("Test message"));
    EXPECT_TRUE(result.is_ok());

    result = ilogger->log(ci::log_level::warning, std::string("Warning message"));
    EXPECT_TRUE(result.is_ok());

    result = ilogger->log(ci::log_level::error, std::string("Error message"));
    EXPECT_TRUE(result.is_ok());
}

/**
 * @brief Test log method with source location
 */
TEST_F(ILoggerInterfaceTest, LogWithSourceLocation) {
    ci::ILogger* ilogger = logger_.get();

    // Using the source_location overload
    auto result = ilogger->log(ci::log_level::info,
                               std::string_view("Message with location"),
                               common::source_location::current());
    EXPECT_TRUE(result.is_ok());
}

/**
 * @brief Test log method with explicit file/line/function (legacy API)
 * @note This tests the backward compatibility method that was removed from
 *       common::interfaces::ILogger in v3.0.0 (Issue #217) but is still
 *       available directly on the logger class.
 */
TEST_F(ILoggerInterfaceTest, LogWithExplicitLocation) {
    // Note: This method is no longer part of ILogger interface (removed in v3.0.0)
    // but is still available directly on logger class for backward compatibility

    // Suppress deprecation warning for this test - we're testing backward compatibility
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996)
#endif

    // Call directly on logger (not through ILogger* since it's no longer in interface)
    auto result = logger_->log(ci::log_level::debug,
                               std::string("Debug message"),
                               std::string("test_file.cpp"),
                               42,
                               std::string("test_function"));
    EXPECT_TRUE(result.is_ok());

#if defined(_MSC_VER)
#pragma warning(pop)
#endif
#if defined(__clang__) || defined(__GNUC__)
#pragma GCC diagnostic pop
#endif
}

/**
 * @brief Test log method with log_entry
 */
TEST_F(ILoggerInterfaceTest, LogWithLogEntry) {
    ci::ILogger* ilogger = logger_.get();

    ci::log_entry entry;
    entry.level = ci::log_level::info;
    entry.message = "Entry-based message";
    entry.file = "test.cpp";
    entry.line = 100;
    entry.function = "test_func";

    auto result = ilogger->log(entry);
    EXPECT_TRUE(result.is_ok());
}

/**
 * @brief Test is_enabled method with common::interfaces::log_level
 */
TEST_F(ILoggerInterfaceTest, IsEnabledWithCommonLogLevel) {
    ci::ILogger* ilogger = logger_.get();

    // Default level should be info, so trace and debug should be disabled
    ilogger->set_level(ci::log_level::info);

    EXPECT_FALSE(ilogger->is_enabled(ci::log_level::trace));
    EXPECT_FALSE(ilogger->is_enabled(ci::log_level::debug));
    EXPECT_TRUE(ilogger->is_enabled(ci::log_level::info));
    EXPECT_TRUE(ilogger->is_enabled(ci::log_level::warning));
    EXPECT_TRUE(ilogger->is_enabled(ci::log_level::error));
    EXPECT_TRUE(ilogger->is_enabled(ci::log_level::critical));
}

/**
 * @brief Test set_level and get_level methods
 */
TEST_F(ILoggerInterfaceTest, SetAndGetLevel) {
    ci::ILogger* ilogger = logger_.get();

    // Set level to warning
    auto result = ilogger->set_level(ci::log_level::warning);
    EXPECT_TRUE(result.is_ok());

    // Verify level was set
    EXPECT_EQ(ilogger->get_level(), ci::log_level::warning);

    // Set level to debug
    result = ilogger->set_level(ci::log_level::debug);
    EXPECT_TRUE(result.is_ok());

    EXPECT_EQ(ilogger->get_level(), ci::log_level::debug);
}

/**
 * @brief Test flush method returns VoidResult
 */
TEST_F(ILoggerInterfaceTest, FlushReturnsVoidResult) {
    ci::ILogger* ilogger = logger_.get();

    // Log some messages
    ilogger->log(ci::log_level::info, std::string("Message 1"));
    ilogger->log(ci::log_level::info, std::string("Message 2"));

    // Flush should succeed
    auto result = ilogger->flush();
    EXPECT_TRUE(result.is_ok());
}

/**
 * @brief Test level conversion between logger_system and common types
 */
TEST_F(ILoggerInterfaceTest, LevelConversionConsistency) {
    // Test using ILogger interface set_level/get_level
    logger_->set_level(ci::log_level::warning);
    EXPECT_EQ(logger_->get_level(), ci::log_level::warning);

    // Test using common log_level
    logger_->set_level(ci::log_level::debug);
    EXPECT_EQ(logger_->get_level(), ci::log_level::debug);
}

/**
 * @brief Test logging with common::interfaces::log_level
 */
TEST_F(ILoggerInterfaceTest, LoggingWithCommonLogLevel) {
    // Test logging with common::interfaces::log_level
    logger_->log(ci::log_level::info, std::string("Common log level message"));
    // Using simple message (source_location auto-captured internally)
    logger_->log(ci::log_level::warning, std::string("Common warning message"));

    EXPECT_TRUE(logger_->is_enabled(ci::log_level::info));
    EXPECT_TRUE(logger_->is_enabled(ci::log_level::error));
}

/**
 * @brief Test that logger can be used polymorphically
 */
TEST_F(ILoggerInterfaceTest, PolymorphicUsage) {
    // Function that accepts ILogger interface
    auto log_via_interface = [](ci::ILogger& logger, const std::string& msg) {
        return logger.log(ci::log_level::info, msg);
    };

    // Should work with our logger
    auto result = log_via_interface(*logger_, "Polymorphic message");
    EXPECT_TRUE(result.is_ok());
}

/**
 * @brief Test all log levels through ILogger interface
 */
TEST_F(ILoggerInterfaceTest, AllLogLevelsThroughInterface) {
    ci::ILogger* ilogger = logger_.get();

    // Set level to trace to allow all messages
    ilogger->set_level(ci::log_level::trace);

    // Test all levels
    EXPECT_TRUE(ilogger->log(ci::log_level::trace, std::string("Trace")).is_ok());
    EXPECT_TRUE(ilogger->log(ci::log_level::debug, std::string("Debug")).is_ok());
    EXPECT_TRUE(ilogger->log(ci::log_level::info, std::string("Info")).is_ok());
    EXPECT_TRUE(ilogger->log(ci::log_level::warning, std::string("Warning")).is_ok());
    EXPECT_TRUE(ilogger->log(ci::log_level::error, std::string("Error")).is_ok());
    EXPECT_TRUE(ilogger->log(ci::log_level::critical, std::string("Critical")).is_ok());
}

} // namespace kcenon::logger::test
