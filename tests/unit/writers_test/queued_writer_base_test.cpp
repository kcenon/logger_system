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
 * @file queued_writer_base_test.cpp
 * @brief Unit tests for queued_writer_base (queue/vector container, overflow, copy)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/queued_writer_base.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/core/error_codes.h>

#include <memory>
#include <queue>
#include <string>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Mock writer for wrapped writer
// =============================================================================

class queued_mock_writer : public log_writer_interface {
public:
    common::VoidResult write(const log_entry& /*entry*/) override {
        return common::ok();
    }

    common::VoidResult flush() override {
        return common::ok();
    }

    std::string get_name() const override { return "queued_mock"; }

    bool is_healthy() const override { return healthy_; }

    void set_healthy(bool healthy) { healthy_ = healthy; }

private:
    bool healthy_ = true;
};

// =============================================================================
// Concrete test writer exposing protected members for testing
// =============================================================================

template <typename Container>
class test_queued_writer : public queued_writer_base<Container> {
public:
    explicit test_queued_writer(std::unique_ptr<log_writer_interface> wrapped,
                                std::size_t max_queue_size)
        : queued_writer_base<Container>(std::move(wrapped), max_queue_size, "test_queued") {
    }

    // Expose write as a delegation to try_enqueue
    common::VoidResult write(const log_entry& entry) override {
        return this->try_enqueue(entry);
    }

    // Expose shutting_down for testing
    void set_shutting_down(bool value) {
        this->shutting_down_ = value;
    }
};

// Type aliases for test writers
using queue_writer = test_queued_writer<std::queue<log_entry>>;
using vector_writer = test_queued_writer<std::vector<log_entry>>;

// =============================================================================
// Helper
// =============================================================================

namespace {
log_entry make_entry(const std::string& msg) {
    return log_entry(log_level::info, msg);
}
} // namespace

// =============================================================================
// copy_log_entry helper function
// =============================================================================

TEST(CopyLogEntryTest, CopiesBasicFields) {
    log_entry original(log_level::error, "test message");

    auto copied = copy_log_entry(original);

    EXPECT_EQ(copied.level, log_level::error);
    EXPECT_EQ(copied.message.to_string(), "test message");
}

TEST(CopyLogEntryTest, CopiesTimestamp) {
    auto ts = std::chrono::system_clock::now();
    log_entry original(log_level::warn, "msg", ts);

    auto copied = copy_log_entry(original);

    EXPECT_EQ(copied.timestamp, ts);
}

TEST(CopyLogEntryTest, CopiesLocationFields) {
    log_entry original(log_level::info, "msg", "file.cpp", 42, "my_func");

    auto copied = copy_log_entry(original);

    EXPECT_EQ(copied.level, log_level::info);
    EXPECT_EQ(copied.message.to_string(), "msg");
    ASSERT_TRUE(copied.location.has_value());
    EXPECT_EQ(copied.location->file.to_string(), "file.cpp");
    EXPECT_EQ(copied.location->line, 42u);
    EXPECT_EQ(copied.location->function.to_string(), "my_func");
}

TEST(CopyLogEntryTest, CopiesEntryWithoutLocation) {
    log_entry original(log_level::debug, "no location");

    auto copied = copy_log_entry(original);

    EXPECT_EQ(copied.message.to_string(), "no location");
    EXPECT_FALSE(copied.location.has_value());
}

// =============================================================================
// Queue container path (std::queue<log_entry>)
// =============================================================================

class QueueWriterTest : public ::testing::Test {
protected:
    std::unique_ptr<queue_writer> writer_;
    queued_mock_writer* mock_ptr_ = nullptr;

    void SetUp() override {
        auto mock = std::make_unique<queued_mock_writer>();
        mock_ptr_ = mock.get();
        writer_ = std::make_unique<queue_writer>(std::move(mock), 5);
    }
};

TEST_F(QueueWriterTest, InitialQueueIsEmpty) {
    EXPECT_EQ(writer_->get_queue_size(), 0u);
}

TEST_F(QueueWriterTest, GetMaxQueueSize) {
    EXPECT_EQ(writer_->get_max_queue_size(), 5u);
}

TEST_F(QueueWriterTest, EnqueueIncreasesSize) {
    auto result = writer_->write(make_entry("msg1"));
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(writer_->get_queue_size(), 1u);
}

