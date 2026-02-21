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
 * @file batch_writer_test.cpp
 * @brief Unit tests for batch_writer (Decorator pattern, batched I/O)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/batch_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/core/error_codes.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Mock writer for batch_writer testing
// =============================================================================

class batch_mock_writer : public log_writer_interface {
public:
    batch_mock_writer() = default;
    ~batch_mock_writer() override = default;

    common::VoidResult write(const log_entry& entry) override {
        std::lock_guard<std::mutex> lock(mutex_);
        messages_.push_back(entry.message.to_string());
        write_count_++;
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    std::string get_name() const override { return "batch_mock"; }
    bool is_healthy() const override { return healthy_.load(); }

    void set_healthy(bool healthy) { healthy_ = healthy; }

    int write_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return write_count_;
    }

    int flush_count() const { return flush_count_; }

    std::vector<std::string> get_messages() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return messages_;
    }

private:
    mutable std::mutex mutex_;
    std::vector<std::string> messages_;
    int write_count_ = 0;
    std::atomic<int> flush_count_{0};
    std::atomic<bool> healthy_{true};
};

// =============================================================================
// Test fixture
// =============================================================================

class BatchWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto mock = std::make_unique<batch_mock_writer>();
        mock_ptr_ = mock.get();

        batch_writer::config cfg;
        cfg.max_batch_size = 5;
        cfg.flush_interval = std::chrono::milliseconds(1000);
        writer_ = std::make_unique<batch_writer>(std::move(mock), cfg);
    }

    std::unique_ptr<batch_writer> writer_;
    batch_mock_writer* mock_ptr_ = nullptr;
};

// =============================================================================
// Construction tests
// =============================================================================

TEST_F(BatchWriterTest, ConstructionWithDefaultConfig) {
    auto mock = std::make_unique<batch_mock_writer>();
    batch_writer::config cfg;
    auto writer = std::make_unique<batch_writer>(std::move(mock), cfg);
    EXPECT_NE(writer, nullptr);
    EXPECT_TRUE(writer->is_healthy());
}

TEST_F(BatchWriterTest, ConstructionWithCustomConfig) {
    auto mock = std::make_unique<batch_mock_writer>();
    batch_writer::config cfg;
    cfg.max_batch_size = 50;
    cfg.flush_interval = std::chrono::milliseconds(500);
    cfg.preserve_order = false;
    auto writer = std::make_unique<batch_writer>(std::move(mock), cfg);
    EXPECT_NE(writer, nullptr);
}

// =============================================================================
// Auto-flush at max_batch_size threshold
// =============================================================================

TEST_F(BatchWriterTest, AutoFlushAtMaxBatchSize) {
    // Write exactly max_batch_size entries (5)
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "msg" + std::to_string(i));
        auto result = writer_->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    // After reaching max_batch_size, entries should have been flushed
    EXPECT_EQ(mock_ptr_->write_count(), 5);
    EXPECT_EQ(writer_->get_current_batch_size(), 0u);
}

TEST_F(BatchWriterTest, NoFlushBelowThreshold) {
    // Write fewer entries than max_batch_size
    for (int i = 0; i < 3; ++i) {
        log_entry entry(log_level::info, "msg" + std::to_string(i));
        writer_->write(entry);
    }

    // Entries should still be in the batch
    EXPECT_EQ(mock_ptr_->write_count(), 0);
    EXPECT_EQ(writer_->get_current_batch_size(), 3u);
}

// =============================================================================
// Explicit flush
// =============================================================================

TEST_F(BatchWriterTest, ExplicitFlush) {
    for (int i = 0; i < 3; ++i) {
        log_entry entry(log_level::info, "flush_test_" + std::to_string(i));
        writer_->write(entry);
    }

    EXPECT_EQ(mock_ptr_->write_count(), 0);

    auto result = writer_->flush();
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 3);
    EXPECT_EQ(writer_->get_current_batch_size(), 0u);
}

TEST_F(BatchWriterTest, FlushEmptyBatch) {
    auto result = writer_->flush();
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 0);
}

// =============================================================================
// Batch size tracking
// =============================================================================

TEST_F(BatchWriterTest, GetCurrentBatchSize) {
    EXPECT_EQ(writer_->get_current_batch_size(), 0u);

    log_entry entry1(log_level::info, "first");
    writer_->write(entry1);
    EXPECT_EQ(writer_->get_current_batch_size(), 1u);

    log_entry entry2(log_level::info, "second");
    writer_->write(entry2);
    EXPECT_EQ(writer_->get_current_batch_size(), 2u);

    writer_->flush();
    EXPECT_EQ(writer_->get_current_batch_size(), 0u);
}

// =============================================================================
// Statistics tests
// =============================================================================

TEST_F(BatchWriterTest, GetStatsInitialValues) {
    const auto& stats = writer_->get_stats();
    EXPECT_EQ(stats.total_batches.load(), 0u);
    EXPECT_EQ(stats.total_entries.load(), 0u);
    EXPECT_EQ(stats.flush_on_size.load(), 0u);
    EXPECT_EQ(stats.flush_on_timeout.load(), 0u);
    EXPECT_DOUBLE_EQ(stats.average_batch_size(), 0.0);
}

