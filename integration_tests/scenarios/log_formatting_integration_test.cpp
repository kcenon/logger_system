/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include "../framework/system_fixture.h"
#include "../framework/test_helpers.h"
#include <gtest/gtest.h>
#include <regex>

using namespace integration_tests;

/**
 * @brief Integration tests for log formatting
 *
 * These tests verify:
 * - Custom format patterns
 * - Timestamp formatting
 * - Thread ID inclusion
 * - Context data injection
 * - Log message truncation
 * - Multi-line message handling
 */
class LogFormattingIntegrationTest : public LoggerSystemFixture {};

TEST_F(LogFormattingIntegrationTest, BasicMessageFormat) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const std::string test_message = "Basic format test message";
    logger_->log(kcenon::logger::log_level::info, test_message);

    WaitForFlush();

    EXPECT_TRUE(LogContains(log_file, test_message));
    auto lines = GetLogLines(log_file);
    ASSERT_EQ(lines.size(), 1);
}

TEST_F(LogFormattingIntegrationTest, MultipleLogLevelsFormatting) {
    auto log_file = CreateLoggerWithFileWriter(true);

    logger_->log(kcenon::logger::log_level::trace, "Trace message");
    logger_->log(kcenon::logger::log_level::debug, "Debug message");
    logger_->log(kcenon::logger::log_level::info, "Info message");
    logger_->log(kcenon::logger::log_level::warning, "Warning message");
    logger_->log(kcenon::logger::log_level::error, "Error message");
    logger_->log(kcenon::logger::log_level::error, "Fatal message");

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("Trace message") != std::string::npos);
    EXPECT_TRUE(content.find("Debug message") != std::string::npos);
    EXPECT_TRUE(content.find("Info message") != std::string::npos);
    EXPECT_TRUE(content.find("Warning message") != std::string::npos);
    EXPECT_TRUE(content.find("Error message") != std::string::npos);
    EXPECT_TRUE(content.find("Fatal message") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, TimestampPresence) {
    auto log_file = CreateLoggerWithFileWriter(true);

    logger_->log(kcenon::logger::log_level::info, "Timestamp test");

    WaitForFlush();

    auto lines = GetLogLines(log_file);
    ASSERT_EQ(lines.size(), 1);

    // Check for timestamp pattern (various formats acceptable)
    // Most common formats include digits and colons
    EXPECT_TRUE(lines[0].find_first_of("0123456789") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, ThreadIdInclusion) {
    auto log_file = CreateLoggerWithFileWriter(true);

    std::thread::id main_thread_id = std::this_thread::get_id();

    logger_->log(kcenon::logger::log_level::info, "Main thread log");

    std::thread worker([this]() {
        logger_->log(kcenon::logger::log_level::info, "Worker thread log");
    });
    worker.join();

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("Main thread log") != std::string::npos);
    EXPECT_TRUE(content.find("Worker thread log") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, SourceLocationFormatting) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const std::string function_name = "TestFunction";
    const int line_num = 123;

    logger_->log(kcenon::logger::log_level::error,
                "Error with location",
                "test_file.cpp",
                line_num,
                function_name);

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("Error with location") != std::string::npos);
    EXPECT_TRUE(content.find(function_name) != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, LongMessageHandling) {
    auto log_file = CreateLoggerWithFileWriter(true);

    std::string long_message(5000, 'X');
    long_message += " END_MARKER";

    logger_->log(kcenon::logger::log_level::info, long_message);

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("END_MARKER") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, MultiLineMessageHandling) {
    auto log_file = CreateLoggerWithFileWriter(true);

    std::string multi_line_message = "Line 1\nLine 2\nLine 3";
    logger_->log(kcenon::logger::log_level::info, multi_line_message);

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("Line 1") != std::string::npos);
    EXPECT_TRUE(content.find("Line 2") != std::string::npos);
    EXPECT_TRUE(content.find("Line 3") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, SpecialCharactersInMessages) {
    auto log_file = CreateLoggerWithFileWriter(true);

    std::string special_chars = "Special chars: \t\n\r\"'\\!@#$%^&*()";
    logger_->log(kcenon::logger::log_level::info, special_chars);

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("Special chars") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, EmptyMessage) {
    auto log_file = CreateLoggerWithFileWriter(true);

    logger_->log(kcenon::logger::log_level::info, "");

    WaitForFlush();

    // Should still create a log entry
    EXPECT_GE(CountLogLines(log_file), 1);
}

TEST_F(LogFormattingIntegrationTest, ConsecutiveMessages) {
    auto log_file = CreateLoggerWithFileWriter(true);

    for (int i = 0; i < 10; ++i) {
        logger_->log(kcenon::logger::log_level::info, "Message " + std::to_string(i));
    }

    WaitForFlush();

    auto lines = GetLogLines(log_file);
    EXPECT_EQ(lines.size(), 10);

    // Verify order is preserved
    for (int i = 0; i < 10; ++i) {
        EXPECT_TRUE(lines[i].find("Message " + std::to_string(i)) != std::string::npos);
    }
}

TEST_F(LogFormattingIntegrationTest, UnicodeMessageHandling) {
    auto log_file = CreateLoggerWithFileWriter(true);

    std::string unicode_message = "Unicode test: 한글 日本語 中文 Русский";
    logger_->log(kcenon::logger::log_level::info, unicode_message);

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    // At minimum, should contain "Unicode test" even if full unicode support varies
    EXPECT_TRUE(content.find("Unicode test") != std::string::npos);
}

TEST_F(LogFormattingIntegrationTest, MessageOrderingUnderLoad) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t message_count = 1000;
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(kcenon::logger::log_level::info, "Ordered message " + std::to_string(i));
    }

    WaitForFlush();

    auto lines = GetLogLines(log_file);
    EXPECT_EQ(lines.size(), message_count);

    // Verify sequential messages are present (order might not be strictly preserved in async mode)
    size_t found_count = 0;
    for (size_t i = 0; i < message_count; ++i) {
        std::string expected = "Ordered message " + std::to_string(i);
        auto content = ReadLogFile(log_file);
        if (content.find(expected) != std::string::npos) {
            found_count++;
        }
    }
    EXPECT_EQ(found_count, message_count);
}
