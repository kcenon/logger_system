// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file formatters_test.cpp
 * @brief Unit tests for all log formatter implementations
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/formatters/logfmt_formatter.h>
#include <kcenon/logger/formatters/template_formatter.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <chrono>
#include <string>

using namespace kcenon::logger;
using log_level = kcenon::common::interfaces::log_level;

// =============================================================================
// Helper to create test log entries
// =============================================================================

namespace {

log_entry make_simple_entry(log_level level, const std::string& msg) {
    return log_entry(level, msg);
}

log_entry make_entry_with_location(log_level level, const std::string& msg) {
    return log_entry(level, msg, "test_file.cpp", 42, "test_function");
}

log_entry make_entry_with_fields(log_level level, const std::string& msg) {
    log_entry entry(level, msg);
    entry.thread_id = small_string_64("12345");
    entry.category = small_string_128("database");
    entry.fields = log_fields{};
    entry.fields->emplace("user_id", int64_t{123});
    entry.fields->emplace("latency_ms", 45.67);
    entry.fields->emplace("success", true);
    entry.fields->emplace("service", std::string("auth"));
    return entry;
}

} // namespace

// =============================================================================
// JSON Formatter
// =============================================================================

class JsonFormatterTest : public ::testing::Test {
protected:
    json_formatter formatter_;
};

TEST_F(JsonFormatterTest, BasicFormat) {
    auto entry = make_simple_entry(log_level::info, "Hello");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("{"), std::string::npos);
    EXPECT_NE(result.find("}"), std::string::npos);
    EXPECT_NE(result.find("\"message\":\"Hello\""), std::string::npos);
}

TEST_F(JsonFormatterTest, IncludesLevel) {
    auto entry = make_simple_entry(log_level::error, "Error occurred");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\"level\":\"ERROR\""), std::string::npos);
}

TEST_F(JsonFormatterTest, IncludesTimestamp) {
    auto entry = make_simple_entry(log_level::info, "test");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\"timestamp\":\""), std::string::npos);
    EXPECT_NE(result.find("Z\""), std::string::npos);
}

TEST_F(JsonFormatterTest, IncludesSourceLocation) {
    auto entry = make_entry_with_location(log_level::debug, "debug msg");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\"file\":\"test_file.cpp\""), std::string::npos);
    EXPECT_NE(result.find("\"line\":42"), std::string::npos);
    EXPECT_NE(result.find("\"function\":\"test_function\""), std::string::npos);
}

TEST_F(JsonFormatterTest, IncludesCategory) {
    auto entry = make_simple_entry(log_level::info, "msg");
    entry.category = small_string_128("network");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\"category\":\"network\""), std::string::npos);
}

TEST_F(JsonFormatterTest, IncludesStructuredFields) {
    auto entry = make_entry_with_fields(log_level::info, "test");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\"user_id\":123"), std::string::npos);
    EXPECT_NE(result.find("\"success\":true"), std::string::npos);
    EXPECT_NE(result.find("\"service\":\"auth\""), std::string::npos);
}

TEST_F(JsonFormatterTest, PrettyPrint) {
    format_options opts;
    opts.pretty_print = true;
    json_formatter pretty_fmt(opts);

    auto entry = make_simple_entry(log_level::info, "test");
    auto result = pretty_fmt.format(entry);

    // Pretty print should contain newlines
    EXPECT_NE(result.find("\n"), std::string::npos);
}

TEST_F(JsonFormatterTest, DisableTimestamp) {
    format_options opts;
    opts.include_timestamp = false;
    json_formatter fmt(opts);

    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result.find("timestamp"), std::string::npos);
}

TEST_F(JsonFormatterTest, DisableLevel) {
    format_options opts;
    opts.include_level = false;
    json_formatter fmt(opts);

    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result.find("\"level\""), std::string::npos);
}

TEST_F(JsonFormatterTest, GetName) {
    EXPECT_EQ(formatter_.get_name(), "json_formatter");
}

