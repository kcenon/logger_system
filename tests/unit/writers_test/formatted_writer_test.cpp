/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <kcenon/logger/writers/formatted_writer.h>
#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <memory>
#include <string>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

/**
 * @brief Mock writer to track write operations
 */
class mock_writer : public log_writer_interface {
public:
    mock_writer() = default;
    ~mock_writer() override = default;

    common::VoidResult write(const log_entry& entry) override {
        entries_.push_back(entry.message.to_string());
        levels_.push_back(entry.level);
        write_count_++;
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    std::string get_name() const override { return "mock_writer"; }

    bool is_healthy() const override { return healthy_; }

    void set_healthy(bool healthy) { healthy_ = healthy; }

    int write_count() const { return write_count_; }
    int flush_count() const { return flush_count_; }
    const std::vector<std::string>& entries() const { return entries_; }
    const std::vector<log_level>& levels() const { return levels_; }

private:
    std::vector<std::string> entries_;
    std::vector<log_level> levels_;
    int write_count_ = 0;
    int flush_count_ = 0;
    bool healthy_ = true;
};

/**
 * @brief Simple mock formatter for testing
 */
class mock_formatter : public log_formatter_interface {
public:
    mock_formatter() = default;
    ~mock_formatter() override = default;

    std::string format(const log_entry& entry) const override {
        format_count_++;
        return "[MOCK] " + entry.message.to_string();
    }

    std::string get_name() const override { return "mock_formatter"; }

    mutable int format_count_ = 0;
};

/**
 * @brief Test fixture for formatted_writer tests
 */
class FormattedWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_ = std::make_unique<mock_writer>();
        mock_ptr_ = mock_.get();
    }

    std::unique_ptr<mock_writer> mock_;
    mock_writer* mock_ptr_ = nullptr;
};

/**
 * @test Verify construction with valid arguments
 */
TEST_F(FormattedWriterTest, ConstructorValid) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    EXPECT_NE(writer, nullptr);
    EXPECT_NE(writer->get_formatter(), nullptr);
    EXPECT_NE(writer->get_wrapped_writer(), nullptr);
}

/**
 * @test Verify construction with nullptr formatter passes all entries unchanged
 */
TEST_F(FormattedWriterTest, ConstructorNullFormatter) {
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), nullptr);

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_formatter(), nullptr);

    log_entry entry(log_level::debug, "test message");
    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
    EXPECT_EQ(mock_ptr_->entries()[0], "test message");
}

/**
 * @test Verify construction with nullptr writer throws
 */
TEST_F(FormattedWriterTest, ConstructorNullWriterThrows) {
    auto formatter = std::make_unique<mock_formatter>();

    EXPECT_THROW(
        formatted_writer(nullptr, std::move(formatter)),
        std::invalid_argument);
}

/**
 * @test Verify formatter is applied to log entries
 */
TEST_F(FormattedWriterTest, FormatterApplied) {
    auto formatter = std::make_unique<mock_formatter>();
    auto* formatter_ptr = formatter.get();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry(log_level::info, "original message");
    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(formatter_ptr->format_count_, 1);
    EXPECT_EQ(mock_ptr_->write_count(), 1);
    // The message should be formatted by the mock formatter
    EXPECT_EQ(mock_ptr_->entries()[0], "[MOCK] original message");
}

/**
 * @test Verify JSON formatter integration
 */
TEST_F(FormattedWriterTest, JsonFormatterIntegration) {
    auto formatter = std::make_unique<json_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry(log_level::error, "test error");
    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);

    // The formatted message should contain JSON structure
    const std::string& formatted = mock_ptr_->entries()[0];
    EXPECT_TRUE(formatted.find("{") != std::string::npos);
    EXPECT_TRUE(formatted.find("\"message\"") != std::string::npos);
    EXPECT_TRUE(formatted.find("test error") != std::string::npos);
    EXPECT_TRUE(formatted.find("\"level\"") != std::string::npos);
    EXPECT_TRUE(formatted.find("}") != std::string::npos);
}

/**
 * @test Verify timestamp formatter integration
 */
TEST_F(FormattedWriterTest, TimestampFormatterIntegration) {
    auto formatter = std::make_unique<timestamp_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry(log_level::warning, "warning message");
    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);

    // The formatted message should contain timestamp format elements
    const std::string& formatted = mock_ptr_->entries()[0];
    EXPECT_TRUE(formatted.find("[") != std::string::npos);
    EXPECT_TRUE(formatted.find("]") != std::string::npos);
    EXPECT_TRUE(formatted.find("WARNING") != std::string::npos);
    EXPECT_TRUE(formatted.find("warning message") != std::string::npos);
}

/**
 * @test Verify multiple writes with formatter
 */
TEST_F(FormattedWriterTest, MultipleWrites) {
    auto formatter = std::make_unique<mock_formatter>();
    auto* formatter_ptr = formatter.get();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry1(log_level::info, "message 1");
    log_entry entry2(log_level::warning, "message 2");
    log_entry entry3(log_level::error, "message 3");

    writer->write(entry1);
    writer->write(entry2);
    writer->write(entry3);

    EXPECT_EQ(formatter_ptr->format_count_, 3);
    EXPECT_EQ(mock_ptr_->write_count(), 3);
    EXPECT_EQ(mock_ptr_->entries()[0], "[MOCK] message 1");
    EXPECT_EQ(mock_ptr_->entries()[1], "[MOCK] message 2");
    EXPECT_EQ(mock_ptr_->entries()[2], "[MOCK] message 3");
}

