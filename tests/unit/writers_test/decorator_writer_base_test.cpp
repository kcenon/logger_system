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
 * @file decorator_writer_base_test.cpp
 * @brief Unit tests for decorator_writer_base class
 * @author kcenon
 * @since 4.0.0
 *
 * Part of the Decorator pattern refactoring (#356, #361).
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/decorator_writer_base.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/interfaces/writer_category.h>

#include <memory>
#include <string>
#include <vector>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

namespace {

/**
 * @brief Mock writer to track operations for testing
 */
class mock_writer : public log_writer_interface {
public:
    mock_writer() = default;
    ~mock_writer() override = default;

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

private:
    std::vector<std::string> entries_;
    int write_count_ = 0;
    int flush_count_ = 0;
    bool healthy_ = true;
};

}  // namespace

/**
 * @brief Concrete test decorator implementation
 *
 * This class demonstrates how to create a decorator using decorator_writer_base.
 * It simply delegates all writes with an optional prefix.
 */
class test_decorator : public decorator_writer_base {
public:
    explicit test_decorator(std::unique_ptr<log_writer_interface> wrapped,
                            std::string prefix = "")
        : decorator_writer_base(std::move(wrapped), "test")
        , prefix_(std::move(prefix)) {}

    common::VoidResult write(const log_entry& entry) override {
        if (prefix_.empty()) {
            return wrapped().write(entry);
        }

        // Create a new entry with prefixed message (log_entry is non-copyable)
        log_entry prefixed_entry(entry.level, prefix_ + entry.message.to_string());
        prefixed_entry.category = entry.category;
        prefixed_entry.location = entry.location;
        return wrapped().write(prefixed_entry);
    }

private:
    std::string prefix_;
};

/**
 * @brief Decorator that tracks additional health state
 */
class health_tracking_decorator : public decorator_writer_base {
public:
    explicit health_tracking_decorator(std::unique_ptr<log_writer_interface> wrapped)
        : decorator_writer_base(std::move(wrapped), "health_tracking")
        , self_healthy_(true) {}

    common::VoidResult write(const log_entry& entry) override {
        return wrapped().write(entry);
    }

    bool is_healthy() const override {
        // Combine self health with wrapped health
        return self_healthy_ && decorator_writer_base::is_healthy();
    }

    void set_self_healthy(bool healthy) { self_healthy_ = healthy; }

private:
    bool self_healthy_;
};

/**
 * @brief Test fixture for decorator_writer_base tests
 */
class DecoratorWriterBaseTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_ = std::make_unique<mock_writer>();
        mock_ptr_ = mock_.get();
    }

    std::unique_ptr<mock_writer> mock_;
    mock_writer* mock_ptr_ = nullptr;
};

/**
 * @test Verify construction with valid arguments
 */
TEST_F(DecoratorWriterBaseTest, ConstructorValid) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_));

    EXPECT_NE(decorator, nullptr);
    EXPECT_NE(decorator->get_wrapped_writer(), nullptr);
}

/**
 * @test Verify construction with nullptr throws
 */
TEST_F(DecoratorWriterBaseTest, ConstructorNullWriterThrows) {
    EXPECT_THROW(
        test_decorator(nullptr),
        std::invalid_argument);
}

/**
 * @test Verify get_name returns correct format
 */
TEST_F(DecoratorWriterBaseTest, GetNameFormat) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_));

    std::string name = decorator->get_name();

    EXPECT_EQ(name, "test_mock_writer");
}

/**
 * @test Verify nested decorators produce correct name chain
 */
TEST_F(DecoratorWriterBaseTest, NestedDecoratorNames) {
    auto inner = std::make_unique<test_decorator>(std::move(mock_));
    auto outer = std::make_unique<health_tracking_decorator>(std::move(inner));

    std::string name = outer->get_name();

    EXPECT_EQ(name, "health_tracking_test_mock_writer");
}

/**
 * @test Verify is_healthy delegates to wrapped writer
 */
TEST_F(DecoratorWriterBaseTest, IsHealthyDelegates) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_));

    EXPECT_TRUE(decorator->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(decorator->is_healthy());

    mock_ptr_->set_healthy(true);
    EXPECT_TRUE(decorator->is_healthy());
}

/**
 * @test Verify is_healthy can be overridden for composite health
 */
