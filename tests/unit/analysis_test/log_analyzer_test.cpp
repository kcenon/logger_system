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
 * @file log_analyzer_test.cpp
 * @brief Unit tests for log_analyzer (stats caching, filtering, search, error rate)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/analysis/log_analyzer.h>

#include <chrono>
#include <string>
#include <vector>

using namespace kcenon::logger::analysis;

// =============================================================================
// Helper: create analyzed_log_entry with a given level, message, and timestamp
// =============================================================================

namespace {

analyzed_log_entry make_entry(
    log_level level,
    const std::string& message,
    std::chrono::system_clock::time_point ts = std::chrono::system_clock::now()) {
    return analyzed_log_entry{level, message, ts, "test.cpp", 1, "test_func"};
}

} // namespace

// =============================================================================
// Test fixture
// =============================================================================

class LogAnalyzerTest : public ::testing::Test {
protected:
    log_analyzer analyzer_;
};

// =============================================================================
// add_entry / add_entries
// =============================================================================

TEST_F(LogAnalyzerTest, AddSingleEntry) {
    analyzer_.add_entry(make_entry(log_level::info, "test message"));

    const auto& stats = analyzer_.get_stats();
    EXPECT_EQ(stats.total_entries, 1u);
}

TEST_F(LogAnalyzerTest, AddMultipleEntries) {
    std::vector<analyzed_log_entry> entries;
    entries.push_back(make_entry(log_level::info, "msg1"));
    entries.push_back(make_entry(log_level::warn, "msg2"));
    entries.push_back(make_entry(log_level::error, "msg3"));

    analyzer_.add_entries(entries);

    const auto& stats = analyzer_.get_stats();
    EXPECT_EQ(stats.total_entries, 3u);
}

TEST_F(LogAnalyzerTest, AddEntriesAccumulates) {
    analyzer_.add_entry(make_entry(log_level::info, "first"));

    std::vector<analyzed_log_entry> batch;
    batch.push_back(make_entry(log_level::debug, "second"));
    batch.push_back(make_entry(log_level::trace, "third"));
    analyzer_.add_entries(batch);

    EXPECT_EQ(analyzer_.get_stats().total_entries, 3u);
}

// =============================================================================
// clear
// =============================================================================

TEST_F(LogAnalyzerTest, ClearRemovesAllEntries) {
    analyzer_.add_entry(make_entry(log_level::info, "a"));
    analyzer_.add_entry(make_entry(log_level::error, "b"));
    EXPECT_EQ(analyzer_.get_stats().total_entries, 2u);

    analyzer_.clear();
    EXPECT_EQ(analyzer_.get_stats().total_entries, 0u);
}

// =============================================================================
// get_stats dirty flag caching
// =============================================================================

TEST_F(LogAnalyzerTest, StatsCachingDirtyFlagClearedAfterGetStats) {
    analyzer_.add_entry(make_entry(log_level::info, "entry1"));

    // First call computes stats (stats_dirty_ = true -> false)
    const auto& stats1 = analyzer_.get_stats();
    EXPECT_EQ(stats1.total_entries, 1u);

    // Second call should return cached result without recomputation
    const auto& stats2 = analyzer_.get_stats();
    EXPECT_EQ(stats2.total_entries, 1u);
    // Both should reference the same cached object
    EXPECT_EQ(&stats1, &stats2);
}

TEST_F(LogAnalyzerTest, StatsBecomesDirtyAfterAddEntry) {
    analyzer_.add_entry(make_entry(log_level::info, "first"));
    const auto& stats1 = analyzer_.get_stats();
    EXPECT_EQ(stats1.total_entries, 1u);

    // Adding another entry should invalidate the cache
    analyzer_.add_entry(make_entry(log_level::warn, "second"));
    const auto& stats2 = analyzer_.get_stats();
    EXPECT_EQ(stats2.total_entries, 2u);
}

TEST_F(LogAnalyzerTest, StatsBecomesDirtyAfterClear) {
    analyzer_.add_entry(make_entry(log_level::info, "entry"));
    EXPECT_EQ(analyzer_.get_stats().total_entries, 1u);

    analyzer_.clear();
    EXPECT_EQ(analyzer_.get_stats().total_entries, 0u);
}

// =============================================================================
// get_stats level counts
// =============================================================================

