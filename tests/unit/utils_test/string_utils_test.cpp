// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file string_utils_test.cpp
 * @brief Unit tests for string utility functions
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/utils/string_utils.h>

using namespace kcenon::logger::utils;
using log_level = kcenon::common::interfaces::log_level;

// =============================================================================
// level_to_string
// =============================================================================

TEST(StringUtilsTest, LevelToStringAllLevels) {
    EXPECT_EQ(string_utils::level_to_string(log_level::critical), "CRITICAL");
    EXPECT_EQ(string_utils::level_to_string(log_level::error), "ERROR");
    EXPECT_EQ(string_utils::level_to_string(log_level::warning), "WARNING");
    EXPECT_EQ(string_utils::level_to_string(log_level::info), "INFO");
    EXPECT_EQ(string_utils::level_to_string(log_level::debug), "DEBUG");
    EXPECT_EQ(string_utils::level_to_string(log_level::trace), "TRACE");
    EXPECT_EQ(string_utils::level_to_string(log_level::off), "OFF");
}

// =============================================================================
// level_to_color
// =============================================================================

TEST(StringUtilsTest, LevelToColorReturnsAnsiCodes) {
    EXPECT_FALSE(string_utils::level_to_color(log_level::critical).empty());
    EXPECT_FALSE(string_utils::level_to_color(log_level::error).empty());
    EXPECT_FALSE(string_utils::level_to_color(log_level::warning).empty());
    EXPECT_FALSE(string_utils::level_to_color(log_level::info).empty());
    EXPECT_FALSE(string_utils::level_to_color(log_level::debug).empty());
    EXPECT_FALSE(string_utils::level_to_color(log_level::trace).empty());
    EXPECT_FALSE(string_utils::level_to_color(log_level::off).empty());
}

TEST(StringUtilsTest, LevelToColorDisabledReturnsEmpty) {
    EXPECT_EQ(string_utils::level_to_color(log_level::error, false), "");
    EXPECT_EQ(string_utils::level_to_color(log_level::info, false), "");
}

TEST(StringUtilsTest, LevelToColorContainsEscapeSequence) {
    std::string color = string_utils::level_to_color(log_level::error);
    EXPECT_NE(color.find("\033["), std::string::npos);
}

// =============================================================================
// color_reset
// =============================================================================

TEST(StringUtilsTest, ColorResetReturnsResetSequence) {
    EXPECT_STREQ(string_utils::color_reset(), "\033[0m");
}

// =============================================================================
// escape_json
// =============================================================================

TEST(StringUtilsTest, EscapeJsonQuotes) {
    EXPECT_EQ(string_utils::escape_json("say \"hello\""), "say \\\"hello\\\"");
}

TEST(StringUtilsTest, EscapeJsonBackslash) {
    EXPECT_EQ(string_utils::escape_json("path\\to\\file"), "path\\\\to\\\\file");
}

TEST(StringUtilsTest, EscapeJsonSlash) {
    EXPECT_EQ(string_utils::escape_json("a/b"), "a\\/b");
}

TEST(StringUtilsTest, EscapeJsonControlChars) {
    EXPECT_EQ(string_utils::escape_json("line1\nline2"), "line1\\nline2");
    EXPECT_EQ(string_utils::escape_json("col1\tcol2"), "col1\\tcol2");
    EXPECT_EQ(string_utils::escape_json("a\rb"), "a\\rb");
    EXPECT_EQ(string_utils::escape_json("a\bb"), "a\\bb");
    EXPECT_EQ(string_utils::escape_json("a\fb"), "a\\fb");
}

TEST(StringUtilsTest, EscapeJsonUnicodeControlChars) {
    // Control character 0x01 should be escaped as \u0001
    std::string input(1, '\x01');
    std::string result = string_utils::escape_json(input);
    EXPECT_NE(result.find("\\u"), std::string::npos);
}

TEST(StringUtilsTest, EscapeJsonEmptyString) {
    EXPECT_EQ(string_utils::escape_json(""), "");
}

TEST(StringUtilsTest, EscapeJsonPlainText) {
    EXPECT_EQ(string_utils::escape_json("hello world"), "hello world");
}

// =============================================================================
// escape_xml
// =============================================================================

TEST(StringUtilsTest, EscapeXmlAmpersand) {
    EXPECT_EQ(string_utils::escape_xml("a&b"), "a&amp;b");
}

TEST(StringUtilsTest, EscapeXmlAngleBrackets) {
    EXPECT_EQ(string_utils::escape_xml("<tag>"), "&lt;tag&gt;");
}

TEST(StringUtilsTest, EscapeXmlQuotes) {
    EXPECT_EQ(string_utils::escape_xml("say \"hi\""), "say &quot;hi&quot;");
}

TEST(StringUtilsTest, EscapeXmlApostrophe) {
    EXPECT_EQ(string_utils::escape_xml("it's"), "it&apos;s");
}