TEST_F(JsonFormatterTest, EscapesSpecialChars) {
    auto entry = make_simple_entry(log_level::info, "line1\nline2\ttab");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\\n"), std::string::npos);
    EXPECT_NE(result.find("\\t"), std::string::npos);
}

TEST_F(JsonFormatterTest, OtelContext) {
    auto entry = make_simple_entry(log_level::info, "traced");
    entry.otel_ctx = kcenon::logger::otlp::otel_context{};
    // Use valid 32-char trace_id and 16-char span_id (hex)
    entry.otel_ctx->trace_id = "0af7651916cd43dd8448eb211c80319c";
    entry.otel_ctx->span_id = "b7ad6b7169203331";
    entry.otel_ctx->trace_flags = "01";
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("\"trace_id\":\"0af7651916cd43dd8448eb211c80319c\""), std::string::npos);
    EXPECT_NE(result.find("\"span_id\":\"b7ad6b7169203331\""), std::string::npos);
    EXPECT_NE(result.find("\"trace_flags\":\"01\""), std::string::npos);
}

// =============================================================================
// Logfmt Formatter
// =============================================================================

class LogfmtFormatterTest : public ::testing::Test {
protected:
    logfmt_formatter formatter_;
};

TEST_F(LogfmtFormatterTest, BasicFormat) {
    auto entry = make_simple_entry(log_level::info, "Hello");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("level=info"), std::string::npos);
    EXPECT_NE(result.find("msg=Hello"), std::string::npos);
}

TEST_F(LogfmtFormatterTest, IncludesTimestamp) {
    auto entry = make_simple_entry(log_level::info, "test");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("ts="), std::string::npos);
    EXPECT_NE(result.find("Z"), std::string::npos);
}

TEST_F(LogfmtFormatterTest, QuotesSpacesInValues) {
    auto entry = make_simple_entry(log_level::info, "hello world");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("msg=\"hello world\""), std::string::npos);
}

TEST_F(LogfmtFormatterTest, IncludesSourceLocation) {
    auto entry = make_entry_with_location(log_level::error, "error");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("file=test_file.cpp"), std::string::npos);
    EXPECT_NE(result.find("line=42"), std::string::npos);
    EXPECT_NE(result.find("function=test_function"), std::string::npos);
}

TEST_F(LogfmtFormatterTest, IncludesCategory) {
    auto entry = make_simple_entry(log_level::info, "msg");
    entry.category = small_string_128("network");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("category=network"), std::string::npos);
}

TEST_F(LogfmtFormatterTest, IncludesStructuredFields) {
    auto entry = make_entry_with_fields(log_level::info, "test");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("user_id=123"), std::string::npos);
    EXPECT_NE(result.find("success=true"), std::string::npos);
    EXPECT_NE(result.find("service=auth"), std::string::npos);
}

TEST_F(LogfmtFormatterTest, LevelFormats) {
    auto entry_trace = make_simple_entry(log_level::trace, "t");
    EXPECT_NE(formatter_.format(entry_trace).find("level=trace"), std::string::npos);

    auto entry_debug = make_simple_entry(log_level::debug, "d");
    EXPECT_NE(formatter_.format(entry_debug).find("level=debug"), std::string::npos);

    auto entry_warn = make_simple_entry(log_level::warning, "w");
    EXPECT_NE(formatter_.format(entry_warn).find("level=warn"), std::string::npos);

    auto entry_crit = make_simple_entry(log_level::critical, "c");
    EXPECT_NE(formatter_.format(entry_crit).find("level=critical"), std::string::npos);
}

TEST_F(LogfmtFormatterTest, GetName) {
    EXPECT_EQ(formatter_.get_name(), "logfmt_formatter");
}

TEST_F(LogfmtFormatterTest, DisableLevel) {
    format_options opts;
    opts.include_level = false;
    logfmt_formatter fmt(opts);

    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result.find("level="), std::string::npos);
}

