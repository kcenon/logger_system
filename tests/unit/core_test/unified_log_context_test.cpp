/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>

#include "kcenon/logger/core/unified_log_context.h"

#include <future>
#include <thread>
#include <vector>

using namespace kcenon::logger;

/**
 * @brief Test fixture for unified_log_context tests
 */
class UnifiedLogContextTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// =========================================================================
// Basic Set/Get Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, DefaultConstructorCreatesEmptyContext) {
    unified_log_context ctx;

    EXPECT_TRUE(ctx.empty());
    EXPECT_EQ(ctx.size(), 0);
}

TEST_F(UnifiedLogContextTest, SetAndGetStringValue) {
    unified_log_context ctx;

    ctx.set("key", std::string("value"));

    auto result = ctx.get_as<std::string>("key");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, "value");
}

TEST_F(UnifiedLogContextTest, SetAndGetIntValue) {
    unified_log_context ctx;

    ctx.set("user_id", int64_t{12345});

    auto result = ctx.get_as<int64_t>("user_id");
    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(*result, 12345);
}

TEST_F(UnifiedLogContextTest, SetAndGetDoubleValue) {
    unified_log_context ctx;

    ctx.set("ratio", 3.14159);

    auto result = ctx.get_as<double>("ratio");
    ASSERT_TRUE(result.has_value());
    EXPECT_DOUBLE_EQ(*result, 3.14159);
}

TEST_F(UnifiedLogContextTest, SetAndGetBoolValue) {
    unified_log_context ctx;

    ctx.set("enabled", true);

    auto result = ctx.get_as<bool>("enabled");
    ASSERT_TRUE(result.has_value());
    EXPECT_TRUE(*result);
}

TEST_F(UnifiedLogContextTest, GetNonexistentKeyReturnsEmpty) {
    unified_log_context ctx;

    auto result = ctx.get("nonexistent");
    EXPECT_FALSE(result.has_value());
}

TEST_F(UnifiedLogContextTest, GetAsWrongTypeReturnsEmpty) {
    unified_log_context ctx;
    ctx.set("key", std::string("value"));

    auto result = ctx.get_as<int64_t>("key");
    EXPECT_FALSE(result.has_value());
}

TEST_F(UnifiedLogContextTest, GetStringReturnsDefaultForNonexistent) {
    unified_log_context ctx;

    auto result = ctx.get_string("nonexistent", "default");
    EXPECT_EQ(result, "default");
}

TEST_F(UnifiedLogContextTest, GetStringReturnsDefaultForWrongType) {
    unified_log_context ctx;
    ctx.set("key", int64_t{123});

    auto result = ctx.get_string("key", "default");
    EXPECT_EQ(result, "default");
}

// =========================================================================
// Trace Context Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, SetTraceContext) {
    unified_log_context ctx;

    ctx.set_trace("trace123", "span456", "parentspan789");

    EXPECT_EQ(ctx.get_string("trace_id"), "trace123");
    EXPECT_EQ(ctx.get_string("span_id"), "span456");
    EXPECT_EQ(ctx.get_string("parent_span_id"), "parentspan789");
}

TEST_F(UnifiedLogContextTest, SetTraceContextWithoutParent) {
    unified_log_context ctx;

    ctx.set_trace("trace123", "span456");

    EXPECT_EQ(ctx.get_string("trace_id"), "trace123");
    EXPECT_EQ(ctx.get_string("span_id"), "span456");
    EXPECT_FALSE(ctx.has("parent_span_id"));
}

TEST_F(UnifiedLogContextTest, TraceContextHasCorrectCategory) {
    unified_log_context ctx;
    ctx.set_trace("trace", "span");

    auto category = ctx.get_category("trace_id");
    ASSERT_TRUE(category.has_value());
    EXPECT_EQ(*category, context_category::trace);
}

