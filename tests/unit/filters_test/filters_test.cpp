// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file filters_test.cpp
 * @brief Unit tests for all log filter implementations
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/filters/log_filter.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <memory>
#include <string>

using namespace kcenon::logger;
using namespace kcenon::logger::filters;
using log_level = kcenon::common::interfaces::log_level;

// =============================================================================
// level_filter
// =============================================================================

TEST(LevelFilterTest, PassesAtOrAboveMinLevel) {
    level_filter filter(log_level::warning);

    log_entry critical_entry(log_level::critical, "critical");
    log_entry error_entry(log_level::error, "error");
    log_entry warning_entry(log_level::warning, "warning");
    log_entry info_entry(log_level::info, "info");
    log_entry debug_entry(log_level::debug, "debug");

    EXPECT_TRUE(filter.should_log(critical_entry));
    EXPECT_TRUE(filter.should_log(error_entry));
    EXPECT_TRUE(filter.should_log(warning_entry));
    EXPECT_FALSE(filter.should_log(info_entry));
    EXPECT_FALSE(filter.should_log(debug_entry));
}

TEST(LevelFilterTest, GetName) {
    level_filter filter(log_level::info);
    EXPECT_EQ(filter.get_name(), "level_filter");
}

// =============================================================================
// exact_level_filter
// =============================================================================

TEST(ExactLevelFilterTest, PassesOnlyExactLevel) {
    exact_level_filter filter(log_level::error);

    log_entry error_entry(log_level::error, "error");
    log_entry warning_entry(log_level::warning, "warning");
    log_entry critical_entry(log_level::critical, "critical");

    EXPECT_TRUE(filter.should_log(error_entry));
    EXPECT_FALSE(filter.should_log(warning_entry));
    EXPECT_FALSE(filter.should_log(critical_entry));
}

TEST(ExactLevelFilterTest, GetName) {
    exact_level_filter filter(log_level::info);
    EXPECT_EQ(filter.get_name(), "exact_level_filter");
}

// =============================================================================
// regex_filter
// =============================================================================

TEST(RegexFilterTest, IncludeMatchingMessages) {
    regex_filter filter("ERROR|FATAL", true);

    log_entry match_entry(log_level::info, "ERROR in module");
    log_entry no_match_entry(log_level::info, "everything is fine");

    EXPECT_TRUE(filter.should_log(match_entry));
    EXPECT_FALSE(filter.should_log(no_match_entry));
}

TEST(RegexFilterTest, ExcludeMatchingMessages) {
    regex_filter filter("DEBUG|TRACE", false);

    log_entry match_entry(log_level::info, "DEBUG output");
    log_entry no_match_entry(log_level::info, "normal message");

    EXPECT_FALSE(filter.should_log(match_entry));
    EXPECT_TRUE(filter.should_log(no_match_entry));
}

TEST(RegexFilterTest, GetName) {
    regex_filter filter("test");
    EXPECT_EQ(filter.get_name(), "regex_filter");
}

// =============================================================================
// composite_filter (AND logic)
// =============================================================================

TEST(CompositeFilterTest, AndLogicAllPass) {
    composite_filter filter(composite_filter::logic_type::AND);
    filter.add_filter(std::make_unique<level_filter>(log_level::info));
    filter.add_filter(std::make_unique<regex_filter>("important", true));

    log_entry entry(log_level::error, "important event");
    EXPECT_TRUE(filter.should_log(entry));
}

