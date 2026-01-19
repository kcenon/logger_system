/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/core/structured_log_builder.h>
#include <kcenon/logger/core/log_context_scope.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/formatters/logfmt_formatter.h>
#include <kcenon/logger/formatters/template_formatter.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/common/patterns/result.h>

#include <atomic>
#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

using namespace kcenon::logger;
using namespace kcenon::common;
using namespace std::chrono_literals;
namespace ci = kcenon::common::interfaces;
// Use logger_system::log_level for comparisons with captured entries (which store logger_system::log_level)
namespace ls = logger_system;

namespace {

// Test writer that captures log output
class capture_writer : public base_writer {
public:
    VoidResult write(logger_system::log_level level,
               const std::string& message,
               const std::string& file,
               int line,
               const std::string& function,
               const std::chrono::system_clock::time_point& timestamp) override {
        std::lock_guard<std::mutex> lock(mutex_);
        entries_.push_back({level, message, file, line, function, timestamp});
        return ok();
    }

    VoidResult flush() override { return ok(); }

    std::string get_name() const override { return "capture_writer"; }

    struct entry {
        logger_system::log_level level;
        std::string message;
        std::string file;
        int line;
        std::string function;
        std::chrono::system_clock::time_point timestamp;
    };

    std::vector<entry> get_entries() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return entries_;
    }

    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        entries_.clear();
    }

private:
    mutable std::mutex mutex_;
    std::vector<entry> entries_;
};

} // namespace

class StructuredLoggingTest : public ::testing::Test {
protected:
    void SetUp() override {
        metrics::g_logger_stats.reset();
    }

    void TearDown() override {}
};

// Test 1: Basic structured log builder
TEST_F(StructuredLoggingTest, BasicStructuredLogBuilder) {
    bool callback_invoked = false;
    log_entry* captured_entry = nullptr;

    structured_log_builder builder(
        ls::log_level::info,
        [&callback_invoked, &captured_entry](log_entry&& entry) {
            callback_invoked = true;
            // Store a copy of entry data for validation
        },
        nullptr
    );

    builder.message("Test message")
           .field("user_id", 12345)
           .field("action", "login")
           .emit();

    EXPECT_TRUE(callback_invoked);
}

// Test 2: Structured log builder with various field types
TEST_F(StructuredLoggingTest, StructuredLogBuilderFieldTypes) {
    log_fields captured_fields;

    structured_log_builder builder(
        ls::log_level::info,
        [&captured_fields](log_entry&& entry) {
            if (entry.fields) {
                captured_fields = *entry.fields;
            }
        },
        nullptr
    );

    builder.message("Test")
           .field("string_field", "hello")
           .field("int_field", 42)
           .field("int64_field", static_cast<int64_t>(9876543210))
           .field("double_field", 3.14159)
           .field("bool_field", true)
           .emit();

    EXPECT_EQ(captured_fields.size(), 5);

    // Verify string field
    auto str_val = std::get<std::string>(captured_fields["string_field"]);
    EXPECT_EQ(str_val, "hello");

    // Verify int field (stored as int64_t)
    auto int_val = std::get<int64_t>(captured_fields["int_field"]);
    EXPECT_EQ(int_val, 42);

    // Verify int64 field
    auto int64_val = std::get<int64_t>(captured_fields["int64_field"]);
    EXPECT_EQ(int64_val, 9876543210);

    // Verify double field
    auto double_val = std::get<double>(captured_fields["double_field"]);
    EXPECT_NEAR(double_val, 3.14159, 0.00001);

    // Verify bool field
    auto bool_val = std::get<bool>(captured_fields["bool_field"]);
    EXPECT_TRUE(bool_val);
}

