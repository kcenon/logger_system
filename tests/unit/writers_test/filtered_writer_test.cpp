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

#include <kcenon/logger/writers/filtered_writer.h>
#include <kcenon/logger/filters/log_filter.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <memory>
#include <string>
#include <vector>

using namespace kcenon::logger;
using namespace kcenon::logger::filters;
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
 * @brief Test fixture for filtered_writer tests
 */
class FilteredWriterTest : public ::testing::Test {
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
TEST_F(FilteredWriterTest, ConstructorValid) {
    auto filter = std::make_unique<level_filter>(log_level::info);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    EXPECT_NE(writer, nullptr);
    EXPECT_NE(writer->get_filter(), nullptr);
    EXPECT_NE(writer->get_wrapped_writer(), nullptr);
}

/**
 * @test Verify construction with nullptr filter passes all entries
 */
TEST_F(FilteredWriterTest, ConstructorNullFilter) {
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), nullptr);

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_filter(), nullptr);

    log_entry entry(log_level::debug, "test message");
    auto result = writer->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
}

/**
 * @test Verify construction with nullptr writer throws
 */
TEST_F(FilteredWriterTest, ConstructorNullWriterThrows) {
    auto filter = std::make_unique<level_filter>(log_level::info);

    EXPECT_THROW(
        filtered_writer(nullptr, std::move(filter)),
        std::invalid_argument);
}

/**
 * @test Verify level filter passes entries at or above threshold
 */
TEST_F(FilteredWriterTest, LevelFilterPassesAboveThreshold) {
    auto filter = std::make_unique<level_filter>(log_level::warning);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    // Below threshold - should be filtered
    log_entry debug_entry(log_level::debug, "debug message");
    auto result1 = writer->write(debug_entry);
    EXPECT_TRUE(result1.is_ok());

    log_entry info_entry(log_level::info, "info message");
    auto result2 = writer->write(info_entry);
    EXPECT_TRUE(result2.is_ok());

    // At or above threshold - should pass
    log_entry warning_entry(log_level::warning, "warning message");
    auto result3 = writer->write(warning_entry);
    EXPECT_TRUE(result3.is_ok());

    log_entry error_entry(log_level::error, "error message");
    auto result4 = writer->write(error_entry);
    EXPECT_TRUE(result4.is_ok());

    // Verify only warning and error were written
    EXPECT_EQ(mock_ptr_->write_count(), 2);
    ASSERT_EQ(mock_ptr_->levels().size(), 2u);
    EXPECT_EQ(mock_ptr_->levels()[0], log_level::warning);
    EXPECT_EQ(mock_ptr_->levels()[1], log_level::error);
}

/**
 * @test Verify exact level filter only passes specified level
 */
TEST_F(FilteredWriterTest, ExactLevelFilterPassesOnlySpecifiedLevel) {
    auto filter = std::make_unique<exact_level_filter>(log_level::info);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    log_entry debug_entry(log_level::debug, "debug");
    writer->write(debug_entry);

    log_entry info_entry(log_level::info, "info");
    writer->write(info_entry);

    log_entry warning_entry(log_level::warning, "warning");
    writer->write(warning_entry);

    EXPECT_EQ(mock_ptr_->write_count(), 1);
    ASSERT_EQ(mock_ptr_->levels().size(), 1u);
    EXPECT_EQ(mock_ptr_->levels()[0], log_level::info);
}

/**
 * @test Verify composite AND filter requires all conditions
 */
TEST_F(FilteredWriterTest, CompositeAndFilterRequiresAll) {
    auto composite = std::make_unique<composite_filter>(composite_filter::logic_type::AND);
    composite->add_filter(std::make_unique<level_filter>(log_level::info));

    // Add a function filter that checks for "important" keyword
    composite->add_filter(std::make_unique<function_filter>([](const log_entry& e) {
        return e.message.to_string().find("important") != std::string::npos;
    }));

    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(composite));

    // Level OK but no "important" - filtered
    log_entry entry1(log_level::info, "regular message");
    writer->write(entry1);

    // Has "important" but level too low - filtered
    log_entry entry2(log_level::debug, "important debug");
    writer->write(entry2);

    // Both conditions met - passes
    log_entry entry3(log_level::info, "important info");
    writer->write(entry3);

    log_entry entry4(log_level::error, "important error");
    writer->write(entry4);

    EXPECT_EQ(mock_ptr_->write_count(), 2);
}

/**
 * @test Verify composite OR filter passes if any condition met
 */