TEST_F(LogfmtFormatterTest, OtelContext) {
    auto entry = make_simple_entry(log_level::info, "traced");
    entry.otel_ctx = kcenon::logger::otlp::otel_context{};
    entry.otel_ctx->trace_id = "0af7651916cd43dd8448eb211c80319c";
    entry.otel_ctx->span_id = "b7ad6b7169203331";
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("trace_id=0af7651916cd43dd8448eb211c80319c"), std::string::npos);
    EXPECT_NE(result.find("span_id=b7ad6b7169203331"), std::string::npos);
}

// =============================================================================
// Template Formatter
// =============================================================================

class TemplateFormatterTest : public ::testing::Test {
protected:
};

TEST_F(TemplateFormatterTest, DefaultTemplate) {
    template_formatter fmt;
    auto entry = make_simple_entry(log_level::info, "Hello");
    entry.thread_id = small_string_64("1234");
    auto result = fmt.format(entry);

    EXPECT_NE(result.find("INFO"), std::string::npos);
    EXPECT_NE(result.find("Hello"), std::string::npos);
    EXPECT_NE(result.find("1234"), std::string::npos);
}

TEST_F(TemplateFormatterTest, CustomTemplate) {
    template_formatter fmt("{level} - {message}");
    auto entry = make_simple_entry(log_level::error, "Failed");
    auto result = fmt.format(entry);

    EXPECT_EQ(result.find("ERROR - Failed"), 0u);
}

TEST_F(TemplateFormatterTest, PlaceholderTimestampLocal) {
    template_formatter fmt("{timestamp_local}");
    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    // Should contain a local timestamp
    EXPECT_FALSE(result.empty());
}

TEST_F(TemplateFormatterTest, PlaceholderLevelLower) {
    template_formatter fmt("{level_lower}");
    auto entry = make_simple_entry(log_level::error, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result, "error");
}

TEST_F(TemplateFormatterTest, PlaceholderFilename) {
    template_formatter fmt("{filename}:{line}");
    auto entry = make_entry_with_location(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result, "test_file.cpp:42");
}

TEST_F(TemplateFormatterTest, PlaceholderFunction) {
    template_formatter fmt("{function}");
    auto entry = make_entry_with_location(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result, "test_function");
}

TEST_F(TemplateFormatterTest, PlaceholderCategory) {
    template_formatter fmt("{category}");
    auto entry = make_simple_entry(log_level::info, "test");
    entry.category = small_string_128("db");
    auto result = fmt.format(entry);

    EXPECT_EQ(result, "db");
}

TEST_F(TemplateFormatterTest, PlaceholderMissingOptionalField) {
    template_formatter fmt("[{thread_id}] {message}");
    auto entry = make_simple_entry(log_level::info, "test");
    // No thread_id set
    auto result = fmt.format(entry);

    EXPECT_NE(result.find("test"), std::string::npos);
    EXPECT_EQ(result.find("[]"), 0u);
}

TEST_F(TemplateFormatterTest, WidthFormatting) {
    template_formatter fmt("{level:10}|");
    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    // "INFO" (4 chars) should be padded to 10 chars
    EXPECT_NE(result.find("INFO      |"), std::string::npos);
}

TEST_F(TemplateFormatterTest, UnclosedBraceTreatedAsLiteral) {
    template_formatter fmt("hello {world");
    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_NE(result.find("hello {world"), std::string::npos);
}

TEST_F(TemplateFormatterTest, StructuredFieldPlaceholder) {
    template_formatter fmt("{user_id}");
    auto entry = make_entry_with_fields(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result, "123");
}

TEST_F(TemplateFormatterTest, SetTemplate) {
    template_formatter fmt("{level}");
    auto entry = make_simple_entry(log_level::info, "test");
    EXPECT_EQ(fmt.format(entry), "INFO");

    fmt.set_template("{message}");
    EXPECT_EQ(fmt.format(std::move(entry)), "test");
}