// Test 3: Context fields are included
TEST_F(StructuredLoggingTest, ContextFieldsIncluded) {
    log_fields context;
    context["service"] = std::string("api-gateway");
    context["version"] = std::string("1.0.0");

    log_fields captured_fields;

    structured_log_builder builder(
        ls::log_level::info,
        [&captured_fields](log_entry&& entry) {
            if (entry.fields) {
                captured_fields = *entry.fields;
            }
        },
        &context
    );

    builder.message("Request processed")
           .field("request_id", "req-123")
           .emit();

    EXPECT_EQ(captured_fields.size(), 3);
    EXPECT_EQ(std::get<std::string>(captured_fields["service"]), "api-gateway");
    EXPECT_EQ(std::get<std::string>(captured_fields["version"]), "1.0.0");
    EXPECT_EQ(std::get<std::string>(captured_fields["request_id"]), "req-123");
}

// Test 4: Logger structured logging methods (deprecated convenience method)
TEST_F(StructuredLoggingTest, LoggerStructuredMethods) {
    auto test_logger = std::make_shared<logger>(false);  // Synchronous mode
    test_logger->start();

    auto writer = std::make_unique<capture_writer>();
    auto* writer_ptr = writer.get();
    test_logger->add_writer("capture", std::move(writer));

    // Test log_structured with ls::log_level::info (canonical API)
    test_logger->log_structured(ls::log_level::info)
        .message("User logged in")
        .field("user_id", 12345)
        .emit();

    test_logger->flush();

    auto entries = writer_ptr->get_entries();
    ASSERT_GE(entries.size(), 1);
    EXPECT_EQ(entries[0].level, ls::log_level::info);
    EXPECT_EQ(entries[0].message, "User logged in");

    test_logger->stop();
}

// Test 5: Logger context field management
TEST_F(StructuredLoggingTest, LoggerContextManagement) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    // Initially no context
    EXPECT_FALSE(test_logger->has_context());

    // Set context fields one by one and verify each
    test_logger->set_context("request_id", std::string("req-123"));
    EXPECT_TRUE(test_logger->has_context());
    {
        auto ctx = test_logger->get_context();
        ASSERT_EQ(ctx.size(), 1);
        ASSERT_TRUE(std::holds_alternative<std::string>(ctx.at("request_id")));
        EXPECT_EQ(std::get<std::string>(ctx.at("request_id")), "req-123");
    }

    test_logger->set_context("user_id", static_cast<int64_t>(456));
    {
        auto ctx = test_logger->get_context();
        ASSERT_EQ(ctx.size(), 2);
        ASSERT_TRUE(std::holds_alternative<int64_t>(ctx.at("user_id")));
        EXPECT_EQ(std::get<int64_t>(ctx.at("user_id")), 456);
    }

    test_logger->set_context("is_admin", true);
    {
        auto ctx = test_logger->get_context();
        ASSERT_EQ(ctx.size(), 3);
        ASSERT_TRUE(std::holds_alternative<bool>(ctx.at("is_admin")));
        EXPECT_TRUE(std::get<bool>(ctx.at("is_admin")));
    }

    test_logger->set_context("response_time", 1.5);
    {
        auto ctx = test_logger->get_context();
        ASSERT_EQ(ctx.size(), 4);
        ASSERT_TRUE(std::holds_alternative<double>(ctx.at("response_time")));
        EXPECT_NEAR(std::get<double>(ctx.at("response_time")), 1.5, 0.001);
    }

    // Remove a context field
    test_logger->remove_context("is_admin");
    EXPECT_EQ(test_logger->get_context().size(), 3);

    // Clear all context
    test_logger->clear_context();
    EXPECT_FALSE(test_logger->has_context());
    EXPECT_EQ(test_logger->get_context().size(), 0);

    test_logger->stop();
}

// Test 6: JSON formatter with structured fields
TEST_F(StructuredLoggingTest, JsonFormatterStructuredFields) {
    json_formatter formatter;

    log_entry entry(ls::log_level::info, "Test message");
    entry.fields = log_fields{
        {"user_id", static_cast<int64_t>(12345)},
        {"action", std::string("login")},
        {"success", true},
        {"latency_ms", 42.5}
    };

    std::string output = formatter.format(entry);

    // Verify JSON contains expected fields
    EXPECT_NE(output.find("\"message\":\"Test message\""), std::string::npos);
    EXPECT_NE(output.find("\"user_id\":12345"), std::string::npos);
    EXPECT_NE(output.find("\"action\":\"login\""), std::string::npos);
    EXPECT_NE(output.find("\"success\":true"), std::string::npos);
    EXPECT_NE(output.find("\"latency_ms\":"), std::string::npos);
}

