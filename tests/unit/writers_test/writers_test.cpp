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
   this software without specific property written permission.

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

#include <gtest/gtest.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <memory>
#include <chrono>
#include <thread>

using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;
namespace common = kcenon::common;
using log_level = ci::log_level;

class ConsoleWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        writer_ = std::make_unique<console_writer>();
        timestamp_ = std::chrono::system_clock::now();
    }

    std::unique_ptr<console_writer> writer_;
    std::chrono::system_clock::time_point timestamp_;
};

// Test basic console writer construction
TEST_F(ConsoleWriterTest, ConstructorTest) {
    EXPECT_NE(writer_, nullptr);
    
    // Test with custom parameters
    auto stderr_writer = std::make_unique<console_writer>(true, false);  // use stderr, no auto color
    EXPECT_NE(stderr_writer, nullptr);
    
    auto color_writer = std::make_unique<console_writer>(false, true);   // use stdout, auto color
    EXPECT_NE(color_writer, nullptr);
}

// Test basic write functionality
TEST_F(ConsoleWriterTest, BasicWrite) {
    log_entry entry(log_level::info, "Test message", timestamp_);
    EXPECT_NO_THROW(writer_->write(entry));

    EXPECT_NO_THROW(writer_->flush());
}

// Test write with source location
TEST_F(ConsoleWriterTest, WriteWithSourceLocation) {
    log_entry entry(log_level::error, "Error message with location", __FILE__, __LINE__, __func__, timestamp_);
    EXPECT_NO_THROW(writer_->write(entry));

    EXPECT_NO_THROW(writer_->flush());
}

// Test all log levels
TEST_F(ConsoleWriterTest, AllLogLevels) {
    EXPECT_NO_THROW(writer_->write(log_entry(log_level::trace, "Trace", timestamp_)));
    EXPECT_NO_THROW(writer_->write(log_entry(log_level::debug, "Debug", timestamp_)));
    EXPECT_NO_THROW(writer_->write(log_entry(log_level::info, "Info", timestamp_)));
    EXPECT_NO_THROW(writer_->write(log_entry(log_level::warning, "Warning", timestamp_)));
    EXPECT_NO_THROW(writer_->write(log_entry(log_level::error, "Error", timestamp_)));
    EXPECT_NO_THROW(writer_->write(log_entry(log_level::critical, "Critical", timestamp_)));

    writer_->flush();
}

// Test color functionality
TEST_F(ConsoleWriterTest, ColorFunctionality) {
    // Test enabling color
    writer_->set_use_color(true);
    EXPECT_TRUE(writer_->use_color());

    log_entry colored_entry(log_level::error, "Colored error message", timestamp_);
    EXPECT_NO_THROW(writer_->write(colored_entry));

    // Test disabling color
    writer_->set_use_color(false);
    EXPECT_FALSE(writer_->use_color());

    log_entry non_colored_entry(log_level::warning, "Non-colored warning message", timestamp_);
    EXPECT_NO_THROW(writer_->write(non_colored_entry));

    writer_->flush();
}

// Test stderr usage
TEST_F(ConsoleWriterTest, StderrUsage) {
    auto stderr_writer = std::make_unique<console_writer>(true);

    log_entry entry(log_level::critical, "Critical message to stderr", timestamp_);
    EXPECT_NO_THROW(stderr_writer->write(entry));

    stderr_writer->flush();
}

// Test empty and special messages
TEST_F(ConsoleWriterTest, SpecialMessages) {
    // Empty message
    log_entry empty_entry(log_level::info, "", timestamp_);
    EXPECT_NO_THROW(writer_->write(empty_entry));

    // Very long message
    std::string long_message(1000, 'X');
    log_entry long_entry(log_level::info, long_message, timestamp_);
    EXPECT_NO_THROW(writer_->write(long_entry));

    // Message with special characters
    log_entry special_entry(log_level::info, "Message with special chars: \\n\\t\\r\\0", timestamp_);
    EXPECT_NO_THROW(writer_->write(special_entry));

    writer_->flush();
}

// Test multithreaded access
TEST_F(ConsoleWriterTest, MultithreadedAccess) {
    std::vector<std::thread> threads;
    const int num_threads = 4;
    const int messages_per_thread = 10;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, t]() {
            for (int i = 0; i < messages_per_thread; ++i) {
                log_entry entry(
                    log_level::info,
                    "Thread " + std::to_string(t) + " Message " + std::to_string(i),
                    timestamp_
                );
                writer_->write(std::move(entry));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    writer_->flush();
}

// Test flush functionality
TEST_F(ConsoleWriterTest, FlushFunctionality) {
    // Write several messages
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "Message " + std::to_string(i), timestamp_);
        writer_->write(std::move(entry));
    }

    // Flush should complete without issues
    EXPECT_NO_THROW(writer_->flush());

    // Multiple flushes should be safe
    EXPECT_NO_THROW(writer_->flush());
    EXPECT_NO_THROW(writer_->flush());
}

// Mock writer for testing base_writer functionality
class MockWriter : public base_writer {
public:
    common::VoidResult write(const log_entry& entry) override {
        last_level_ = entry.level;
        last_message_ = std::string(entry.message.data());
        write_count_++;
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    std::string get_name() const override {
        return "mock";
    }

    std::string last_message_;
    log_level last_level_ = log_level::trace;
    int write_count_ = 0;
    int flush_count_ = 0;
};

class BaseWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_writer_ = std::make_unique<MockWriter>();
        timestamp_ = std::chrono::system_clock::now();
    }

    std::unique_ptr<MockWriter> mock_writer_;
    std::chrono::system_clock::time_point timestamp_;
};

// Test base writer formatting
TEST_F(BaseWriterTest, MessageFormatting) {
    log_entry entry(log_level::warning, "Test warning message", "/path/to/test.cpp", 42, "test_function", timestamp_);
    mock_writer_->write(entry);

    EXPECT_EQ(mock_writer_->write_count_, 1);
    EXPECT_EQ(mock_writer_->last_level_, log_level::warning);
    EXPECT_FALSE(mock_writer_->last_message_.empty());
    EXPECT_EQ(mock_writer_->last_message_, "Test warning message");
}

// Test color functionality in base writer
TEST_F(BaseWriterTest, ColorFunctionality) {
    // Test color enabled
    mock_writer_->set_use_color(true);
    EXPECT_TRUE(mock_writer_->use_color());
    
    // Test color disabled
    mock_writer_->set_use_color(false);
    EXPECT_FALSE(mock_writer_->use_color());
}

// Test flush count
TEST_F(BaseWriterTest, FlushCount) {
    EXPECT_EQ(mock_writer_->flush_count_, 0);
    
    mock_writer_->flush();
    EXPECT_EQ(mock_writer_->flush_count_, 1);
    
    mock_writer_->flush();
    mock_writer_->flush();
    EXPECT_EQ(mock_writer_->flush_count_, 3);
}