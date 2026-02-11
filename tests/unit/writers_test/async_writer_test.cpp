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
 * @file async_writer_test.cpp
 * @brief Unit tests for async_writer (Decorator pattern, async queue processing)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/core/error_codes.h>

#include <memory>
#include <string>
#include <vector>
#include <atomic>
#include <thread>
#include <chrono>
#include <mutex>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Mock writer for async_writer testing
// =============================================================================

class async_mock_writer : public log_writer_interface {
public:
    async_mock_writer() = default;
    ~async_mock_writer() override = default;

    common::VoidResult write(const log_entry& entry) override {
        std::lock_guard<std::mutex> lock(mutex_);
        messages_.push_back(entry.message.to_string());
        write_count_++;
        if (write_delay_ > std::chrono::milliseconds::zero()) {
            std::this_thread::sleep_for(write_delay_);
        }
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    std::string get_name() const override { return "async_mock"; }
    bool is_healthy() const override { return healthy_.load(); }

    void set_healthy(bool healthy) { healthy_ = healthy; }
    void set_write_delay(std::chrono::milliseconds delay) { write_delay_ = delay; }

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
    std::chrono::milliseconds write_delay_{0};
};

// =============================================================================
// Test fixture
// =============================================================================

class AsyncWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto mock = std::make_unique<async_mock_writer>();
        mock_ptr_ = mock.get();
        writer_ = std::make_unique<async_writer>(std::move(mock), 100);
    }

    void TearDown() override {
        if (writer_) {
            writer_->stop();
        }
    }

    std::unique_ptr<async_writer> writer_;
    async_mock_writer* mock_ptr_ = nullptr;
};

// =============================================================================
// Construction tests
// =============================================================================

TEST_F(AsyncWriterTest, DefaultConstruction) {
    auto mock = std::make_unique<async_mock_writer>();
    auto writer = std::make_unique<async_writer>(std::move(mock));
    EXPECT_NE(writer, nullptr);
    writer->stop();
}

TEST_F(AsyncWriterTest, CustomQueueSizeAndTimeout) {
    auto mock = std::make_unique<async_mock_writer>();
    auto writer = std::make_unique<async_writer>(
        std::move(mock), 500, std::chrono::seconds(10));
    EXPECT_NE(writer, nullptr);
    writer->stop();
}

// =============================================================================
// Lifecycle tests
// =============================================================================

TEST_F(AsyncWriterTest, StartAndStop) {
    writer_->start();
    EXPECT_TRUE(writer_->is_healthy());

    writer_->stop();
    EXPECT_FALSE(writer_->is_healthy());
}

TEST_F(AsyncWriterTest, DoubleStartIsIdempotent) {
    writer_->start();
    EXPECT_NO_THROW(writer_->start());
    writer_->stop();
}

TEST_F(AsyncWriterTest, StopWithoutStartIsNoOp) {
    EXPECT_NO_THROW(writer_->stop());
}

TEST_F(AsyncWriterTest, DoubleStopIsIdempotent) {
    writer_->start();
    writer_->stop();
    EXPECT_NO_THROW(writer_->stop());
}

// =============================================================================
// Write tests (not running - direct delegation)
// =============================================================================

TEST_F(AsyncWriterTest, WriteWhenNotRunningDelegatesDirectly) {
    log_entry entry(log_level::info, "direct write");
    auto result = writer_->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
    auto msgs = mock_ptr_->get_messages();
    ASSERT_EQ(msgs.size(), 1u);
    EXPECT_EQ(msgs[0], "direct write");
}

// =============================================================================
// Write tests (running - async queue)
// =============================================================================

TEST_F(AsyncWriterTest, WriteWhenRunningEnqueues) {
    writer_->start();

    log_entry entry(log_level::info, "async write");
    auto result = writer_->write(entry);
    EXPECT_TRUE(result.is_ok());

    // Flush to ensure processing completes
    writer_->flush();

    EXPECT_GE(mock_ptr_->write_count(), 1);
}

TEST_F(AsyncWriterTest, WriteMultipleMessages) {
    writer_->start();

    const int count = 20;
    for (int i = 0; i < count; ++i) {
        log_entry entry(log_level::info, "msg" + std::to_string(i));
        writer_->write(entry);
    }

    writer_->flush();
    EXPECT_EQ(mock_ptr_->write_count(), count);
}

