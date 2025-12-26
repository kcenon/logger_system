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
 * @file executor_integration_test.cpp
 * @brief Unit tests for the IExecutor-based integration
 * @author kcenon
 * @since 1.5.0
 *
 * @details This file tests the executor_integration module which provides
 * IExecutor-based async processing using the common_system interface.
 *
 * Test coverage:
 * - Default disabled state
 * - Enable with default standalone executor
 * - Enable with custom executor
 * - Task submission and execution
 * - Executor type detection
 * - Thread safety
 *
 * @note Part of Issue #253: Migrate to IExecutor interface
 */

#include <gtest/gtest.h>
#include <kcenon/logger/integration/executor_integration.h>
#include <kcenon/logger/integration/standalone_executor.h>

#include <atomic>
#include <chrono>
#include <thread>
#include <vector>

#if LOGGER_HAS_IEXECUTOR

using namespace kcenon::logger::integration;

/**
 * @brief Test fixture for executor_integration tests
 */
class ExecutorIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Ensure we start in a known state (disabled)
        executor_integration::disable();
    }

    void TearDown() override {
        // Clean up after each test
        executor_integration::disable();
    }
};

// ============================================================================
// Basic State Tests
// ============================================================================

/**
 * @brief Test has_executor_support() compile-time detection
 */
TEST_F(ExecutorIntegrationTest, HasExecutorSupport) {
    EXPECT_TRUE(has_executor_support())
        << "has_executor_support() should return true when LOGGER_HAS_IEXECUTOR is defined";
}

/**
 * @brief Test default state is disabled
 */
TEST_F(ExecutorIntegrationTest, DefaultStateIsDisabled) {
    EXPECT_FALSE(executor_integration::is_enabled())
        << "Executor should be disabled by default";
    EXPECT_EQ(executor_integration::get_executor_type(), executor_type::none)
        << "Executor type should be 'none' by default";
    EXPECT_EQ(executor_integration::get_executor_name(), "none")
        << "Executor name should be 'none' by default";
    EXPECT_EQ(executor_integration::get_executor(), nullptr)
        << "get_executor() should return nullptr when disabled";
}

/**
 * @brief Test submit_task returns false when disabled
 */
TEST_F(ExecutorIntegrationTest, SubmitTaskWhenDisabled) {
    std::atomic<bool> executed{false};

    bool submitted = executor_integration::submit_task([&executed]() {
        executed = true;
    });

    EXPECT_FALSE(submitted) << "submit_task() should return false when disabled";
    EXPECT_FALSE(executed) << "Task should not be executed when disabled";
}

// ============================================================================
// Enable/Disable Tests
// ============================================================================

/**
 * @brief Test enable() with default standalone executor
 */
TEST_F(ExecutorIntegrationTest, EnableWithDefaultExecutor) {
    executor_integration::enable();

    EXPECT_TRUE(executor_integration::is_enabled())
        << "Should be enabled after enable()";
    EXPECT_EQ(executor_integration::get_executor_type(), executor_type::standalone)
        << "Default executor type should be standalone";
    EXPECT_EQ(executor_integration::get_executor_name(), "standalone")
        << "Default executor name should be 'standalone'";

    auto executor = executor_integration::get_executor();
    EXPECT_NE(executor, nullptr) << "get_executor() should return valid executor";
    EXPECT_TRUE(executor->is_running()) << "Executor should be running";
    EXPECT_GE(executor->worker_count(), 1u) << "Should have at least 1 worker";
}

/**
 * @brief Test disable() after enable()
 */
TEST_F(ExecutorIntegrationTest, DisableAfterEnable) {
    executor_integration::enable();
    ASSERT_TRUE(executor_integration::is_enabled());

    executor_integration::disable();

    EXPECT_FALSE(executor_integration::is_enabled())
        << "Should be disabled after disable()";
    EXPECT_EQ(executor_integration::get_executor_type(), executor_type::none)
        << "Executor type should be 'none' after disable()";
    EXPECT_EQ(executor_integration::get_executor(), nullptr)
        << "get_executor() should return nullptr after disable()";
}

/**
 * @brief Test enable() with custom executor
 */