TEST_F(QueueWriterTest, EnqueueMultipleEntries) {
    for (int i = 0; i < 3; ++i) {
        auto result = writer_->write(make_entry("msg " + std::to_string(i)));
        EXPECT_TRUE(result.is_ok());
    }
    EXPECT_EQ(writer_->get_queue_size(), 3u);
}

TEST_F(QueueWriterTest, OverflowReturnsQueueFullError) {
    // Fill the queue to max (5 entries)
    for (int i = 0; i < 5; ++i) {
        EXPECT_TRUE(writer_->write(make_entry("msg")).is_ok());
    }
    EXPECT_EQ(writer_->get_queue_size(), 5u);

    // 6th entry should fail with queue_full
    auto result = writer_->write(make_entry("overflow"));
    EXPECT_TRUE(result.is_err());
}

TEST_F(QueueWriterTest, GetNameIncludesDecoratorPrefix) {
    auto name = writer_->get_name();
    EXPECT_NE(name.find("test_queued"), std::string::npos);
}

TEST_F(QueueWriterTest, IsHealthyDelegatesToWrapped) {
    EXPECT_TRUE(writer_->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer_->is_healthy());
}

TEST_F(QueueWriterTest, ShuttingDownMakesUnhealthy) {
    EXPECT_TRUE(writer_->is_healthy());

    writer_->set_shutting_down(true);
    EXPECT_FALSE(writer_->is_healthy());
}

TEST_F(QueueWriterTest, HasAsyncWriterTag) {
    auto* tag = dynamic_cast<async_writer_tag*>(writer_.get());
    EXPECT_NE(tag, nullptr);
}

// =============================================================================
// Vector container path (std::vector<log_entry>)
// =============================================================================

class VectorWriterTest : public ::testing::Test {
protected:
    std::unique_ptr<vector_writer> writer_;
    queued_mock_writer* mock_ptr_ = nullptr;

    void SetUp() override {
        auto mock = std::make_unique<queued_mock_writer>();
        mock_ptr_ = mock.get();
        writer_ = std::make_unique<vector_writer>(std::move(mock), 3);
    }
};

TEST_F(VectorWriterTest, InitialQueueIsEmpty) {
    EXPECT_EQ(writer_->get_queue_size(), 0u);
}

TEST_F(VectorWriterTest, GetMaxQueueSize) {
    EXPECT_EQ(writer_->get_max_queue_size(), 3u);
}

TEST_F(VectorWriterTest, EnqueueIncreasesSize) {
    auto result = writer_->write(make_entry("vector msg"));
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(writer_->get_queue_size(), 1u);
}

TEST_F(VectorWriterTest, EnqueueMultipleEntries) {
    for (int i = 0; i < 3; ++i) {
        auto result = writer_->write(make_entry("msg " + std::to_string(i)));
        EXPECT_TRUE(result.is_ok());
    }
    EXPECT_EQ(writer_->get_queue_size(), 3u);
}

TEST_F(VectorWriterTest, OverflowReturnsQueueFullError) {
    // Fill to max (3 entries)
    for (int i = 0; i < 3; ++i) {
        EXPECT_TRUE(writer_->write(make_entry("msg")).is_ok());
    }

    // 4th entry should fail
    auto result = writer_->write(make_entry("overflow"));
    EXPECT_TRUE(result.is_err());
}

TEST_F(VectorWriterTest, IsHealthyDelegatesToWrapped) {
    EXPECT_TRUE(writer_->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer_->is_healthy());
}

TEST_F(VectorWriterTest, ShuttingDownMakesUnhealthy) {
    writer_->set_shutting_down(true);
    EXPECT_FALSE(writer_->is_healthy());
}

TEST_F(VectorWriterTest, HasAsyncWriterTag) {
    auto* tag = dynamic_cast<async_writer_tag*>(writer_.get());
    EXPECT_NE(tag, nullptr);
}

// =============================================================================
// Constructor validation
// =============================================================================

TEST(QueuedWriterBaseConstructorTest, ThrowsOnNullWrappedWriter) {
    EXPECT_THROW(
        queue_writer(nullptr, 10),
        std::invalid_argument
    );
}

TEST(QueuedWriterBaseConstructorTest, ZeroMaxQueueSizeAllowed) {
    auto mock = std::make_unique<queued_mock_writer>();
    queue_writer writer(std::move(mock), 0);

    // With max_queue_size = 0, every enqueue should overflow
    auto result = writer.write(make_entry("msg"));
    EXPECT_TRUE(result.is_err());
}
