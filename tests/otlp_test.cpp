/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file otlp_test.cpp
 * @brief Unit tests for OpenTelemetry integration
 * @since 3.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/otlp/otel_context.h>
#include <kcenon/logger/writers/otlp_writer.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <chrono>
#include <thread>
#include <vector>
#include <future>

namespace kcenon::logger::test {

// ============================================================================
// otel_context Tests
// ============================================================================

class OtelContextTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Clear any existing context before each test
        otlp::otel_context_storage::clear();
    }

    void TearDown() override {
        otlp::otel_context_storage::clear();
    }
};

TEST_F(OtelContextTest, DefaultContextIsEmpty) {
    EXPECT_FALSE(otlp::otel_context_storage::has_context());
    EXPECT_FALSE(otlp::otel_context_storage::get().has_value());
}

TEST_F(OtelContextTest, SetAndGetContext) {
    otlp::otel_context ctx{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331",
        .trace_flags = "01",
        .trace_state = "congo=t61rcWkgMzE"
    };

    otlp::otel_context_storage::set(ctx);

    EXPECT_TRUE(otlp::otel_context_storage::has_context());

    auto retrieved = otlp::otel_context_storage::get();
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->trace_id, "0af7651916cd43dd8448eb211c80319c");
    EXPECT_EQ(retrieved->span_id, "b7ad6b7169203331");
    EXPECT_EQ(retrieved->trace_flags, "01");
    EXPECT_EQ(retrieved->trace_state, "congo=t61rcWkgMzE");
}

TEST_F(OtelContextTest, ClearContext) {
    otlp::otel_context ctx{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331"
    };

    otlp::otel_context_storage::set(ctx);
    EXPECT_TRUE(otlp::otel_context_storage::has_context());

    otlp::otel_context_storage::clear();
    EXPECT_FALSE(otlp::otel_context_storage::has_context());
    EXPECT_FALSE(otlp::otel_context_storage::get().has_value());
}

TEST_F(OtelContextTest, ContextValidation) {
    // Valid context
    otlp::otel_context valid{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331"
    };
    EXPECT_TRUE(valid.is_valid());

    // Invalid trace_id (too short)
    otlp::otel_context invalid_trace{
        .trace_id = "0af765",
        .span_id = "b7ad6b7169203331"
    };
    EXPECT_FALSE(invalid_trace.is_valid());

    // Invalid span_id (too short)
    otlp::otel_context invalid_span{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad"
    };
    EXPECT_FALSE(invalid_span.is_valid());

    // Invalid hex characters
    otlp::otel_context invalid_hex{
        .trace_id = "0af7651916cd43dd8448eb211c80319z",
        .span_id = "b7ad6b7169203331"
    };
    EXPECT_FALSE(invalid_hex.is_valid());

    // Empty context
    otlp::otel_context empty = otlp::otel_context::empty();
    EXPECT_FALSE(empty.is_valid());
}

TEST_F(OtelContextTest, SampledFlagCheck) {
    otlp::otel_context sampled{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331",
        .trace_flags = "01"
    };
    EXPECT_TRUE(sampled.is_sampled());

    otlp::otel_context not_sampled{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331",
        .trace_flags = "00"
    };
    EXPECT_FALSE(not_sampled.is_sampled());

    otlp::otel_context no_flags{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331"
    };
    EXPECT_FALSE(no_flags.is_sampled());
}

