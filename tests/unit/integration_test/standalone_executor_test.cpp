// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file standalone_executor_test.cpp
 * @brief Unit tests for standalone_executor (IExecutor implementation)
 * @since 1.5.0
 *
 * Test coverage:
 * - Basic lifecycle (start, execute, shutdown)
 * - Job execution and completion
 * - Delayed execution
 * - Queue overflow and dropped count
 * - Exception propagation
 * - Graceful shutdown with flush
 * - Factory creation
 * - Thread safety
 */

#include <gtest/gtest.h>

#include <kcenon/logger/integration/standalone_executor.h>

#if LOGGER_HAS_IEXECUTOR

#include <atomic>
#include <chrono>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

using namespace kcenon::logger::integration;
namespace common = kcenon::common;

// =============================================================================
// Test Fixture
// =============================================================================

class StandaloneExecutorTest : public ::testing::Test {
protected:
    void SetUp() override {
        executor_ = std::make_unique<standalone_executor>(128, "test_executor");
    }

    void TearDown() override {
        if (executor_) {
            executor_->shutdown(true);
        }
    }

    std::unique_ptr<standalone_executor> executor_;
};

// =============================================================================
// Helper: counting job
// =============================================================================

class counting_job : public common::interfaces::IJob {
public:
    explicit counting_job(std::atomic<int>& counter)
        : counter_(counter) {}

    common::VoidResult execute() override {
        counter_.fetch_add(1);
        return common::ok();
    }

    std::string get_name() const override { return "counting_job"; }

private:
    std::atomic<int>& counter_;
};

class blocking_job : public common::interfaces::IJob {
public:
    explicit blocking_job(std::atomic<bool>& started, std::atomic<bool>& can_finish)
        : started_(started), can_finish_(can_finish) {}

    common::VoidResult execute() override {
        started_ = true;
        while (!can_finish_.load()) {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        return common::ok();
    }

    std::string get_name() const override { return "blocking_job"; }

private:
    std::atomic<bool>& started_;
    std::atomic<bool>& can_finish_;
};

class throwing_job : public common::interfaces::IJob {
public:
    common::VoidResult execute() override {
        throw std::runtime_error("test exception");
    }

    std::string get_name() const override { return "throwing_job"; }
};

// =============================================================================
// Lifecycle Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, InitialStateIsNotRunning) {
    EXPECT_FALSE(executor_->is_running());
    EXPECT_EQ(executor_->pending_tasks(), 0u);
    EXPECT_EQ(executor_->worker_count(), 1u);
}

TEST_F(StandaloneExecutorTest, StartSetsRunning) {
    executor_->start();
    EXPECT_TRUE(executor_->is_running());
}

TEST_F(StandaloneExecutorTest, DoubleStartIsIdempotent) {
    executor_->start();
    executor_->start();
    EXPECT_TRUE(executor_->is_running());
}

TEST_F(StandaloneExecutorTest, ShutdownStopsExecutor) {
    executor_->start();
    executor_->shutdown(true);
    EXPECT_FALSE(executor_->is_running());
}

TEST_F(StandaloneExecutorTest, DoubleShutdownIsIdempotent) {
    executor_->start();
    executor_->shutdown(true);
    executor_->shutdown(true);
    EXPECT_FALSE(executor_->is_running());
}

// =============================================================================
// Job Execution Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, ExecuteJobSuccessfully) {
    executor_->start();

    std::atomic<int> counter{0};
    auto job = std::make_unique<counting_job>(counter);

    auto result = executor_->execute(std::move(job));
    ASSERT_TRUE(result.is_ok());

    result.value().wait();
    EXPECT_EQ(counter.load(), 1);
}

TEST_F(StandaloneExecutorTest, ExecuteMultipleJobs) {
    executor_->start();

    std::atomic<int> counter{0};
    constexpr int num_jobs = 10;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < num_jobs; ++i) {
        auto job = std::make_unique<counting_job>(counter);
        auto result = executor_->execute(std::move(job));
        ASSERT_TRUE(result.is_ok());
        futures.push_back(std::move(result.value()));
    }

    for (auto& f : futures) {
        f.wait();
    }

    EXPECT_EQ(counter.load(), num_jobs);
}

TEST_F(StandaloneExecutorTest, ExecuteWhenNotRunningReturnsError) {
    std::atomic<int> counter{0};
    auto job = std::make_unique<counting_job>(counter);

    auto result = executor_->execute(std::move(job));
    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(counter.load(), 0);
}

// =============================================================================
// Function Job Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, FunctionJobExecutes) {
    executor_->start();

    std::atomic<bool> executed{false};
    auto job = std::make_unique<function_job>([&executed]() {
        executed = true;
    }, "test_func_job");

    EXPECT_EQ(job->get_name(), "test_func_job");

    auto result = executor_->execute(std::move(job));
    ASSERT_TRUE(result.is_ok());
    result.value().wait();

    EXPECT_TRUE(executed.load());
}