TEST_F(FilteredWriterTest, CompositeOrFilterPassesAny) {
    auto composite = std::make_unique<composite_filter>(composite_filter::logic_type::OR);
    composite->add_filter(std::make_unique<exact_level_filter>(log_level::error));
    composite->add_filter(std::make_unique<function_filter>([](const log_entry& e) {
        return e.message.to_string().find("urgent") != std::string::npos;
    }));

    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(composite));

    // Neither condition - filtered
    log_entry entry1(log_level::info, "normal message");
    writer->write(entry1);

    // Error level - passes
    log_entry entry2(log_level::error, "error message");
    writer->write(entry2);

    // Has "urgent" - passes
    log_entry entry3(log_level::debug, "urgent debug");
    writer->write(entry3);

    // Both conditions - passes
    log_entry entry4(log_level::error, "urgent error");
    writer->write(entry4);

    EXPECT_EQ(mock_ptr_->write_count(), 3);
}

/**
 * @test Verify flush is delegated to wrapped writer
 */
TEST_F(FilteredWriterTest, FlushDelegates) {
    auto filter = std::make_unique<level_filter>(log_level::info);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    auto result = writer->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

/**
 * @test Verify get_name returns appropriate format
 */
TEST_F(FilteredWriterTest, GetNameFormat) {
    auto filter = std::make_unique<level_filter>(log_level::info);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    std::string name = writer->get_name();

    EXPECT_TRUE(name.find("filtered") != std::string::npos);
    EXPECT_TRUE(name.find("mock_writer") != std::string::npos);
    EXPECT_TRUE(name.find("level_filter") != std::string::npos);
}

/**
 * @test Verify get_name with nullptr filter
 */
TEST_F(FilteredWriterTest, GetNameNullFilter) {
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), nullptr);

    std::string name = writer->get_name();

    EXPECT_EQ(name, "filtered_mock_writer");
}

/**
 * @test Verify is_healthy delegates to wrapped writer
 */
TEST_F(FilteredWriterTest, IsHealthyDelegates) {
    auto filter = std::make_unique<level_filter>(log_level::info);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    EXPECT_TRUE(writer->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(writer->is_healthy());

    mock_ptr_->set_healthy(true);
    EXPECT_TRUE(writer->is_healthy());
}

/**
 * @test Verify filtered entries return success (not error)
 */
TEST_F(FilteredWriterTest, FilteredEntriesReturnSuccess) {
    auto filter = std::make_unique<level_filter>(log_level::error);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    // This entry should be filtered out
    log_entry entry(log_level::debug, "should be filtered");
    auto result = writer->write(entry);

    // Should return success even though filtered
    EXPECT_TRUE(result.is_ok());
    // But nothing was written
    EXPECT_EQ(mock_ptr_->write_count(), 0);
}

/**
 * @test Verify factory function works correctly
 */
TEST_F(FilteredWriterTest, FactoryFunction) {
    auto filter = std::make_unique<level_filter>(log_level::info);
    auto writer = make_filtered_writer(std::move(mock_), std::move(filter));

    EXPECT_NE(writer, nullptr);
    EXPECT_NE(writer->get_filter(), nullptr);
}

/**
 * @test Verify category filter works with filtered_writer
 */
TEST_F(FilteredWriterTest, CategoryFilter) {
    auto filter = std::make_unique<category_filter>(
        std::vector<std::string>{"network", "database"}, true);
    auto writer = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    // Entry without category - filtered
    log_entry entry1(log_level::info, "no category");
    writer->write(entry1);

    // Entry with non-matching category - filtered
    log_entry entry2(log_level::info, "ui message");
    entry2.category = "ui";
    writer->write(entry2);

    // Entry with matching category - passes
    log_entry entry3(log_level::info, "network message");
    entry3.category = "network";
    writer->write(entry3);

    log_entry entry4(log_level::info, "database message");
    entry4.category = "database";
    writer->write(entry4);

    EXPECT_EQ(mock_ptr_->write_count(), 2);
}

/**
 * @test Verify move semantics work correctly
 */
TEST_F(FilteredWriterTest, MoveSemantics) {
    auto filter = std::make_unique<level_filter>(log_level::info);
    auto writer1 = std::make_unique<filtered_writer>(std::move(mock_), std::move(filter));

    // Move to another unique_ptr
    auto writer2 = std::move(writer1);

    EXPECT_NE(writer2, nullptr);
    EXPECT_EQ(writer1, nullptr);

    log_entry entry(log_level::info, "test");
    auto result = writer2->write(entry);
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
}