// =========================================================================
// Request Context Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, SetRequestContext) {
    unified_log_context ctx;

    ctx.set_request("req-123", "corr-456");

    EXPECT_EQ(ctx.get_string("request_id"), "req-123");
    EXPECT_EQ(ctx.get_string("correlation_id"), "corr-456");
}

TEST_F(UnifiedLogContextTest, SetRequestContextWithoutCorrelation) {
    unified_log_context ctx;

    ctx.set_request("req-123");

    EXPECT_EQ(ctx.get_string("request_id"), "req-123");
    EXPECT_FALSE(ctx.has("correlation_id"));
}

TEST_F(UnifiedLogContextTest, RequestContextHasCorrectCategory) {
    unified_log_context ctx;
    ctx.set_request("req-123");

    auto category = ctx.get_category("request_id");
    ASSERT_TRUE(category.has_value());
    EXPECT_EQ(*category, context_category::request);
}

// =========================================================================
// OpenTelemetry Context Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, SetOtelContext) {
    unified_log_context ctx;
    otlp::otel_context otel_ctx{
        .trace_id = "0af7651916cd43dd8448eb211c80319c",
        .span_id = "b7ad6b7169203331",
        .trace_flags = "01",
        .trace_state = "congo=t61rcWkgMzE"};

    ctx.set_otel(otel_ctx);

    EXPECT_EQ(ctx.get_string("otel_trace_id"), "0af7651916cd43dd8448eb211c80319c");
    EXPECT_EQ(ctx.get_string("otel_span_id"), "b7ad6b7169203331");
    EXPECT_EQ(ctx.get_string("otel_trace_flags"), "01");
    EXPECT_EQ(ctx.get_string("otel_trace_state"), "congo=t61rcWkgMzE");
}

TEST_F(UnifiedLogContextTest, OtelContextHasCorrectCategory) {
    unified_log_context ctx;
    otlp::otel_context otel_ctx{.trace_id = "trace", .span_id = "span"};
    ctx.set_otel(otel_ctx);

    auto category = ctx.get_category("otel_trace_id");
    ASSERT_TRUE(category.has_value());
    EXPECT_EQ(*category, context_category::otel);
}

// =========================================================================
// Query Methods Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, HasReturnsTrueForExistingKey) {
    unified_log_context ctx;
    ctx.set("key", std::string("value"));

    EXPECT_TRUE(ctx.has("key"));
}

TEST_F(UnifiedLogContextTest, HasReturnsFalseForNonexistentKey) {
    unified_log_context ctx;

    EXPECT_FALSE(ctx.has("nonexistent"));
}

TEST_F(UnifiedLogContextTest, SizeReturnsCorrectCount) {
    unified_log_context ctx;
    ctx.set("key1", std::string("value1"));
    ctx.set("key2", std::string("value2"));
    ctx.set("key3", std::string("value3"));

    EXPECT_EQ(ctx.size(), 3);
}

TEST_F(UnifiedLogContextTest, KeysReturnsAllKeys) {
    unified_log_context ctx;
    ctx.set("key1", std::string("value1"));
    ctx.set("key2", std::string("value2"));

    auto keys = ctx.keys();
    ASSERT_EQ(keys.size(), 2);

    std::sort(keys.begin(), keys.end());
    EXPECT_EQ(keys[0], "key1");
    EXPECT_EQ(keys[1], "key2");
}

TEST_F(UnifiedLogContextTest, KeysByCategoryReturnsFilteredKeys) {
    unified_log_context ctx;
    ctx.set("custom_field", std::string("value"), context_category::custom);
    ctx.set_trace("trace", "span");

    auto trace_keys = ctx.keys(context_category::trace);
    EXPECT_EQ(trace_keys.size(), 2);

    auto custom_keys = ctx.keys(context_category::custom);
    EXPECT_EQ(custom_keys.size(), 1);
    EXPECT_EQ(custom_keys[0], "custom_field");
}