TEST_F(OtelContextTest, ThreadLocalIsolation) {
    otlp::otel_context main_ctx{
        .trace_id = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
        .span_id = "aaaaaaaaaaaaaaaa"
    };

    otlp::otel_context_storage::set(main_ctx);

    std::promise<bool> other_thread_has_context;
    std::promise<std::string> other_thread_trace_id;

    std::thread other_thread([&]() {
        // Other thread should not see main thread's context
        other_thread_has_context.set_value(otlp::otel_context_storage::has_context());

        // Set different context in other thread
        otlp::otel_context other_ctx{
            .trace_id = "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb",
            .span_id = "bbbbbbbbbbbbbbbb"
        };
        otlp::otel_context_storage::set(other_ctx);

        auto ctx = otlp::otel_context_storage::get();
        other_thread_trace_id.set_value(ctx ? ctx->trace_id : "");
    });

    other_thread.join();

    // Other thread should not have had main thread's context initially
    EXPECT_FALSE(other_thread_has_context.get_future().get());

    // Other thread should have had its own context
    EXPECT_EQ(other_thread_trace_id.get_future().get(), "bbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");

    // Main thread context should be unchanged
    auto main_retrieved = otlp::otel_context_storage::get();
    ASSERT_TRUE(main_retrieved.has_value());
    EXPECT_EQ(main_retrieved->trace_id, "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
}

// ============================================================================
// otel_context_scope Tests
// ============================================================================

TEST_F(OtelContextTest, ScopeGuardSetsAndRestores) {
    // No context initially
    EXPECT_FALSE(otlp::otel_context_storage::has_context());

    {
        otlp::otel_context ctx{
            .trace_id = "cccccccccccccccccccccccccccccccc",
            .span_id = "cccccccccccccccc"
        };

        otlp::otel_context_scope scope(ctx);

        // Context should be set
        EXPECT_TRUE(otlp::otel_context_storage::has_context());
        auto retrieved = otlp::otel_context_storage::get();
        ASSERT_TRUE(retrieved.has_value());
        EXPECT_EQ(retrieved->trace_id, "cccccccccccccccccccccccccccccccc");
    }

    // Context should be cleared after scope ends
    EXPECT_FALSE(otlp::otel_context_storage::has_context());
}

TEST_F(OtelContextTest, NestedScopeGuards) {
    otlp::otel_context outer_ctx{
        .trace_id = "dddddddddddddddddddddddddddddddd",
        .span_id = "dddddddddddddddd"
    };

    {
        otlp::otel_context_scope outer_scope(outer_ctx);
        EXPECT_EQ(otlp::otel_context_storage::get()->trace_id,
                  "dddddddddddddddddddddddddddddddd");

        otlp::otel_context inner_ctx{
            .trace_id = "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee",
            .span_id = "eeeeeeeeeeeeeeee"
        };

        {
            otlp::otel_context_scope inner_scope(inner_ctx);
            EXPECT_EQ(otlp::otel_context_storage::get()->trace_id,
                      "eeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
        }

        // After inner scope, outer context should be restored
        EXPECT_EQ(otlp::otel_context_storage::get()->trace_id,
                  "dddddddddddddddddddddddddddddddd");
    }

    // After outer scope, no context
    EXPECT_FALSE(otlp::otel_context_storage::has_context());
}

// ============================================================================
// otlp_writer Tests
// ============================================================================

class OtlpWriterTest : public ::testing::Test {
protected:
    void SetUp() override {
        otlp::otel_context_storage::clear();
    }

    void TearDown() override {
        otlp::otel_context_storage::clear();
    }
};

TEST_F(OtlpWriterTest, ConfigurationDefaults) {
    otlp_writer::config cfg;

    EXPECT_EQ(cfg.endpoint, "http://localhost:4318/v1/logs");
    EXPECT_EQ(cfg.protocol, otlp_writer::protocol_type::http);
    EXPECT_EQ(cfg.timeout, std::chrono::milliseconds(5000));
    EXPECT_FALSE(cfg.use_tls);
    EXPECT_EQ(cfg.max_batch_size, 512);
    EXPECT_EQ(cfg.flush_interval, std::chrono::milliseconds(5000));
    EXPECT_EQ(cfg.max_queue_size, 10000);
    EXPECT_EQ(cfg.max_retries, 3);
}

TEST_F(OtlpWriterTest, CreateWriter) {
    otlp_writer::config cfg{
        .endpoint = "http://localhost:4318/v1/logs",
        .service_name = "test-service",
        .service_version = "1.0.0"
    };

    // Should not throw
    EXPECT_NO_THROW({
        otlp_writer writer(cfg);
        EXPECT_EQ(writer.get_name(), "otlp");
        EXPECT_TRUE(writer.is_healthy());
    });
}

TEST_F(OtlpWriterTest, WriteLogsQueued) {
    otlp_writer::config cfg{
        .endpoint = "http://localhost:4318/v1/logs",
        .service_name = "test-service",
        .max_batch_size = 100,
        .flush_interval = std::chrono::milliseconds(100000)  // Long interval
    };

    otlp_writer writer(cfg);

    // Write some logs
    auto now = std::chrono::system_clock::now();
    for (int i = 0; i < 10; ++i) {
        log_entry entry(kcenon::common::interfaces::log_level::info,
                       "Test message " + std::to_string(i),
                       __FILE__,
                       __LINE__,
                       __FUNCTION__,
                       now);
        auto result = writer.write(entry);
        EXPECT_TRUE(result.is_ok());
    }

    // Stats should show no exports yet (batch not full, interval not reached)
    auto stats = writer.get_stats();
    EXPECT_EQ(stats.logs_dropped, 0);
}

TEST_F(OtlpWriterTest, FlushWritesImmediately) {
    otlp_writer::config cfg{
        .endpoint = "http://localhost:4318/v1/logs",
        .service_name = "test-service"
    };

    otlp_writer writer(cfg);

    // Write and flush
    auto now = std::chrono::system_clock::now();
    {
        log_entry entry(kcenon::common::interfaces::log_level::error,
                       "Error message",
                       __FILE__,
                       __LINE__,
                       __FUNCTION__,
                       now);
        writer.write(entry);
    }

    auto result = writer.flush();
    EXPECT_TRUE(result.is_ok());
}

TEST_F(OtlpWriterTest, WriteWithOtelContext) {
    otlp_writer::config cfg{
        .endpoint = "http://localhost:4318/v1/logs",
        .service_name = "test-service"
    };

    otlp_writer writer(cfg);

    // Set OTEL context
    otlp::otel_context ctx{
        .trace_id = "ffffffffffffffffffffffffffffffff",
        .span_id = "ffffffffffffffff",
        .trace_flags = "01"
    };
    otlp::otel_context_storage::set(ctx);

    // Write log - should pick up context
    auto now = std::chrono::system_clock::now();
    log_entry entry(kcenon::common::interfaces::log_level::info,
                   "Message with trace context",
                   __FILE__,
                   __LINE__,
                   __FUNCTION__,
                   now);
    auto result = writer.write(entry);

    EXPECT_TRUE(result.is_ok());
    writer.flush();
}

TEST_F(OtlpWriterTest, WriteLogEntry) {
    otlp_writer::config cfg{
        .endpoint = "http://localhost:4318/v1/logs",
        .service_name = "test-service"
    };

    otlp_writer writer(cfg);

    // Create log entry with OTEL context
    log_entry entry(kcenon::common::interfaces::log_level::warn, "Warning message");
    entry.otel_ctx = otlp::otel_context{
        .trace_id = "11111111111111111111111111111111",
        .span_id = "1111111111111111"
    };

    auto result = writer.write(entry);
    EXPECT_TRUE(result.is_ok());
}

// ============================================================================
// Logger OTEL Context Integration Tests
// ============================================================================

class LoggerOtelIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        otlp::otel_context_storage::clear();
    }

    void TearDown() override {
        otlp::otel_context_storage::clear();
    }
};

