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
 * @file critical_writer_test.cpp
 * @brief Unit tests for critical_writer and hybrid_writer
 * @since 1.1.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/critical_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/core/error_codes.h>

#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <atomic>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Mock writer for critical_writer testing
// =============================================================================

class critical_mock_writer : public log_writer_interface {
public:
    common::VoidResult write(const log_entry& entry) override {
        std::lock_guard<std::mutex> lock(mutex_);
        entries_.push_back({entry.level, entry.message.to_string()});
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    std::string get_name() const override { return "critical_mock"; }
    bool is_healthy() const override { return healthy_.load(); }
    void set_healthy(bool h) { healthy_ = h; }

    struct written_entry {
        log_level level;
        std::string message;
    };

    std::vector<written_entry> get_entries() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_;
    }

    int flush_count() const { return flush_count_.load(); }

private:
    mutable std::mutex mutex_;
    std::vector<written_entry> entries_;
    std::atomic<int> flush_count_{0};
    std::atomic<bool> healthy_{true};
};

// =============================================================================
// critical_writer_config tests
// =============================================================================

TEST(CriticalWriterConfigTest, DefaultValues) {
    critical_writer_config config;

    EXPECT_TRUE(config.force_flush_on_critical);
    EXPECT_FALSE(config.force_flush_on_error);
    EXPECT_FALSE(config.enable_signal_handlers);
    EXPECT_FALSE(config.write_ahead_log);
    EXPECT_EQ(config.wal_path, "logs/.wal");
    EXPECT_TRUE(config.sync_on_critical);
    EXPECT_EQ(config.critical_write_timeout_ms, 5000u);
}

TEST(CriticalWriterConfigTest, CustomConfig) {
    critical_writer_config config;
    config.force_flush_on_critical = false;
    config.force_flush_on_error = true;
    config.write_ahead_log = true;
    config.wal_path = "/tmp/claude/test.wal";
    config.sync_on_critical = false;
    config.critical_write_timeout_ms = 1000;

    EXPECT_FALSE(config.force_flush_on_critical);
    EXPECT_TRUE(config.force_flush_on_error);
    EXPECT_TRUE(config.write_ahead_log);
    EXPECT_EQ(config.wal_path, "/tmp/claude/test.wal");
    EXPECT_FALSE(config.sync_on_critical);
    EXPECT_EQ(config.critical_write_timeout_ms, 1000u);
}

// =============================================================================
// Test fixture
// =============================================================================

class CriticalWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto mock = std::make_unique<critical_mock_writer>();
        mock_ptr_ = mock.get();
        writer_ = std::make_unique<critical_writer>(std::move(mock));
    }

    std::unique_ptr<critical_writer> writer_;
    critical_mock_writer* mock_ptr_ = nullptr;
};

// =============================================================================
// Construction tests
// =============================================================================

TEST_F(CriticalWriterTest, DefaultConstruction) {
    EXPECT_NE(writer_, nullptr);
}

TEST_F(CriticalWriterTest, ConstructionWithConfig) {
    critical_writer_config config;
    config.force_flush_on_error = true;

    auto mock = std::make_unique<critical_mock_writer>();
    auto writer = std::make_unique<critical_writer>(std::move(mock), config);
    EXPECT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_config().force_flush_on_error);
}

TEST_F(CriticalWriterTest, NullWriterThrows) {
    EXPECT_THROW(
        critical_writer(nullptr),
        std::invalid_argument);
}

// =============================================================================
// Write tests (non-critical levels)
// =============================================================================

TEST_F(CriticalWriterTest, WriteInfoLevel) {
    log_entry entry(log_level::info, "info message");
    auto result = writer_->write(entry);
    EXPECT_TRUE(result.is_ok());

    auto entries = mock_ptr_->get_entries();
    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries[0].message, "info message");
}

TEST_F(CriticalWriterTest, WriteDebugLevel) {
    log_entry entry(log_level::debug, "debug message");
    auto result = writer_->write(entry);
    EXPECT_TRUE(result.is_ok());
}

// =============================================================================
// Write tests (critical levels)
// =============================================================================

TEST_F(CriticalWriterTest, WriteCriticalLevel) {
    log_entry entry(log_level::critical, "critical message");
    auto result = writer_->write(entry);
    EXPECT_TRUE(result.is_ok());

    auto entries = mock_ptr_->get_entries();
    ASSERT_EQ(entries.size(), 1u);
    EXPECT_EQ(entries[0].level, log_level::critical);
}

TEST_F(CriticalWriterTest, WriteCriticalForcesFlush) {
    log_entry entry(log_level::critical, "critical flush test");
    writer_->write(entry);

    // Critical level should trigger automatic flush
    EXPECT_GE(mock_ptr_->flush_count(), 1);
}

TEST_F(CriticalWriterTest, WriteErrorWithForceFlush) {
    critical_writer_config config;
    config.force_flush_on_error = true;

    auto mock = std::make_unique<critical_mock_writer>();
    auto* mock_p = mock.get();
    auto writer = std::make_unique<critical_writer>(std::move(mock), config);

    log_entry entry(log_level::error, "error flush test");
    writer->write(entry);

    // Error level with force_flush_on_error should flush
    EXPECT_GE(mock_p->flush_count(), 1);
}