// Test 7: JSON formatter with category
TEST_F(StructuredLoggingTest, JsonFormatterWithCategory) {
    json_formatter formatter;

    log_entry entry(ls::log_level::info, "Database query");
    entry.category = small_string_128("database");

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("\"category\":\"database\""), std::string::npos);
}

// Test 8: Structured logging with all level methods (deprecated convenience methods)
TEST_F(StructuredLoggingTest, AllStructuredLevelMethods) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->set_level(ci::log_level::trace);
    test_logger->start();

    auto writer = std::make_unique<capture_writer>();
    auto* writer_ptr = writer.get();
    test_logger->add_writer("capture", std::move(writer));

    test_logger->log_structured(ls::log_level::trace).message("Trace").emit();
    test_logger->log_structured(ls::log_level::debug).message("Debug").emit();
    test_logger->log_structured(ls::log_level::info).message("Info").emit();
    test_logger->log_structured(ls::log_level::warn).message("Warn").emit();
    test_logger->log_structured(ls::log_level::error).message("Error").emit();
    test_logger->log_structured(ls::log_level::fatal).message("Fatal").emit();

    test_logger->flush();

    auto entries = writer_ptr->get_entries();
    ASSERT_EQ(entries.size(), 6);

    EXPECT_EQ(entries[0].level, ls::log_level::trace);
    EXPECT_EQ(entries[1].level, ls::log_level::debug);
    EXPECT_EQ(entries[2].level, ls::log_level::info);
    EXPECT_EQ(entries[3].level, ls::log_level::warn);
    EXPECT_EQ(entries[4].level, ls::log_level::error);
    EXPECT_EQ(entries[5].level, ls::log_level::fatal);

    test_logger->stop();
}

// Test 8b: Structured logging with generic log_structured method (canonical API)
TEST_F(StructuredLoggingTest, GenericLogStructuredMethod) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->set_level(ci::log_level::trace);
    test_logger->start();

    auto writer = std::make_unique<capture_writer>();
    auto* writer_ptr = writer.get();
    test_logger->add_writer("capture", std::move(writer));

    // Use the canonical log_structured(level) API
    test_logger->log_structured(ls::log_level::trace).message("Trace").emit();
    test_logger->log_structured(ls::log_level::debug).message("Debug").emit();
    test_logger->log_structured(ls::log_level::info).message("Info").emit();
    test_logger->log_structured(ls::log_level::warn).message("Warn").emit();
    test_logger->log_structured(ls::log_level::error).message("Error").emit();
    test_logger->log_structured(ls::log_level::fatal).message("Fatal").emit();

    test_logger->flush();

    auto entries = writer_ptr->get_entries();
    ASSERT_EQ(entries.size(), 6);

    EXPECT_EQ(entries[0].level, ls::log_level::trace);
    EXPECT_EQ(entries[1].level, ls::log_level::debug);
    EXPECT_EQ(entries[2].level, ls::log_level::info);
    EXPECT_EQ(entries[3].level, ls::log_level::warn);
    EXPECT_EQ(entries[4].level, ls::log_level::error);
    EXPECT_EQ(entries[5].level, ls::log_level::fatal);

    test_logger->stop();
}

// Test 8c: Structured logging with fields using generic API
TEST_F(StructuredLoggingTest, GenericLogStructuredWithFields) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    auto writer = std::make_unique<capture_writer>();
    auto* writer_ptr = writer.get();
    test_logger->add_writer("capture", std::move(writer));

    // Use the canonical log_structured(level) API with fields
    test_logger->log_structured(ls::log_level::info)
        .message("User action completed")
        .field("user_id", 12345)
        .field("action", "purchase")
        .field("amount", 99.99)
        .field("success", true)
        .emit();

    test_logger->flush();

    auto entries = writer_ptr->get_entries();
    ASSERT_EQ(entries.size(), 1);
    EXPECT_EQ(entries[0].level, ls::log_level::info);
    EXPECT_EQ(entries[0].message, "User action completed");

    test_logger->stop();
}

