/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/core/structured_log_builder.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/common/patterns/result.h>

#include <thread>
#include <chrono>
#include <vector>
#include <mutex>

using namespace kcenon::logger;
using namespace kcenon::common;
using namespace std::chrono_literals;

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
        log_level::info,
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
        log_level::info,
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
        log_level::info,
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

// Test 4: Logger structured logging methods
TEST_F(StructuredLoggingTest, LoggerStructuredMethods) {
    auto test_logger = std::make_shared<logger>(false);  // Synchronous mode
    test_logger->start();

    auto writer = std::make_unique<capture_writer>();
    auto* writer_ptr = writer.get();
    test_logger->add_writer("capture", std::move(writer));

    // Test info_structured
    test_logger->info_structured()
        .message("User logged in")
        .field("user_id", 12345)
        .emit();

    test_logger->flush();

    auto entries = writer_ptr->get_entries();
    ASSERT_GE(entries.size(), 1);
    EXPECT_EQ(entries[0].level, log_level::info);
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

    log_entry entry(log_level::info, "Test message");
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

    log_entry entry(log_level::info, "Database query");
    entry.category = small_string_128("database");

    std::string output = formatter.format(entry);

    EXPECT_NE(output.find("\"category\":\"database\""), std::string::npos);
}

// Test 8: Structured logging with all level methods
TEST_F(StructuredLoggingTest, AllStructuredLevelMethods) {
    auto test_logger = std::make_shared<logger>(false);
    test_logger->set_min_level(log_level::trace);
    test_logger->start();

    auto writer = std::make_unique<capture_writer>();
    auto* writer_ptr = writer.get();
    test_logger->add_writer("capture", std::move(writer));

    test_logger->trace_structured().message("Trace").emit();
    test_logger->debug_structured().message("Debug").emit();
    test_logger->info_structured().message("Info").emit();
    test_logger->warn_structured().message("Warn").emit();
    test_logger->error_structured().message("Error").emit();
    test_logger->fatal_structured().message("Fatal").emit();

    test_logger->flush();

    auto entries = writer_ptr->get_entries();
    ASSERT_EQ(entries.size(), 6);

    EXPECT_EQ(entries[0].level, log_level::trace);
    EXPECT_EQ(entries[1].level, log_level::debug);
    EXPECT_EQ(entries[2].level, log_level::info);
    EXPECT_EQ(entries[3].level, log_level::warn);
    EXPECT_EQ(entries[4].level, log_level::error);
    EXPECT_EQ(entries[5].level, log_level::fatal);

    test_logger->stop();
}

// Test 9: log_entry with fields
TEST_F(StructuredLoggingTest, LogEntryWithFields) {
    log_entry entry(log_level::info, "Test");

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
