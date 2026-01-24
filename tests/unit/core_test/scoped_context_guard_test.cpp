/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>

#include "kcenon/logger/core/logger.h"
#include "kcenon/logger/core/scoped_context_guard.h"
#include "kcenon/logger/writers/console_writer.h"

#include <memory>

using namespace kcenon::logger;

/**
 * @brief Test fixture for scoped_context_guard tests
 */
class ScopedContextGuardTest : public ::testing::Test {
protected:
    void SetUp() override {
        log = std::make_unique<logger>();
        log->start();
    }

    void TearDown() override {
        if (log) {
            log->stop();
        }
    }

    std::unique_ptr<logger> log;
};

// =========================================================================
// Constructor and Destructor Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, DefaultConstructorSavesContext) {
    // Set initial context
    log->context().set("initial_key", std::string("initial_value"));

    {
        scoped_context_guard guard(*log);
        // Context should still have initial_key
        EXPECT_TRUE(log->context().has("initial_key"));
    }

    // After guard destruction, context should still have initial_key
    EXPECT_TRUE(log->context().has("initial_key"));
}

TEST_F(ScopedContextGuardTest, SingleValueConstructorSetsValue) {
    {
        scoped_context_guard guard(*log, "test_key", std::string("test_value"));

        // Value should be set
        auto value = log->context().get_as<std::string>("test_key");
        ASSERT_TRUE(value.has_value());
        EXPECT_EQ(*value, "test_value");
    }

    // After guard destruction, key should be removed
    EXPECT_FALSE(log->context().has("test_key"));
}

TEST_F(ScopedContextGuardTest, DestructorRestoresPreviousContext) {
    // Set initial context
    log->context().set("key", std::string("original"));

    {
        scoped_context_guard guard(*log);
        guard.set("key", std::string("modified"));

        // Value should be modified
        auto value = log->context().get_as<std::string>("key");
        ASSERT_TRUE(value.has_value());
        EXPECT_EQ(*value, "modified");
    }

    // After guard destruction, original value should be restored
    auto value = log->context().get_as<std::string>("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(*value, "original");
}

TEST_F(ScopedContextGuardTest, DestructorRemovesAddedKeys) {
    {
        scoped_context_guard guard(*log);
        guard.set("new_key", std::string("value"));

        // Key should be set
        EXPECT_TRUE(log->context().has("new_key"));
    }

    // After guard destruction, key should be removed
    EXPECT_FALSE(log->context().has("new_key"));
}

// =========================================================================
// Setter Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, SetMethodWorks) {
    scoped_context_guard guard(*log);
    guard.set("string_key", std::string("value"))
         .set("int_key", int64_t{123})
         .set("double_key", 3.14)
         .set("bool_key", true);

    EXPECT_EQ(*log->context().get_as<std::string>("string_key"), "value");
    EXPECT_EQ(*log->context().get_as<int64_t>("int_key"), 123);
    EXPECT_DOUBLE_EQ(*log->context().get_as<double>("double_key"), 3.14);
    EXPECT_TRUE(*log->context().get_as<bool>("bool_key"));
}

TEST_F(ScopedContextGuardTest, SetMethodIsChainable) {
    scoped_context_guard guard(*log);

    auto& result = guard.set("key1", std::string("value1"))
                        .set("key2", int64_t{42});

    // Should return reference to guard for chaining
    EXPECT_EQ(&result, &guard);
}

TEST_F(ScopedContextGuardTest, SetTraceMethodWorks) {
    scoped_context_guard guard(*log);
    guard.set_trace("trace123", "span456");

    EXPECT_EQ(*log->context().get_as<std::string>("trace_id"), "trace123");
    EXPECT_EQ(*log->context().get_as<std::string>("span_id"), "span456");
}

TEST_F(ScopedContextGuardTest, SetTraceWithParentSpanWorks) {
    scoped_context_guard guard(*log);
    guard.set_trace("trace123", "span456", "parent789");

    EXPECT_EQ(*log->context().get_as<std::string>("trace_id"), "trace123");
    EXPECT_EQ(*log->context().get_as<std::string>("span_id"), "span456");
    EXPECT_EQ(*log->context().get_as<std::string>("parent_span_id"), "parent789");
}

TEST_F(ScopedContextGuardTest, SetRequestMethodWorks) {
    scoped_context_guard guard(*log);
    guard.set_request("req-123");

    EXPECT_EQ(*log->context().get_as<std::string>("request_id"), "req-123");
}

TEST_F(ScopedContextGuardTest, SetRequestWithCorrelationIdWorks) {
    scoped_context_guard guard(*log);
    guard.set_request("req-123", "corr-456");

    EXPECT_EQ(*log->context().get_as<std::string>("request_id"), "req-123");
    EXPECT_EQ(*log->context().get_as<std::string>("correlation_id"), "corr-456");
}

TEST_F(ScopedContextGuardTest, SetOtelMethodWorks) {
    otlp::otel_context otel_ctx;
    otel_ctx.trace_id = "otel_trace_123";
    otel_ctx.span_id = "otel_span_456";
    otel_ctx.trace_flags = "01";
    otel_ctx.trace_state = "state";

    scoped_context_guard guard(*log);
    guard.set_otel(otel_ctx);

    EXPECT_EQ(*log->context().get_as<std::string>("otel_trace_id"), "otel_trace_123");
    EXPECT_EQ(*log->context().get_as<std::string>("otel_span_id"), "otel_span_456");
    EXPECT_EQ(*log->context().get_as<std::string>("otel_trace_flags"), "01");
    EXPECT_EQ(*log->context().get_as<std::string>("otel_trace_state"), "state");
}

