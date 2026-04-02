// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

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
