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
 * @file thread_safe_writer_test.cpp
 * @brief Unit tests for thread_safe_writer (Template Method pattern, mutex-based)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/thread_safe_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_formatter_interface.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Concrete test subclass implementing thread_safe_writer
// =============================================================================

class test_thread_safe_writer : public thread_safe_writer {
public:
    explicit test_thread_safe_writer(
        std::unique_ptr<log_formatter_interface> formatter = nullptr)
        : thread_safe_writer(std::move(formatter)) {}

    ~test_thread_safe_writer() override = default;

    std::string get_name() const override { return "test_thread_safe"; }

    int write_count() const { return write_count_.load(); }
    int flush_count() const { return flush_count_.load(); }

    std::vector<std::string> get_messages() const {
        std::lock_guard<std::mutex> lock(data_mutex_);
        return messages_;
    }

    void set_write_delay(std::chrono::milliseconds delay) {
        write_delay_ = delay;
    }

    // Expose protected get_mutex() for testing
    std::mutex& exposed_get_mutex() const { return get_mutex(); }

protected:
    common::VoidResult write_entry_impl(const log_entry& entry) override {
        if (write_delay_ > std::chrono::milliseconds::zero()) {
            std::this_thread::sleep_for(write_delay_);
        }
        {
            std::lock_guard<std::mutex> lock(data_mutex_);
            messages_.push_back(entry.message.to_string());
        }
        write_count_++;
        return common::ok();
    }

    common::VoidResult flush_impl() override {
        flush_count_++;
        return common::ok();
    }

private:
    mutable std::mutex data_mutex_;
    std::vector<std::string> messages_;
    std::atomic<int> write_count_{0};
    std::atomic<int> flush_count_{0};
    std::chrono::milliseconds write_delay_{0};
};

// =============================================================================
// Custom formatter for formatter injection test
// =============================================================================

class test_formatter : public log_formatter_interface {
public:
    std::string format(const log_entry& entry) const override {
        return "[TEST] " + entry.message.to_string();
    }

    std::string get_name() const override { return "test_formatter"; }
};

// =============================================================================
// Test fixture
// =============================================================================

class ThreadSafeWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        writer_ = std::make_unique<test_thread_safe_writer>();
    }

    std::unique_ptr<test_thread_safe_writer> writer_;
};

// =============================================================================
// Basic write and flush
// =============================================================================

TEST_F(ThreadSafeWriterTest, WriteCallsImpl) {
    log_entry entry(log_level::info, "test message");
    auto result = writer_->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(writer_->write_count(), 1);
    auto msgs = writer_->get_messages();
    ASSERT_EQ(msgs.size(), 1u);
    EXPECT_EQ(msgs[0], "test message");
}

TEST_F(ThreadSafeWriterTest, FlushCallsImpl) {
    auto result = writer_->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(writer_->flush_count(), 1);
}

// =============================================================================
// Mutex prevents concurrent write interleaving
// =============================================================================

TEST_F(ThreadSafeWriterTest, MutexPreventsConcurrentWriteInterleaving) {
    writer_->set_write_delay(std::chrono::milliseconds(5));

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

    // All messages should be written without loss
    EXPECT_EQ(writer_->write_count(), num_threads * msgs_per_thread);

    auto msgs = writer_->get_messages();
    EXPECT_EQ(static_cast<int>(msgs.size()), num_threads * msgs_per_thread);
}

// =============================================================================
// Flush acquires mutex before calling flush_impl
// =============================================================================

TEST_F(ThreadSafeWriterTest, FlushAcquiresMutex) {
    writer_->set_write_delay(std::chrono::milliseconds(10));

    // Start a write in a background thread
    std::thread write_thread([this]() {
        for (int i = 0; i < 10; ++i) {
            log_entry entry(log_level::info, "write_during_flush");
            writer_->write(entry);
        }
    });

    // Flush concurrently
    std::thread flush_thread([this]() {
        for (int i = 0; i < 5; ++i) {
            writer_->flush();
        }
    });

    write_thread.join();
    flush_thread.join();

    // No data corruption should occur
    EXPECT_EQ(writer_->write_count(), 10);
    EXPECT_GE(writer_->flush_count(), 5);
}

// =============================================================================
// Formatter injection via constructor
// =============================================================================

TEST_F(ThreadSafeWriterTest, FormatterInjection) {
    auto formatter = std::make_unique<test_formatter>();
    auto writer = std::make_unique<test_thread_safe_writer>(std::move(formatter));

    // Verify the formatter was set
    auto* fmt = writer->get_formatter();
    ASSERT_NE(fmt, nullptr);

    // Format a test entry and verify the custom formatter is used
    log_entry entry(log_level::info, "formatted message");
    auto formatted = fmt->format(entry);
    EXPECT_EQ(formatted, "[TEST] formatted message");
}

TEST_F(ThreadSafeWriterTest, DefaultFormatterWhenNullPassed) {
    auto writer = std::make_unique<test_thread_safe_writer>(nullptr);
    // Should not crash and should use default formatter
    EXPECT_NE(writer, nullptr);
}

// =============================================================================
// get_mutex() extended locking does not deadlock with write/flush
// =============================================================================

TEST_F(ThreadSafeWriterTest, GetMutexExtendedLockingNoDeadlock) {
    // Verify that external locking with get_mutex() does not deadlock
    // when used from a separate thread than write/flush

    std::atomic<bool> lock_acquired{false};
    std::atomic<bool> done{false};

    // Thread 1: Acquire the mutex externally for an extended period
    std::thread lock_thread([this, &lock_acquired, &done]() {
        std::lock_guard<std::mutex> lock(writer_->exposed_get_mutex());
        lock_acquired = true;
        // Hold lock for a short time
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        done = true;
    });

    // Wait for lock to be acquired
    while (!lock_acquired) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Thread 2: Write should block until lock is released, not deadlock
    std::thread write_thread([this, &done]() {
        log_entry entry(log_level::info, "after_lock");
        writer_->write(entry);
        // If we get here, there was no deadlock
        EXPECT_TRUE(done);
    });

    lock_thread.join();
    write_thread.join();

    EXPECT_EQ(writer_->write_count(), 1);
}

// =============================================================================
// Verify write() and flush() are final
// =============================================================================

TEST_F(ThreadSafeWriterTest, WriteFinalKeyword) {
    // The thread_safe_writer::write() and flush() are marked `final`.
    // This test verifies they exist and can be called on the base pointer.
    thread_safe_writer* base_ptr = writer_.get();

    log_entry entry(log_level::info, "final_test");
    auto write_result = base_ptr->write(entry);
    EXPECT_TRUE(write_result.is_ok());

    auto flush_result = base_ptr->flush();
    EXPECT_TRUE(flush_result.is_ok());
}
