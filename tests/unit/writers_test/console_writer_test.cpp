// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file console_writer_test.cpp
 * @brief Unit tests for console_writer (console output, color support, stream selection)
 * @since 4.0.0
 *
 * Test coverage:
 * - Construction with default and custom parameters
 * - Write operations with stream capture
 * - Color support detection and configuration
 * - Stream selection (stdout vs stderr for error levels)
 * - Flush behavior
 * - Health check
 * - Thread safety
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/core/error_codes.h>

#include <sstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Helper: Create log_entry for testing
// =============================================================================

static log_entry make_test_entry(log_level level, const std::string& message) {
    log_entry entry;
    entry.level = level;
    entry.message = message;
    entry.timestamp = std::chrono::system_clock::now();
    return entry;
}

// =============================================================================
// Stream capture helper
// =============================================================================

class stream_capture {
public:
    explicit stream_capture(std::ostream& stream)
        : stream_(stream), old_buf_(stream.rdbuf(capture_.rdbuf())) {}

    ~stream_capture() { stream_.rdbuf(old_buf_); }

    std::string str() const { return capture_.str(); }

private:
    std::ostream& stream_;
    std::ostringstream capture_;
    std::streambuf* old_buf_;
};

// =============================================================================
// Construction Tests
// =============================================================================

TEST(ConsoleWriterTest, DefaultConstruction) {
    console_writer writer;
    EXPECT_EQ(writer.get_name(), "console");
    EXPECT_TRUE(writer.is_healthy());
}

TEST(ConsoleWriterTest, ConstructWithStderr) {
    console_writer writer(true);
    EXPECT_EQ(writer.get_name(), "console");
    EXPECT_TRUE(writer.is_healthy());
}

TEST(ConsoleWriterTest, ConstructWithColorDisabled) {
    console_writer writer(false, false);
    EXPECT_FALSE(writer.use_color());
}

// =============================================================================
// Write Tests
// =============================================================================

TEST(ConsoleWriterTest, WriteInfoMessageToStdout) {
    console_writer writer(false, false); // no stderr, no auto-detect color

    auto entry = make_test_entry(log_level::info, "test info message");

    stream_capture capture(std::cout);
    auto result = writer.write(entry);

    EXPECT_TRUE(result.is_ok());
    std::string output = capture.str();
    EXPECT_FALSE(output.empty());
}

TEST(ConsoleWriterTest, WriteErrorMessageToStderr) {
    console_writer writer(false, false);

    auto entry = make_test_entry(log_level::error, "test error message");

    stream_capture capture(std::cerr);
    auto result = writer.write(entry);

    EXPECT_TRUE(result.is_ok());
    std::string output = capture.str();
    // Error-level messages should go to stderr
    EXPECT_FALSE(output.empty());
}

TEST(ConsoleWriterTest, WriteFatalMessageToStderr) {
    console_writer writer(false, false);

    auto entry = make_test_entry(log_level::fatal, "test fatal message");

    stream_capture capture(std::cerr);
    auto result = writer.write(entry);

    EXPECT_TRUE(result.is_ok());
    std::string output = capture.str();
    EXPECT_FALSE(output.empty());
}

TEST(ConsoleWriterTest, WriteWithStderrFlagSendsAllToStderr) {
    console_writer writer(true, false); // use_stderr=true

    auto entry = make_test_entry(log_level::info, "info to stderr");

    stream_capture capture(std::cerr);
    auto result = writer.write(entry);

    EXPECT_TRUE(result.is_ok());
    std::string output = capture.str();
    EXPECT_FALSE(output.empty());
}

// =============================================================================
// Multiple Log Levels
// =============================================================================

TEST(ConsoleWriterTest, WriteMultipleLogLevels) {
    console_writer writer(false, false);

    std::vector<log_level> levels = {
        log_level::trace,
        log_level::debug,
        log_level::info,
        log_level::warning,
        log_level::error,
        log_level::fatal,
    };

    for (auto level : levels) {
        auto entry = make_test_entry(level, "test message");
        auto result = writer.write(entry);
        EXPECT_TRUE(result.is_ok()) << "Write failed for level " << static_cast<int>(level);
    }
}

// =============================================================================
// Color Configuration
// =============================================================================

TEST(ConsoleWriterTest, SetUseColor) {
    console_writer writer(false, false);

    writer.set_use_color(true);
    EXPECT_TRUE(writer.use_color());

    writer.set_use_color(false);
    EXPECT_FALSE(writer.use_color());
}

TEST(ConsoleWriterTest, ColorOutputContainsAnsiCodes) {
    console_writer writer(false, false);
    writer.set_use_color(true);

    auto entry = make_test_entry(log_level::error, "colored error");

    stream_capture capture(std::cerr);
    writer.write(entry);

    std::string output = capture.str();
    // When color is enabled, ANSI codes should be present
    if (!output.empty()) {
        // Check for ANSI escape sequence prefix
        EXPECT_NE(output.find("\033["), std::string::npos)
            << "Expected ANSI color codes in output: " << output;
    }
}

TEST(ConsoleWriterTest, NoColorOutputLacksAnsiCodes) {
    console_writer writer(false, false);
    writer.set_use_color(false);

    auto entry = make_test_entry(log_level::info, "plain message");

    stream_capture capture(std::cout);
    writer.write(entry);

    std::string output = capture.str();
    if (!output.empty()) {
        EXPECT_EQ(output.find("\033["), std::string::npos)
            << "Unexpected ANSI color codes in non-color output: " << output;
    }
}

// =============================================================================
// Flush Tests
// =============================================================================

TEST(ConsoleWriterTest, FlushSucceeds) {
    console_writer writer;
    auto result = writer.flush();
    EXPECT_TRUE(result.is_ok());
}

// =============================================================================
// Health Check
// =============================================================================

TEST(ConsoleWriterTest, IsHealthyByDefault) {
    console_writer writer;
    EXPECT_TRUE(writer.is_healthy());
}

// =============================================================================
// Thread Safety
// =============================================================================

TEST(ConsoleWriterTest, ConcurrentWrites) {
    console_writer writer(false, false);
    constexpr int num_threads = 4;
    constexpr int writes_per_thread = 50;

    stream_capture cout_capture(std::cout);
    stream_capture cerr_capture(std::cerr);

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&writer, t]() {
            for (int i = 0; i < writes_per_thread; ++i) {
                auto level = (i % 2 == 0) ? log_level::info : log_level::error;
                auto entry = make_test_entry(level, "thread " + std::to_string(t) + " msg " + std::to_string(i));
                auto result = writer.write(entry);
                EXPECT_TRUE(result.is_ok());
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // All writes should have produced some output
    std::string all_output = cout_capture.str() + cerr_capture.str();
    EXPECT_FALSE(all_output.empty());
}

// =============================================================================
// Stream Selection Tests
// =============================================================================

TEST(ConsoleWriterTest, SetUseStderrChangesOutput) {
    console_writer writer(false, false);

    // Initially stdout for info
    {
        stream_capture capture(std::cout);
        writer.write(make_test_entry(log_level::info, "to stdout"));
        EXPECT_FALSE(capture.str().empty());
    }

    // After set_use_stderr(true), all output goes to stderr
    writer.set_use_stderr(true);
    {
        stream_capture capture(std::cerr);
        writer.write(make_test_entry(log_level::info, "now to stderr"));
        EXPECT_FALSE(capture.str().empty());
    }
}
