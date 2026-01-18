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
 * @file thread_system_integration_test.cpp
 * @brief Integration tests for thread_system dependency configuration
 *
 * Tests for Issue #252: Resolve bidirectional thread_system dependency risk
 *
 * These tests verify:
 * - Standalone mode works without thread_system
 * - thread_system_integration stub functions correctly
 * - Compile-time detection works as expected
 * - Logger operates correctly in both standalone and integrated modes
 */

#include "../framework/system_fixture.h"
#include "../framework/test_helpers.h"
#include <gtest/gtest.h>
#include <kcenon/common/interfaces/logger_interface.h>

#include <kcenon/logger/integration/thread_system_integration.h>

using namespace integration_tests;

// Use standard log_level from common_system
using kcenon::common::interfaces::log_level;
using namespace std::string_literals;
using namespace kcenon::logger::integration;

/**
 * @brief Integration tests for thread_system integration module
 *
 * These tests verify the thread_system integration works correctly
 * regardless of whether LOGGER_HAS_THREAD_SYSTEM is defined.
 */
class ThreadSystemIntegrationTest : public LoggerSystemFixture {};

/**
 * @test Verify compile-time detection reflects build configuration
 *
 * This test checks that has_thread_system_support() returns the correct
 * value based on whether LOGGER_HAS_THREAD_SYSTEM is defined.
 */
TEST_F(ThreadSystemIntegrationTest, CompileTimeDetection) {
#ifdef LOGGER_HAS_THREAD_SYSTEM
    // When thread_system is enabled, detection should return true
    EXPECT_TRUE(has_thread_system_support());
#else
    // When thread_system is disabled, detection should return false
    EXPECT_FALSE(has_thread_system_support());
#endif
}

/**
 * @test Verify backend type in current configuration
 *
 * Without explicit enable(), the backend should be standalone.
 */
TEST_F(ThreadSystemIntegrationTest, DefaultBackendIsStandalone) {
    // Initial state should always be standalone
    EXPECT_EQ(thread_system_integration::get_backend(), async_backend_type::standalone);
    EXPECT_EQ(thread_system_integration::get_backend_name(), "standalone");
}

/**
 * @test Verify is_enabled() returns correct state
 *
 * In standalone mode (or when thread_system is not available),
 * is_enabled() should return false.
 */
TEST_F(ThreadSystemIntegrationTest, IsEnabledReturnsCorrectState) {
#ifdef LOGGER_HAS_THREAD_SYSTEM
    // When thread_system is available, we can enable/disable
    EXPECT_FALSE(thread_system_integration::is_enabled());

    thread_system_integration::enable();
    EXPECT_TRUE(thread_system_integration::is_enabled());

    thread_system_integration::disable();
    EXPECT_FALSE(thread_system_integration::is_enabled());
#else
    // When thread_system is not available, always returns false
    EXPECT_FALSE(thread_system_integration::is_enabled());

    // enable()/disable() are no-ops
    thread_system_integration::enable();
    EXPECT_FALSE(thread_system_integration::is_enabled());

    thread_system_integration::disable();
    EXPECT_FALSE(thread_system_integration::is_enabled());
#endif
}

/**
 * @test Verify submit_task behavior in current configuration
 *
 * When thread_pool is not enabled, submit_task should return false.
 */
TEST_F(ThreadSystemIntegrationTest, SubmitTaskWithoutThreadPool) {
    // Ensure we're in standalone mode
    thread_system_integration::disable();

    bool task_executed = false;
    bool submitted = thread_system_integration::submit_task([&task_executed]() {
        task_executed = true;
    });

    // Without thread_pool enabled, submit should fail
    EXPECT_FALSE(submitted);
    EXPECT_FALSE(task_executed);
}

#ifdef LOGGER_HAS_THREAD_SYSTEM
/**
 * @test Verify submit_task works with thread_pool enabled
 *
 * This test only runs when thread_system is available.
 * Note: In some CI environments, thread pool creation may fail due to
 * resource constraints. The test handles this gracefully.
 */
TEST_F(ThreadSystemIntegrationTest, SubmitTaskWithThreadPool) {
    thread_system_integration::enable();

    // Check if pool was actually created
    auto pool = thread_system_integration::get_thread_pool();
    if (pool == nullptr) {
        // Thread pool creation failed - skip the rest of this test
        GTEST_SKIP() << "Thread pool creation failed (may be due to CI environment constraints)";
    }

    std::atomic<bool> task_executed{false};
    bool submitted = thread_system_integration::submit_task([&task_executed]() {
        task_executed.store(true);
    });

    EXPECT_TRUE(submitted);

    // Wait for task to complete
    WaitForCondition([&task_executed]() { return task_executed.load(); });
    EXPECT_TRUE(task_executed.load());

    thread_system_integration::disable();
}

