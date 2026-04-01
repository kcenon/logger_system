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
 * @file writer_category_test.cpp
 * @brief Unit tests for writer category type traits and to_string
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/interfaces/writer_category.h>
#include <kcenon/logger/interfaces/log_writer_interface.h>

#include <string>

using namespace kcenon::logger;

// =============================================================================
// Tag constants
// =============================================================================

TEST(WriterCategoryTest, SyncTagCategory) {
    EXPECT_EQ(sync_writer_tag::category, writer_category::synchronous);
}

TEST(WriterCategoryTest, AsyncTagCategory) {
    EXPECT_EQ(async_writer_tag::category, writer_category::asynchronous);
}

TEST(WriterCategoryTest, DecoratorTagCategory) {
    EXPECT_EQ(decorator_writer_tag::category, writer_category::decorator);
}

TEST(WriterCategoryTest, CompositeTagCategory) {
    EXPECT_EQ(composite_writer_tag::category, writer_category::composite);
}

// =============================================================================
// to_string
// =============================================================================

TEST(WriterCategoryTest, ToStringSynchronous) {
    EXPECT_STREQ(to_string(writer_category::synchronous), "synchronous");
}

TEST(WriterCategoryTest, ToStringAsynchronous) {
    EXPECT_STREQ(to_string(writer_category::asynchronous), "asynchronous");
}

TEST(WriterCategoryTest, ToStringDecorator) {
    EXPECT_STREQ(to_string(writer_category::decorator), "decorator");
}

TEST(WriterCategoryTest, ToStringComposite) {
    EXPECT_STREQ(to_string(writer_category::composite), "composite");
}

// =============================================================================
// Type traits
// =============================================================================

namespace {

// Test classes with different tags
class test_sync_writer : public log_writer_interface, public sync_writer_tag {
public:
    kcenon::common::VoidResult write(const log_entry&) override { return kcenon::common::ok(); }
    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }
    std::string get_name() const override { return "test_sync"; }
    bool is_healthy() const override { return true; }
};

class test_async_writer : public log_writer_interface, public async_writer_tag {
public:
    kcenon::common::VoidResult write(const log_entry&) override { return kcenon::common::ok(); }
    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }
    std::string get_name() const override { return "test_async"; }
    bool is_healthy() const override { return true; }
};

class test_decorator_writer : public log_writer_interface, public decorator_writer_tag {
public:
    kcenon::common::VoidResult write(const log_entry&) override { return kcenon::common::ok(); }
    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }
    std::string get_name() const override { return "test_decorator"; }
    bool is_healthy() const override { return true; }
};

class test_composite_writer : public log_writer_interface, public composite_writer_tag {
public:
    kcenon::common::VoidResult write(const log_entry&) override { return kcenon::common::ok(); }
    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }
    std::string get_name() const override { return "test_composite"; }
    bool is_healthy() const override { return true; }
};

class test_untagged_writer : public log_writer_interface {
public:
    kcenon::common::VoidResult write(const log_entry&) override { return kcenon::common::ok(); }
    kcenon::common::VoidResult flush() override { return kcenon::common::ok(); }
    std::string get_name() const override { return "test_untagged"; }
    bool is_healthy() const override { return true; }
};

} // namespace

TEST(WriterCategoryTest, IsSyncWriterTrait) {
    EXPECT_TRUE(is_sync_writer_v<test_sync_writer>);
    EXPECT_FALSE(is_sync_writer_v<test_async_writer>);
    EXPECT_FALSE(is_sync_writer_v<test_decorator_writer>);
    EXPECT_FALSE(is_sync_writer_v<test_composite_writer>);
}

TEST(WriterCategoryTest, IsAsyncWriterTrait) {
    EXPECT_FALSE(is_async_writer_v<test_sync_writer>);
    EXPECT_TRUE(is_async_writer_v<test_async_writer>);
    EXPECT_FALSE(is_async_writer_v<test_decorator_writer>);
    EXPECT_FALSE(is_async_writer_v<test_composite_writer>);
}

TEST(WriterCategoryTest, IsDecoratorWriterTrait) {
    EXPECT_FALSE(is_decorator_writer_v<test_sync_writer>);
    EXPECT_FALSE(is_decorator_writer_v<test_async_writer>);
    EXPECT_TRUE(is_decorator_writer_v<test_decorator_writer>);
    EXPECT_FALSE(is_decorator_writer_v<test_composite_writer>);
}

TEST(WriterCategoryTest, IsCompositeWriterTrait) {
    EXPECT_FALSE(is_composite_writer_v<test_sync_writer>);
    EXPECT_FALSE(is_composite_writer_v<test_async_writer>);
    EXPECT_FALSE(is_composite_writer_v<test_decorator_writer>);
    EXPECT_TRUE(is_composite_writer_v<test_composite_writer>);
}

// =============================================================================
// get_writer_category
// =============================================================================

TEST(WriterCategoryTest, GetWriterCategorySync) {
    EXPECT_EQ(get_writer_category<test_sync_writer>(), writer_category::synchronous);
}

TEST(WriterCategoryTest, GetWriterCategoryAsync) {
    EXPECT_EQ(get_writer_category<test_async_writer>(), writer_category::asynchronous);
}

TEST(WriterCategoryTest, GetWriterCategoryDecorator) {
    EXPECT_EQ(get_writer_category<test_decorator_writer>(), writer_category::decorator);
}

TEST(WriterCategoryTest, GetWriterCategoryComposite) {
    EXPECT_EQ(get_writer_category<test_composite_writer>(), writer_category::composite);
}

TEST(WriterCategoryTest, GetWriterCategoryUntagged) {
    EXPECT_EQ(get_writer_category<test_untagged_writer>(), writer_category::synchronous);
}