TEST_F(StandaloneExecutorTest, FunctionJobExceptionReturnsError) {
    function_job job([]() {
        throw std::runtime_error("function error");
    });

    auto result = job.execute();
    EXPECT_TRUE(result.is_err());
}

TEST_F(StandaloneExecutorTest, FunctionJobUnknownExceptionReturnsError) {
    function_job job([]() {
        throw 42; // non-std::exception throw
    });

    auto result = job.execute();
    EXPECT_TRUE(result.is_err());
}

// =============================================================================
// Exception Propagation Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, JobExceptionPropagatesViaFuture) {
    executor_->start();

    auto job = std::make_unique<throwing_job>();
    auto result = executor_->execute(std::move(job));
    ASSERT_TRUE(result.is_ok());

    EXPECT_THROW(result.value().get(), std::runtime_error);
}

// =============================================================================
// Delayed Execution Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, DelayedExecution) {
    executor_->start();

    std::atomic<int> counter{0};
    auto job = std::make_unique<counting_job>(counter);

    auto before = std::chrono::steady_clock::now();
    auto result = executor_->execute_delayed(
        std::move(job), std::chrono::milliseconds(50));
    ASSERT_TRUE(result.is_ok());

    result.value().wait();
    auto after = std::chrono::steady_clock::now();

    EXPECT_EQ(counter.load(), 1);
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(after - before);
    EXPECT_GE(elapsed.count(), 40); // Allow some timing slack
}

TEST_F(StandaloneExecutorTest, DelayedExecutionWhenNotRunningReturnsError) {
    std::atomic<int> counter{0};
    auto job = std::make_unique<counting_job>(counter);

    auto result = executor_->execute_delayed(
        std::move(job), std::chrono::milliseconds(10));
    EXPECT_TRUE(result.is_err());
}

// =============================================================================
// Queue Overflow Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, QueueOverflowIncreasesDroppedCount) {
    // Create executor with tiny queue
    auto small_executor = std::make_unique<standalone_executor>(2, "small");
    small_executor->start();

    // Block the worker so queue fills up
    std::atomic<bool> started{false};
    std::atomic<bool> can_finish{false};
    auto blocker = std::make_unique<blocking_job>(started, can_finish);
    small_executor->execute(std::move(blocker));

    // Wait for blocking job to start processing
    while (!started.load()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    // Now flood the queue - some should be dropped
    std::atomic<int> counter{0};
    int submitted = 0;
    int errors = 0;
    for (int i = 0; i < 20; ++i) {
        auto job = std::make_unique<counting_job>(counter);
        auto result = small_executor->execute(std::move(job));
        if (result.is_ok()) {
            submitted++;
        } else {
            errors++;
        }
    }

    // At least some should have been dropped or errored
    EXPECT_GT(small_executor->dropped_count() + errors, 0u);

    can_finish = true;
    small_executor->shutdown(true);
}

// =============================================================================
// Metadata Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, GetName) {
    EXPECT_EQ(executor_->get_name(), "test_executor");
}

TEST_F(StandaloneExecutorTest, WorkerCountIsAlwaysOne) {
    EXPECT_EQ(executor_->worker_count(), 1u);
    executor_->start();
    EXPECT_EQ(executor_->worker_count(), 1u);
}

TEST_F(StandaloneExecutorTest, InitialDroppedCountIsZero) {
    EXPECT_EQ(executor_->dropped_count(), 0u);
}

// =============================================================================
// Graceful Shutdown Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, ShutdownWithFlushProcessesRemaining) {
    executor_->start();

    std::atomic<int> counter{0};
    constexpr int num_jobs = 5;

    for (int i = 0; i < num_jobs; ++i) {
        auto job = std::make_unique<counting_job>(counter);
        executor_->execute(std::move(job));
    }

    executor_->shutdown(true); // wait_for_completion = true

    EXPECT_EQ(counter.load(), num_jobs);
}

// =============================================================================
// Factory Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, FactoryCreatesRunningExecutor) {
    auto executor = standalone_executor_factory::create(256, "factory_test");
    ASSERT_NE(executor, nullptr);
    EXPECT_TRUE(executor->is_running());
    executor->shutdown(true);
}

// =============================================================================
// Concurrent Submission Tests
// =============================================================================

TEST_F(StandaloneExecutorTest, ConcurrentJobSubmission) {
    executor_->start();

    std::atomic<int> counter{0};
    constexpr int num_threads = 4;
    constexpr int jobs_per_thread = 25;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([this, &counter]() {
            for (int i = 0; i < jobs_per_thread; ++i) {
                auto job = std::make_unique<counting_job>(counter);
                executor_->execute(std::move(job));
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    executor_->shutdown(true);

    // All successfully enqueued jobs should have executed
    int expected = num_threads * jobs_per_thread;
    int actual = counter.load() + static_cast<int>(executor_->dropped_count());
    EXPECT_EQ(actual, expected);
}

#else // !LOGGER_HAS_IEXECUTOR

TEST(StandaloneExecutorTest, SkippedWithoutIExecutor) {
    GTEST_SKIP() << "IExecutor interface not available";
}

#endif // LOGGER_HAS_IEXECUTOR