TEST_F(ExecutorIntegrationTest, EnableWithCustomExecutor) {
    auto custom_executor = standalone_executor_factory::create(1024, "custom_test_executor");

    executor_integration::enable(custom_executor);

    EXPECT_TRUE(executor_integration::is_enabled());
    EXPECT_EQ(executor_integration::get_executor_type(), executor_type::external)
        << "Custom executor should be detected as external";
    EXPECT_EQ(executor_integration::get_executor(), custom_executor)
        << "get_executor() should return the custom executor";
}

/**
 * @brief Test set_executor() directly
 */
TEST_F(ExecutorIntegrationTest, SetExecutor) {
    auto executor = standalone_executor_factory::create();

    executor_integration::set_executor(executor);

    EXPECT_TRUE(executor_integration::is_enabled())
        << "set_executor() should enable integration";
    EXPECT_EQ(executor_integration::get_executor(), executor);

    // Set to nullptr should disable
    executor_integration::set_executor(nullptr);

    EXPECT_FALSE(executor_integration::is_enabled())
        << "set_executor(nullptr) should disable integration";
}

// ============================================================================
// Task Submission Tests
// ============================================================================

/**
 * @brief Test submit_task() with enabled executor
 */
TEST_F(ExecutorIntegrationTest, SubmitTaskWithEnabledExecutor) {
    executor_integration::enable();
    ASSERT_TRUE(executor_integration::is_enabled());

    std::atomic<bool> executed{false};

    bool submitted = executor_integration::submit_task([&executed]() {
        executed = true;
    });

    EXPECT_TRUE(submitted) << "submit_task() should return true when enabled";

    // Wait for task execution
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (!executed && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::yield();
    }

    EXPECT_TRUE(executed) << "Task should be executed by executor";
}

/**
 * @brief Test multiple tasks submitted concurrently
 */