// Test 9: log_entry with fields
TEST_F(StructuredLoggingTest, LogEntryWithFields) {
    log_entry entry(ls::log_level::info, "Test");

    EXPECT_FALSE(entry.fields.has_value());

    entry.fields = log_fields{};
    entry.fields->emplace("key", std::string("value"));

    EXPECT_TRUE(entry.fields.has_value());
    EXPECT_EQ(entry.fields->size(), 1);
}

// Test 10: Thread safety of context operations
TEST_F(StructuredLoggingTest, ContextThreadSafety) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    std::vector<std::thread> threads;
    const int num_threads = 10;
    const int operations_per_thread = 100;

    for (int t = 0; t < num_threads; ++t) {
        threads.emplace_back([&test_logger, t, operations_per_thread]() {
            for (int i = 0; i < operations_per_thread; ++i) {
                std::string key = "key_" + std::to_string(t) + "_" + std::to_string(i);
                test_logger->set_context(key, "value");
                test_logger->has_context();
                test_logger->remove_context(key);
            }
        });
    }

    for (auto& thread : threads) {
        thread.join();
    }

    // Should complete without crashes
    EXPECT_TRUE(true);

    test_logger->stop();
}

// Test 11: Generic context ID API
TEST_F(StructuredLoggingTest, GenericContextIdAPI) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    // Initially no context IDs
    EXPECT_FALSE(test_logger->has_context_id("correlation_id"));
    EXPECT_FALSE(test_logger->has_context_id("trace_id"));
    EXPECT_EQ(test_logger->get_context_id("correlation_id"), "");

    // Set context IDs using generic API
    test_logger->set_context_id("correlation_id", "abc-123");
    test_logger->set_context_id("trace_id", "0af7651916cd43dd8448eb211c80319c");
    test_logger->set_context_id("span_id", "b7ad6b7169203331");

    EXPECT_TRUE(test_logger->has_context_id("correlation_id"));
    EXPECT_TRUE(test_logger->has_context_id("trace_id"));
    EXPECT_TRUE(test_logger->has_context_id("span_id"));

    EXPECT_EQ(test_logger->get_context_id("correlation_id"), "abc-123");
    EXPECT_EQ(test_logger->get_context_id("trace_id"), "0af7651916cd43dd8448eb211c80319c");
    EXPECT_EQ(test_logger->get_context_id("span_id"), "b7ad6b7169203331");

    // Clear single context ID
    test_logger->clear_context_id("span_id");
    EXPECT_FALSE(test_logger->has_context_id("span_id"));
    EXPECT_TRUE(test_logger->has_context_id("correlation_id"));

    // Clear all context IDs
    test_logger->clear_all_context_ids();
    EXPECT_FALSE(test_logger->has_context_id("correlation_id"));
    EXPECT_FALSE(test_logger->has_context_id("trace_id"));

    test_logger->stop();
}

// Test 11b: Custom context ID keys
TEST_F(StructuredLoggingTest, CustomContextIdKeys) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    // Set custom context ID key
    test_logger->set_context_id("custom_key", "custom_value");
    EXPECT_TRUE(test_logger->has_context_id("custom_key"));
    EXPECT_EQ(test_logger->get_context_id("custom_key"), "custom_value");

    // clear_all_context_ids only clears known keys, not custom ones
    test_logger->clear_all_context_ids();
    EXPECT_TRUE(test_logger->has_context_id("custom_key"));

    // Clear custom key manually
    test_logger->clear_context_id("custom_key");
    EXPECT_FALSE(test_logger->has_context_id("custom_key"));

    test_logger->stop();
}

