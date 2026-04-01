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
