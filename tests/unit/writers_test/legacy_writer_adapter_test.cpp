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
 * @file legacy_writer_adapter_test.cpp
 * @brief Unit tests for legacy_writer_adapter (backward compatibility adapter)
 * @since 3.5.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/legacy_writer_adapter.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <memory>
#include <string>
#include <vector>
#include <chrono>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Mock legacy writer
// =============================================================================

class mock_legacy_writer : public legacy_writer_interface {
public:
    common::VoidResult write(common::interfaces::log_level level,
                             const std::string& message,
                             const std::string& file,
                             int line,
                             const std::string& function,
                             const std::chrono::system_clock::time_point& /*timestamp*/) override {
        entries_.push_back({level, message, file, line, function});
        return common::ok();
    }

    common::VoidResult flush() override {
        flush_count_++;
        return common::ok();
    }

    std::string get_name() const override { return "mock_legacy"; }
    bool is_healthy() const override { return healthy_; }
    void set_healthy(bool h) { healthy_ = h; }

    struct written_entry {
        common::interfaces::log_level level;
        std::string message;
        std::string file;
        int line;
        std::string function;
    };

    const std::vector<written_entry>& entries() const { return entries_; }
    int flush_count() const { return flush_count_; }

private:
    std::vector<written_entry> entries_;
    int flush_count_ = 0;
    bool healthy_ = true;
};

// =============================================================================
// Test fixture
// =============================================================================

class LegacyWriterAdapterTest : public ::testing::Test {
protected:
    void SetUp() override {
        auto legacy = std::make_unique<mock_legacy_writer>();
        legacy_ptr_ = legacy.get();
        adapter_ = std::make_unique<legacy_writer_adapter>(std::move(legacy));
    }

    std::unique_ptr<legacy_writer_adapter> adapter_;
    mock_legacy_writer* legacy_ptr_ = nullptr;
};

// =============================================================================
// Construction tests
// =============================================================================

TEST_F(LegacyWriterAdapterTest, ConstructionSucceeds) {
    EXPECT_NE(adapter_, nullptr);
}

TEST_F(LegacyWriterAdapterTest, NullWriterThrows) {
    EXPECT_THROW(
        legacy_writer_adapter(nullptr),
        std::invalid_argument);
}

// =============================================================================
// Write delegation tests
// =============================================================================

TEST_F(LegacyWriterAdapterTest, WriteSimpleEntry) {
    log_entry entry(log_level::info, "hello legacy");
    auto result = adapter_->write(entry);

    EXPECT_TRUE(result.is_ok());
    ASSERT_EQ(legacy_ptr_->entries().size(), 1u);
    EXPECT_EQ(legacy_ptr_->entries()[0].message, "hello legacy");
    EXPECT_EQ(legacy_ptr_->entries()[0].file, "");
    EXPECT_EQ(legacy_ptr_->entries()[0].line, 0);
    EXPECT_EQ(legacy_ptr_->entries()[0].function, "");
}

TEST_F(LegacyWriterAdapterTest, WriteWithSourceLocation) {
    log_entry entry(log_level::error, "error msg",
                    "src/main.cpp", 100, "main");
    auto result = adapter_->write(entry);

    EXPECT_TRUE(result.is_ok());
    ASSERT_EQ(legacy_ptr_->entries().size(), 1u);
    EXPECT_EQ(legacy_ptr_->entries()[0].message, "error msg");
    EXPECT_EQ(legacy_ptr_->entries()[0].file, "src/main.cpp");
    EXPECT_EQ(legacy_ptr_->entries()[0].line, 100);
    EXPECT_EQ(legacy_ptr_->entries()[0].function, "main");
}

TEST_F(LegacyWriterAdapterTest, WritePreservesLogLevel) {
    const log_level levels[] = {
        log_level::trace, log_level::debug, log_level::info,
        log_level::warning, log_level::error, log_level::critical
    };

    for (auto level : levels) {
        log_entry entry(level, "level test");
        adapter_->write(entry);
    }

    ASSERT_EQ(legacy_ptr_->entries().size(), 6u);
    for (size_t i = 0; i < 6; ++i) {
        EXPECT_EQ(legacy_ptr_->entries()[i].level, levels[i]);
    }
}

TEST_F(LegacyWriterAdapterTest, WriteMultipleEntries) {
    for (int i = 0; i < 10; ++i) {
        log_entry entry(log_level::debug, "msg" + std::to_string(i));
        adapter_->write(entry);
    }

    EXPECT_EQ(legacy_ptr_->entries().size(), 10u);
}

// =============================================================================
// Flush tests
// =============================================================================

TEST_F(LegacyWriterAdapterTest, FlushDelegatesToLegacy) {
    adapter_->flush();
    EXPECT_EQ(legacy_ptr_->flush_count(), 1);

    adapter_->flush();
    adapter_->flush();
    EXPECT_EQ(legacy_ptr_->flush_count(), 3);
}

// =============================================================================
// Name tests
// =============================================================================

TEST_F(LegacyWriterAdapterTest, GetNamePrefixesLegacyAdapter) {
    EXPECT_EQ(adapter_->get_name(), "legacy_adapter_mock_legacy");
}

// =============================================================================
// Health tests
// =============================================================================

TEST_F(LegacyWriterAdapterTest, IsHealthyDelegatesToLegacy) {
    EXPECT_TRUE(adapter_->is_healthy());

    legacy_ptr_->set_healthy(false);
    EXPECT_FALSE(adapter_->is_healthy());

    legacy_ptr_->set_healthy(true);
    EXPECT_TRUE(adapter_->is_healthy());
}

// =============================================================================
// Accessor tests
// =============================================================================

TEST_F(LegacyWriterAdapterTest, GetLegacyWriterReturnsNonNull) {
    EXPECT_EQ(adapter_->get_legacy_writer(), legacy_ptr_);
}

// =============================================================================
// Category tag test
// =============================================================================

TEST_F(LegacyWriterAdapterTest, HasDecoratorWriterTag) {
    auto* tag = dynamic_cast<decorator_writer_tag*>(adapter_.get());
    EXPECT_NE(tag, nullptr);
}

TEST_F(LegacyWriterAdapterTest, ImplementsLogWriterInterface) {
    auto* iface = dynamic_cast<log_writer_interface*>(adapter_.get());
    EXPECT_NE(iface, nullptr);
}
