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
 * @file buffered_writer_test.cpp
 * @brief Unit tests for buffered_writer decorator
 * @author kcenon
 * @since 4.0.0
 *
 * Part of the Decorator pattern refactoring (#356, #363).
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/buffered_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/writer_category.h>

#include <chrono>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

/**
 * @brief Mock writer to track operations for testing
 */
class mock_writer_for_buffer : public log_writer_interface {
public:
    mock_writer_for_buffer() = default;
    ~mock_writer_for_buffer() override = default;

    common::VoidResult write(const log_entry& entry) override {
        entries_.push_back(entry.message.to_string());
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
    void clear() {
        entries_.clear();
        write_count_ = 0;
        flush_count_ = 0;
    }

private:
    std::vector<std::string> entries_;
    int write_count_ = 0;
    int flush_count_ = 0;
    bool healthy_ = true;
};

/**
 * @brief Test fixture for buffered_writer tests
 */
class BufferedWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_ = std::make_unique<mock_writer_for_buffer>();
        mock_ptr_ = mock_.get();
    }

    std::unique_ptr<mock_writer_for_buffer> mock_;
    mock_writer_for_buffer* mock_ptr_ = nullptr;
};

/**
 * @test Verify construction with valid arguments
 */
TEST_F(BufferedWriterTest, ConstructorValid) {
    auto writer = std::make_unique<buffered_writer>(std::move(mock_), 10);

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_max_entries(), 10u);
    EXPECT_EQ(writer->get_buffer_count(), 0u);
}

/**
 * @test Verify construction with nullptr throws
 */
TEST_F(BufferedWriterTest, ConstructorNullWriterThrows) {
    EXPECT_THROW(
        buffered_writer(nullptr, 10),
        std::invalid_argument);
}

/**
 * @test Verify construction with zero buffer size throws
 */
TEST_F(BufferedWriterTest, ConstructorZeroBufferSizeThrows) {
    EXPECT_THROW(
        buffered_writer(std::move(mock_), 0),
        std::invalid_argument);
}

/**
 * @test Verify get_name returns correct format
 */
TEST_F(BufferedWriterTest, GetNameFormat) {
    auto writer = std::make_unique<buffered_writer>(std::move(mock_), 10);

    std::string name = writer->get_name();

    EXPECT_EQ(name, "buffered_mock_writer");
}

/**
 * @test Verify entries are buffered and not written immediately
 */
TEST_F(BufferedWriterTest, EntriesAreBuffered) {
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), 10, std::chrono::milliseconds{0});

    log_entry entry(log_level::info, "test message");
    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(writer->get_buffer_count(), 1u);
    EXPECT_EQ(mock_ptr_->write_count(), 0);  // Not written yet
}

/**
 * @test Verify buffer flushes when full
 */
TEST_F(BufferedWriterTest, BufferFlushesWhenFull) {
    const size_t buffer_size = 5;
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), buffer_size, std::chrono::milliseconds{0});

    // Write buffer_size entries
    for (size_t i = 0; i < buffer_size; ++i) {
        log_entry entry(log_level::info, "msg" + std::to_string(i));
        auto result = writer->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    // Buffer should have been flushed
    EXPECT_EQ(writer->get_buffer_count(), 0u);
    EXPECT_EQ(mock_ptr_->write_count(), static_cast<int>(buffer_size));
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

/**
 * @test Verify manual flush writes all buffered entries
 */
TEST_F(BufferedWriterTest, ManualFlush) {
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), 100, std::chrono::milliseconds{0});

    // Write some entries
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "msg" + std::to_string(i));
        writer->write(entry);
    }

    EXPECT_EQ(writer->get_buffer_count(), 5u);
    EXPECT_EQ(mock_ptr_->write_count(), 0);

    // Manual flush
    auto result = writer->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(writer->get_buffer_count(), 0u);
    EXPECT_EQ(mock_ptr_->write_count(), 5);
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

/**
 * @brief Mock writer that tracks writes in a shared counter
 */
class counting_mock_writer : public log_writer_interface {
public:
    explicit counting_mock_writer(std::shared_ptr<std::atomic<int>> counter)
        : counter_(std::move(counter)) {}

    common::VoidResult write(const log_entry&) override {
        counter_->fetch_add(1, std::memory_order_relaxed);
        return common::ok();
    }

    common::VoidResult flush() override { return common::ok(); }
    std::string get_name() const override { return "counting_mock"; }
    bool is_healthy() const override { return true; }

private:
    std::shared_ptr<std::atomic<int>> counter_;
};

/**
 * @test Verify destructor flushes remaining entries
 */
TEST_F(BufferedWriterTest, DestructorFlushes) {
    // Use a shared counter that survives the writer destruction
    auto write_counter = std::make_shared<std::atomic<int>>(0);

    {
        auto writer = std::make_unique<buffered_writer>(
            std::make_unique<counting_mock_writer>(write_counter),
            100, std::chrono::milliseconds{0});

        // Write some entries
        for (int i = 0; i < 3; ++i) {
            log_entry entry(log_level::info, "msg" + std::to_string(i));
            writer->write(entry);
        }

        // Entries should still be buffered
        EXPECT_EQ(write_counter->load(), 0);
    }  // writer destroyed here - should flush

    // Entries should have been flushed on destruction
    EXPECT_EQ(write_counter->load(), 3);
}

/**
 * @test Verify is_healthy delegates to wrapped writer
 */
TEST_F(BufferedWriterTest, IsHealthyDelegates) {
    auto writer = std::make_unique<buffered_writer>(std::move(mock_), 10);

    EXPECT_TRUE(writer->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer->is_healthy());
}

/**
 * @test Verify entry content is preserved through buffering
 */
