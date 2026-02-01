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
 * @brief Unit tests for writer_builder
 * @author kcenon
 * @since 4.1.0
 *
 * Part of EPIC #391 - Decorator pattern refactoring.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/writers/async_writer.h>

#include <chrono>
#include <cstdio>
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

/**
 * @brief Mock writer to track operations for testing
 */
class mock_writer_for_builder : public log_writer_interface {
public:
    mock_writer_for_builder() = default;
    ~mock_writer_for_builder() override = default;

    common::VoidResult write(const log_entry& entry) override {
        entries_.push_back(entry.message.to_string());
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
    void clear() {
        entries_.clear();
        write_count_ = 0;
        flush_count_ = 0;
    }

private:
    std::vector<std::string> entries_;
    int write_count_ = 0;
    int flush_count_ = 0;
    bool healthy_ = true;
};

/**
 * @brief Mock filter for testing
 */
class mock_level_filter : public log_filter_interface {
public:
    explicit mock_level_filter(log_level min_level) : min_level_(min_level) {}

    bool should_log(const log_entry& entry) const override {
        return entry.level >= min_level_;
    }

    std::string get_name() const override { return "level_filter"; }

private:
    log_level min_level_;
};

/**
 * @brief Test fixture for writer_builder tests
 */
class WriterBuilderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a temporary directory for test files
        test_dir_ = std::filesystem::temp_directory_path() / "writer_builder_test";
        std::filesystem::create_directories(test_dir_);
    }

    void TearDown() override {
        // Clean up test files
        std::error_code ec;
        std::filesystem::remove_all(test_dir_, ec);
    }

    std::filesystem::path test_dir_;
};

//============================================================================
// Basic Construction Tests
//============================================================================

/**
 * @test Verify builder starts with no core writer
 */
TEST_F(WriterBuilderTest, StartsWithNoCoreWriter) {
    writer_builder builder;
    EXPECT_FALSE(builder.has_core_writer());
}

/**
 * @test Verify build() without core writer throws
 */
TEST_F(WriterBuilderTest, BuildWithoutCoreWriterThrows) {
    writer_builder builder;
    EXPECT_THROW(builder.build(), std::logic_error);
}

/**
 * @test Verify reset() clears the builder
 */
TEST_F(WriterBuilderTest, ResetClearsBuilder) {
    writer_builder builder;
    builder.custom(std::make_unique<mock_writer_for_builder>());

    EXPECT_TRUE(builder.has_core_writer());

    builder.reset();

    EXPECT_FALSE(builder.has_core_writer());
}

//============================================================================
// Core Writer Tests
//============================================================================

/**
 * @test Verify custom() sets a custom writer
 */
TEST_F(WriterBuilderTest, CustomWriterSetsWriter) {
    writer_builder builder;

    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = builder.custom(std::move(mock)).build();

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "mock_writer");
}

/**
 * @test Verify custom() with nullptr throws
 */
TEST_F(WriterBuilderTest, CustomNullptrThrows) {
    writer_builder builder;
    EXPECT_THROW(builder.custom(nullptr), std::invalid_argument);
}

/**
 * @test Verify file() creates a file writer
 */
TEST_F(WriterBuilderTest, FileWriterCreation) {
    auto log_path = test_dir_ / "test.log";

    auto writer = writer_builder()
        .file(log_path.string())
        .build();

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "file");

    // Write something and verify file is created
    log_entry entry(log_level::info, "test message");
    writer->write(entry);
    writer->flush();

    EXPECT_TRUE(std::filesystem::exists(log_path));
}

/**
 * @test Verify console() creates a console writer
 */
TEST_F(WriterBuilderTest, ConsoleWriterCreation) {
    auto writer = writer_builder()
        .console()
        .build();

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "console");
}

/**
 * @test Verify only one core writer can be set
 */
TEST_F(WriterBuilderTest, OnlyOneCoreWriterAllowed) {
    writer_builder builder;
    builder.custom(std::make_unique<mock_writer_for_builder>());

    EXPECT_THROW(
        builder.custom(std::make_unique<mock_writer_for_builder>()),
        std::logic_error);
}

/**
 * @test Verify file() cannot be called after custom()
 */