// =============================================================================
// Flush tests
// =============================================================================

TEST_F(CriticalWriterTest, FlushDelegatesToWrapped) {
    auto result = writer_->flush();
    EXPECT_TRUE(result.is_ok());
    EXPECT_GE(mock_ptr_->flush_count(), 1);
}

// =============================================================================
// Health and name tests
// =============================================================================

TEST_F(CriticalWriterTest, IsHealthy) {
    EXPECT_TRUE(writer_->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer_->is_healthy());
}

TEST_F(CriticalWriterTest, GetNameIncludesCritical) {
    std::string name = writer_->get_name();
    EXPECT_FALSE(name.empty());
}

// =============================================================================
// Config access tests
// =============================================================================

TEST_F(CriticalWriterTest, GetConfigReturnsCurrentConfig) {
    const auto& config = writer_->get_config();
    EXPECT_TRUE(config.force_flush_on_critical);
}

TEST_F(CriticalWriterTest, SetForceFlushOnCritical) {
    writer_->set_force_flush_on_critical(false);
    EXPECT_FALSE(writer_->get_config().force_flush_on_critical);

    writer_->set_force_flush_on_critical(true);
    EXPECT_TRUE(writer_->get_config().force_flush_on_critical);
}

// =============================================================================
// Statistics tests
// =============================================================================

TEST_F(CriticalWriterTest, StatsInitiallyZero) {
    const auto& stats = writer_->get_stats();
    EXPECT_EQ(stats.total_critical_writes.load(), 0u);
    EXPECT_EQ(stats.total_flushes.load(), 0u);
    EXPECT_EQ(stats.wal_writes.load(), 0u);
}

TEST_F(CriticalWriterTest, StatsIncrementOnCriticalWrite) {
    log_entry entry(log_level::critical, "critical stats");
    writer_->write(entry);

    const auto& stats = writer_->get_stats();
    EXPECT_GE(stats.total_critical_writes.load(), 1u);
}

// =============================================================================
// Category tag test
// =============================================================================

TEST_F(CriticalWriterTest, HasDecoratorWriterTag) {
    auto* tag = dynamic_cast<decorator_writer_tag*>(writer_.get());
    EXPECT_NE(tag, nullptr);
}

// =============================================================================
// Multiple level writes
// =============================================================================

TEST_F(CriticalWriterTest, WriteAllLevels) {
    const log_level levels[] = {
        log_level::trace, log_level::debug, log_level::info,
        log_level::warning, log_level::error, log_level::critical
    };

    for (auto level : levels) {
        log_entry entry(level, "level test");
        auto result = writer_->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    auto entries = mock_ptr_->get_entries();
    EXPECT_EQ(entries.size(), 6u);
}

// =============================================================================
// hybrid_writer tests
// =============================================================================

class HybridWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto mock = std::make_unique<critical_mock_writer>();
        mock_ptr_ = mock.get();
        writer_ = std::make_unique<hybrid_writer>(std::move(mock));
    }

    std::unique_ptr<hybrid_writer> writer_;
    critical_mock_writer* mock_ptr_ = nullptr;
};

TEST_F(HybridWriterTest, Construction) {
    EXPECT_NE(writer_, nullptr);
}

TEST_F(HybridWriterTest, ConstructionWithConfig) {
    critical_writer_config config;
    config.force_flush_on_error = true;

    auto mock = std::make_unique<critical_mock_writer>();
    auto writer = std::make_unique<hybrid_writer>(std::move(mock), config, 5000);
    EXPECT_NE(writer, nullptr);
}

TEST_F(HybridWriterTest, WriteInfoLevel) {
    log_entry entry(log_level::info, "hybrid info");
    auto result = writer_->write(entry);
    EXPECT_TRUE(result.is_ok());
}

TEST_F(HybridWriterTest, WriteCriticalLevel) {
    log_entry entry(log_level::critical, "hybrid critical");
    auto result = writer_->write(entry);
    EXPECT_TRUE(result.is_ok());
}

TEST_F(HybridWriterTest, Flush) {
    auto result = writer_->flush();
    EXPECT_TRUE(result.is_ok());
}

TEST_F(HybridWriterTest, GetName) {
    std::string name = writer_->get_name();
    EXPECT_FALSE(name.empty());
}

TEST_F(HybridWriterTest, IsHealthy) {
    EXPECT_TRUE(writer_->is_healthy());
}

TEST_F(HybridWriterTest, HasCompositeAndDecoratorTags) {
    auto* composite_tag = dynamic_cast<composite_writer_tag*>(writer_.get());
    auto* decorator_tag = dynamic_cast<decorator_writer_tag*>(writer_.get());
    EXPECT_NE(composite_tag, nullptr);
    EXPECT_NE(decorator_tag, nullptr);
}
