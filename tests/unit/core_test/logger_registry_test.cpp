// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file logger_registry_test.cpp
 * @brief Unit tests for logger_registry (thread-safe registration/unregistration)
 * @since 2.0.0
 *
 * Test coverage:
 * - Basic registration and unregistration
 * - Null pointer handling
 * - Duplicate registration prevention
 * - Thread safety (concurrent registration/unregistration)
 * - Snapshot consistency
 */

#include <gtest/gtest.h>

#include <kcenon/logger/core/logger_registry.h>
#include <kcenon/logger/security/signal_manager.h>

#include <thread>
#include <vector>
#include <algorithm>
#include <atomic>

using namespace kcenon::logger::core;
using namespace kcenon::logger::security;

// =============================================================================
// Mock critical_logger_interface for testing
// =============================================================================

class mock_critical_logger : public critical_logger_interface {
public:
    explicit mock_critical_logger(int id = 0) : id_(id) {}
    ~mock_critical_logger() override = default;

    int get_emergency_fd() const override { return -1; }
    const char* get_emergency_buffer() const override { return nullptr; }
    size_t get_emergency_buffer_size() const override { return 0; }

    int id() const { return id_; }

private:
    int id_;
};

// =============================================================================
// Basic Operation Tests
// =============================================================================

TEST(LoggerRegistryTest, DefaultConstructionIsEmpty) {
    logger_registry registry;
    EXPECT_TRUE(registry.empty());
    EXPECT_EQ(registry.size(), 0u);
}

TEST(LoggerRegistryTest, RegisterSingleLogger) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.register_logger(&logger);

    EXPECT_FALSE(registry.empty());
    EXPECT_EQ(registry.size(), 1u);
}

TEST(LoggerRegistryTest, RegisterMultipleLoggers) {
    logger_registry registry;
    mock_critical_logger logger1(1);
    mock_critical_logger logger2(2);
    mock_critical_logger logger3(3);

    registry.register_logger(&logger1);
    registry.register_logger(&logger2);
    registry.register_logger(&logger3);

    EXPECT_EQ(registry.size(), 3u);
}

TEST(LoggerRegistryTest, UnregisterLogger) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.register_logger(&logger);
    EXPECT_EQ(registry.size(), 1u);

    registry.unregister_logger(&logger);
    EXPECT_TRUE(registry.empty());
    EXPECT_EQ(registry.size(), 0u);
}

TEST(LoggerRegistryTest, UnregisterFromMultiple) {
    logger_registry registry;
    mock_critical_logger logger1(1);
    mock_critical_logger logger2(2);
    mock_critical_logger logger3(3);

    registry.register_logger(&logger1);
    registry.register_logger(&logger2);
    registry.register_logger(&logger3);

    registry.unregister_logger(&logger2);

    EXPECT_EQ(registry.size(), 2u);

    auto loggers = registry.get_registered_loggers();
    EXPECT_EQ(loggers.size(), 2u);
    EXPECT_NE(std::find(loggers.begin(), loggers.end(), &logger1), loggers.end());
    EXPECT_NE(std::find(loggers.begin(), loggers.end(), &logger3), loggers.end());
    EXPECT_EQ(std::find(loggers.begin(), loggers.end(), &logger2), loggers.end());
}

// =============================================================================
// Null Pointer Handling
// =============================================================================

TEST(LoggerRegistryTest, RegisterNullPointerIsNoOp) {
    logger_registry registry;

    registry.register_logger(nullptr);

    EXPECT_TRUE(registry.empty());
    EXPECT_EQ(registry.size(), 0u);
}

TEST(LoggerRegistryTest, UnregisterNullPointerIsNoOp) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.register_logger(&logger);
    registry.unregister_logger(nullptr);

    EXPECT_EQ(registry.size(), 1u);
}

// =============================================================================
// Duplicate Prevention
// =============================================================================

TEST(LoggerRegistryTest, DuplicateRegistrationIsNoOp) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.register_logger(&logger);
    registry.register_logger(&logger);
    registry.register_logger(&logger);

    EXPECT_EQ(registry.size(), 1u);
}

// =============================================================================
// Unregister Edge Cases
// =============================================================================

TEST(LoggerRegistryTest, UnregisterNonRegisteredIsNoOp) {
    logger_registry registry;
    mock_critical_logger logger1(1);
    mock_critical_logger logger2(2);

    registry.register_logger(&logger1);
    registry.unregister_logger(&logger2);

    EXPECT_EQ(registry.size(), 1u);
}