TEST_F(BatchWriterTest, StatsTrackFlushOnSize) {
    // Write exactly max_batch_size entries to trigger size-based flush
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "stats_msg_" + std::to_string(i));
        writer_->write(entry);
    }

    const auto& stats = writer_->get_stats();
    EXPECT_EQ(stats.total_entries.load(), 5u);
    EXPECT_GE(stats.total_batches.load(), 1u);
    EXPECT_GE(stats.flush_on_size.load(), 1u);
}

TEST_F(BatchWriterTest, StatsTrackManualFlush) {
    log_entry entry(log_level::info, "manual_flush_test");
    writer_->write(entry);
    writer_->flush();

    const auto& stats = writer_->get_stats();
    EXPECT_GE(stats.manual_flushes.load(), 1u);
}

TEST_F(BatchWriterTest, StatsAverageBatchSize) {
    // Write and flush two batches of different sizes
    for (int i = 0; i < 2; ++i) {
        log_entry entry(log_level::info, "batch1_" + std::to_string(i));
        writer_->write(entry);
    }
    writer_->flush();

    for (int i = 0; i < 4; ++i) {
        log_entry entry(log_level::info, "batch2_" + std::to_string(i));
        writer_->write(entry);
    }
    writer_->flush();

    const auto& stats = writer_->get_stats();
    EXPECT_EQ(stats.total_entries.load(), 6u);
    EXPECT_EQ(stats.total_batches.load(), 2u);
    EXPECT_DOUBLE_EQ(stats.average_batch_size(), 3.0);
}

TEST_F(BatchWriterTest, ResetStats) {
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "reset_test");
        writer_->write(entry);
    }

    writer_->reset_stats();
    const auto& stats = writer_->get_stats();
    EXPECT_EQ(stats.total_batches.load(), 0u);
    EXPECT_EQ(stats.total_entries.load(), 0u);
    EXPECT_EQ(stats.flush_on_size.load(), 0u);
    EXPECT_EQ(stats.flush_on_timeout.load(), 0u);
    EXPECT_EQ(stats.manual_flushes.load(), 0u);
}

// =============================================================================
// Health delegation
// =============================================================================

TEST_F(BatchWriterTest, IsHealthyDelegatesToUnderlying) {
    EXPECT_TRUE(writer_->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer_->is_healthy());

    mock_ptr_->set_healthy(true);
    EXPECT_TRUE(writer_->is_healthy());
}

// =============================================================================
// Destructor flush
// =============================================================================

namespace {
    std::atomic<int> g_destructor_write_count{0};

    class destructor_counting_writer : public log_writer_interface {
    public:
        common::VoidResult write(const log_entry&) override {
            g_destructor_write_count++;
            return common::ok();
        }
        common::VoidResult flush() override { return common::ok(); }
        std::string get_name() const override { return "destructor_counting"; }
        bool is_healthy() const override { return true; }
    };
}

TEST_F(BatchWriterTest, DestructorFlushesRemainingEntries) {
    g_destructor_write_count = 0;

    batch_writer::config cfg;
    cfg.max_batch_size = 100;
    {
        auto writer = std::make_unique<batch_writer>(
            std::make_unique<destructor_counting_writer>(), cfg);
        for (int i = 0; i < 3; ++i) {
            log_entry entry(log_level::info, "destructor_test_" + std::to_string(i));
            writer->write(entry);
        }
        // Writer destroyed here - should flush remaining entries
    }

    // Counter survives mock destruction
    EXPECT_EQ(g_destructor_write_count.load(), 3);
}

// =============================================================================
// Name test
// =============================================================================

TEST_F(BatchWriterTest, GetNameIncludesWrappedName) {
    auto name = writer_->get_name();
    EXPECT_NE(name.find("batch"), std::string::npos);
    EXPECT_NE(name.find("batch_mock"), std::string::npos);
}

// =============================================================================
// Factory function
// =============================================================================

TEST_F(BatchWriterTest, MakeBatchWriterFactory) {
    auto mock = std::make_unique<batch_mock_writer>();
    auto writer = make_batch_writer(std::move(mock), 50, std::chrono::milliseconds(2000));
    EXPECT_NE(writer, nullptr);
    EXPECT_TRUE(writer->is_healthy());
}

TEST_F(BatchWriterTest, MakeBatchWriterDefaultParams) {
    auto mock = std::make_unique<batch_mock_writer>();
    auto writer = make_batch_writer(std::move(mock));
    EXPECT_NE(writer, nullptr);
}

// =============================================================================
// Message content preservation
// =============================================================================

TEST_F(BatchWriterTest, PreservesMessageContent) {
    log_entry entry1(log_level::info, "hello world");
    log_entry entry2(log_level::error, "error occurred");
    writer_->write(entry1);
    writer_->write(entry2);
    writer_->flush();

    auto messages = mock_ptr_->get_messages();
    ASSERT_EQ(messages.size(), 2u);
    EXPECT_EQ(messages[0], "hello world");
    EXPECT_EQ(messages[1], "error occurred");
}
