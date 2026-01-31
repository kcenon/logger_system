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
 * @file writer_builder_test.cpp
 * @brief Unit tests for writer_builder fluent API
 * @author kcenon
 * @since 4.0.0
 *
 * Part of EPIC #391 - Decorator pattern refactoring.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/security/secure_key_storage.h>
#include <kcenon/logger/writers/encrypted_writer.h>

#include <filesystem>
#include <fstream>
#include <memory>
#include <string>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

/**
 * @brief Test fixture for writer_builder tests
 */
class WriterBuilderTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_file_ = "test_builder.log";
        // Clean up any existing test file
        std::filesystem::remove(test_file_);
    }

    void TearDown() override {
        // Clean up test file
        std::filesystem::remove(test_file_);
    }

    std::string test_file_;
};

//========================================================================
// Core Writer Tests
//========================================================================

/**
 * @test Verify basic file writer creation
 */
TEST_F(WriterBuilderTest, BasicFileWriter) {
    auto writer = writer_builder()
        .file(test_file_)
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_name().find("file") != std::string::npos);
    EXPECT_TRUE(writer->is_open());
    EXPECT_TRUE(writer->is_healthy());
}

/**
 * @test Verify console writer creation
 */
TEST_F(WriterBuilderTest, ConsoleWriter) {
    auto writer = writer_builder()
        .console()
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_name().find("console") != std::string::npos);
    EXPECT_TRUE(writer->is_healthy());
}

/**
 * @test Verify console writer with stderr
 */
TEST_F(WriterBuilderTest, ConsoleWriterStderr) {
    auto writer = writer_builder()
        .console(true)
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->is_healthy());
}

/**
 * @test Verify rotating file writer (size-based)
 */
TEST_F(WriterBuilderTest, RotatingFileWriterSize) {
    auto writer = writer_builder()
        .rotating_file(test_file_, 1024 * 1024, 5)
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_name().find("rotating") != std::string::npos);
}

/**
 * @test Verify rotating file writer (time-based)
 */
TEST_F(WriterBuilderTest, RotatingFileWriterTime) {
    auto writer = writer_builder()
        .rotating_file(test_file_, rotation_type::daily, 7)
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_name().find("rotating") != std::string::npos);
}

/**
 * @test Verify network writer creation
 */
TEST_F(WriterBuilderTest, NetworkWriter) {
    auto writer = writer_builder()
        .network("localhost", 8080)
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_name().find("network") != std::string::npos);
}

//========================================================================
// Decorator Tests
//========================================================================

/**
 * @test Verify buffered decorator
 */
TEST_F(WriterBuilderTest, BufferedDecorator) {
    auto writer = writer_builder()
        .file(test_file_)
        .buffered(100)
        .build();

    ASSERT_NE(writer, nullptr);
    std::string name = writer->get_name();
    EXPECT_TRUE(name.find("buffered") != std::string::npos);
}

/**
 * @test Verify decorator chaining order
 */
TEST_F(WriterBuilderTest, DecoratorChaining) {
    auto writer = writer_builder()
        .file(test_file_)
        .buffered(100)
        .build();

    ASSERT_NE(writer, nullptr);
    std::string name = writer->get_name();

    // Verify both decorator and core writer are in name
    EXPECT_TRUE(name.find("buffered") != std::string::npos);
    EXPECT_TRUE(name.find("file") != std::string::npos);
}

/**
 * @test Verify encrypted decorator with valid key
 */
TEST_F(WriterBuilderTest, EncryptedDecorator) {
    auto key_result = security::secure_key_storage::generate_key(32);
    ASSERT_TRUE(key_result.has_value());

    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_result.value())
    );

    auto writer = writer_builder()
        .file(test_file_ + ".enc")
        .encrypted(std::move(config))
        .build();

    ASSERT_NE(writer, nullptr);
    EXPECT_TRUE(writer->get_name().find("encrypted") != std::string::npos);
}

/**
 * @test Verify multiple decorators
 */
