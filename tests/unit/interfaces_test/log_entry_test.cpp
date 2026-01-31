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

/**
 * @file log_entry_test.cpp
 * @brief Unit tests for log_entry structure
 * @details Tests log_entry construction, serialization, and field handling
 * as required by Issue #392
 */

#include <gtest/gtest.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_writer_interface.h>
#include <chrono>
#include <thread>

using namespace kcenon::logger;

class LogEntryTest : public ::testing::Test {
protected:
    void SetUp() override {
        timestamp_ = std::chrono::system_clock::now();
    }

    std::chrono::system_clock::time_point timestamp_;
};

// Test log_entry construction with all log levels
TEST_F(LogEntryTest, AllLogLevelsRepresentation) {
    // Test TRACE level
    {
        log_entry entry(log_level::trace, "Trace message", timestamp_);
        EXPECT_EQ(entry.level, log_level::trace);
        EXPECT_EQ(std::string(entry.message), "Trace message");
        EXPECT_EQ(entry.timestamp, timestamp_);
    }

    // Test DEBUG level
    {
        log_entry entry(log_level::debug, "Debug message", timestamp_);
        EXPECT_EQ(entry.level, log_level::debug);
    }

    // Test INFO level
    {
        log_entry entry(log_level::info, "Info message", timestamp_);
        EXPECT_EQ(entry.level, log_level::info);
    }

    // Test WARN level
    {
        log_entry entry(log_level::warn, "Warn message", timestamp_);
        EXPECT_EQ(entry.level, log_level::warn);
    }

    // Test ERROR level
    {
        log_entry entry(log_level::error, "Error message", timestamp_);
        EXPECT_EQ(entry.level, log_level::error);
    }

    // Test FATAL level
    {
        log_entry entry(log_level::fatal, "Fatal message", timestamp_);
        EXPECT_EQ(entry.level, log_level::fatal);
    }
}

// Test log_entry construction with source location
TEST_F(LogEntryTest, ConstructionWithSourceLocation) {
    log_entry entry(
        log_level::info,
        "Test message",
        __FILE__,
        __LINE__,
        __FUNCTION__,
        timestamp_
    );

    EXPECT_EQ(entry.level, log_level::info);
    EXPECT_EQ(std::string(entry.message), "Test message");
    EXPECT_EQ(entry.timestamp, timestamp_);
    EXPECT_TRUE(entry.location.has_value());

    if (entry.location) {
        EXPECT_FALSE(std::string(entry.location->file).empty());
        EXPECT_GT(entry.location->line, 0);
        EXPECT_FALSE(std::string(entry.location->function).empty());
    }
}

// Test timestamp serialization and comparison
TEST_F(LogEntryTest, TimestampSerialization) {
    auto now = std::chrono::system_clock::now();
    auto later = now + std::chrono::seconds(5);

    log_entry entry1(log_level::info, "First message", now);
    log_entry entry2(log_level::info, "Second message", later);

    // Verify timestamps are preserved
    EXPECT_EQ(entry1.timestamp, now);
    EXPECT_EQ(entry2.timestamp, later);

    // Verify chronological ordering
    EXPECT_LT(entry1.timestamp, entry2.timestamp);

    // Test timestamp duration
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(
        entry2.timestamp - entry1.timestamp
    );
    EXPECT_EQ(duration.count(), 5);
}

// Test structured fields handling
TEST_F(LogEntryTest, StructuredFieldsHandling) {
    log_entry entry(log_level::info, "Structured message", timestamp_);

    // Initially fields should not be present
    EXPECT_FALSE(entry.fields.has_value());

    // Add structured fields
    log_fields fields;
    fields["user_id"] = std::string("12345");
    fields["count"] = static_cast<int64_t>(42);
    fields["ratio"] = 3.14;
    fields["enabled"] = true;

    entry.fields = fields;

    // Verify fields are present
    EXPECT_TRUE(entry.fields.has_value());
    EXPECT_EQ(entry.fields->size(), 4u);

    // Verify field values
    EXPECT_EQ(std::get<std::string>((*entry.fields)["user_id"]), "12345");
    EXPECT_EQ(std::get<int64_t>((*entry.fields)["count"]), 42);
    EXPECT_DOUBLE_EQ(std::get<double>((*entry.fields)["ratio"]), 3.14);
    EXPECT_TRUE(std::get<bool>((*entry.fields)["enabled"]));
}