// =========================================================================
// Removal Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, RemoveDeletesKey) {
    unified_log_context ctx;
    ctx.set("key", std::string("value"));

    ctx.remove("key");

    EXPECT_FALSE(ctx.has("key"));
    EXPECT_TRUE(ctx.empty());
}

TEST_F(UnifiedLogContextTest, ClearRemovesAllEntries) {
    unified_log_context ctx;
    ctx.set("key1", std::string("value1"));
    ctx.set("key2", std::string("value2"));
    ctx.set_trace("trace", "span");

    ctx.clear();

    EXPECT_TRUE(ctx.empty());
    EXPECT_EQ(ctx.size(), 0);
}

TEST_F(UnifiedLogContextTest, ClearByCategoryRemovesOnlyThatCategory) {
    unified_log_context ctx;
    ctx.set("custom_field", std::string("value"), context_category::custom);
    ctx.set_trace("trace", "span");

    ctx.clear(context_category::trace);

    EXPECT_TRUE(ctx.has("custom_field"));
    EXPECT_FALSE(ctx.has("trace_id"));
    EXPECT_FALSE(ctx.has("span_id"));
}

// =========================================================================
// Export Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, ToFieldsConvertsAllEntries) {
    unified_log_context ctx;
    ctx.set("string_field", std::string("value"));
    ctx.set("int_field", int64_t{123});
    ctx.set("double_field", 3.14);
    ctx.set("bool_field", true);

    auto fields = ctx.to_fields();

    EXPECT_EQ(fields.size(), 4);
    EXPECT_EQ(std::get<std::string>(fields["string_field"]), "value");
    EXPECT_EQ(std::get<int64_t>(fields["int_field"]), 123);
    EXPECT_DOUBLE_EQ(std::get<double>(fields["double_field"]), 3.14);
    EXPECT_TRUE(std::get<bool>(fields["bool_field"]));
}

TEST_F(UnifiedLogContextTest, ToFieldsSkipsMonostateValues) {
    unified_log_context ctx;
    ctx.set("null_field", std::monostate{});
    ctx.set("string_field", std::string("value"));

    auto fields = ctx.to_fields();

    EXPECT_EQ(fields.size(), 1);
    EXPECT_TRUE(fields.find("null_field") == fields.end());
    EXPECT_TRUE(fields.find("string_field") != fields.end());
}

// =========================================================================
// Merge Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, MergeWithOverwrite) {
    unified_log_context ctx1;
    ctx1.set("key1", std::string("value1"));
    ctx1.set("shared", std::string("from_ctx1"));

    unified_log_context ctx2;
    ctx2.set("key2", std::string("value2"));
    ctx2.set("shared", std::string("from_ctx2"));

    ctx1.merge(ctx2, true);

    EXPECT_EQ(ctx1.get_string("key1"), "value1");
    EXPECT_EQ(ctx1.get_string("key2"), "value2");
    EXPECT_EQ(ctx1.get_string("shared"), "from_ctx2");
}

TEST_F(UnifiedLogContextTest, MergeWithoutOverwrite) {
    unified_log_context ctx1;
    ctx1.set("key1", std::string("value1"));
    ctx1.set("shared", std::string("from_ctx1"));

    unified_log_context ctx2;
    ctx2.set("key2", std::string("value2"));
    ctx2.set("shared", std::string("from_ctx2"));

    ctx1.merge(ctx2, false);

    EXPECT_EQ(ctx1.get_string("key1"), "value1");
    EXPECT_EQ(ctx1.get_string("key2"), "value2");
    EXPECT_EQ(ctx1.get_string("shared"), "from_ctx1");
}

// =========================================================================
// Copy/Move Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, CopyConstructor) {
    unified_log_context ctx1;
    ctx1.set("key", std::string("value"));

    unified_log_context ctx2(ctx1);

    EXPECT_EQ(ctx2.get_string("key"), "value");
    EXPECT_EQ(ctx1.get_string("key"), "value");
}