TEST(LoggerRegistryTest, UnregisterFromEmptyIsNoOp) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.unregister_logger(&logger);

    EXPECT_TRUE(registry.empty());
}

TEST(LoggerRegistryTest, DoubleUnregisterIsNoOp) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.register_logger(&logger);
    registry.unregister_logger(&logger);
    registry.unregister_logger(&logger);

    EXPECT_TRUE(registry.empty());
}

// =============================================================================
// Snapshot (get_registered_loggers)
// =============================================================================

TEST(LoggerRegistryTest, GetRegisteredLoggersReturnsSnapshot) {
    logger_registry registry;
    mock_critical_logger logger1(1);
    mock_critical_logger logger2(2);

    registry.register_logger(&logger1);
    registry.register_logger(&logger2);

    auto snapshot = registry.get_registered_loggers();
    EXPECT_EQ(snapshot.size(), 2u);

    // Snapshot is independent of registry changes
    registry.unregister_logger(&logger1);
    EXPECT_EQ(snapshot.size(), 2u);
    EXPECT_EQ(registry.size(), 1u);
}

TEST(LoggerRegistryTest, GetRegisteredLoggersEmptyRegistry) {
    logger_registry registry;
    auto snapshot = registry.get_registered_loggers();
    EXPECT_TRUE(snapshot.empty());
}

// =============================================================================
// Re-registration After Unregistration
// =============================================================================

TEST(LoggerRegistryTest, ReRegisterAfterUnregister) {
    logger_registry registry;
    mock_critical_logger logger;

    registry.register_logger(&logger);
    registry.unregister_logger(&logger);
    EXPECT_TRUE(registry.empty());

    registry.register_logger(&logger);
    EXPECT_EQ(registry.size(), 1u);
}

// =============================================================================
// Thread Safety
// =============================================================================

TEST(LoggerRegistryTest, ConcurrentRegistration) {
    logger_registry registry;
    constexpr int num_threads = 8;
    constexpr int loggers_per_thread = 100;

    // Each thread has its own set of loggers
    std::vector<std::vector<mock_critical_logger>> thread_loggers(num_threads);
    for (int t = 0; t < num_threads; ++t) {
        thread_loggers[t].reserve(loggers_per_thread);
        for (int i = 0; i < loggers_per_thread; ++i) {
            thread_loggers[t].emplace_back(t * loggers_per_thread + i);
        }
    }

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&registry, &thread_loggers, t]() {
            for (auto& logger : thread_loggers[t]) {
                registry.register_logger(&logger);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_EQ(registry.size(), static_cast<size_t>(num_threads * loggers_per_thread));
}

TEST(LoggerRegistryTest, ConcurrentRegistrationAndUnregistration) {
    logger_registry registry;
    constexpr int num_loggers = 50;

    std::vector<mock_critical_logger> loggers;
    loggers.reserve(num_loggers);
    for (int i = 0; i < num_loggers; ++i) {
        loggers.emplace_back(i);
    }

    // Register all first
    for (auto& logger : loggers) {
        registry.register_logger(&logger);
    }
    EXPECT_EQ(registry.size(), static_cast<size_t>(num_loggers));

    // Concurrently unregister from multiple threads
    std::vector<std::thread> threads;
    for (int i = 0; i < num_loggers; ++i) {
        threads.emplace_back([&registry, &loggers, i]() {
            registry.unregister_logger(&loggers[i]);
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    EXPECT_TRUE(registry.empty());
}

TEST(LoggerRegistryTest, ConcurrentReadsWhileWriting) {
    logger_registry registry;
    constexpr int num_loggers = 20;
    std::atomic<bool> done{false};

    std::vector<mock_critical_logger> loggers;
    loggers.reserve(num_loggers);
    for (int i = 0; i < num_loggers; ++i) {
        loggers.emplace_back(i);
    }

    // Writer thread
    std::thread writer([&]() {
        for (auto& logger : loggers) {
            registry.register_logger(&logger);
            std::this_thread::yield();
        }
        done = true;
    });

    // Reader threads
    std::vector<std::thread> readers;
    for (int r = 0; r < 4; ++r) {
        readers.emplace_back([&]() {
            while (!done.load()) {
                auto snapshot = registry.get_registered_loggers();
                auto sz = registry.size();
                auto is_empty = registry.empty();
                (void)snapshot;
                (void)sz;
                (void)is_empty;
                std::this_thread::yield();
            }
        });
    }

    writer.join();
    for (auto& reader : readers) {
        reader.join();
    }

    EXPECT_EQ(registry.size(), static_cast<size_t>(num_loggers));
}