// =========================================================================
// Nested Scopes Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, NestedScopesWork) {
    {
        scoped_context_guard outer(*log);
        outer.set("level", std::string("outer"));
        outer.set("outer_only", std::string("value"));

        EXPECT_EQ(*log->context().get_as<std::string>("level"), "outer");
        EXPECT_TRUE(log->context().has("outer_only"));

        {
            scoped_context_guard inner(*log);
            inner.set("level", std::string("inner"));
            inner.set("inner_only", std::string("value"));

            // Inner scope should override outer
            EXPECT_EQ(*log->context().get_as<std::string>("level"), "inner");
            EXPECT_TRUE(log->context().has("outer_only"));
            EXPECT_TRUE(log->context().has("inner_only"));
        }

        // After inner scope, level should be restored to outer
        EXPECT_EQ(*log->context().get_as<std::string>("level"), "outer");
        EXPECT_TRUE(log->context().has("outer_only"));
        EXPECT_FALSE(log->context().has("inner_only"));
    }

    // After outer scope, all should be removed
    EXPECT_FALSE(log->context().has("level"));
    EXPECT_FALSE(log->context().has("outer_only"));
}

TEST_F(ScopedContextGuardTest, DeeplyNestedScopesWork) {
    {
        scoped_context_guard level1(*log);
        level1.set("depth", int64_t{1});

        {
            scoped_context_guard level2(*log);
            level2.set("depth", int64_t{2});

            {
                scoped_context_guard level3(*log);
                level3.set("depth", int64_t{3});

                EXPECT_EQ(*log->context().get_as<int64_t>("depth"), 3);
            }

            EXPECT_EQ(*log->context().get_as<int64_t>("depth"), 2);
        }

        EXPECT_EQ(*log->context().get_as<int64_t>("depth"), 1);
    }

    EXPECT_FALSE(log->context().has("depth"));
}

// =========================================================================
// Exception Safety Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, ExceptionInScopeRestoresContext) {
    log->context().set("key", std::string("original"));

    try {
        scoped_context_guard guard(*log);
        guard.set("key", std::string("modified"));
        guard.set("temp_key", std::string("temp"));

        // Simulate exception
        throw std::runtime_error("test exception");
    } catch (const std::runtime_error&) {
        // Exception caught
    }

    // Context should be restored despite exception
    auto value = log->context().get_as<std::string>("key");
    ASSERT_TRUE(value.has_value());
    EXPECT_EQ(*value, "original");
    EXPECT_FALSE(log->context().has("temp_key"));
}

// =========================================================================
// Move Semantics Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, MoveConstructorWorks) {
    {
        scoped_context_guard guard1(*log);
        guard1.set("key", std::string("value"));

        scoped_context_guard guard2(std::move(guard1));

        // Context should still have the key
        EXPECT_TRUE(log->context().has("key"));
    }

    // After both guards destroyed, key should be removed
    EXPECT_FALSE(log->context().has("key"));
}

TEST_F(ScopedContextGuardTest, MoveAssignmentWorks) {
    {
        scoped_context_guard guard1(*log);
        guard1.set("key1", std::string("value1"));

        scoped_context_guard guard2(*log);
        guard2.set("key2", std::string("value2"));

        guard2 = std::move(guard1);

        // guard2 now manages key1, key2 should be removed
        EXPECT_TRUE(log->context().has("key1"));
        EXPECT_FALSE(log->context().has("key2"));
    }

    EXPECT_FALSE(log->context().has("key1"));
}

// =========================================================================
// Category Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, CategoryIsPreservedOnRestore) {
    log->context().set("key", std::string("original"), context_category::trace);

    {
        scoped_context_guard guard(*log);
        guard.set("key", std::string("modified"), context_category::custom);

        // Value should be modified with custom category
        EXPECT_EQ(*log->context().get_category("key"), context_category::custom);
    }

    // After restoration, original category should be restored
    EXPECT_EQ(*log->context().get_category("key"), context_category::trace);
}

// =========================================================================
// Multiple Keys Tests
// =========================================================================

TEST_F(ScopedContextGuardTest, MultipleKeysAreRemovedCorrectly) {
    {
        scoped_context_guard guard(*log);
        guard.set("key1", std::string("value1"))
             .set("key2", int64_t{42})
             .set("key3", true)
             .set_trace("trace", "span")
             .set_request("request");

        // All keys should be set
        EXPECT_TRUE(log->context().has("key1"));
        EXPECT_TRUE(log->context().has("key2"));
        EXPECT_TRUE(log->context().has("key3"));
        EXPECT_TRUE(log->context().has("trace_id"));
        EXPECT_TRUE(log->context().has("span_id"));
        EXPECT_TRUE(log->context().has("request_id"));
    }

    // All keys should be removed
    EXPECT_FALSE(log->context().has("key1"));
    EXPECT_FALSE(log->context().has("key2"));
    EXPECT_FALSE(log->context().has("key3"));
    EXPECT_FALSE(log->context().has("trace_id"));
    EXPECT_FALSE(log->context().has("span_id"));
    EXPECT_FALSE(log->context().has("request_id"));
}