// Test 11c: Trace ID using generic context API
TEST_F(StructuredLoggingTest, TraceIdGenericAPI) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    // Initially no trace ID
    EXPECT_FALSE(test_logger->has_context_id("trace_id"));
    EXPECT_EQ(test_logger->get_context_id("trace_id"), "");

    // Set trace ID using generic API
    test_logger->set_context_id("trace_id", "0af7651916cd43dd8448eb211c80319c");
    EXPECT_TRUE(test_logger->has_context_id("trace_id"));
    EXPECT_EQ(test_logger->get_context_id("trace_id"), "0af7651916cd43dd8448eb211c80319c");

    // Clear trace ID
    test_logger->clear_context_id("trace_id");
    EXPECT_FALSE(test_logger->has_context_id("trace_id"));
    EXPECT_EQ(test_logger->get_context_id("trace_id"), "");

    test_logger->stop();
}

// Test 12: Span ID using generic context API
TEST_F(StructuredLoggingTest, SpanIdGenericAPI) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    // Initially no span ID
    EXPECT_FALSE(test_logger->has_context_id("span_id"));
    EXPECT_EQ(test_logger->get_context_id("span_id"), "");

    // Set span ID using generic API
    test_logger->set_context_id("span_id", "b7ad6b7169203331");
    EXPECT_TRUE(test_logger->has_context_id("span_id"));
    EXPECT_EQ(test_logger->get_context_id("span_id"), "b7ad6b7169203331");

    // Clear span ID
    test_logger->clear_context_id("span_id");
    EXPECT_FALSE(test_logger->has_context_id("span_id"));
    EXPECT_EQ(test_logger->get_context_id("span_id"), "");

    test_logger->stop();
}

// Test 13: Parent Span ID using generic context API
TEST_F(StructuredLoggingTest, ParentSpanIdGenericAPI) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->start();

    // Initially no parent span ID
    EXPECT_FALSE(test_logger->has_context_id("parent_span_id"));
    EXPECT_EQ(test_logger->get_context_id("parent_span_id"), "");

    // Set parent span ID using generic API
    test_logger->set_context_id("parent_span_id", "a1b2c3d4e5f67890");
    EXPECT_TRUE(test_logger->has_context_id("parent_span_id"));
    EXPECT_EQ(test_logger->get_context_id("parent_span_id"), "a1b2c3d4e5f67890");

    // Clear parent span ID
    test_logger->clear_context_id("parent_span_id");
    EXPECT_FALSE(test_logger->has_context_id("parent_span_id"));
    EXPECT_EQ(test_logger->get_context_id("parent_span_id"), "");

    test_logger->stop();
}

// Test 14: Thread-local context storage
TEST_F(StructuredLoggingTest, ThreadLocalContextStorage) {
    // Set thread-local context
    log_context_storage::set("thread_key", "thread_value");
    EXPECT_TRUE(log_context_storage::has_context());

    auto fields = log_context_storage::get();
    EXPECT_EQ(fields.size(), 1);
    EXPECT_EQ(std::get<std::string>(fields["thread_key"]), "thread_value");

    // Add more fields
    log_context_storage::set("int_key", static_cast<int64_t>(42));
    log_context_storage::set("bool_key", true);

    fields = log_context_storage::get();
    EXPECT_EQ(fields.size(), 3);

    // Get specific field
    auto value = log_context_storage::get_field("int_key");
    EXPECT_TRUE(value.has_value());
    EXPECT_EQ(std::get<int64_t>(*value), 42);

    // Remove a field
    log_context_storage::remove("thread_key");
    fields = log_context_storage::get();
    EXPECT_EQ(fields.size(), 2);

    // Clear all
    log_context_storage::clear();
    EXPECT_FALSE(log_context_storage::has_context());
}

// Test 15: Log context scope basic usage
TEST_F(StructuredLoggingTest, LogContextScopeBasic) {
    // Clear any existing context
    log_context_storage::clear();
    EXPECT_FALSE(log_context_storage::has_context());

    {
        log_context_scope scope({
            {"request_id", std::string("req-123")},
            {"user_id", static_cast<int64_t>(456)}
        });

        // Context should be set within scope
        EXPECT_TRUE(log_context_storage::has_context());
        auto fields = log_context_storage::get();
        EXPECT_EQ(fields.size(), 2);
        EXPECT_EQ(std::get<std::string>(fields["request_id"]), "req-123");
        EXPECT_EQ(std::get<int64_t>(fields["user_id"]), 456);
    }

    // Context should be cleared after scope exits
    EXPECT_FALSE(log_context_storage::has_context());
}

