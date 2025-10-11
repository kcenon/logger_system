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

using namespace integration_tests;

/**
 * @brief Integration tests for logger lifecycle management
 *
 * These tests verify:
 * - Logger initialization and configuration
 * - Multiple logger instances
 * - Log level changes at runtime
 * - Sink management (file, console, custom sinks)
 * - Concurrent logging from multiple threads
 * - Logger shutdown and cleanup
 */
class LoggerLifecycleTest : public LoggerSystemFixture {};

TEST_F(LoggerLifecycleTest, CreateAndDestroyLogger) {
    CreateLogger(true);
    EXPECT_NE(logger_, nullptr);
    EXPECT_FALSE(logger_->is_running());
}

TEST_F(LoggerLifecycleTest, StartAndStopLogger) {
    CreateLogger(true);

    auto result = logger_->start();
    ASSERT_TRUE(result) << "Failed to start logger: " << result.error_message();
    EXPECT_TRUE(logger_->is_running());

    result = logger_->stop();
    ASSERT_TRUE(result) << "Failed to stop logger: " << result.error_message();
    EXPECT_FALSE(logger_->is_running());
}

TEST_F(LoggerLifecycleTest, LogMessagesAfterStart) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t message_count = 100;
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Test message " + std::to_string(i));
    }

    WaitForFlush();

    EXPECT_TRUE(WaitForLogLines(log_file, message_count));
    EXPECT_EQ(CountLogLines(log_file), message_count);
}

TEST_F(LoggerLifecycleTest, LogMessagesBeforeStart) {
    CreateLogger(true);

    auto log_file = GetTempFilePath("test.log");
    auto writer = std::make_unique<kcenon::logger::file_writer>(log_file);
    logger_->add_writer(std::move(writer));

    // Log before starting
    const size_t message_count = 50;
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(logger_system::log_level::info, "Test message " + std::to_string(i));
    }

    // Now start the logger
    auto result = logger_->start();
    ASSERT_TRUE(result);

    WaitForFlush();

    EXPECT_TRUE(WaitForLogLines(log_file, message_count));
    EXPECT_EQ(CountLogLines(log_file), message_count);
}

TEST_F(LoggerLifecycleTest, MultipleStartStopCycles) {
    auto log_file = CreateLoggerWithFileWriter(true);

    for (int cycle = 0; cycle < 3; ++cycle) {
        auto result = logger_->start();
        ASSERT_TRUE(result) << "Failed to start in cycle " << cycle;

        for (size_t i = 0; i < 20; ++i) {
            logger_->log(logger_system::log_level::info, "Cycle " + std::to_string(cycle) + " message " + std::to_string(i));
        }

        WaitForFlush();

        result = logger_->stop();
        ASSERT_TRUE(result) << "Failed to stop in cycle " << cycle;
    }

    EXPECT_EQ(CountLogLines(log_file), 60);
}

TEST_F(LoggerLifecycleTest, ChangeLogLevelAtRuntime) {
    auto log_file = CreateLoggerWithFileWriter(true);

    // Set minimum level to warning
    logger_->set_min_level(logger_system::log_level::warn);
    EXPECT_EQ(logger_->get_min_level(), logger_system::log_level::warn);

    // These should not be logged
    logger_->log(logger_system::log_level::debug, "Debug message");
    logger_->log(logger_system::log_level::info, "Info message");

    // These should be logged
    logger_->log(logger_system::log_level::warn, "Warning message");
    logger_->log(logger_system::log_level::error, "Error message");

    WaitForFlush();

    EXPECT_EQ(CountLogLines(log_file), 2);
    EXPECT_TRUE(LogContains(log_file, "Warning message"));
    EXPECT_TRUE(LogContains(log_file, "Error message"));
    EXPECT_FALSE(LogContains(log_file, "Debug message"));
    EXPECT_FALSE(LogContains(log_file, "Info message"));
}

TEST_F(LoggerLifecycleTest, AddMultipleWriters) {
    CreateLogger(true);
    logger_->start();

    auto file1 = GetTempFilePath("log1.log");
    auto file2 = GetTempFilePath("log2.log");

    auto writer1 = std::make_unique<kcenon::logger::file_writer>(file1);
    auto writer2 = std::make_unique<kcenon::logger::file_writer>(file2);

    logger_->add_writer(std::move(writer1));
    logger_->add_writer(std::move(writer2));

    const std::string test_message = "Test message to multiple files";
    logger_->log(logger_system::log_level::info, test_message);

    WaitForFlush();

    // Both files should contain the message
    EXPECT_TRUE(LogContains(file1, test_message));
    EXPECT_TRUE(LogContains(file2, test_message));
}