TEST_F(LoggerOtelIntegrationTest, LoggerContextMethods) {
    logger log(false, 8192);  // Synchronous mode

    // Initially no context
    EXPECT_FALSE(log.has_otel_context());

    // Set context
    otlp::otel_context ctx{
        .trace_id = "22222222222222222222222222222222",
        .span_id = "2222222222222222"
    };
    log.set_otel_context(ctx);

    EXPECT_TRUE(log.has_otel_context());

    auto retrieved = log.get_otel_context();
    ASSERT_TRUE(retrieved.has_value());
    EXPECT_EQ(retrieved->trace_id, "22222222222222222222222222222222");

    // Clear context
    log.clear_otel_context();
    EXPECT_FALSE(log.has_otel_context());
}

TEST_F(LoggerOtelIntegrationTest, LogEntryHasOtelContext) {
    // When OTEL context is set, log_entry should be able to hold it
    log_entry entry(kcenon::common::interfaces::log_level::info, "Test message");

    // Initially no OTEL context
    EXPECT_FALSE(entry.otel_ctx.has_value());

    // Set OTEL context
    entry.otel_ctx = otlp::otel_context{
        .trace_id = "33333333333333333333333333333333",
        .span_id = "3333333333333333"
    };

    EXPECT_TRUE(entry.otel_ctx.has_value());
    EXPECT_EQ(entry.otel_ctx->trace_id, "33333333333333333333333333333333");
}

// ============================================================================
// Concurrent Access Tests
// ============================================================================

TEST_F(OtlpWriterTest, ConcurrentWrites) {
    otlp_writer::config cfg{
        .endpoint = "http://localhost:4318/v1/logs",
        .service_name = "test-service",
        .max_batch_size = 1000,
        .max_queue_size = 100000
    };

    otlp_writer writer(cfg);

    constexpr int num_threads = 4;
    constexpr int logs_per_thread = 100;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&writer, t]() {
            auto now = std::chrono::system_clock::now();
            for (int i = 0; i < logs_per_thread; ++i) {
                log_entry entry(kcenon::common::interfaces::log_level::info,
                               "Thread " + std::to_string(t) + " message " + std::to_string(i),
                               __FILE__,
                               __LINE__,
                               __FUNCTION__,
                               now);
                writer.write(entry);
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }

    // Should not have dropped any logs
    auto stats = writer.get_stats();
    EXPECT_EQ(stats.logs_dropped, 0);

    writer.flush();
}

TEST_F(OtelContextTest, ConcurrentContextOperations) {
    constexpr int num_threads = 4;
    constexpr int iterations = 100;

    std::vector<std::thread> threads;
    threads.reserve(num_threads);

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([t, iterations]() {
            for (int i = 0; i < iterations; ++i) {
                std::string trace_id(32, '0' + (t % 10));
                std::string span_id(16, '0' + (t % 10));

                otlp::otel_context ctx{
                    .trace_id = trace_id,
                    .span_id = span_id
                };

                otlp::otel_context_storage::set(ctx);

                auto retrieved = otlp::otel_context_storage::get();
                ASSERT_TRUE(retrieved.has_value());
                EXPECT_EQ(retrieved->trace_id, trace_id);

                otlp::otel_context_storage::clear();
            }
        });
    }

    for (auto& t : threads) {
        t.join();
    }
}

} // namespace kcenon::logger::test