TEST_F(ExecutorIntegrationTest, SubmitMultipleTasks) {
    executor_integration::enable();
    ASSERT_TRUE(executor_integration::is_enabled());

    const int num_tasks = 100;
    std::atomic<int> counter{0};

    for (int i = 0; i < num_tasks; ++i) {
        bool submitted = executor_integration::submit_task([&counter]() {
            counter.fetch_add(1, std::memory_order_relaxed);
        });
        EXPECT_TRUE(submitted);
    }

    // Wait for all tasks to complete
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
    while (counter.load() < num_tasks && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_EQ(counter.load(), num_tasks) << "All tasks should be executed";
}

/**
 * @brief Test delayed task submission
 */
TEST_F(ExecutorIntegrationTest, SubmitDelayedTask) {
    executor_integration::enable();
    ASSERT_TRUE(executor_integration::is_enabled());

    std::atomic<bool> executed{false};
    auto start = std::chrono::steady_clock::now();

    bool submitted = executor_integration::submit_task_delayed(
        [&executed]() { executed = true; },
        std::chrono::milliseconds(100)
    );

    EXPECT_TRUE(submitted) << "submit_task_delayed() should return true when enabled";

    // Wait for task execution
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(5);
    while (!executed && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::yield();
    }

    auto elapsed = std::chrono::steady_clock::now() - start;

    EXPECT_TRUE(executed) << "Delayed task should be executed";
    EXPECT_GE(elapsed, std::chrono::milliseconds(90))
        << "Task should be delayed by approximately the specified duration";
}

// ============================================================================
// Standalone Executor Tests
// ============================================================================

/**
 * @brief Test standalone_executor basic functionality
 */
TEST_F(ExecutorIntegrationTest, StandaloneExecutorBasics) {
    auto executor = std::make_shared<standalone_executor>(1024, "test_executor");

    EXPECT_FALSE(executor->is_running()) << "Should not be running before start()";
    EXPECT_EQ(executor->worker_count(), 0u) << "No workers before start()";
    EXPECT_EQ(executor->pending_tasks(), 0u) << "No pending tasks initially";

    executor->start();

    EXPECT_TRUE(executor->is_running()) << "Should be running after start()";
    EXPECT_EQ(executor->worker_count(), 1u) << "Standalone has 1 worker";

    executor->shutdown();

    EXPECT_FALSE(executor->is_running()) << "Should not be running after shutdown()";
}

/**
 * @brief Test standalone_executor job execution
 */
TEST_F(ExecutorIntegrationTest, StandaloneExecutorJobExecution) {
    auto executor = std::make_shared<standalone_executor>();
    executor->start();

    std::atomic<bool> executed{false};
    auto job = std::make_unique<function_job>([&executed]() {
        executed = true;
    });

    auto result = executor->execute(std::move(job));
    EXPECT_TRUE(result.is_ok()) << "execute() should succeed";

    // Wait for the future
    auto future = std::move(result.unwrap());
    EXPECT_NO_THROW(future.get()) << "Future should complete without exception";
    EXPECT_TRUE(executed) << "Job should be executed";

    executor->shutdown();
}

/**
 * @brief Test standalone_executor factory
 */
TEST_F(ExecutorIntegrationTest, StandaloneExecutorFactory) {
    auto executor = standalone_executor_factory::create(4096, "factory_test");

    EXPECT_NE(executor, nullptr) << "Factory should create executor";
    EXPECT_TRUE(executor->is_running()) << "Factory-created executor should be running";

    executor->shutdown();
}

// ============================================================================
// Thread Safety Tests
// ============================================================================

/**
 * @brief Test thread safety of state checking operations
 *
 * Note: Concurrent enable/disable can cause deadlock due to executor shutdown
 * blocking. This test focuses on safe concurrent operations.
 */
TEST_F(ExecutorIntegrationTest, ThreadSafetyOfStateChecking) {
    // Enable once, then test concurrent state checks and task submissions
    executor_integration::enable();
    ASSERT_TRUE(executor_integration::is_enabled());

    const int num_threads = 10;
    const int iterations = 100;
    std::atomic<int> errors{0};
    std::atomic<int> task_count{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&errors, &task_count, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                try {
                    // Check state consistency
                    bool enabled = executor_integration::is_enabled();
                    auto type = executor_integration::get_executor_type();

                    if (enabled && type == executor_type::none) {
                        errors.fetch_add(1);
                    }

                    // Submit tasks concurrently
                    if (enabled) {
                        executor_integration::submit_task([&task_count]() {
                            task_count.fetch_add(1, std::memory_order_relaxed);
                        });
                    }

                    std::this_thread::yield();
                } catch (...) {
                    errors.fetch_add(1);
                }
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(errors.load(), 0) << "No errors should occur during concurrent operations";
    EXPECT_GT(task_count.load(), 0) << "Some tasks should have been executed";
}

/**
 * @brief Test concurrent task submission
 */
TEST_F(ExecutorIntegrationTest, ConcurrentTaskSubmission) {
    executor_integration::enable();
    ASSERT_TRUE(executor_integration::is_enabled());

    const int num_threads = 5;
    const int tasks_per_thread = 50;
    std::atomic<int> counter{0};
    std::vector<std::thread> threads;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&counter, tasks_per_thread]() {
            for (int i = 0; i < tasks_per_thread; ++i) {
                executor_integration::submit_task([&counter]() {
                    counter.fetch_add(1, std::memory_order_relaxed);
                });
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Wait for all tasks to complete
    auto deadline = std::chrono::steady_clock::now() + std::chrono::seconds(10);
    const int total_tasks = num_threads * tasks_per_thread;
    while (counter.load() < total_tasks && std::chrono::steady_clock::now() < deadline) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    EXPECT_EQ(counter.load(), total_tasks) << "All concurrently submitted tasks should complete";
}

// ============================================================================
// Metrics Tests
// ============================================================================

/**
 * @brief Test pending_tasks() and worker_count()
 */
TEST_F(ExecutorIntegrationTest, MetricsMethods) {
    EXPECT_EQ(executor_integration::pending_tasks(), 0u)
        << "pending_tasks() should be 0 when disabled";
    EXPECT_EQ(executor_integration::worker_count(), 0u)
        << "worker_count() should be 0 when disabled";

    executor_integration::enable();

    EXPECT_GE(executor_integration::worker_count(), 1u)
        << "worker_count() should be >= 1 when enabled";
}

#endif // LOGGER_HAS_IEXECUTOR