// Test move semantics
TEST_F(LogEntryTest, MoveSemantics) {
    log_entry original(log_level::warn, "Original message", timestamp_);
    original.category = small_string_128("test_category");
    original.thread_id = small_string_64("thread_123");

    // Move construction
    log_entry moved(std::move(original));

    EXPECT_EQ(moved.level, log_level::warn);
    EXPECT_EQ(std::string(moved.message), "Original message");
    EXPECT_TRUE(moved.category.has_value());
    EXPECT_TRUE(moved.thread_id.has_value());

    if (moved.category) {
        EXPECT_EQ(std::string(*moved.category), "test_category");
    }
    if (moved.thread_id) {
        EXPECT_EQ(std::string(*moved.thread_id), "thread_123");
    }
}

// Test optional fields
TEST_F(LogEntryTest, OptionalFields) {
    log_entry entry(log_level::debug, "Test", timestamp_);

    // All optional fields should be empty initially
    EXPECT_FALSE(entry.location.has_value());
    EXPECT_FALSE(entry.thread_id.has_value());
    EXPECT_FALSE(entry.category.has_value());
    EXPECT_FALSE(entry.otel_ctx.has_value());
    EXPECT_FALSE(entry.fields.has_value());

    // Set optional fields
    entry.thread_id = small_string_64("thread_456");
    entry.category = small_string_128("network");

    EXPECT_TRUE(entry.thread_id.has_value());
    EXPECT_TRUE(entry.category.has_value());
    EXPECT_EQ(std::string(*entry.thread_id), "thread_456");
    EXPECT_EQ(std::string(*entry.category), "network");
}

// Test small_string optimization
TEST_F(LogEntryTest, SmallStringOptimization) {
    // Short message - should use SSO
    {
        std::string short_msg = "Short";
        log_entry entry(log_level::info, short_msg, timestamp_);
        EXPECT_EQ(std::string(entry.message), short_msg);
    }

    // Long message - may use heap allocation
    {
        std::string long_msg(512, 'x'); // Exceeds SSO threshold
        log_entry entry(log_level::info, long_msg, timestamp_);
        EXPECT_EQ(entry.message.size(), long_msg.size());
        EXPECT_EQ(std::string(entry.message), long_msg);
    }
}

// Test log_value variant types
TEST_F(LogEntryTest, LogValueVariantTypes) {
    log_value string_val = std::string("text");
    log_value int_val = static_cast<int64_t>(100);
    log_value double_val = 2.718;
    log_value bool_val = false;

    EXPECT_EQ(std::get<std::string>(string_val), "text");
    EXPECT_EQ(std::get<int64_t>(int_val), 100);
    EXPECT_DOUBLE_EQ(std::get<double>(double_val), 2.718);
    EXPECT_FALSE(std::get<bool>(bool_val));

    // Test variant index
    EXPECT_EQ(string_val.index(), 0u); // std::string
    EXPECT_EQ(int_val.index(), 1u);    // int64_t
    EXPECT_EQ(double_val.index(), 2u); // double
    EXPECT_EQ(bool_val.index(), 3u);   // bool
}

// Test source_location structure
TEST_F(LogEntryTest, SourceLocationStructure) {
    // Test construction from std::string
    {
        source_location loc("file.cpp", 42, "function_name");
        EXPECT_EQ(std::string(loc.file), "file.cpp");
        EXPECT_EQ(loc.line, 42);
        EXPECT_EQ(std::string(loc.function), "function_name");
    }

    // Test construction from C-strings
    {
        source_location loc("main.cpp", 100, "main");
        EXPECT_EQ(std::string(loc.file), "main.cpp");
        EXPECT_EQ(loc.line, 100);
        EXPECT_EQ(std::string(loc.function), "main");
    }

    // Test default construction
    {
        source_location loc("", 0, "");
        EXPECT_TRUE(std::string(loc.file).empty());
        EXPECT_EQ(loc.line, 0);
        EXPECT_TRUE(std::string(loc.function).empty());
    }
}

// Test log_writer_ptr type alias
TEST_F(LogEntryTest, WriterPtrTypeAlias) {
    // This test verifies the log_writer_ptr type alias exists and is usable
    // We cannot instantiate it directly since log_writer_interface is abstract
    using ptr_type = decltype(log_writer_ptr());
    EXPECT_TRUE((std::is_same_v<ptr_type, std::unique_ptr<log_writer_interface>>));
}