TEST_F(WriterBuilderTest, MultipleDecorators) {
    auto key_result = security::secure_key_storage::generate_key(32);
    ASSERT_TRUE(key_result.has_value());

    encryption_config config(
        encryption_algorithm::aes_256_gcm,
        std::move(key_result.value())
    );

    auto writer = writer_builder()
        .file(test_file_)
        .buffered(50)
        .encrypted(std::move(config))
        .build();

    ASSERT_NE(writer, nullptr);
    std::string name = writer->get_name();

    // All decorators should be present in name
    EXPECT_TRUE(name.find("encrypted") != std::string::npos);
    EXPECT_TRUE(name.find("buffered") != std::string::npos);
    EXPECT_TRUE(name.find("file") != std::string::npos);
}

//========================================================================
// Error Handling Tests
//========================================================================

/**
 * @test Verify build() throws without core writer
 */
TEST_F(WriterBuilderTest, BuildThrowsWithoutCoreWriter) {
    writer_builder builder;
    EXPECT_THROW(builder.build(), std::logic_error);
}

/**
 * @test Verify decorator throws without core writer
 */
TEST_F(WriterBuilderTest, DecoratorThrowsWithoutCoreWriter) {
    writer_builder builder;
    EXPECT_THROW(builder.buffered(), std::logic_error);
}

/**
 * @test Verify replacing core writer
 */
TEST_F(WriterBuilderTest, ReplacesCoreWriter) {
    auto writer = writer_builder()
        .file("first.log")
        .console()  // This should replace file writer
        .build();

    ASSERT_NE(writer, nullptr);
    std::string name = writer->get_name();

    // Should be console, not file
    EXPECT_TRUE(name.find("console") != std::string::npos);
    EXPECT_TRUE(name.find("file") == std::string::npos);
}

//========================================================================
// Functional Tests
//========================================================================

/**
 * @test Verify writer can write log entries
 */
TEST_F(WriterBuilderTest, WriterCanWrite) {
    auto writer = writer_builder()
        .file(test_file_)
        .build();

    ASSERT_NE(writer, nullptr);

    log_entry entry(log_level::info, "Test message");

    auto result = writer->write(entry);
    EXPECT_TRUE(result.is_ok());

    result = writer->flush();
    EXPECT_TRUE(result.is_ok());

    // Verify file was created
    EXPECT_TRUE(std::filesystem::exists(test_file_));
}

/**
 * @test Verify buffered writer accumulates entries
 */
TEST_F(WriterBuilderTest, BufferedWriterAccumulates) {
    auto writer = writer_builder()
        .file(test_file_)
        .buffered(10)
        .build();

    ASSERT_NE(writer, nullptr);

    // Write multiple entries
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "Test message " + std::to_string(i));

        auto result = writer->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    // Flush to ensure all entries are written
    auto result = writer->flush();
    EXPECT_TRUE(result.is_ok());

    // Verify file exists
    EXPECT_TRUE(std::filesystem::exists(test_file_));
}

/**
 * @test Verify method chaining returns builder reference
 */
TEST_F(WriterBuilderTest, MethodChainingReturnsSelf) {
    writer_builder builder;

    // All methods should return the same builder instance
    auto& b1 = builder.file(test_file_);
    auto& b2 = b1.buffered();

    // Verify they reference the same object
    EXPECT_EQ(&builder, &b1);
    EXPECT_EQ(&builder, &b2);
}

//========================================================================
// Builder State Tests
//========================================================================

/**
 * @test Verify builder can be reused after build
 */
TEST_F(WriterBuilderTest, BuilderStateAfterBuild) {
    writer_builder builder;

    auto writer1 = builder.file("test1.log").build();
    ASSERT_NE(writer1, nullptr);

    // After build(), builder is in unspecified state
    // Building again without configuring should throw
    EXPECT_THROW(builder.build(), std::logic_error);

    // Cleanup
    std::filesystem::remove("test1.log");
}