TEST(CompositeFilterTest, AndLogicOneFails) {
    composite_filter filter(composite_filter::logic_type::AND);
    filter.add_filter(std::make_unique<level_filter>(log_level::error));
    filter.add_filter(std::make_unique<regex_filter>("important", true));

    log_entry entry(log_level::info, "important event");
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(CompositeFilterTest, AndLogicEmpty) {
    composite_filter filter(composite_filter::logic_type::AND);

    log_entry entry(log_level::info, "anything");
    EXPECT_TRUE(filter.should_log(entry));
}

// =============================================================================
// composite_filter (OR logic)
// =============================================================================

TEST(CompositeFilterTest, OrLogicOneMatches) {
    composite_filter filter(composite_filter::logic_type::OR);
    filter.add_filter(std::make_unique<level_filter>(log_level::error));
    filter.add_filter(std::make_unique<regex_filter>("critical", true));

    log_entry entry(log_level::info, "critical issue");
    EXPECT_TRUE(filter.should_log(entry));
}

TEST(CompositeFilterTest, OrLogicNoneMatches) {
    composite_filter filter(composite_filter::logic_type::OR);
    filter.add_filter(std::make_unique<level_filter>(log_level::error));
    filter.add_filter(std::make_unique<regex_filter>("critical", true));

    log_entry entry(log_level::info, "normal operation");
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(CompositeFilterTest, OrLogicEmpty) {
    composite_filter filter(composite_filter::logic_type::OR);

    log_entry entry(log_level::info, "anything");
    EXPECT_TRUE(filter.should_log(entry));
}

TEST(CompositeFilterTest, GetNameAnd) {
    composite_filter filter(composite_filter::logic_type::AND);
    EXPECT_EQ(filter.get_name(), "composite_and_filter");
}

TEST(CompositeFilterTest, GetNameOr) {
    composite_filter filter(composite_filter::logic_type::OR);
    EXPECT_EQ(filter.get_name(), "composite_or_filter");
}

// =============================================================================
// function_filter
// =============================================================================

TEST(FunctionFilterTest, CustomPredicate) {
    function_filter filter([](const log_entry& entry) {
        return entry.message.to_string().length() > 10;
    });

    log_entry short_entry(log_level::info, "short");
    log_entry long_entry(log_level::info, "this is a longer message");

    EXPECT_FALSE(filter.should_log(short_entry));
    EXPECT_TRUE(filter.should_log(long_entry));
}

TEST(FunctionFilterTest, GetName) {
    function_filter filter([](const log_entry&) { return true; });
    EXPECT_EQ(filter.get_name(), "function_filter");
}

// =============================================================================
// field_exists_filter
// =============================================================================

TEST(FieldExistsFilterTest, PassesWhenFieldExists) {
    field_exists_filter filter("user_id", true);

    log_entry entry_with_field(log_level::info, "test");
    entry_with_field.fields = log_fields{};
    entry_with_field.fields->emplace("user_id", int64_t{42});

    log_entry entry_without_field(log_level::info, "test");

    EXPECT_TRUE(filter.should_log(entry_with_field));
    EXPECT_FALSE(filter.should_log(entry_without_field));
}

TEST(FieldExistsFilterTest, PassesWhenFieldNotExists) {
    field_exists_filter filter("user_id", false);

    log_entry entry_with_field(log_level::info, "test");
    entry_with_field.fields = log_fields{};
    entry_with_field.fields->emplace("user_id", int64_t{42});

    log_entry entry_without_field(log_level::info, "test");

    EXPECT_FALSE(filter.should_log(entry_with_field));
    EXPECT_TRUE(filter.should_log(entry_without_field));
}

TEST(FieldExistsFilterTest, NoFieldsAtAll) {
    field_exists_filter filter("user_id", true);

    log_entry entry(log_level::info, "test");
    // fields is std::nullopt by default
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldExistsFilterTest, GetName) {
    field_exists_filter filter("test");
    EXPECT_EQ(filter.get_name(), "field_exists_filter");
}

// =============================================================================
// field_value_filter
// =============================================================================

TEST(FieldValueFilterTest, MatchesExpectedValue) {
    field_value_filter filter("status", std::string("active"));

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("status", std::string("active"));

    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldValueFilterTest, DoesNotMatchDifferentValue) {
    field_value_filter filter("status", std::string("active"));

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("status", std::string("inactive"));

    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldValueFilterTest, NegateMode) {
    field_value_filter filter("status", std::string("error"), true);

    log_entry entry_error(log_level::info, "test");
    entry_error.fields = log_fields{};
    entry_error.fields->emplace("status", std::string("error"));

    log_entry entry_ok(log_level::info, "test");
    entry_ok.fields = log_fields{};
    entry_ok.fields->emplace("status", std::string("ok"));

    EXPECT_FALSE(filter.should_log(entry_error)); // matches, but negated
    EXPECT_TRUE(filter.should_log(entry_ok));     // doesn't match, negated passes
}

TEST(FieldValueFilterTest, MissingFieldNonNegate) {
    field_value_filter filter("status", std::string("active"));

    log_entry entry(log_level::info, "test");
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldValueFilterTest, MissingFieldNegate) {
    field_value_filter filter("status", std::string("active"), true);

    log_entry entry(log_level::info, "test");
    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldValueFilterTest, IntegerValue) {
    field_value_filter filter("count", int64_t{5});

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("count", int64_t{5});

    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldValueFilterTest, BooleanValue) {
    field_value_filter filter("success", true);

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("success", true);

    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldValueFilterTest, GetName) {
    field_value_filter filter("test", std::string("val"));
    EXPECT_EQ(filter.get_name(), "field_value_filter");
}

// =============================================================================
// field_range_filter
// =============================================================================

TEST(FieldRangeFilterTest, IntegerInRange) {
    field_range_filter filter("count", 1.0, 10.0);

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("count", int64_t{5});

    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldRangeFilterTest, IntegerAtBoundaryInclusive) {
    field_range_filter filter("count", 1.0, 10.0, true, true);

    log_entry entry_min(log_level::info, "test");
    entry_min.fields = log_fields{};
    entry_min.fields->emplace("count", int64_t{1});

    log_entry entry_max(log_level::info, "test");
    entry_max.fields = log_fields{};
    entry_max.fields->emplace("count", int64_t{10});

    EXPECT_TRUE(filter.should_log(entry_min));
    EXPECT_TRUE(filter.should_log(entry_max));
}

TEST(FieldRangeFilterTest, IntegerAtBoundaryExclusive) {
    field_range_filter filter("count", 1.0, 10.0, false, false);

    log_entry entry_min(log_level::info, "test");
    entry_min.fields = log_fields{};
    entry_min.fields->emplace("count", int64_t{1});

    log_entry entry_max(log_level::info, "test");
    entry_max.fields = log_fields{};
    entry_max.fields->emplace("count", int64_t{10});

    EXPECT_FALSE(filter.should_log(entry_min));
    EXPECT_FALSE(filter.should_log(entry_max));
}

TEST(FieldRangeFilterTest, DoubleInRange) {
    field_range_filter filter("latency", 0.0, 100.0);

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("latency", 45.5);

    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldRangeFilterTest, OutOfRange) {
    field_range_filter filter("count", 1.0, 10.0);

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("count", int64_t{15});

    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldRangeFilterTest, NonNumericFieldFails) {
    field_range_filter filter("name", 0.0, 100.0);

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("name", std::string("Alice"));

    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldRangeFilterTest, MissingFieldFails) {
    field_range_filter filter("count", 0.0, 100.0);

    log_entry entry(log_level::info, "test");
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldRangeFilterTest, GetName) {
    field_range_filter filter("test", 0.0, 1.0);
    EXPECT_EQ(filter.get_name(), "field_range_filter");
}

// =============================================================================
// field_regex_filter
// =============================================================================

TEST(FieldRegexFilterTest, MatchesPattern) {
    field_regex_filter filter("service", "^auth.*");

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("service", std::string("auth-service"));

    EXPECT_TRUE(filter.should_log(entry));
}

TEST(FieldRegexFilterTest, DoesNotMatchPattern) {
    field_regex_filter filter("service", "^auth.*");

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("service", std::string("user-service"));

    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldRegexFilterTest, ExcludeMode) {
    field_regex_filter filter("service", "^internal", false);

    log_entry internal_entry(log_level::info, "test");
    internal_entry.fields = log_fields{};
    internal_entry.fields->emplace("service", std::string("internal-api"));

    log_entry external_entry(log_level::info, "test");
    external_entry.fields = log_fields{};
    external_entry.fields->emplace("service", std::string("public-api"));

    EXPECT_FALSE(filter.should_log(internal_entry));
    EXPECT_TRUE(filter.should_log(external_entry));
}

TEST(FieldRegexFilterTest, NonStringFieldFails) {
    field_regex_filter filter("count", "\\d+");

    log_entry entry(log_level::info, "test");
    entry.fields = log_fields{};
    entry.fields->emplace("count", int64_t{42});

    // Non-string fields should not match (include mode)
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldRegexFilterTest, MissingField) {
    field_regex_filter filter("service", "test", true);

    log_entry entry(log_level::info, "test");
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(FieldRegexFilterTest, GetName) {
    field_regex_filter filter("test", "pattern");
    EXPECT_EQ(filter.get_name(), "field_regex_filter");
}

// =============================================================================
// category_filter
// =============================================================================

TEST(CategoryFilterTest, IncludeMatchingCategory) {
    category_filter filter({"database", "network"}, true);

    log_entry db_entry(log_level::info, "test");
    db_entry.category = small_string_128("database");

    log_entry net_entry(log_level::info, "test");
    net_entry.category = small_string_128("network");

    log_entry other_entry(log_level::info, "test");
    other_entry.category = small_string_128("security");

    EXPECT_TRUE(filter.should_log(db_entry));
    EXPECT_TRUE(filter.should_log(net_entry));
    EXPECT_FALSE(filter.should_log(other_entry));
}

TEST(CategoryFilterTest, ExcludeMatchingCategory) {
    category_filter filter({"debug", "trace"}, false);

    log_entry debug_entry(log_level::info, "test");
    debug_entry.category = small_string_128("debug");

    log_entry app_entry(log_level::info, "test");
    app_entry.category = small_string_128("application");

    EXPECT_FALSE(filter.should_log(debug_entry));
    EXPECT_TRUE(filter.should_log(app_entry));
}

TEST(CategoryFilterTest, NoCategoryIncludeMode) {
    category_filter filter({"database"}, true);

    log_entry entry(log_level::info, "test");
    // No category set
    EXPECT_FALSE(filter.should_log(entry));
}

TEST(CategoryFilterTest, NoCategoryExcludeMode) {
    category_filter filter({"database"}, false);

    log_entry entry(log_level::info, "test");
    // No category set - passes in exclude mode
    EXPECT_TRUE(filter.should_log(entry));
}

TEST(CategoryFilterTest, GetName) {
    category_filter filter({});
    EXPECT_EQ(filter.get_name(), "category_filter");
}
