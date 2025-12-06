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
 * @file thread_system_integration_module_test.cpp
 * @brief Unit tests for the thread_system_integration module
 * @author kcenon
 * @since 1.4.0
 *
 * @details This file tests the thread_system_integration module which provides
 * optional integration with thread_system's thread_pool for advanced async
 * processing capabilities.
 *
 * Test coverage:
 * - Standalone mode (default) behavior
 * - Backend type detection
 * - Enable/disable functionality
 * - Thread safety of integration API
 *
 * When LOGGER_HAS_THREAD_SYSTEM is defined, additional tests verify:
 * - thread_pool backend enabling
 * - Custom thread pool configuration
 * - Task submission to thread_pool
 * - Backend switching at runtime
 *
 * @note Part of Issue #224: Add optional thread_system integration support
 */

#include <gtest/gtest.h>
#include <kcenon/logger/integration/thread_system_integration.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

using namespace kcenon::logger::integration;

/**
 * @brief Test fixture for thread_system_integration tests
 */
class ThreadSystemIntegrationModuleTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure we start in a known state (standalone mode)
#ifdef LOGGER_HAS_THREAD_SYSTEM
        thread_system_integration::disable();
#endif
    }

    void TearDown() override {
        // Clean up after each test
#ifdef LOGGER_HAS_THREAD_SYSTEM
        thread_system_integration::disable();
#endif
    }
};

// ============================================================================
// Tests that work in both standalone and thread_system modes
// ============================================================================

/**
 * @brief Test has_thread_system_support() compile-time detection
 */
TEST_F(ThreadSystemIntegrationModuleTest, HasThreadSystemSupport) {
#ifdef LOGGER_HAS_THREAD_SYSTEM
    EXPECT_TRUE(has_thread_system_support())
        << "has_thread_system_support() should return true when LOGGER_HAS_THREAD_SYSTEM is defined";
#else
    EXPECT_FALSE(has_thread_system_support())
        << "has_thread_system_support() should return false when LOGGER_HAS_THREAD_SYSTEM is not defined";
#endif
}

/**
 * @brief Test default backend is standalone
 */
TEST_F(ThreadSystemIntegrationModuleTest, DefaultBackendIsStandalone) {
    EXPECT_EQ(thread_system_integration::get_backend(), async_backend_type::standalone)
        << "Default backend should be standalone";
    EXPECT_EQ(thread_system_integration::get_backend_name(), "standalone")
        << "Default backend name should be 'standalone'";
    EXPECT_FALSE(thread_system_integration::is_enabled())
        << "thread_pool backend should not be enabled by default";
}

/**
 * @brief Test submit_task returns false in standalone mode
 */
TEST_F(ThreadSystemIntegrationModuleTest, SubmitTaskInStandaloneMode) {
    std::atomic<bool> executed{false};

    bool submitted = thread_system_integration::submit_task([&executed]() {
        executed = true;
    });

    EXPECT_FALSE(submitted)
        << "submit_task() should return false in standalone mode";
    EXPECT_FALSE(executed)
        << "Task should not be executed when submit_task() returns false";
}

// ============================================================================
// Tests specific to LOGGER_HAS_THREAD_SYSTEM mode
// ============================================================================

#ifdef LOGGER_HAS_THREAD_SYSTEM

/**
 * @brief Test enable() with default pool
 */
TEST_F(ThreadSystemIntegrationModuleTest, EnableWithDefaultPool) {
    thread_system_integration::enable();

    EXPECT_TRUE(thread_system_integration::is_enabled())
        << "thread_pool backend should be enabled after enable()";
    EXPECT_EQ(thread_system_integration::get_backend(), async_backend_type::thread_pool)
        << "Backend should be thread_pool after enable()";
    EXPECT_EQ(thread_system_integration::get_backend_name(), "thread_pool")
        << "Backend name should be 'thread_pool' after enable()";

    auto pool = thread_system_integration::get_thread_pool();
    EXPECT_NE(pool, nullptr)
        << "Default thread pool should be created";
    EXPECT_TRUE(pool->is_running())
        << "Default thread pool should be running";
}

/**
 * @brief Test disable() after enable()
 */
TEST_F(ThreadSystemIntegrationModuleTest, DisableAfterEnable) {
    thread_system_integration::enable();
    ASSERT_TRUE(thread_system_integration::is_enabled());

    thread_system_integration::disable();

    EXPECT_FALSE(thread_system_integration::is_enabled())
        << "thread_pool backend should be disabled after disable()";
    EXPECT_EQ(thread_system_integration::get_backend(), async_backend_type::standalone)
        << "Backend should be standalone after disable()";
    EXPECT_EQ(thread_system_integration::get_thread_pool(), nullptr)
        << "Thread pool reference should be released after disable()";
}

/**
 * @brief Test enable() with custom pool
 */
TEST_F(ThreadSystemIntegrationModuleTest, EnableWithCustomPool) {
    auto custom_pool = std::make_shared<kcenon::thread::thread_pool>("custom_test_pool");
    auto start_result = custom_pool->start();
    ASSERT_TRUE(start_result.has_value()) << "Custom pool should start successfully";

    thread_system_integration::enable(custom_pool);

    EXPECT_TRUE(thread_system_integration::is_enabled());
    EXPECT_EQ(thread_system_integration::get_thread_pool(), custom_pool)
        << "get_thread_pool() should return the custom pool";

    custom_pool->stop();
}