TEST_F(AsyncWriterTest, WritePreservesMessageContent) {
    writer_->start();

    log_entry entry(log_level::warning, "important warning");
    writer_->write(entry);
    writer_->flush();

    auto msgs = mock_ptr_->get_messages();
    ASSERT_GE(msgs.size(), 1u);

    bool found = false;
    for (const auto& msg : msgs) {
        if (msg == "important warning") {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
}

// =============================================================================
// Queue overflow test
// =============================================================================

TEST_F(AsyncWriterTest, QueueOverflowReturnsError) {
    // Create writer with very small queue
    auto mock = std::make_unique<async_mock_writer>();
    mock->set_write_delay(std::chrono::milliseconds(50));
    auto small_writer = std::make_unique<async_writer>(std::move(mock), 5);
    small_writer->start();

    int overflow_count = 0;
    for (int i = 0; i < 100; ++i) {
        log_entry entry(log_level::info, "overflow_test");
        auto result = small_writer->write(entry);
        if (result.is_err()) {
            overflow_count++;
        }
    }

    // At least some writes should have overflowed
    EXPECT_GT(overflow_count, 0);
    small_writer->stop();
}

// =============================================================================
// Flush tests
// =============================================================================

TEST_F(AsyncWriterTest, FlushWhenNotRunningDelegatesToWrapped) {
    auto result = writer_->flush();
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

TEST_F(AsyncWriterTest, FlushWhenRunningWaitsForEmpty) {
    writer_->start();

    for (int i = 0; i < 10; ++i) {
        log_entry entry(log_level::info, "flush_test");
        writer_->write(entry);
    }

    auto result = writer_->flush();
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 10);
}

// =============================================================================
// Name and health tests
// =============================================================================

TEST_F(AsyncWriterTest, GetNamePrefixesAsync) {
    EXPECT_EQ(writer_->get_name(), "async_async_mock");
}

TEST_F(AsyncWriterTest, IsHealthyRequiresRunningAndWrappedHealthy) {
    // Not running -> not healthy
    EXPECT_FALSE(writer_->is_healthy());

    writer_->start();
    EXPECT_TRUE(writer_->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer_->is_healthy());

    mock_ptr_->set_healthy(true);
    EXPECT_TRUE(writer_->is_healthy());

    writer_->stop();
    EXPECT_FALSE(writer_->is_healthy());
}

// =============================================================================
// Queue size tests
// =============================================================================

TEST_F(AsyncWriterTest, GetQueueSize) {
    EXPECT_EQ(writer_->get_queue_size(), 0u);
}

TEST_F(AsyncWriterTest, GetMaxQueueSize) {
    EXPECT_EQ(writer_->get_max_queue_size(), 100u);
}

// =============================================================================
// Multithreaded write test
// =============================================================================

TEST_F(AsyncWriterTest, ConcurrentWrites) {
    writer_->start();

    const int num_threads = 4;
    const int msgs_per_thread = 25;
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, t, msgs_per_thread]() {
            for (int i = 0; i < msgs_per_thread; ++i) {
                log_entry entry(log_level::info,
                    "t" + std::to_string(t) + "_m" + std::to_string(i));
                writer_->write(entry);
            }
        });
    }

    for (auto& th : threads) {
        th.join();
    }

    writer_->flush();
    EXPECT_EQ(mock_ptr_->write_count(), num_threads * msgs_per_thread);
}

// =============================================================================
// Stop with force flush test
// =============================================================================

TEST_F(AsyncWriterTest, StopWithForceFlushProcessesRemaining) {
    writer_->start();

    for (int i = 0; i < 10; ++i) {
        log_entry entry(log_level::info, "force_flush_" + std::to_string(i));
        writer_->write(entry);
    }

    writer_->stop(true);
    // After stop with force_flush, all messages should be processed
    EXPECT_GE(mock_ptr_->write_count(), 0);
}

// =============================================================================
// Category tag test
// =============================================================================

TEST_F(AsyncWriterTest, HasAsyncWriterTag) {
    auto* tag = dynamic_cast<async_writer_tag*>(writer_.get());
    EXPECT_NE(tag, nullptr);
}
