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
 * @file path_validator_test.cpp
 * @brief Unit tests for path_validator (security path validation)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/security/path_validator.h>

#include <filesystem>
#include <fstream>
#include <string>

using namespace kcenon::logger::security;
using namespace kcenon::logger;

// =============================================================================
// is_safe_filename
// =============================================================================

TEST(PathValidatorTest, IsSafeFilenameValid) {
    EXPECT_TRUE(path_validator::is_safe_filename("app.log"));
    EXPECT_TRUE(path_validator::is_safe_filename("my-file_2024.log"));
    EXPECT_TRUE(path_validator::is_safe_filename("a"));
    EXPECT_TRUE(path_validator::is_safe_filename("test123"));
}

TEST(PathValidatorTest, IsSafeFilenameRejectsEmpty) {
    EXPECT_FALSE(path_validator::is_safe_filename(""));
}

TEST(PathValidatorTest, IsSafeFilenameRejectsDot) {
    EXPECT_FALSE(path_validator::is_safe_filename("."));
}

TEST(PathValidatorTest, IsSafeFilenameRejectsDoubleDot) {
    EXPECT_FALSE(path_validator::is_safe_filename(".."));
}

TEST(PathValidatorTest, IsSafeFilenameRejectsSpecialChars) {
    EXPECT_FALSE(path_validator::is_safe_filename("file name.log"));   // space
    EXPECT_FALSE(path_validator::is_safe_filename("file/name.log"));   // slash
    EXPECT_FALSE(path_validator::is_safe_filename("file\\name.log"));  // backslash
    EXPECT_FALSE(path_validator::is_safe_filename("file:name.log"));   // colon
    EXPECT_FALSE(path_validator::is_safe_filename("file*name.log"));   // asterisk
    EXPECT_FALSE(path_validator::is_safe_filename("file?name.log"));   // question mark
    EXPECT_FALSE(path_validator::is_safe_filename("file<name>.log"));  // angle brackets
}

// =============================================================================
// sanitize_filename
// =============================================================================

TEST(PathValidatorTest, SanitizeFilenameReplacesInvalid) {
    auto result = path_validator::sanitize_filename("file name?.log");
    EXPECT_EQ(result.find(' '), std::string::npos);
    EXPECT_EQ(result.find('?'), std::string::npos);
    EXPECT_NE(result.find('_'), std::string::npos);
}

TEST(PathValidatorTest, SanitizeFilenamePreservesValid) {
    EXPECT_EQ(path_validator::sanitize_filename("app.log"), "app.log");
    EXPECT_EQ(path_validator::sanitize_filename("my-file_2024.txt"), "my-file_2024.txt");
}

TEST(PathValidatorTest, SanitizeFilenameEmpty) {
    EXPECT_EQ(path_validator::sanitize_filename(""), "unnamed");
}

TEST(PathValidatorTest, SanitizeFilenameDot) {
    auto result = path_validator::sanitize_filename(".");
    EXPECT_NE(result, ".");
}

TEST(PathValidatorTest, SanitizeFilenameDoubleDot) {
    auto result = path_validator::sanitize_filename("..");
    EXPECT_NE(result, "..");
}

TEST(PathValidatorTest, SanitizeFilenameCustomReplacement) {
    auto result = path_validator::sanitize_filename("file name.log", '-');
    EXPECT_NE(result.find('-'), std::string::npos);
    EXPECT_EQ(result.find(' '), std::string::npos);
}

// =============================================================================
// validate - basic
// =============================================================================

TEST(PathValidatorTest, ValidateAcceptsPathWithinBase) {
    auto base = std::filesystem::temp_directory_path();
    path_validator validator(base);

    auto result = validator.validate(base / "app.log");
    EXPECT_TRUE(result.is_ok());
}

TEST(PathValidatorTest, AllowedBaseAccessor) {
    auto base = std::filesystem::temp_directory_path();
    path_validator validator(base);

    EXPECT_FALSE(validator.allowed_base().empty());
}

// =============================================================================
// safe_join
// =============================================================================

TEST(PathValidatorTest, SafeJoinRejectsAbsolutePath) {
    auto base = std::filesystem::temp_directory_path();
    auto result = path_validator::safe_join(base, "/etc/passwd");

    EXPECT_FALSE(result.has_value());
}

TEST(PathValidatorTest, SafeJoinAcceptsRelativePath) {
    auto base = std::filesystem::temp_directory_path();
    auto result = path_validator::safe_join(base, "logs/app.log");

    EXPECT_TRUE(result.has_value());
}