TEST_F(TemplateFormatterTest, GetTemplate) {
    template_formatter fmt("{level} {message}");
    EXPECT_EQ(fmt.get_template(), "{level} {message}");
}

TEST_F(TemplateFormatterTest, GetName) {
    template_formatter fmt;
    EXPECT_EQ(fmt.get_name(), "template_formatter");
}

TEST_F(TemplateFormatterTest, OtelPlaceholders) {
    template_formatter fmt("{trace_id}-{span_id}");
    auto entry = make_simple_entry(log_level::info, "test");
    entry.otel_ctx = kcenon::logger::otlp::otel_context{};
    entry.otel_ctx->trace_id = "0af7651916cd43dd8448eb211c80319c";
    entry.otel_ctx->span_id = "b7ad6b7169203331";
    auto result = fmt.format(entry);

    EXPECT_EQ(result, "0af7651916cd43dd8448eb211c80319c-b7ad6b7169203331");
}

// =============================================================================
// Timestamp Formatter
// =============================================================================

class TimestampFormatterTest : public ::testing::Test {
protected:
    timestamp_formatter formatter_;
};

TEST_F(TimestampFormatterTest, BasicFormat) {
    auto entry = make_simple_entry(log_level::info, "Hello");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("[INFO]"), std::string::npos);
    EXPECT_NE(result.find("Hello"), std::string::npos);
}

TEST_F(TimestampFormatterTest, IncludesTimestamp) {
    auto entry = make_simple_entry(log_level::info, "test");
    auto result = formatter_.format(entry);

    // Timestamp format: [YYYY-MM-DD HH:MM:SS.mmm]
    EXPECT_NE(result.find("[20"), std::string::npos);
}

TEST_F(TimestampFormatterTest, IncludesSourceLocation) {
    auto entry = make_entry_with_location(log_level::error, "error msg");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("test_file.cpp:42"), std::string::npos);
    EXPECT_NE(result.find("test_function()"), std::string::npos);
}

TEST_F(TimestampFormatterTest, IncludesThreadId) {
    auto entry = make_simple_entry(log_level::info, "test");
    entry.thread_id = small_string_64("9876");
    auto result = formatter_.format(entry);

    EXPECT_NE(result.find("[thread:9876]"), std::string::npos);
}

TEST_F(TimestampFormatterTest, WithColors) {
    format_options opts;
    opts.use_colors = true;
    timestamp_formatter color_fmt(opts);

    auto entry = make_simple_entry(log_level::error, "test");
    auto result = color_fmt.format(entry);

    // Should contain ANSI escape codes
    EXPECT_NE(result.find("\033["), std::string::npos);
}

TEST_F(TimestampFormatterTest, DisableTimestamp) {
    format_options opts;
    opts.include_timestamp = false;
    timestamp_formatter fmt(opts);

    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    // Should not start with timestamp brackets
    EXPECT_EQ(result.find("[20"), std::string::npos);
}

TEST_F(TimestampFormatterTest, DisableLevel) {
    format_options opts;
    opts.include_level = false;
    timestamp_formatter fmt(opts);

    auto entry = make_simple_entry(log_level::info, "test");
    auto result = fmt.format(entry);

    EXPECT_EQ(result.find("[INFO]"), std::string::npos);
}

TEST_F(TimestampFormatterTest, GetName) {
    EXPECT_EQ(formatter_.get_name(), "timestamp_formatter");
}

// =============================================================================
// Format options set/get
// =============================================================================

TEST(FormatOptionsTest, SetAndGetOptions) {
    timestamp_formatter fmt;

    format_options opts;
    opts.include_timestamp = false;
    opts.include_level = false;
    opts.use_colors = true;
    opts.pretty_print = true;

    fmt.set_options(opts);
    auto retrieved = fmt.get_options();

    EXPECT_FALSE(retrieved.include_timestamp);
    EXPECT_FALSE(retrieved.include_level);
    EXPECT_TRUE(retrieved.use_colors);
    EXPECT_TRUE(retrieved.pretty_print);
}