TEST_F(WriterBuilderTest, FileAfterCustomThrows) {
    writer_builder builder;
    builder.custom(std::make_unique<mock_writer_for_builder>());

    EXPECT_THROW(
        builder.file("test.log"),
        std::logic_error);
}

//============================================================================
// Decorator Tests
//============================================================================

/**
 * @test Verify buffered() requires core writer
 */
TEST_F(WriterBuilderTest, BufferedRequiresCoreWriter) {
    writer_builder builder;
    EXPECT_THROW(builder.buffered(), std::logic_error);
}

/**
 * @test Verify async() requires core writer
 */
TEST_F(WriterBuilderTest, AsyncRequiresCoreWriter) {
    writer_builder builder;
    EXPECT_THROW(builder.async(), std::logic_error);
}

/**
 * @test Verify filtered() requires core writer
 */
TEST_F(WriterBuilderTest, FilteredRequiresCoreWriter) {
    writer_builder builder;
    EXPECT_THROW(
        builder.filtered(std::make_unique<mock_level_filter>(log_level::info)),
        std::logic_error);
}

/**
 * @test Verify buffered() wraps the core writer
 */
TEST_F(WriterBuilderTest, BufferedDecorator) {
    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = writer_builder()
        .custom(std::move(mock))
        .buffered(100, std::chrono::milliseconds(0))
        .build();

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "buffered_mock_writer");

    // Write entries - should be buffered
    log_entry entry(log_level::info, "test");
    writer->write(entry);

    // Not written yet (buffered)
    EXPECT_EQ(mock_ptr->write_count(), 0);

    // Flush should write the entry
    writer->flush();
    EXPECT_EQ(mock_ptr->write_count(), 1);
}

/**
 * @test Verify filtered() applies filter
 */
TEST_F(WriterBuilderTest, FilteredDecorator) {
    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = writer_builder()
        .custom(std::move(mock))
        .filtered(std::make_unique<mock_level_filter>(log_level::warning))
        .build();

    EXPECT_NE(writer, nullptr);

    // Info level should be filtered out
    log_entry info_entry(log_level::info, "info message");
    writer->write(info_entry);
    EXPECT_EQ(mock_ptr->write_count(), 0);

    // Warning level should pass through
    log_entry warning_entry(log_level::warning, "warning message");
    writer->write(warning_entry);
    EXPECT_EQ(mock_ptr->write_count(), 1);
}

/**
 * @test Verify async() wraps the writer
 */
TEST_F(WriterBuilderTest, AsyncDecorator) {
    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = writer_builder()
        .custom(std::move(mock))
        .async(1000)
        .build();

    EXPECT_NE(writer, nullptr);
    EXPECT_EQ(writer->get_name(), "async_mock_writer");

    // Cast to async_writer to start it
    auto* async_ptr = dynamic_cast<async_writer*>(writer.get());
    ASSERT_NE(async_ptr, nullptr);

    async_ptr->start();

    // Write entry
    log_entry entry(log_level::info, "async test");
    writer->write(entry);

    // Give async writer time to process
    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    EXPECT_EQ(mock_ptr->write_count(), 1);

    async_ptr->stop();
}

//============================================================================
// Chained Decorator Tests
//============================================================================

/**
 * @test Verify multiple decorators can be chained
 */
TEST_F(WriterBuilderTest, ChainedDecorators) {
    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = writer_builder()
        .custom(std::move(mock))
        .filtered(std::make_unique<mock_level_filter>(log_level::warning))
        .buffered(100, std::chrono::milliseconds(0))
        .build();

    EXPECT_NE(writer, nullptr);

    // Name should reflect the decorator chain
    std::string name = writer->get_name();
    EXPECT_TRUE(name.find("buffered") != std::string::npos);
    EXPECT_TRUE(name.find("filtered") != std::string::npos);
    EXPECT_TRUE(name.find("mock_writer") != std::string::npos);

    // Info should be filtered
    log_entry info_entry(log_level::info, "info");
    writer->write(info_entry);
    writer->flush();
    EXPECT_EQ(mock_ptr->write_count(), 0);

    // Warning should pass through
    log_entry warning_entry(log_level::warning, "warning");
    writer->write(warning_entry);
    writer->flush();
    EXPECT_EQ(mock_ptr->write_count(), 1);
}