TEST_F(LogAnalyzerTest, StatsLevelCounts) {
    analyzer_.add_entry(make_entry(log_level::info, "i1"));
    analyzer_.add_entry(make_entry(log_level::info, "i2"));
    analyzer_.add_entry(make_entry(log_level::error, "e1"));
    analyzer_.add_entry(make_entry(log_level::warn, "w1"));

    const auto& stats = analyzer_.get_stats();
    EXPECT_EQ(stats.level_counts.at(log_level::info), 2u);
    EXPECT_EQ(stats.level_counts.at(log_level::error), 1u);
    EXPECT_EQ(stats.level_counts.at(log_level::warn), 1u);
}

// =============================================================================
// get_stats timestamps
// =============================================================================

TEST_F(LogAnalyzerTest, StatsTracksEarliestAndLatestTimestamps) {
    auto t1 = std::chrono::system_clock::now() - std::chrono::hours(2);
    auto t2 = std::chrono::system_clock::now() - std::chrono::hours(1);
    auto t3 = std::chrono::system_clock::now();

    analyzer_.add_entry(make_entry(log_level::info, "mid", t2));
    analyzer_.add_entry(make_entry(log_level::info, "earliest", t1));
    analyzer_.add_entry(make_entry(log_level::info, "latest", t3));

    const auto& stats = analyzer_.get_stats();
    EXPECT_EQ(stats.earliest_timestamp, t1);
    EXPECT_EQ(stats.latest_timestamp, t3);
}

TEST_F(LogAnalyzerTest, StatsEmptyEntriesHandledGracefully) {
    const auto& stats = analyzer_.get_stats();
    EXPECT_EQ(stats.total_entries, 0u);
    EXPECT_TRUE(stats.level_counts.empty());
}

// =============================================================================
// filter_by_level
// =============================================================================

TEST_F(LogAnalyzerTest, FilterByLevelReturnsMatchingEntries) {
    analyzer_.add_entry(make_entry(log_level::info, "info msg"));
    analyzer_.add_entry(make_entry(log_level::error, "error msg"));
    analyzer_.add_entry(make_entry(log_level::info, "another info"));

    auto filtered = analyzer_.filter_by_level(log_level::info);
    EXPECT_EQ(filtered.size(), 2u);
    EXPECT_EQ(filtered[0].message, "info msg");
    EXPECT_EQ(filtered[1].message, "another info");
}

TEST_F(LogAnalyzerTest, FilterByLevelReturnsEmptyWhenNoMatch) {
    analyzer_.add_entry(make_entry(log_level::info, "info only"));

    auto filtered = analyzer_.filter_by_level(log_level::fatal);
    EXPECT_TRUE(filtered.empty());
}

// =============================================================================
// filter_by_time_range
// =============================================================================

TEST_F(LogAnalyzerTest, FilterByTimeRangeReturnsEntriesInRange) {
    auto now = std::chrono::system_clock::now();
    auto t1 = now - std::chrono::hours(3);
    auto t2 = now - std::chrono::hours(2);
    auto t3 = now - std::chrono::hours(1);

    analyzer_.add_entry(make_entry(log_level::info, "old", t1));
    analyzer_.add_entry(make_entry(log_level::info, "mid", t2));
    analyzer_.add_entry(make_entry(log_level::info, "recent", t3));

    // Filter: between 2.5 hours ago and 0.5 hours ago
    auto start = now - std::chrono::minutes(150);
    auto end = now - std::chrono::minutes(30);

    auto filtered = analyzer_.filter_by_time_range(start, end);
    EXPECT_EQ(filtered.size(), 2u);
    EXPECT_EQ(filtered[0].message, "mid");
    EXPECT_EQ(filtered[1].message, "recent");
}

TEST_F(LogAnalyzerTest, FilterByTimeRangeEmptyResult) {
    auto now = std::chrono::system_clock::now();
    analyzer_.add_entry(make_entry(log_level::info, "entry", now));

    // Range entirely in the past
    auto start = now - std::chrono::hours(10);
    auto end = now - std::chrono::hours(5);

    auto filtered = analyzer_.filter_by_time_range(start, end);
    EXPECT_TRUE(filtered.empty());
}

// =============================================================================
// search_messages
// =============================================================================