TEST_F(UnifiedLogContextTest, MoveConstructor) {
    unified_log_context ctx1;
    ctx1.set("key", std::string("value"));

    unified_log_context ctx2(std::move(ctx1));

    EXPECT_EQ(ctx2.get_string("key"), "value");
}

TEST_F(UnifiedLogContextTest, CopyAssignment) {
    unified_log_context ctx1;
    ctx1.set("key", std::string("value"));

    unified_log_context ctx2;
    ctx2 = ctx1;

    EXPECT_EQ(ctx2.get_string("key"), "value");
    EXPECT_EQ(ctx1.get_string("key"), "value");
}

TEST_F(UnifiedLogContextTest, MoveAssignment) {
    unified_log_context ctx1;
    ctx1.set("key", std::string("value"));

    unified_log_context ctx2;
    ctx2 = std::move(ctx1);

    EXPECT_EQ(ctx2.get_string("key"), "value");
}

// =========================================================================
// Chaining Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, SetReturnsReferenceForChaining) {
    unified_log_context ctx;

    ctx.set("key1", std::string("value1"))
        .set("key2", int64_t{123})
        .set("key3", true);

    EXPECT_EQ(ctx.size(), 3);
}

TEST_F(UnifiedLogContextTest, SetTraceReturnsReferenceForChaining) {
    unified_log_context ctx;

    ctx.set_trace("trace", "span").set_request("req-123");

    EXPECT_TRUE(ctx.has("trace_id"));
    EXPECT_TRUE(ctx.has("request_id"));
}

// =========================================================================
// Thread Safety Tests
// =========================================================================

TEST_F(UnifiedLogContextTest, ConcurrentReadsAreThreadSafe) {
    unified_log_context ctx;
    ctx.set("key", std::string("value"));

    std::vector<std::future<bool>> futures;
    for (int i = 0; i < 100; ++i) {
        futures.push_back(std::async(std::launch::async, [&ctx]() {
            for (int j = 0; j < 1000; ++j) {
                auto val = ctx.get_string("key");
                if (val != "value") {
                    return false;
                }
            }
            return true;
        }));
    }

    for (auto& future : futures) {
        EXPECT_TRUE(future.get());
    }
}

TEST_F(UnifiedLogContextTest, ConcurrentWritesAreThreadSafe) {
    unified_log_context ctx;

    std::vector<std::future<void>> futures;
    for (int i = 0; i < 100; ++i) {
        futures.push_back(std::async(std::launch::async, [&ctx, i]() {
            for (int j = 0; j < 100; ++j) {
                ctx.set("key_" + std::to_string(i) + "_" + std::to_string(j),
                        std::string("value"));
            }
        }));
    }

    for (auto& future : futures) {
        future.wait();
    }

    EXPECT_EQ(ctx.size(), 10000);
}

TEST_F(UnifiedLogContextTest, ConcurrentReadWriteIsThreadSafe) {
    unified_log_context ctx;
    ctx.set("counter", int64_t{0});

    std::atomic<bool> running{true};
    std::vector<std::future<void>> readers;
    std::vector<std::future<void>> writers;

    for (int i = 0; i < 10; ++i) {
        readers.push_back(std::async(std::launch::async, [&ctx, &running]() {
            while (running) {
                ctx.get("counter");
                ctx.has("counter");
                ctx.keys();
            }
        }));
    }

    for (int i = 0; i < 10; ++i) {
        writers.push_back(std::async(std::launch::async, [&ctx, i]() {
            for (int j = 0; j < 100; ++j) {
                ctx.set("key_" + std::to_string(i) + "_" + std::to_string(j),
                        std::string("value"));
            }
        }));
    }

    for (auto& future : writers) {
        future.wait();
    }

    running = false;

    for (auto& future : readers) {
        future.wait();
    }

    EXPECT_EQ(ctx.size(), 1001);
}