/**
 * @test Verify file + buffered + async decorator chain
 */
TEST_F(WriterBuilderTest, FileBufferedAsyncChain) {
    auto log_path = test_dir_ / "chained.log";

    auto writer = writer_builder()
        .file(log_path.string())
        .buffered(50)
        .async(5000)
        .build();

    EXPECT_NE(writer, nullptr);

    std::string name = writer->get_name();
    EXPECT_TRUE(name.find("async") != std::string::npos);
    EXPECT_TRUE(name.find("buffered") != std::string::npos);
    EXPECT_TRUE(name.find("file") != std::string::npos);

    // Start async writer
    auto* async_ptr = dynamic_cast<async_writer*>(writer.get());
    ASSERT_NE(async_ptr, nullptr);
    async_ptr->start();

    // Write entries
    for (int i = 0; i < 10; ++i) {
        log_entry entry(log_level::info, "message " + std::to_string(i));
        writer->write(entry);
    }

    // Wait for async processing
    writer->flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    async_ptr->stop();

    // Verify file exists
    EXPECT_TRUE(std::filesystem::exists(log_path));
}

//============================================================================
// Builder Reuse Tests
//============================================================================

/**
 * @test Verify builder can be reused after build()
 */
TEST_F(WriterBuilderTest, BuilderReusableAfterBuild) {
    writer_builder builder;

    // First build
    auto mock1 = std::make_unique<mock_writer_for_builder>();
    auto writer1 = builder.custom(std::move(mock1)).build();
    EXPECT_NE(writer1, nullptr);

    // Builder should be empty after build
    EXPECT_FALSE(builder.has_core_writer());

    // Second build with same builder
    auto mock2 = std::make_unique<mock_writer_for_builder>();
    auto writer2 = builder.custom(std::move(mock2)).build();
    EXPECT_NE(writer2, nullptr);

    // Both writers should be independent
    EXPECT_NE(writer1.get(), writer2.get());
}

/**
 * @test Verify builder move semantics
 */
TEST_F(WriterBuilderTest, BuilderMoveSemantics) {
    writer_builder builder1;
    builder1.custom(std::make_unique<mock_writer_for_builder>());

    EXPECT_TRUE(builder1.has_core_writer());

    // Move construction
    writer_builder builder2 = std::move(builder1);
    EXPECT_TRUE(builder2.has_core_writer());

    // Build from moved builder
    auto writer = builder2.build();
    EXPECT_NE(writer, nullptr);
}

//============================================================================
// Integration Tests
//============================================================================

/**
 * @test Verify complete workflow with file writer
 */
TEST_F(WriterBuilderTest, CompleteFileWorkflow) {
    auto log_path = test_dir_ / "complete.log";

    auto writer = writer_builder()
        .file(log_path.string())
        .buffered(100, std::chrono::milliseconds(0))
        .build();

    // Write multiple entries
    for (int i = 0; i < 5; ++i) {
        log_entry entry(log_level::info, "line " + std::to_string(i));
        auto result = writer->write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    // Flush and close
    writer->flush();
    writer->close();

    // Verify file contents
    std::ifstream file(log_path);
    ASSERT_TRUE(file.is_open());

    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }

    EXPECT_EQ(lines.size(), 5u);
}

/**
 * @test Verify is_healthy propagates through decorators
 */
TEST_F(WriterBuilderTest, IsHealthyPropagates) {
    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = writer_builder()
        .custom(std::move(mock))
        .buffered()
        .build();

    EXPECT_TRUE(writer->is_healthy());

    mock_ptr->set_healthy(false);
    EXPECT_FALSE(writer->is_healthy());
}

/**
 * @test Verify null filter acts as pass-through
 */
TEST_F(WriterBuilderTest, NullFilterPassThrough) {
    auto mock = std::make_unique<mock_writer_for_builder>();
    auto mock_ptr = mock.get();

    auto writer = writer_builder()
        .custom(std::move(mock))
        .filtered(nullptr)
        .build();

    // All entries should pass through
    log_entry entry(log_level::debug, "debug message");
    writer->write(entry);

    EXPECT_EQ(mock_ptr->write_count(), 1);
}