TEST_F(BufferedWriterTest, EntryContentPreserved) {
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), 100, std::chrono::milliseconds{0});

    log_entry entry(log_level::info, "preserved message");
    writer->write(entry);
    writer->flush();

    ASSERT_EQ(mock_ptr_->entries().size(), 1u);
    EXPECT_EQ(mock_ptr_->entries()[0], "preserved message");
}

/**
 * @test Verify multiple entries preserve order
 */
TEST_F(BufferedWriterTest, EntryOrderPreserved) {
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), 100, std::chrono::milliseconds{0});

    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "msg" + std::to_string(i));
        writer->write(entry);
    }
    writer->flush();

    ASSERT_EQ(mock_ptr_->entries().size(), 5u);
    for (int i = 0; i < 5; ++i) {
        EXPECT_EQ(mock_ptr_->entries()[i], "msg" + std::to_string(i));
    }
}

/**
 * @test Verify statistics are tracked correctly
 */
TEST_F(BufferedWriterTest, StatisticsTracking) {
    const size_t buffer_size = 3;
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), buffer_size, std::chrono::milliseconds{0});

    // Write entries to trigger buffer full flush
    for (size_t i = 0; i < buffer_size; ++i) {
        log_entry entry(log_level::info, "msg");
        writer->write(entry);
    }

    const auto& stats = writer->get_stats();
    EXPECT_EQ(stats.total_entries_written.load(), buffer_size);
    EXPECT_GE(stats.flush_on_full.load(), 1u);

    // Manual flush (empty buffer)
    writer->flush();

    // Write more and manual flush
    log_entry entry(log_level::info, "another");
    writer->write(entry);
    writer->flush();

    EXPECT_EQ(stats.total_entries_written.load(), buffer_size + 1);
    EXPECT_GE(stats.manual_flushes.load(), 1u);
}

/**
 * @test Verify reset_stats clears all counters
 */
TEST_F(BufferedWriterTest, ResetStats) {
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), 10, std::chrono::milliseconds{0});

    // Generate some activity
    log_entry entry(log_level::info, "test");
    writer->write(entry);
    writer->flush();

    const auto& stats = writer->get_stats();
    EXPECT_GT(stats.total_entries_written.load(), 0u);

    // Reset
    writer->reset_stats();

    EXPECT_EQ(stats.total_entries_written.load(), 0u);
    EXPECT_EQ(stats.total_flushes.load(), 0u);
    EXPECT_EQ(stats.flush_on_full.load(), 0u);
    EXPECT_EQ(stats.manual_flushes.load(), 0u);
}

/**
 * @test Verify factory function creates valid writer
 */
TEST_F(BufferedWriterTest, FactoryFunction) {
    auto writer = make_buffered_writer(std::move(mock_), 50);

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_max_entries(), 50u);
}

/**
 * @test Verify factory function with nullptr throws
 */
TEST_F(BufferedWriterTest, FactoryFunctionNullThrows) {
    EXPECT_THROW(
        make_buffered_writer(nullptr),
        std::invalid_argument);
}

/**
 * @test Verify get_flush_interval returns correct value
 */
TEST_F(BufferedWriterTest, GetFlushInterval) {
    auto interval = std::chrono::milliseconds{3000};
    auto writer = std::make_unique<buffered_writer>(std::move(mock_), 10, interval);

    EXPECT_EQ(writer->get_flush_interval(), interval);
}

/**
 * @test Verify has decorator_writer_tag
 */
TEST_F(BufferedWriterTest, HasDecoratorWriterTag) {
    EXPECT_TRUE(is_decorator_writer_v<buffered_writer>);
}

/**
 * @test Verify time-based flush works
 */
TEST_F(BufferedWriterTest, TimeBasedFlush) {
    auto interval = std::chrono::milliseconds{50};
    auto writer = std::make_unique<buffered_writer>(std::move(mock_), 100, interval);

    // Write an entry
    log_entry entry1(log_level::info, "first");
    writer->write(entry1);

    EXPECT_EQ(mock_ptr_->write_count(), 0);
    EXPECT_EQ(writer->get_buffer_count(), 1u);

    // Wait for flush interval to pass
    std::this_thread::sleep_for(std::chrono::milliseconds{100});

    // Write another entry - should trigger time-based flush
    log_entry entry2(log_level::info, "second");
    writer->write(entry2);

    // Both entries should now be written (buffer flushed + new entry added then flushed)
    // or first entry written and second buffered depending on timing
    // At minimum, the first entry should have been flushed due to time
    EXPECT_GE(mock_ptr_->write_count(), 1);
}

/**
 * @test Verify decorator composition works
 */
TEST_F(BufferedWriterTest, DecoratorComposition) {
    // Create a mock, wrap with buffered_writer
    auto buffered = std::make_unique<buffered_writer>(
        std::move(mock_), 5, std::chrono::milliseconds{0});

    // Write entries
    for (int i = 0; i < 3; ++i) {
        log_entry entry(log_level::info, "composed" + std::to_string(i));
        auto result = buffered->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    EXPECT_EQ(buffered->get_buffer_count(), 3u);

    // Flush
    buffered->flush();

    EXPECT_EQ(mock_ptr_->write_count(), 3);
    ASSERT_EQ(mock_ptr_->entries().size(), 3u);
    EXPECT_EQ(mock_ptr_->entries()[0], "composed0");
}

/**
 * @test Verify empty flush is safe
 */
TEST_F(BufferedWriterTest, EmptyFlushIsSafe) {
    auto writer = std::make_unique<buffered_writer>(
        std::move(mock_), 10, std::chrono::milliseconds{0});

    // Flush empty buffer
    auto result = writer->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 0);
    // Wrapped writer's flush should still be called
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}
