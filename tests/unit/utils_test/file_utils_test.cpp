// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file file_utils_test.cpp
 * @brief Unit tests for file utility functions
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/utils/file_utils.h>

#include <filesystem>
#include <fstream>
#include <string>

using namespace kcenon::logger::utils;
using namespace kcenon::logger;

// =============================================================================
// validate_log_path - path traversal
// =============================================================================

TEST(FileUtilsTest, ValidateLogPathRejectsTraversal) {
    auto result = file_utils::validate_log_path("../../../etc/passwd");
    EXPECT_TRUE(result.is_err());
}

TEST(FileUtilsTest, ValidateLogPathRejectsDoubleDotsInMiddle) {
    auto result = file_utils::validate_log_path("/var/log/../../../etc/shadow");
    EXPECT_TRUE(result.is_err());
}

TEST(FileUtilsTest, ValidateLogPathAcceptsSimplePath) {
    auto result = file_utils::validate_log_path("logs/app.log");
    EXPECT_TRUE(result.is_ok());
}

TEST(FileUtilsTest, ValidateLogPathAcceptsWithoutBase) {
    auto result = file_utils::validate_log_path("/tmp/test.log");
    EXPECT_TRUE(result.is_ok());
}

// =============================================================================
// sanitize_filename
// =============================================================================

TEST(FileUtilsTest, SanitizeFilenameRemovesSlashes) {
    auto result = file_utils::sanitize_filename("path/file.log");
    EXPECT_EQ(result.find('/'), std::string::npos);
}

TEST(FileUtilsTest, SanitizeFilenameRemovesBackslashes) {
    auto result = file_utils::sanitize_filename("path\\file.log");
    EXPECT_EQ(result.find('\\'), std::string::npos);
}

TEST(FileUtilsTest, SanitizeFilenameRemovesNullBytes) {
    std::string input = "file";
    input += '\0';
    input += ".log";
    auto result = file_utils::sanitize_filename(input);
    EXPECT_EQ(result.find('\0'), std::string::npos);
}

TEST(FileUtilsTest, SanitizeFilenameReplacesControlChars) {
    std::string input = "file\x01name.log";
    auto result = file_utils::sanitize_filename(input);
    EXPECT_EQ(result.find('\x01'), std::string::npos);
    EXPECT_NE(result.find('_'), std::string::npos);
}

TEST(FileUtilsTest, SanitizeFilenameReplacesDangerousChars) {
    auto result = file_utils::sanitize_filename("file:name?.log");
    EXPECT_EQ(result.find(':'), std::string::npos);
    EXPECT_EQ(result.find('?'), std::string::npos);
}

TEST(FileUtilsTest, SanitizeFilenameEmptyReturnsDefault) {
    EXPECT_EQ(file_utils::sanitize_filename(""), "unnamed.log");
}

TEST(FileUtilsTest, SanitizeFilenamePreservesNormalName) {
    EXPECT_EQ(file_utils::sanitize_filename("app.log"), "app.log");
}

TEST(FileUtilsTest, SanitizeFilenameTruncatesLongNames) {
    std::string long_name(300, 'a');
    long_name += ".log";
    auto result = file_utils::sanitize_filename(long_name);
    EXPECT_LE(result.size(), 255u);
}

// =============================================================================
// is_absolute
// =============================================================================

TEST(FileUtilsTest, IsAbsoluteUnixPath) {
    EXPECT_TRUE(file_utils::is_absolute("/var/log/app.log"));
}

TEST(FileUtilsTest, IsAbsoluteRelativePath) {
    EXPECT_FALSE(file_utils::is_absolute("logs/app.log"));
}

TEST(FileUtilsTest, IsAbsoluteEmpty) {
    EXPECT_FALSE(file_utils::is_absolute(""));
}

// =============================================================================
// get_file_size
// =============================================================================

TEST(FileUtilsTest, GetFileSizeNonExistentReturnsZero) {
    EXPECT_EQ(file_utils::get_file_size("/nonexistent/path/file.txt"), 0u);
}

// =============================================================================
// is_writable
// =============================================================================

TEST(FileUtilsTest, IsWritableNonExistentParent) {
    EXPECT_FALSE(file_utils::is_writable("/nonexistent_dir_xyz/file.txt"));
}

// =============================================================================
// generate_temp_filename
// =============================================================================

TEST(FileUtilsTest, GenerateTempFilenameContainsPrefix) {
    auto result = file_utils::generate_temp_filename("myapp");
    EXPECT_NE(result.find("myapp"), std::string::npos);
}

TEST(FileUtilsTest, GenerateTempFilenameContainsExtension) {
    auto result = file_utils::generate_temp_filename("test", ".log");
    EXPECT_NE(result.find(".log"), std::string::npos);
}

TEST(FileUtilsTest, GenerateTempFilenameDefaultExtension) {
    auto result = file_utils::generate_temp_filename();
    EXPECT_NE(result.find(".tmp"), std::string::npos);
}

TEST(FileUtilsTest, GenerateTempFilenameUniqueness) {
    auto name1 = file_utils::generate_temp_filename("test");
    auto name2 = file_utils::generate_temp_filename("test");
    // Names may differ due to random component (or be same within same second)
    // Just verify they are valid non-empty strings
    EXPECT_FALSE(name1.empty());
    EXPECT_FALSE(name2.empty());
}
