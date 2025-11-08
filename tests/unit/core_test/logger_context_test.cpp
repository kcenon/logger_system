/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "kcenon/logger/core/logger_context.h"
#include "kcenon/logger/security/signal_manager.h"

using namespace kcenon::logger;
using namespace kcenon::logger::core;
using namespace kcenon::logger::security;

/**
 * @brief Mock implementation of signal_manager_interface for testing
 */
class mock_signal_manager : public signal_manager_interface {
public:
    MOCK_METHOD(void, register_logger, (critical_logger_interface*), (override));
    MOCK_METHOD(void, unregister_logger, (critical_logger_interface*), (override));
    MOCK_METHOD(bool, are_handlers_installed, (), (const, override));
    MOCK_METHOD(size_t, logger_count, (), (const, override));
};

/**
 * @brief Test fixture for logger_context tests
 */
class LoggerContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Test setup if needed
    }

    void TearDown() override {
        // Test cleanup if needed
    }
};

/**
 * @test Verify default constructor creates signal_manager
 */
TEST_F(LoggerContextTest, DefaultConstructor) {
    logger_context context;

    EXPECT_TRUE(context.is_initialized());
    EXPECT_NE(context.get_signal_manager(), nullptr);
}

/**
 * @test Verify injection constructor accepts custom implementation
 */
TEST_F(LoggerContextTest, InjectionConstructor) {
    auto mock_mgr = std::make_shared<mock_signal_manager>();

    logger_context context(mock_mgr);

    EXPECT_TRUE(context.is_initialized());
    EXPECT_EQ(context.get_signal_manager(), mock_mgr);
}

/**
 * @test Verify multiple independent contexts can exist
 */
TEST_F(LoggerContextTest, MultipleIndependentContexts) {
    logger_context context1;
    logger_context context2;

    // Each context should have its own signal manager
    EXPECT_NE(context1.get_signal_manager(), context2.get_signal_manager());
    EXPECT_TRUE(context1.is_initialized());
    EXPECT_TRUE(context2.is_initialized());
}

/**
 * @test Verify signal manager can be replaced dynamically
 */
TEST_F(LoggerContextTest, DynamicSignalManagerReplacement) {
    logger_context context;
    auto original_mgr = context.get_signal_manager();

    auto new_mgr = std::make_shared<signal_manager>();
    context.set_signal_manager(new_mgr);

    EXPECT_EQ(context.get_signal_manager(), new_mgr);
    EXPECT_NE(context.get_signal_manager(), original_mgr);
}

/**
 * @test Verify mock injection for testing
 */
TEST_F(LoggerContextTest, MockInjection) {
    auto mock_mgr = std::make_shared<mock_signal_manager>();

    // Set expectations
    EXPECT_CALL(*mock_mgr, are_handlers_installed())
        .WillOnce(::testing::Return(true));

    EXPECT_CALL(*mock_mgr, logger_count())
        .WillOnce(::testing::Return(5));

    logger_context context(mock_mgr);

    // Verify mock behavior
    EXPECT_TRUE(context.get_signal_manager()->are_handlers_installed());
    EXPECT_EQ(context.get_signal_manager()->logger_count(), 5);
}

/**
 * @test Verify real signal_manager works through context
 */
TEST_F(LoggerContextTest, RealSignalManagerIntegration) {
    logger_context context;
    auto signal_mgr = context.get_signal_manager();

    // Initially no handlers should be installed
    EXPECT_FALSE(signal_mgr->are_handlers_installed());
    EXPECT_EQ(signal_mgr->logger_count(), 0);
}
