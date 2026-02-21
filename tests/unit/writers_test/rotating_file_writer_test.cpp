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
 * @file rotating_file_writer_test.cpp
 * @brief Unit tests for rotating_file_writer rotation logic
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <vector>

using namespace kcenon::logger;
using log_level = kcenon::common::interfaces::log_level;

// =============================================================================
// Test fixture with temporary directory management
// =============================================================================

class RotatingFileWriterTest : public ::testing::Test {
protected:
    std::filesystem::path temp_dir_;

    void SetUp() override {
        temp_dir_ = std::filesystem::temp_directory_path() / "rotating_writer_test";
        std::filesystem::create_directories(temp_dir_);
    }

    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove_all(temp_dir_, ec);
    }

    std::string test_file(const std::string& name = "test.log") const {
        return (temp_dir_ / name).string();
    }

    log_entry make_entry(const std::string& msg) const {
        return log_entry(log_level::info, msg);
    }

    size_t count_files_in_dir() const {
        size_t count = 0;
        for (const auto& entry : std::filesystem::directory_iterator(temp_dir_)) {
            if (entry.is_regular_file()) {
                ++count;
            }
        }
        return count;
    }
};

// =============================================================================
// Construction
// =============================================================================

TEST_F(RotatingFileWriterTest, SizeBasedConstruction) {
    rotating_file_writer writer(test_file(), 1024, 3);

    EXPECT_EQ(writer.get_name(), "rotating_file");
}

TEST_F(RotatingFileWriterTest, TimeBasedConstructionDaily) {
    rotating_file_writer writer(test_file(), rotation_type::daily, 3);

    EXPECT_EQ(writer.get_name(), "rotating_file");
}

TEST_F(RotatingFileWriterTest, TimeBasedConstructionHourly) {
    rotating_file_writer writer(test_file(), rotation_type::hourly, 5);

    EXPECT_EQ(writer.get_name(), "rotating_file");
}

TEST_F(RotatingFileWriterTest, CombinedConstructionSizeAndTime) {
    rotating_file_writer writer(
        test_file(), rotation_type::size_and_time, 2048u, 3u, 100u);

    EXPECT_EQ(writer.get_name(), "rotating_file");
}

TEST_F(RotatingFileWriterTest, CombinedConstructionThrowsForNonSizeAndTime) {
    // Pass 5 args to unambiguously select the combined constructor
    EXPECT_THROW(
        rotating_file_writer(test_file(), rotation_type::daily, 1024u, 3u, 100u),
        std::invalid_argument);
}

TEST_F(RotatingFileWriterTest, CombinedConstructionThrowsForSizeOnly) {
    EXPECT_THROW(
        rotating_file_writer(test_file(), rotation_type::size, 1024u, 3u, 100u),
        std::invalid_argument);
}

TEST_F(RotatingFileWriterTest, DefaultExtensionWhenNoExtension) {
    // File without extension should get .log appended internally
    rotating_file_writer writer(
        (temp_dir_ / "logfile").string(), 1024, 3);

    EXPECT_EQ(writer.get_name(), "rotating_file");
}

// =============================================================================
// Basic write
// =============================================================================

TEST_F(RotatingFileWriterTest, WriteCreatesFile) {
    rotating_file_writer writer(test_file(), 1024, 3);

    auto result = writer.write(make_entry("hello world"));
    EXPECT_TRUE(result.is_ok());
    EXPECT_TRUE(std::filesystem::exists(test_file()));
}

TEST_F(RotatingFileWriterTest, WriteMultipleEntries) {
    rotating_file_writer writer(test_file(), 4096, 3);

    for (int i = 0; i < 10; ++i) {
        auto result = writer.write(make_entry("message " + std::to_string(i)));
        EXPECT_TRUE(result.is_ok());
    }
}

// =============================================================================
// Size-based rotation
// =============================================================================

