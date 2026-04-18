// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file signal_manager_context_test.cpp
 * @brief Unit tests for signal_manager_context
 *
 * @details Verifies lifecycle, thread-safe getters/setters, and initialization
 *          behavior of the signal_manager_context SRP component introduced in 2.0.0.
 */

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <atomic>
#include <memory>
#include <thread>
#include <vector>

#include "kcenon/logger/core/signal_manager_context.h"
#include "kcenon/logger/security/signal_manager.h"

using namespace kcenon::logger::core;
using namespace kcenon::logger::security;

namespace {

/**
 * @brief Mock implementation of signal_manager_interface for testing.
 */
class mock_signal_manager : public signal_manager_interface {
public:
    MOCK_METHOD(void, register_logger, (critical_logger_interface*), (override));
    MOCK_METHOD(void, unregister_logger, (critical_logger_interface*), (override));
    MOCK_METHOD(bool, are_handlers_installed, (), (const, override));
    MOCK_METHOD(size_t, logger_count, (), (const, override));
};

class SignalManagerContextTest : public ::testing::Test {};

/**
 * @test Default-constructed context holds a null signal manager.
 */
TEST_F(SignalManagerContextTest, DefaultConstructorHoldsNullptr) {
    signal_manager_context ctx;
    EXPECT_FALSE(ctx.is_initialized());
    EXPECT_EQ(ctx.get_signal_manager(), nullptr);
}

/**
 * @test Constructor with nullptr is equivalent to the default constructor.
 */
TEST_F(SignalManagerContextTest, NullptrConstructorMatchesDefault) {
    signal_manager_context ctx(nullptr);
    EXPECT_FALSE(ctx.is_initialized());
    EXPECT_EQ(ctx.get_signal_manager(), nullptr);
}

/**
 * @test Constructor accepting a shared_ptr stores the provided instance.
 */
TEST_F(SignalManagerContextTest, ConstructorStoresInjectedInstance) {
    auto mock_mgr = std::make_shared<mock_signal_manager>();
    signal_manager_context ctx(mock_mgr);

    EXPECT_TRUE(ctx.is_initialized());
    EXPECT_EQ(ctx.get_signal_manager(), mock_mgr);
}

/**
 * @test set_signal_manager replaces the stored instance.
 */
TEST_F(SignalManagerContextTest, SetSignalManagerReplacesInstance) {
    auto first = std::make_shared<mock_signal_manager>();
    auto second = std::make_shared<mock_signal_manager>();

    signal_manager_context ctx(first);
    EXPECT_EQ(ctx.get_signal_manager(), first);

    ctx.set_signal_manager(second);
    EXPECT_EQ(ctx.get_signal_manager(), second);
    EXPECT_NE(ctx.get_signal_manager(), first);
    EXPECT_TRUE(ctx.is_initialized());
}

/**
 * @test set_signal_manager(nullptr) clears the stored instance.
 */
TEST_F(SignalManagerContextTest, SetSignalManagerToNullptrClears) {
    auto mgr = std::make_shared<mock_signal_manager>();
    signal_manager_context ctx(mgr);
    ASSERT_TRUE(ctx.is_initialized());

    ctx.set_signal_manager(nullptr);
    EXPECT_FALSE(ctx.is_initialized());
    EXPECT_EQ(ctx.get_signal_manager(), nullptr);
}

/**
 * @test Real signal_manager implementation works through the context.
 *
 * @details Ensures signal_manager_context correctly stores and returns a real
 *          signal_manager instance without installing handlers as a side effect.
 */
TEST_F(SignalManagerContextTest, RealSignalManagerIntegration) {
    auto mgr = std::make_shared<signal_manager>();
    signal_manager_context ctx(mgr);

    ASSERT_TRUE(ctx.is_initialized());
    auto retrieved = ctx.get_signal_manager();
    ASSERT_NE(retrieved, nullptr);

    // Newly constructed signal_manager must have no registered loggers
    // and no installed handlers.
    EXPECT_EQ(retrieved->logger_count(), 0u);
    EXPECT_FALSE(retrieved->are_handlers_installed());
}

/**
 * @test Concurrent readers/writers do not corrupt the stored pointer.
 *
 * @details Exercises the internal mutex by hammering get/set/is_initialized
 *          from multiple threads. The test passes as long as no data race or
 *          crash occurs and the final pointer is one of the expected values.
 */
TEST_F(SignalManagerContextTest, ConcurrentAccessIsThreadSafe) {
    signal_manager_context ctx;

    auto mgr_a = std::make_shared<mock_signal_manager>();
    auto mgr_b = std::make_shared<mock_signal_manager>();

    constexpr int kIterations = 500;
    std::atomic<bool> start{false};
    std::atomic<int> reads_observed{0};

    auto writer = [&](std::shared_ptr<mock_signal_manager> mgr) {
        while (!start.load(std::memory_order_acquire)) {}
        for (int i = 0; i < kIterations; ++i) {
            ctx.set_signal_manager(mgr);
        }
    };

    auto reader = [&]() {
        while (!start.load(std::memory_order_acquire)) {}
        for (int i = 0; i < kIterations; ++i) {
            auto snapshot = ctx.get_signal_manager();
            // Snapshot must be nullptr or one of the two known pointers.
            const bool ok = (snapshot == nullptr)
                         || (snapshot == mgr_a)
                         || (snapshot == mgr_b);
            if (ok) {
                reads_observed.fetch_add(1, std::memory_order_relaxed);
            }
            (void)ctx.is_initialized();
        }
    };

    std::vector<std::thread> threads;
    threads.emplace_back(writer, mgr_a);
    threads.emplace_back(writer, mgr_b);
    threads.emplace_back(reader);
    threads.emplace_back(reader);

    start.store(true, std::memory_order_release);
    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(reads_observed.load(), kIterations * 2);

    auto final_mgr = ctx.get_signal_manager();
    EXPECT_TRUE(final_mgr == mgr_a || final_mgr == mgr_b);
    EXPECT_TRUE(ctx.is_initialized());
}

/**
 * @test Assigning the same pointer repeatedly is idempotent.
 */
TEST_F(SignalManagerContextTest, RepeatedSetIsIdempotent) {
    auto mgr = std::make_shared<mock_signal_manager>();
    signal_manager_context ctx;

    for (int i = 0; i < 10; ++i) {
        ctx.set_signal_manager(mgr);
    }
    EXPECT_EQ(ctx.get_signal_manager(), mgr);
    EXPECT_TRUE(ctx.is_initialized());
    // Reference count should be: ctx(1) + mgr local(1) = 2.
    EXPECT_EQ(mgr.use_count(), 2);
}

} // namespace