/**
 * @test Verify log level is preserved after formatting
 */
TEST_F(FormattedWriterTest, LogLevelPreserved) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry(log_level::critical, "critical message");
    writer->write(entry);

    EXPECT_EQ(mock_ptr_->levels()[0], log_level::critical);
}

/**
 * @test Verify flush is delegated to wrapped writer
 */
TEST_F(FormattedWriterTest, FlushDelegates) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    auto result = writer->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

/**
 * @test Verify get_name returns appropriate format
 */
TEST_F(FormattedWriterTest, GetNameFormat) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    std::string name = writer->get_name();

    EXPECT_TRUE(name.find("formatted") != std::string::npos);
    EXPECT_TRUE(name.find("mock_writer") != std::string::npos);
    EXPECT_TRUE(name.find("mock_formatter") != std::string::npos);
}

/**
 * @test Verify get_name with nullptr formatter
 */
TEST_F(FormattedWriterTest, GetNameNullFormatter) {
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), nullptr);

    std::string name = writer->get_name();

    EXPECT_EQ(name, "formatted_mock_writer");
}

/**
 * @test Verify is_healthy delegates to wrapped writer
 */
TEST_F(FormattedWriterTest, IsHealthyDelegates) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    EXPECT_TRUE(writer->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer->is_healthy());

    mock_ptr_->set_healthy(true);
    EXPECT_TRUE(writer->is_healthy());
}

/**
 * @test Verify factory function works correctly
 */
TEST_F(FormattedWriterTest, FactoryFunction) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = make_formatted_writer(std::move(mock_), std::move(formatter));

    EXPECT_NE(writer, nullptr);
    EXPECT_NE(writer->get_formatter(), nullptr);
}

/**
 * @test Verify factory function with nullptr formatter
 */
TEST_F(FormattedWriterTest, FactoryFunctionNullFormatter) {
    auto writer = make_formatted_writer(std::move(mock_), nullptr);

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_formatter(), nullptr);
}

/**
 * @test Verify move semantics work correctly
 */
TEST_F(FormattedWriterTest, MoveSemantics) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer1 = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    // Move to another unique_ptr
    auto writer2 = std::move(writer1);

    EXPECT_NE(writer2, nullptr);
    EXPECT_EQ(writer1, nullptr);

    log_entry entry(log_level::info, "test");
    auto result = writer2->write(entry);
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
}

/**
 * @test Verify optional fields are preserved through formatting
 */
TEST_F(FormattedWriterTest, OptionalFieldsPreserved) {
    auto formatter = std::make_unique<mock_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry(log_level::info, "message", "test.cpp", 42, "test_function");
    entry.category = "test_category";
    entry.thread_id = "12345";

    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
}

/**
 * @test Verify JSON formatter with structured fields
 */
TEST_F(FormattedWriterTest, JsonFormatterWithFields) {
    auto formatter = std::make_unique<json_formatter>();
    auto writer = std::make_unique<formatted_writer>(std::move(mock_), std::move(formatter));

    log_entry entry(log_level::info, "structured log");
    entry.fields = log_fields{
        {"user_id", int64_t{12345}},
        {"action", std::string{"login"}},
        {"success", true}
    };

    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);

    const std::string& formatted = mock_ptr_->entries()[0];
    EXPECT_TRUE(formatted.find("user_id") != std::string::npos);
    EXPECT_TRUE(formatted.find("action") != std::string::npos);
    EXPECT_TRUE(formatted.find("success") != std::string::npos);
}

/**
 * @test Verify different formatters produce different outputs
 */
TEST_F(FormattedWriterTest, DifferentFormatterOutputs) {
    // First write with JSON formatter
    auto json_formatter_ptr = std::make_unique<json_formatter>();
    auto mock1 = std::make_unique<mock_writer>();
    auto* mock1_ptr = mock1.get();
    auto json_writer = std::make_unique<formatted_writer>(std::move(mock1), std::move(json_formatter_ptr));

    // Second write with timestamp formatter
    auto ts_formatter_ptr = std::make_unique<timestamp_formatter>();
    auto mock2 = std::make_unique<mock_writer>();
    auto* mock2_ptr = mock2.get();
    auto ts_writer = std::make_unique<formatted_writer>(std::move(mock2), std::move(ts_formatter_ptr));

    // Same entry
    log_entry entry1(log_level::info, "same message");
    log_entry entry2(log_level::info, "same message");

    json_writer->write(entry1);
    ts_writer->write(entry2);

    // Outputs should be different
    EXPECT_NE(mock1_ptr->entries()[0], mock2_ptr->entries()[0]);

    // JSON should have curly braces
    EXPECT_TRUE(mock1_ptr->entries()[0].find("{") != std::string::npos);

    // Timestamp should have square brackets but not curly braces
    EXPECT_TRUE(mock2_ptr->entries()[0].find("[") != std::string::npos);
    EXPECT_TRUE(mock2_ptr->entries()[0].find("{") == std::string::npos);
}