TEST_F(DecoratorWriterBaseTest, IsHealthyOverridable) {
    auto decorator = std::make_unique<health_tracking_decorator>(std::move(mock_));

    // Both healthy
    EXPECT_TRUE(decorator->is_healthy());

    // Self unhealthy, wrapped healthy
    decorator->set_self_healthy(false);
    EXPECT_FALSE(decorator->is_healthy());

    // Self healthy, wrapped unhealthy
    decorator->set_self_healthy(true);
    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(decorator->is_healthy());

    // Both unhealthy
    decorator->set_self_healthy(false);
    EXPECT_FALSE(decorator->is_healthy());
}

/**
 * @test Verify flush delegates to wrapped writer
 */
TEST_F(DecoratorWriterBaseTest, FlushDelegates) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_));

    auto result = decorator->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

/**
 * @test Verify write is delegated through decorated write()
 */
TEST_F(DecoratorWriterBaseTest, WriteDelegates) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_));

    log_entry entry(log_level::info, "test message");
    auto result = decorator->write(entry);

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
    ASSERT_EQ(mock_ptr_->entries().size(), 1u);
    EXPECT_EQ(mock_ptr_->entries()[0], "test message");
}

/**
 * @test Verify decorator can modify entries
 */
TEST_F(DecoratorWriterBaseTest, DecoratorCanModifyEntries) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_), "[PREFIX] ");

    log_entry entry(log_level::info, "original");
    auto result = decorator->write(entry);

    EXPECT_TRUE(result.is_ok());
    ASSERT_EQ(mock_ptr_->entries().size(), 1u);
    EXPECT_EQ(mock_ptr_->entries()[0], "[PREFIX] original");
}

/**
 * @test Verify get_wrapped_writer returns correct pointer
 */
TEST_F(DecoratorWriterBaseTest, GetWrappedWriter) {
    auto decorator = std::make_unique<test_decorator>(std::move(mock_));

    const log_writer_interface* wrapped = decorator->get_wrapped_writer();

    EXPECT_EQ(wrapped, mock_ptr_);
}

/**
 * @test Verify decorator has decorator_writer_tag
 */
TEST_F(DecoratorWriterBaseTest, HasDecoratorWriterTag) {
    // Verify type trait works
    EXPECT_TRUE(is_decorator_writer_v<test_decorator>);
    EXPECT_TRUE(is_decorator_writer_v<health_tracking_decorator>);
}

/**
 * @test Verify move semantics work correctly
 */
TEST_F(DecoratorWriterBaseTest, MoveSemantics) {
    auto decorator1 = std::make_unique<test_decorator>(std::move(mock_));

    auto decorator2 = std::move(decorator1);

    EXPECT_NE(decorator2, nullptr);
    EXPECT_EQ(decorator1, nullptr);

    log_entry entry(log_level::info, "test");
    auto result = decorator2->write(entry);
    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->write_count(), 1);
}

/**
 * @test Verify decorator stacking works
 */
TEST_F(DecoratorWriterBaseTest, DecoratorStacking) {
    auto level1 = std::make_unique<test_decorator>(std::move(mock_), "[L1] ");
    auto level2 = std::make_unique<test_decorator>(std::move(level1), "[L2] ");
    auto level3 = std::make_unique<test_decorator>(std::move(level2), "[L3] ");

    log_entry entry(log_level::info, "msg");
    auto result = level3->write(entry);

    EXPECT_TRUE(result.is_ok());
    ASSERT_EQ(mock_ptr_->entries().size(), 1u);
    // Each decorator adds its prefix, innermost decorator applies first
    EXPECT_EQ(mock_ptr_->entries()[0], "[L1] [L2] [L3] msg");

    // Verify name chain
    EXPECT_EQ(level3->get_name(), "test_test_test_mock_writer");
}

/**
 * @test Verify flush propagates through decorator chain
 */
TEST_F(DecoratorWriterBaseTest, FlushPropagatesThroughChain) {
    auto level1 = std::make_unique<test_decorator>(std::move(mock_));
    auto level2 = std::make_unique<test_decorator>(std::move(level1));
    auto level3 = std::make_unique<test_decorator>(std::move(level2));

    auto result = level3->flush();

    EXPECT_TRUE(result.is_ok());
    EXPECT_EQ(mock_ptr_->flush_count(), 1);
}

/**
 * @test Verify is_healthy propagates through decorator chain
 */
TEST_F(DecoratorWriterBaseTest, HealthPropagatesThroughChain) {
    auto level1 = std::make_unique<test_decorator>(std::move(mock_));
    auto level2 = std::make_unique<test_decorator>(std::move(level1));
    auto level3 = std::make_unique<test_decorator>(std::move(level2));

    EXPECT_TRUE(level3->is_healthy());

    mock_ptr_->set_healthy(false);
    EXPECT_FALSE(level3->is_healthy());
}