TEST_F(LogAnalyzerTest, SearchMessagesFindsMatches) {
    analyzer_.add_entry(make_entry(log_level::info, "database connection established"));
    analyzer_.add_entry(make_entry(log_level::error, "database connection failed"));
    analyzer_.add_entry(make_entry(log_level::info, "user logged in"));

    auto results = analyzer_.search_messages("database");
    EXPECT_EQ(results.size(), 2u);
}

TEST_F(LogAnalyzerTest, SearchMessagesPartialMatch) {
    analyzer_.add_entry(make_entry(log_level::info, "processing request #123"));

    auto results = analyzer_.search_messages("request");
    EXPECT_EQ(results.size(), 1u);
    EXPECT_EQ(results[0].message, "processing request #123");
}

TEST_F(LogAnalyzerTest, SearchMessagesNoMatch) {
    analyzer_.add_entry(make_entry(log_level::info, "hello world"));

    auto results = analyzer_.search_messages("nonexistent");
    EXPECT_TRUE(results.empty());
}

// =============================================================================
// get_error_rate
// =============================================================================

TEST_F(LogAnalyzerTest, ErrorRateWithinWindow) {
    auto now = std::chrono::system_clock::now();

    // Add entries within the last 60 minutes
    analyzer_.add_entry(make_entry(log_level::info, "ok", now - std::chrono::minutes(10)));
    analyzer_.add_entry(make_entry(log_level::info, "ok", now - std::chrono::minutes(5)));
    analyzer_.add_entry(make_entry(log_level::error, "err", now - std::chrono::minutes(3)));
    analyzer_.add_entry(make_entry(log_level::fatal, "fatal", now - std::chrono::minutes(1)));

    // 2 error/fatal out of 4 entries = 0.5
    double rate = analyzer_.get_error_rate(std::chrono::minutes(60));
    EXPECT_DOUBLE_EQ(rate, 0.5);
}

TEST_F(LogAnalyzerTest, ErrorRateZeroWhenNoErrors) {
    auto now = std::chrono::system_clock::now();
    analyzer_.add_entry(make_entry(log_level::info, "ok", now));

    double rate = analyzer_.get_error_rate(std::chrono::minutes(60));
    EXPECT_DOUBLE_EQ(rate, 0.0);
}

TEST_F(LogAnalyzerTest, ErrorRateZeroWhenNoEntriesInWindow) {
    auto old = std::chrono::system_clock::now() - std::chrono::hours(24);
    analyzer_.add_entry(make_entry(log_level::error, "old error", old));

    // Window of 60 minutes should not include 24-hour-old entry
    double rate = analyzer_.get_error_rate(std::chrono::minutes(60));
    EXPECT_DOUBLE_EQ(rate, 0.0);
}

TEST_F(LogAnalyzerTest, ErrorRateEmptyAnalyzer) {
    double rate = analyzer_.get_error_rate(std::chrono::minutes(60));
    EXPECT_DOUBLE_EQ(rate, 0.0);
}

// =============================================================================
// generate_summary_report
// =============================================================================

TEST_F(LogAnalyzerTest, GenerateSummaryReportContainsExpectedSections) {
    analyzer_.add_entry(make_entry(log_level::info, "msg1"));
    analyzer_.add_entry(make_entry(log_level::error, "msg2"));

    auto report = analyzer_.generate_summary_report();

    EXPECT_NE(report.find("Log Analysis Summary"), std::string::npos);
    EXPECT_NE(report.find("Total Entries: 2"), std::string::npos);
    EXPECT_NE(report.find("Level Distribution"), std::string::npos);
}

TEST_F(LogAnalyzerTest, GenerateSummaryReportEmptyAnalyzer) {
    auto report = analyzer_.generate_summary_report();

    EXPECT_NE(report.find("Total Entries: 0"), std::string::npos);
}

// =============================================================================
// Factory
// =============================================================================

TEST_F(LogAnalyzerTest, FactoryCreateBasic) {
    auto analyzer = analyzer_factory::create_basic();

    ASSERT_NE(analyzer, nullptr);
    EXPECT_EQ(analyzer->get_stats().total_entries, 0u);
}

TEST_F(LogAnalyzerTest, FactoryCreatedAnalyzerIsFullyFunctional) {
    auto analyzer = analyzer_factory::create_basic();

    analyzer->add_entry(make_entry(log_level::warn, "test"));
    EXPECT_EQ(analyzer->get_stats().total_entries, 1u);
    EXPECT_EQ(analyzer->filter_by_level(log_level::warn).size(), 1u);
}
