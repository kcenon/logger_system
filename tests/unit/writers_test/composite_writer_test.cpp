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

/**
 * @file composite_writer_test.cpp
 * @brief Unit tests for composite_writer (Pipeline Pattern: formatter + sink)
 * @since 1.3.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/composite_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_formatter_interface.h>
#include <kcenon/logger/interfaces/output_sink_interface.h>

#include <memory>
#include <string>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Mock implementations
// =============================================================================

class mock_formatter : public log_formatter_interface {
public:
    explicit mock_formatter(std::string name = "mock_fmt")
        : name_(std::move(name)) {}

    std::string format(const log_entry& entry) const override {
        format_count_++;
        return "[" + std::string(entry.message.to_string()) + "]";
    }

    std::string get_name() const override { return name_; }

    mutable int format_count_ = 0;

private:
    std::string name_;
};

class mock_sink : public output_sink_interface {
public:
    explicit mock_sink(std::string name = "mock_sink")
        : name_(std::move(name)) {}

    common::VoidResult write_raw(std::string_view message) override {
        messages_.emplace_back(message);
        if (fail_writes_) {
            return common::VoidResult(common::error_info{1, "Mock write failure"});
        }
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    bool is_healthy() const override { return healthy_; }
    std::string get_name() const override { return name_; }

    void set_healthy(bool healthy) { healthy_ = healthy; }
    void set_fail_writes(bool fail) { fail_writes_ = fail; }

    const std::vector<std::string>& messages() const { return messages_; }
    int flush_count() const { return flush_count_; }

private:
    std::string name_;
    std::vector<std::string> messages_;
    int flush_count_ = 0;
    bool healthy_ = true;
    bool fail_writes_ = false;
};

// =============================================================================
// Test fixture
// =============================================================================

class CompositeWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto formatter = std::make_unique<mock_formatter>();
        auto sink = std::make_unique<mock_sink>();
        formatter_ptr_ = formatter.get();
        sink_ptr_ = sink.get();
        writer_ = std::make_unique<composite_writer>(
            std::move(formatter), std::move(sink));
    }

    std::unique_ptr<composite_writer> writer_;
    mock_formatter* formatter_ptr_ = nullptr;
    mock_sink* sink_ptr_ = nullptr;
};

// =============================================================================
// Construction tests
// =============================================================================

TEST_F(CompositeWriterTest, ConstructionSucceeds) {
    EXPECT_NE(writer_, nullptr);
}

TEST_F(CompositeWriterTest, NullFormatterThrows) {
    auto sink = std::make_unique<mock_sink>();
    EXPECT_THROW(
        composite_writer(nullptr, std::move(sink)),
        std::invalid_argument);
}

TEST_F(CompositeWriterTest, NullSinkThrows) {
    auto formatter = std::make_unique<mock_formatter>();
    EXPECT_THROW(
        composite_writer(std::move(formatter), nullptr),
        std::invalid_argument);
}

TEST_F(CompositeWriterTest, BothNullThrows) {
    EXPECT_THROW(
        composite_writer(nullptr, nullptr),
        std::invalid_argument);
}

// =============================================================================
// Pipeline write tests
// =============================================================================

TEST_F(CompositeWriterTest, WriteFormatsAndSinks) {
    log_entry entry(log_level::info, "Hello");
    auto result = writer_->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(formatter_ptr_->format_count_, 1);
    ASSERT_EQ(sink_ptr_->messages().size(), 1u);
    EXPECT_EQ(sink_ptr_->messages()[0], "[Hello]");
}

TEST_F(CompositeWriterTest, WriteMultipleEntries) {
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::debug, "msg" + std::to_string(i));
        writer_->write(entry);
    }

    EXPECT_EQ(formatter_ptr_->format_count_, 5);
    EXPECT_EQ(sink_ptr_->messages().size(), 5u);
}

TEST_F(CompositeWriterTest, WriteWithSourceLocation) {
    log_entry entry(log_level::error, "Error occurred",
                    "test.cpp", 42, "test_func");
    auto result = writer_->write(entry);

    EXPECT_TRUE(result.is_ok());
    ASSERT_EQ(sink_ptr_->messages().size(), 1u);
    EXPECT_EQ(sink_ptr_->messages()[0], "[Error occurred]");
}

TEST_F(CompositeWriterTest, WritePropagatesSinkError) {
    sink_ptr_->set_fail_writes(true);

    log_entry entry(log_level::info, "will fail");
    auto result = writer_->write(entry);

    EXPECT_TRUE(result.is_err());
}

// =============================================================================
// Flush tests
// =============================================================================

TEST_F(CompositeWriterTest, FlushDelegatesToSink) {
    writer_->flush();
    EXPECT_EQ(sink_ptr_->flush_count(), 1);

    writer_->flush();
    writer_->flush();
    EXPECT_EQ(sink_ptr_->flush_count(), 3);
}

// =============================================================================
// Name and health tests
// =============================================================================

TEST_F(CompositeWriterTest, GetNameCombinesFormatterAndSink) {
    EXPECT_EQ(writer_->get_name(), "mock_fmt+mock_sink");
}

TEST_F(CompositeWriterTest, GetNameWithCustomNames) {
    auto writer = std::make_unique<composite_writer>(
        std::make_unique<mock_formatter>("json"),
        std::make_unique<mock_sink>("file"));
    EXPECT_EQ(writer->get_name(), "json+file");
}

TEST_F(CompositeWriterTest, IsHealthyDelegatesToSink) {
    EXPECT_TRUE(writer_->is_healthy());

    sink_ptr_->set_healthy(false);
    EXPECT_FALSE(writer_->is_healthy());

    sink_ptr_->set_healthy(true);
    EXPECT_TRUE(writer_->is_healthy());
}

// =============================================================================
// Accessor tests
// =============================================================================

TEST_F(CompositeWriterTest, GetFormatterReturnsNonNull) {
    EXPECT_EQ(writer_->get_formatter(), formatter_ptr_);
}

TEST_F(CompositeWriterTest, GetSinkReturnsNonNull) {
    EXPECT_EQ(writer_->get_sink(), sink_ptr_);
}

// =============================================================================
// Factory function tests
// =============================================================================

TEST_F(CompositeWriterTest, MakeCompositeWriterCreatesWriter) {
    auto writer = make_composite_writer(
        std::make_unique<mock_formatter>("factory_fmt"),
        std::make_unique<mock_sink>("factory_sink"));

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "factory_fmt+factory_sink");
}

// =============================================================================
// Category tag tests
// =============================================================================

TEST_F(CompositeWriterTest, HasCompositeWriterTag) {
    auto* tag = dynamic_cast<composite_writer_tag*>(writer_.get());
    EXPECT_NE(tag, nullptr);
}

TEST_F(CompositeWriterTest, ImplementsLogWriterInterface) {
    auto* iface = dynamic_cast<log_writer_interface*>(writer_.get());
    EXPECT_NE(iface, nullptr);
}

// =============================================================================
// All log levels test
// =============================================================================

TEST_F(CompositeWriterTest, WriteAllLogLevels) {
    const log_level levels[] = {
        log_level::trace, log_level::debug, log_level::info,
        log_level::warning, log_level::error, log_level::critical
    };

    for (auto level : levels) {
        log_entry entry(level, "test");
        auto result = writer_->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    EXPECT_EQ(sink_ptr_->messages().size(), 6u);
}

// =============================================================================
// Empty message test
// =============================================================================

TEST_F(CompositeWriterTest, WriteEmptyMessage) {
    log_entry entry(log_level::info, "");
    auto result = writer_->write(entry);

    EXPECT_TRUE(result.is_ok());
    ASSERT_EQ(sink_ptr_->messages().size(), 1u);
    EXPECT_EQ(sink_ptr_->messages()[0], "[]");
}