TEST(StringUtilsTest, EscapeXmlPlainText) {
    EXPECT_EQ(string_utils::escape_xml("hello"), "hello");
}

TEST(StringUtilsTest, EscapeXmlEmpty) {
    EXPECT_EQ(string_utils::escape_xml(""), "");
}

// =============================================================================
// extract_filename
// =============================================================================

TEST(StringUtilsTest, ExtractFilenameFromUnixPath) {
    EXPECT_EQ(string_utils::extract_filename("/path/to/file.cpp"), "file.cpp");
}

TEST(StringUtilsTest, ExtractFilenameFromWindowsPath) {
    EXPECT_EQ(string_utils::extract_filename("C:\\path\\to\\file.cpp"), "file.cpp");
}

TEST(StringUtilsTest, ExtractFilenameNoPath) {
    EXPECT_EQ(string_utils::extract_filename("file.cpp"), "file.cpp");
}

TEST(StringUtilsTest, ExtractFilenameEmpty) {
    EXPECT_EQ(string_utils::extract_filename(""), "");
}

TEST(StringUtilsTest, ExtractFilenameTrailingSlash) {
    // Edge case: trailing slash returns empty substring after last separator
    EXPECT_EQ(string_utils::extract_filename("/path/to/"), "");
}

// =============================================================================
// trim
// =============================================================================

TEST(StringUtilsTest, TrimBothSides) {
    EXPECT_EQ(string_utils::trim("  hello  "), "hello");
}

TEST(StringUtilsTest, TrimLeading) {
    EXPECT_EQ(string_utils::trim("  hello"), "hello");
}

TEST(StringUtilsTest, TrimTrailing) {
    EXPECT_EQ(string_utils::trim("hello  "), "hello");
}

TEST(StringUtilsTest, TrimTabs) {
    EXPECT_EQ(string_utils::trim("\thello\t"), "hello");
}

TEST(StringUtilsTest, TrimNewlines) {
    EXPECT_EQ(string_utils::trim("\nhello\n"), "hello");
}

TEST(StringUtilsTest, TrimEmpty) {
    EXPECT_EQ(string_utils::trim(""), "");
}

TEST(StringUtilsTest, TrimOnlyWhitespace) {
    EXPECT_EQ(string_utils::trim("   "), "");
}

TEST(StringUtilsTest, TrimNoWhitespace) {
    EXPECT_EQ(string_utils::trim("hello"), "hello");
}

// =============================================================================
// to_lower
// =============================================================================

TEST(StringUtilsTest, ToLowerMixed) {
    EXPECT_EQ(string_utils::to_lower("Hello World"), "hello world");
}

TEST(StringUtilsTest, ToLowerAllUpper) {
    EXPECT_EQ(string_utils::to_lower("HELLO"), "hello");
}

TEST(StringUtilsTest, ToLowerAlreadyLower) {
    EXPECT_EQ(string_utils::to_lower("hello"), "hello");
}

TEST(StringUtilsTest, ToLowerEmpty) {
    EXPECT_EQ(string_utils::to_lower(""), "");
}

TEST(StringUtilsTest, ToLowerWithNumbers) {
    EXPECT_EQ(string_utils::to_lower("ABC123"), "abc123");
}

// =============================================================================
// to_upper
// =============================================================================

TEST(StringUtilsTest, ToUpperMixed) {
    EXPECT_EQ(string_utils::to_upper("Hello World"), "HELLO WORLD");
}

TEST(StringUtilsTest, ToUpperAlreadyUpper) {
    EXPECT_EQ(string_utils::to_upper("HELLO"), "HELLO");
}

TEST(StringUtilsTest, ToUpperAllLower) {
    EXPECT_EQ(string_utils::to_upper("hello"), "HELLO");
}

TEST(StringUtilsTest, ToUpperEmpty) {
    EXPECT_EQ(string_utils::to_upper(""), "");
}

// =============================================================================
// replace_all
// =============================================================================

TEST(StringUtilsTest, ReplaceAllSingle) {
    EXPECT_EQ(string_utils::replace_all("hello world", "world", "earth"), "hello earth");
}

TEST(StringUtilsTest, ReplaceAllMultiple) {
    EXPECT_EQ(string_utils::replace_all("aaa", "a", "bb"), "bbbbbb");
}

TEST(StringUtilsTest, ReplaceAllNoMatch) {
    EXPECT_EQ(string_utils::replace_all("hello", "xyz", "abc"), "hello");
}

TEST(StringUtilsTest, ReplaceAllEmptyFrom) {
    EXPECT_EQ(string_utils::replace_all("hello", "", "x"), "hello");
}

TEST(StringUtilsTest, ReplaceAllEmptyTo) {
    EXPECT_EQ(string_utils::replace_all("hello world", "world", ""), "hello ");
}

TEST(StringUtilsTest, ReplaceAllEmptyString) {
    EXPECT_EQ(string_utils::replace_all("", "a", "b"), "");
}