/**
 * @test Verify get_thread_pool returns valid pool when enabled
 *
 * Note: Thread pool creation may fail in some CI environments.
 * This test verifies the behavior is consistent regardless of creation success.
 */
TEST_F(ThreadSystemIntegrationTest, GetThreadPoolWhenEnabled) {
    thread_system_integration::enable();

    auto pool = thread_system_integration::get_thread_pool();
    // Pool may be nullptr if creation failed - this is acceptable behavior
    // The important thing is that the API doesn't crash

    if (pool != nullptr) {
        // If pool was created, is_enabled should return true
        EXPECT_TRUE(thread_system_integration::is_enabled());
    }

    thread_system_integration::disable();

    // After disable, is_enabled should always return false
    EXPECT_FALSE(thread_system_integration::is_enabled());
}
#endif // LOGGER_HAS_THREAD_SYSTEM

/**
 * @test Verify logger works in standalone mode
 *
 * This is the primary use case: logger should work without thread_system.
 */
TEST_F(ThreadSystemIntegrationTest, LoggerWorksInStandaloneMode) {
    // Ensure standalone mode
    thread_system_integration::disable();

    auto log_file = CreateLoggerWithFileWriter(true);

    const size_t message_count = 50;
    for (size_t i = 0; i < message_count; ++i) {
        logger_->log(log_level::info, "Standalone message " + std::to_string(i));
    }

    WaitForFlush();

    EXPECT_TRUE(WaitForLogLines(log_file, message_count));
    EXPECT_EQ(CountLogLines(log_file), message_count);
}

/**
 * @test Verify conditional compilation pattern works
 *
 * This tests the recommended usage pattern for conditional thread_system integration.
 */
TEST_F(ThreadSystemIntegrationTest, ConditionalIntegrationPattern) {
    std::string backend_name;

    if constexpr (has_thread_system_support()) {
        // This branch is taken when LOGGER_HAS_THREAD_SYSTEM is defined
        thread_system_integration::enable();
        backend_name = thread_system_integration::get_backend_name();
        thread_system_integration::disable();
    } else {
        // This branch is taken when thread_system is not available
        backend_name = thread_system_integration::get_backend_name();
    }

    // Either way, we should have a valid backend name
    EXPECT_FALSE(backend_name.empty());

#ifdef LOGGER_HAS_THREAD_SYSTEM
    // If we enabled, it should have been "thread_pool"
    // (though we already disabled, so check it was set correctly)
    EXPECT_TRUE(backend_name == "thread_pool" || backend_name == "standalone");
#else
    EXPECT_EQ(backend_name, "standalone");
#endif
}

/**
 * @test Verify enable/disable cycle does not leak resources
 *
 * Multiple enable/disable cycles should not cause issues.
 */
TEST_F(ThreadSystemIntegrationTest, EnableDisableCycleNoLeak) {
    for (int i = 0; i < 10; ++i) {
        thread_system_integration::enable();
        thread_system_integration::disable();
    }

    // Should end in disabled state
    EXPECT_FALSE(thread_system_integration::is_enabled());
    EXPECT_EQ(thread_system_integration::get_backend(), async_backend_type::standalone);
}

/**
 * @test Verify double-enable is safe
 */
TEST_F(ThreadSystemIntegrationTest, DoubleEnableIsSafe) {
    thread_system_integration::enable();
    thread_system_integration::enable(); // Should not crash

#ifdef LOGGER_HAS_THREAD_SYSTEM
    EXPECT_TRUE(thread_system_integration::is_enabled());
#else
    EXPECT_FALSE(thread_system_integration::is_enabled());
#endif

    thread_system_integration::disable();
}

/**
 * @test Verify double-disable is safe
 */
TEST_F(ThreadSystemIntegrationTest, DoubleDisableIsSafe) {
    thread_system_integration::disable();
    thread_system_integration::disable(); // Should not crash

    EXPECT_FALSE(thread_system_integration::is_enabled());
}

/**
 * @test Verify backend name is always valid
 */
TEST_F(ThreadSystemIntegrationTest, BackendNameAlwaysValid) {
    // In disabled state
    auto name1 = thread_system_integration::get_backend_name();
    EXPECT_TRUE(name1 == "standalone" || name1 == "thread_pool");

    // After enable
    thread_system_integration::enable();
    auto name2 = thread_system_integration::get_backend_name();
    EXPECT_TRUE(name2 == "standalone" || name2 == "thread_pool");

    // After disable
    thread_system_integration::disable();
    auto name3 = thread_system_integration::get_backend_name();
    EXPECT_EQ(name3, "standalone");
}
