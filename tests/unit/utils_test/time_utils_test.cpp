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
 * @file time_utils_test.cpp
 * @brief Unit tests for time utility functions
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/utils/time_utils.h>

#include <chrono>
#include <regex>
#include <string>

using namespace kcenon::logger::utils;

// =============================================================================
// format_timestamp
// =============================================================================

TEST(TimeUtilsTest, FormatTimestampHasExpectedFormat) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_timestamp(now);

    // Pattern: YYYY-MM-DD HH:MM:SS.mmm
    std::regex pattern(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})");
    EXPECT_TRUE(std::regex_match(result, pattern))
        << "Timestamp format mismatch: " << result;
}

TEST(TimeUtilsTest, FormatTimestampNotEmpty) {
    auto now = std::chrono::system_clock::now();
    EXPECT_FALSE(time_utils::format_timestamp(now).empty());
}

TEST(TimeUtilsTest, FormatTimestampEpoch) {
    auto epoch = std::chrono::system_clock::time_point{};
    auto result = time_utils::format_timestamp(epoch);
    // Should produce a valid timestamp string even for epoch
    EXPECT_FALSE(result.empty());
}

// =============================================================================
// format_iso8601
// =============================================================================

TEST(TimeUtilsTest, FormatISO8601HasExpectedFormat) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_iso8601(now);

    // Pattern: YYYY-MM-DDTHH:MM:SS.mmmZ
    std::regex pattern(R"(\d{4}-\d{2}-\d{2}T\d{2}:\d{2}:\d{2}\.\d{3}Z)");
    EXPECT_TRUE(std::regex_match(result, pattern))
        << "ISO 8601 format mismatch: " << result;
}

TEST(TimeUtilsTest, FormatISO8601EndsWithZ) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_iso8601(now);
    EXPECT_EQ(result.back(), 'Z');
}

TEST(TimeUtilsTest, FormatISO8601ContainsT) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_iso8601(now);
    EXPECT_NE(result.find('T'), std::string::npos);
}

// =============================================================================
// format_compact
// =============================================================================

TEST(TimeUtilsTest, FormatCompactHasExpectedFormat) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_compact(now);

    // Pattern: YYYYMMDDHHMMSSmmm (17 digits)
    std::regex pattern(R"(\d{17})");
    EXPECT_TRUE(std::regex_match(result, pattern))
        << "Compact format mismatch: " << result;
}

TEST(TimeUtilsTest, FormatCompactLength) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_compact(now);
    EXPECT_EQ(result.length(), 17u);
}

// =============================================================================
// format_for_rotation
// =============================================================================

TEST(TimeUtilsTest, FormatForRotationDateOnly) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_for_rotation(now, false);

    // Pattern: YYYYMMDD (8 digits)
    std::regex pattern(R"(\d{8})");
    EXPECT_TRUE(std::regex_match(result, pattern))
        << "Rotation date format mismatch: " << result;
    EXPECT_EQ(result.length(), 8u);
}

TEST(TimeUtilsTest, FormatForRotationWithHour) {
    auto now = std::chrono::system_clock::now();
    auto result = time_utils::format_for_rotation(now, true);

    // Pattern: YYYYMMDD_HH
    std::regex pattern(R"(\d{8}_\d{2})");
    EXPECT_TRUE(std::regex_match(result, pattern))
        << "Rotation date+hour format mismatch: " << result;
    EXPECT_EQ(result.length(), 11u);
}

// =============================================================================
// now()
// =============================================================================

TEST(TimeUtilsTest, NowReturnsReasonableTime) {
    auto before = std::chrono::system_clock::now();
    auto result = time_utils::now();
    auto after = std::chrono::system_clock::now();

    EXPECT_GE(result, before);
    EXPECT_LE(result, after);
}

// =============================================================================
// Consistency checks
// =============================================================================

TEST(TimeUtilsTest, DifferentFormatsForSameTimepoint) {
    auto tp = std::chrono::system_clock::now();

    auto timestamp = time_utils::format_timestamp(tp);
    auto iso8601 = time_utils::format_iso8601(tp);
    auto compact = time_utils::format_compact(tp);

    // All should be non-empty
    EXPECT_FALSE(timestamp.empty());
    EXPECT_FALSE(iso8601.empty());
    EXPECT_FALSE(compact.empty());

    // They should have different formats
    EXPECT_NE(timestamp, iso8601);
    EXPECT_NE(iso8601, compact);
}