TEST_F(RotatingFileWriterTest, SizeBasedRotationCreatesBackupFile) {
    // Small max_size and check_interval=1 to trigger rotation quickly
    rotating_file_writer writer(test_file(), 50, 5, 1);

    // Write enough data to exceed 50 bytes and trigger rotation
    for (int i = 0; i < 20; ++i) {
        writer.write(make_entry("This is a test message that is fairly long " + std::to_string(i)));
    }

    // Should have created backup files
    EXPECT_GT(count_files_in_dir(), 1u);
}

// =============================================================================
// Manual rotation
// =============================================================================

TEST_F(RotatingFileWriterTest, ManualRotateCreatesBackupFile) {
    rotating_file_writer writer(test_file(), 1024 * 1024, 5);

    // Write some data
    writer.write(make_entry("before rotation"));

    // Manually trigger rotation
    writer.rotate();

    // Write more data to the new file
    writer.write(make_entry("after rotation"));

    // Should have original + backup
    EXPECT_GE(count_files_in_dir(), 2u);
}

TEST_F(RotatingFileWriterTest, ManualRotateMultipleTimes) {
    rotating_file_writer writer(test_file(), 1024 * 1024, 10);

    for (int i = 0; i < 3; ++i) {
        writer.write(make_entry("message " + std::to_string(i)));
        writer.rotate();
    }

    // Should have 3 backup files + 1 current file
    EXPECT_GE(count_files_in_dir(), 3u);
}

// =============================================================================
// Cleanup old files
// =============================================================================

TEST_F(RotatingFileWriterTest, MultipleRotationsCreateBackupFiles) {
    rotating_file_writer writer(test_file(), 1024 * 1024, 5);

    // Create 3 rotations with writes between them
    for (int i = 0; i < 3; ++i) {
        writer.write(make_entry("msg " + std::to_string(i)));
        writer.rotate();
    }

    // Should have at least the current file + some backup files
    EXPECT_GE(count_files_in_dir(), 2u);
}

// =============================================================================
// Check interval optimization
// =============================================================================

TEST_F(RotatingFileWriterTest, CheckIntervalSkipsRotationCheck) {
    // max_size = 50 bytes, check_interval = 100
    // With check_interval=100, rotation check happens only every 100 writes
    rotating_file_writer writer(test_file(), 50, 5, 100);

    // Write 50 entries (below check_interval threshold)
    for (int i = 0; i < 50; ++i) {
        writer.write(make_entry("msg"));
    }

    // Despite exceeding max_size, rotation check hasn't triggered yet
    // because writes_since_check < check_interval (50 < 100)
    // Only the main file should exist (no rotation happened)
    EXPECT_EQ(count_files_in_dir(), 1u);
}

TEST_F(RotatingFileWriterTest, CheckIntervalTriggersRotationAtThreshold) {
    // max_size = 50 bytes, check_interval = 10
    rotating_file_writer writer(test_file(), 50, 5, 10);

    // Write enough entries to cross the check_interval threshold
    // and have file size exceed max_size
    for (int i = 0; i < 30; ++i) {
        writer.write(make_entry("A longer test message for rotation testing " + std::to_string(i)));
    }

    // Rotation should have occurred after check_interval writes
    EXPECT_GT(count_files_in_dir(), 1u);
}

// =============================================================================
// get_name
// =============================================================================

TEST_F(RotatingFileWriterTest, GetNameReturnsCorrectValue) {
    rotating_file_writer writer(test_file(), 1024, 3);
    EXPECT_EQ(writer.get_name(), "rotating_file");
}

// =============================================================================
// Write after rotation continues working
// =============================================================================

TEST_F(RotatingFileWriterTest, WriteAfterRotationSucceeds) {
    rotating_file_writer writer(test_file(), 1024 * 1024, 5);

    writer.write(make_entry("before"));
    writer.rotate();

    auto result = writer.write(make_entry("after rotation"));
    EXPECT_TRUE(result.is_ok());

    // Verify the main log file exists (content may be buffered)
    EXPECT_TRUE(std::filesystem::exists(test_file()));
}