// Test 16: Nested log context scopes
TEST_F(StructuredLoggingTest, NestedLogContextScopes) {
    log_context_storage::clear();

    {
        log_context_scope outer({
            {"outer_key", std::string("outer_value")}
        });

        EXPECT_EQ(log_context_storage::get().size(), 1);

        {
            log_context_scope inner({
                {"inner_key", std::string("inner_value")}
            });

            // Both keys should be present
            auto fields = log_context_storage::get();
            EXPECT_EQ(fields.size(), 2);
            EXPECT_EQ(std::get<std::string>(fields["outer_key"]), "outer_value");
            EXPECT_EQ(std::get<std::string>(fields["inner_key"]), "inner_value");
        }

        // After inner scope exits, only outer key should remain
        auto fields = log_context_storage::get();
        EXPECT_EQ(fields.size(), 1);
        EXPECT_EQ(std::get<std::string>(fields["outer_key"]), "outer_value");
    }

    // After outer scope exits, context should be empty
    EXPECT_FALSE(log_context_storage::has_context());
}

// Test 17: Scoped context single field
TEST_F(StructuredLoggingTest, ScopedContextSingleField) {
    log_context_storage::clear();

    {
        scoped_context ctx("order_id", static_cast<int64_t>(12345));

        EXPECT_TRUE(log_context_storage::has_context());
        auto value = log_context_storage::get_field("order_id");
        EXPECT_TRUE(value.has_value());
        EXPECT_EQ(std::get<int64_t>(*value), 12345);
    }

    EXPECT_FALSE(log_context_storage::has_context());
}

// Test 18: Thread isolation of context
TEST_F(StructuredLoggingTest, ThreadIsolation) {
    log_context_storage::clear();

    std::atomic<bool> thread1_done{false};
    std::atomic<bool> thread2_done{false};
    std::atomic<bool> test_passed{true};

    std::thread thread1([&]() {
        log_context_storage::set("thread_id", std::string("thread1"));
        std::this_thread::sleep_for(10ms);

        // Verify our context is still intact
        auto value = log_context_storage::get_field("thread_id");
        if (!value.has_value() || std::get<std::string>(*value) != "thread1") {
            test_passed = false;
        }

        thread1_done = true;
        log_context_storage::clear();
    });

    std::thread thread2([&]() {
        log_context_storage::set("thread_id", std::string("thread2"));
        std::this_thread::sleep_for(10ms);

        // Verify our context is still intact
        auto value = log_context_storage::get_field("thread_id");
        if (!value.has_value() || std::get<std::string>(*value) != "thread2") {
            test_passed = false;
        }

        thread2_done = true;
        log_context_storage::clear();
    });

    thread1.join();
    thread2.join();

    EXPECT_TRUE(test_passed);
}

// Test 19: LogfmtFormatter basic formatting
TEST_F(StructuredLoggingTest, LogfmtFormatterBasic) {
    logfmt_formatter formatter;

    log_entry entry(ls::log_level::info, "Server started");

    std::string output = formatter.format(entry);

    // Verify logfmt output contains expected key=value pairs
    EXPECT_NE(output.find("level=info"), std::string::npos);
    EXPECT_NE(output.find("msg="), std::string::npos);
    EXPECT_NE(output.find("Server started"), std::string::npos);
}

// Test 20: LogfmtFormatter with structured fields
TEST_F(StructuredLoggingTest, LogfmtFormatterWithFields) {
    logfmt_formatter formatter;

    log_entry entry(ls::log_level::error, "Connection failed");
    entry.fields = log_fields{
        {"host", std::string("localhost")},
        {"port", static_cast<int64_t>(5432)},
        {"retry", true},
        {"timeout_ms", 30.5}
    };

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("host=localhost"), std::string::npos);
    EXPECT_NE(output.find("port=5432"), std::string::npos);
    EXPECT_NE(output.find("retry=true"), std::string::npos);
    EXPECT_NE(output.find("timeout_ms="), std::string::npos);
}