TEST_F(LoggerLifecycleTest, RemoveAllWriters) {
    auto log_file = CreateLoggerWithFileWriter(true);

    logger_->log(logger_system::log_level::info, "Before clear");
    WaitForFlush();

    auto result = logger_->clear_writers();
    ASSERT_TRUE(result);

    // This message should not be written anywhere
    logger_->log(logger_system::log_level::info, "After clear");
    WaitForFlush();

    EXPECT_EQ(CountLogLines(log_file), 1);
}

TEST_F(LoggerLifecycleTest, ConcurrentLoggingFromMultipleThreads) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t thread_count = 8;
    const size_t messages_per_thread = 50;
    std::vector<std::thread> threads;

    for (size_t t = 0; t < thread_count; ++t) {
        threads.emplace_back([this, t, messages_per_thread]() {
            for (size_t i = 0; i < messages_per_thread; ++i) {
                logger_->log(logger_system::log_level::info,
                           "Thread " + std::to_string(t) + " message " + std::to_string(i));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    WaitForFlush();

    EXPECT_TRUE(WaitForLogLines(log_file, thread_count * messages_per_thread));
    EXPECT_EQ(CountLogLines(log_file), thread_count * messages_per_thread);
}

TEST_F(LoggerLifecycleTest, SyncVsAsyncMode) {
    // Test synchronous mode
    CreateLogger(false);  // sync mode

    auto log_file = GetTempFilePath("sync.log");
    auto writer = std::make_unique<kcenon::logger::file_writer>(log_file);
    logger_->add_writer(std::move(writer));
    logger_->start();

    logger_->log(logger_system::log_level::info, "Sync message");
    logger_->flush();

    EXPECT_TRUE(WaitForFile(log_file));
    EXPECT_EQ(CountLogLines(log_file), 1);

    logger_->stop();
    logger_.reset();

    // Test asynchronous mode
    CreateLogger(true);  // async mode

    auto async_file = GetTempFilePath("async.log");
    auto async_writer = std::make_unique<kcenon::logger::file_writer>(async_file);
    logger_->add_writer(std::move(async_writer));
    logger_->start();

    logger_->log(logger_system::log_level::info, "Async message");
    WaitForFlush();

    EXPECT_TRUE(WaitForFile(async_file));
    EXPECT_EQ(CountLogLines(async_file), 1);
}

TEST_F(LoggerLifecycleTest, LoggerFlushBehavior) {
    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t batch_size = 100;
    for (size_t i = 0; i < batch_size; ++i) {
        logger_->log(logger_system::log_level::info, "Message " + std::to_string(i));
    }

    // Before flush, file might not have all messages
    logger_->flush();

    // After flush, all messages should be written
    EXPECT_TRUE(WaitForLogLines(log_file, batch_size));
    EXPECT_EQ(CountLogLines(log_file), batch_size);
}

TEST_F(LoggerLifecycleTest, LoggerResourceCleanup) {
    {
        auto logger = std::make_unique<kcenon::logger::logger>(true);
        auto log_file = GetTempFilePath("cleanup.log");
        auto writer = std::make_unique<kcenon::logger::file_writer>(log_file);

        logger->add_writer(std::move(writer));
        logger->start();

        for (size_t i = 0; i < 100; ++i) {
            logger->log(logger_system::log_level::info, "Cleanup test " + std::to_string(i));
        }

        // Logger destroyed when scope exits
    }

    // If we reach here without crashes, cleanup was successful
    SUCCEED();
}

TEST_F(LoggerLifecycleTest, IsEnabledCheck) {
    CreateLogger(true);
    logger_->start();

    logger_->set_min_level(logger_system::log_level::warn);

    EXPECT_FALSE(logger_->is_enabled(logger_system::log_level::trace));
    EXPECT_FALSE(logger_->is_enabled(logger_system::log_level::debug));
    EXPECT_FALSE(logger_->is_enabled(logger_system::log_level::info));
    EXPECT_TRUE(logger_->is_enabled(logger_system::log_level::warn));
    EXPECT_TRUE(logger_->is_enabled(logger_system::log_level::error));
    EXPECT_TRUE(logger_->is_enabled(logger_system::log_level::fatal));
}

TEST_F(LoggerLifecycleTest, LogWithSourceLocation) {
    auto log_file = CreateLoggerWithFileWriter(true);

    logger_->log(logger_system::log_level::error,
                "Error with location",
                __FILE__,
                __LINE__,
                __FUNCTION__);

    WaitForFlush();

    auto content = ReadLogFile(log_file);
    EXPECT_TRUE(content.find("Error with location") != std::string::npos);
    EXPECT_TRUE(content.find(__FUNCTION__) != std::string::npos);
}