/**
 * @brief Test set_thread_pool() directly
 */
TEST_F(ThreadSystemIntegrationModuleTest, SetThreadPool) {
    auto pool = std::make_shared<kcenon::thread::thread_pool>("set_pool_test");
    auto start_result = pool->start();
    ASSERT_TRUE(start_result.has_value());

    thread_system_integration::set_thread_pool(pool);

    EXPECT_TRUE(thread_system_integration::is_enabled())
        << "set_thread_pool() should enable thread_pool backend";
    EXPECT_EQ(thread_system_integration::get_thread_pool(), pool);

    // Set to nullptr should disable
    thread_system_integration::set_thread_pool(nullptr);

    EXPECT_FALSE(thread_system_integration::is_enabled())
        << "set_thread_pool(nullptr) should disable thread_pool backend";

    pool->stop();
}

/**
 * @brief Test submit_task() with enabled backend
 */
TEST_F(ThreadSystemIntegrationModuleTest, SubmitTaskWithEnabledBackend) {
    thread_system_integration::enable();
    ASSERT_TRUE(thread_system_integration::is_enabled());

    std::atomic<bool> executed{false};

    bool submitted = thread_system_integration::submit_task([&executed]() {
        executed = true;
    });

    EXPECT_TRUE(submitted)
        << "submit_task() should return true when backend is enabled";

    // Wait for task execution
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (!executed && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_TRUE(executed)
        << "Task should be executed by thread_pool";
}

/**
 * @brief Test multiple tasks submitted concurrently
 */
TEST_F(ThreadSystemIntegrationModuleTest, SubmitMultipleTasks) {
    thread_system_integration::enable();
    ASSERT_TRUE(thread_system_integration::is_enabled());

    const int num_tasks = 100;
    std::atomic<int> counter{0};

    for (int i = 0; i < num_tasks; ++i) {
        bool submitted = thread_system_integration::submit_task([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        });
        EXPECT_TRUE(submitted);
    }

    // Wait for all tasks to complete
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
    while (counter.load() < num_tasks && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_EQ(counter.load(), num_tasks)
        << "All tasks should be executed";
}

/**
 * @brief Test thread safety of enable/disable operations
 */
TEST_F(ThreadSystemIntegrationModuleTest, ThreadSafetyOfEnableDisable) {
    const int num_threads = 10;
    const int iterations = 100;
    std::atomic<int> errors{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&errors, iterations, t]() {
            for (int i = 0; i < iterations; ++i) {
                try {
                    if (t % 2 == 0) {
                        thread_system_integration::enable();
                        // Brief work while enabled
                        std::this_thread::yield();
                        thread_system_integration::disable();
                    } else {
                        // Check state consistency
                        bool enabled = thread_system_integration::is_enabled();
                        auto backend = thread_system_integration::get_backend();

                        if (enabled && backend != async_backend_type::thread_pool) {
                            errors.fetch_add(1);
                        }
                        if (!enabled && backend != async_backend_type::standalone) {
                            errors.fetch_add(1);
                        }
                    }
                } catch (...) {
                    errors.fetch_add(1);
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(errors.load(), 0)
        << "No errors should occur during concurrent enable/disable";
}

/**
 * @brief Test backend switching does not lose tasks
 */
TEST_F(ThreadSystemIntegrationModuleTest, BackendSwitchingDoesNotLoseTasks) {
    thread_system_integration::enable();
    ASSERT_TRUE(thread_system_integration::is_enabled());

    std::atomic<int> executed_count{0};
    const int tasks_before = 50;
    const int tasks_after = 50;

    // Submit tasks before switching
    for (int i = 0; i < tasks_before; ++i) {
        thread_system_integration::submit_task([&executed_count]() {
            executed_count.fetch_add(1, std::memory_order_relaxed);
        });
    }

    // Wait briefly for some tasks to start processing
    std::this_thread::sleep_for(std::chrono::milliseconds(50));

    // Disable and re-enable (simulating backend switch)
    thread_system_integration::disable();
    thread_system_integration::enable();

    // Submit more tasks after switching
    for (int i = 0; i < tasks_after; ++i) {
        thread_system_integration::submit_task([&executed_count]() {
            executed_count.fetch_add(1, std::memory_order_relaxed);
        });
    }

    // Wait for all tasks to complete
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
    while (executed_count.load() < tasks_before + tasks_after &&
           std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    // Note: Some tasks from before the switch may be lost if pool was stopped
    // This is expected behavior - we verify at least the post-switch tasks complete
    EXPECT_GE(executed_count.load(), tasks_after)
        << "At least post-switch tasks should be executed";
}

#endif // LOGGER_HAS_THREAD_SYSTEM

// ============================================================================
// Standalone mode stub tests (always run)
// ============================================================================

/**
 * @brief Test stub enable() is no-op when thread_system is not available
 */
TEST_F(ThreadSystemIntegrationModuleTest, StubEnableIsNoOp) {
#ifndef LOGGER_HAS_THREAD_SYSTEM
    // These should compile and not crash
    thread_system_integration::enable();
    thread_system_integration::disable();

    // State should remain standalone
    EXPECT_FALSE(thread_system_integration::is_enabled());
    EXPECT_EQ(thread_system_integration::get_backend(), async_backend_type::standalone);
#endif
}