// Test 21: LogfmtFormatter escapes special characters
TEST_F(StructuredLoggingTest, LogfmtFormatterEscaping) {
    logfmt_formatter formatter;

    log_entry entry(ls::log_level::info, "Message with spaces and \"quotes\"");

    std::string output = formatter.format(entry);

    // Message with special chars should be quoted
    EXPECT_NE(output.find("msg=\""), std::string::npos);
}

// Test 22: TemplateFormatter basic formatting
TEST_F(StructuredLoggingTest, TemplateFormatterBasic) {
    template_formatter formatter("[{level}] {message}");

    log_entry entry(ls::log_level::info, "Test message");

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("[INFO]"), std::string::npos);
    EXPECT_NE(output.find("Test message"), std::string::npos);
}

// Test 23: TemplateFormatter with timestamp
TEST_F(StructuredLoggingTest, TemplateFormatterWithTimestamp) {
    template_formatter formatter("{timestamp} [{level}] {message}");

    log_entry entry(ls::log_level::debug, "Debug info");

    std::string output = formatter.format(entry);

    // Check ISO 8601 format exists (year-month-day pattern)
    EXPECT_NE(output.find("T"), std::string::npos);  // ISO 8601 separator
    EXPECT_NE(output.find("[DEBUG]"), std::string::npos);
    EXPECT_NE(output.find("Debug info"), std::string::npos);
}

// Test 24: TemplateFormatter with source location
TEST_F(StructuredLoggingTest, TemplateFormatterWithLocation) {
    template_formatter formatter("{message} ({filename}:{line})");

    log_entry entry(ls::log_level::error, "Error occurred",
                    "/path/to/file.cpp", 42, "test_function");

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("Error occurred"), std::string::npos);
    EXPECT_NE(output.find("file.cpp"), std::string::npos);
    EXPECT_NE(output.find(":42"), std::string::npos);
}

// Test 25: TemplateFormatter with structured fields
TEST_F(StructuredLoggingTest, TemplateFormatterWithFields) {
    template_formatter formatter("{message} user_id={user_id}");

    log_entry entry(ls::log_level::info, "User action");
    entry.fields = log_fields{
        {"user_id", static_cast<int64_t>(12345)}
    };

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("User action"), std::string::npos);
    EXPECT_NE(output.find("user_id=12345"), std::string::npos);
}

// Test 26: TemplateFormatter lowercase level
TEST_F(StructuredLoggingTest, TemplateFormatterLowercaseLevel) {
    template_formatter formatter("{level_lower}: {message}");

    log_entry entry(ls::log_level::warn, "Warning message");

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("warning:"), std::string::npos);
}

// Test 27: TemplateFormatter field width
TEST_F(StructuredLoggingTest, TemplateFormatterFieldWidth) {
    template_formatter formatter("[{level:10}] {message}");

    log_entry entry(ls::log_level::info, "Test");

    std::string output = formatter.format(entry);

    // Level "INFO" padded to 10 characters
    EXPECT_NE(output.find("[INFO      ]"), std::string::npos);
}

// Test 28: TemplateFormatter set_template
TEST_F(StructuredLoggingTest, TemplateFormatterSetTemplate) {
    template_formatter formatter("[{level}] {message}");

    log_entry entry(ls::log_level::info, "Test");

    std::string output1 = formatter.format(entry);
    EXPECT_NE(output1.find("[INFO]"), std::string::npos);

    formatter.set_template("{level}: {message}");

    std::string output2 = formatter.format(entry);
    EXPECT_NE(output2.find("INFO:"), std::string::npos);
}

// Test 29: TemplateFormatter get_name
TEST_F(StructuredLoggingTest, TemplateFormatterGetName) {
    template_formatter formatter;
    EXPECT_EQ(formatter.get_name(), "template_formatter");
}

// Test 30: LogfmtFormatter get_name
TEST_F(StructuredLoggingTest, LogfmtFormatterGetName) {
    logfmt_formatter formatter;
    EXPECT_EQ(formatter.get_name(), "logfmt_formatter");
}
