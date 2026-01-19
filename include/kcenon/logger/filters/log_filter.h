// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file log_filter.h
 * @brief Log filtering functionality
 */

#pragma once

#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <algorithm>
#include <regex>
#include <functional>

namespace kcenon::logger::filters {

/**
 * @brief Level-based log filter (minimum level threshold)
 *
 * Passes messages at or above the specified minimum level.
 */
class level_filter : public log_filter_interface {
private:
    logger_system::log_level min_level_;

public:
    explicit level_filter(logger_system::log_level min_level) : min_level_(min_level) {}

    /// Constructor accepting common::interfaces::log_level for compatibility with kcenon::logger::log_level
    explicit level_filter(kcenon::common::interfaces::log_level min_level)
        : min_level_(static_cast<logger_system::log_level>(static_cast<int>(min_level))) {}

    bool should_log(const log_entry& entry) const override {
        return static_cast<int>(entry.level) >= static_cast<int>(min_level_);
    }

    std::string get_name() const override {
        return "level_filter";
    }
};

/**
 * @brief Exact level filter (matches only the specified level)
 *
 * Passes only messages at exactly the specified level.
 */
class exact_level_filter : public log_filter_interface {
private:
    logger_system::log_level level_;

public:
    explicit exact_level_filter(logger_system::log_level level) : level_(level) {}

    /// Constructor accepting common::interfaces::log_level for compatibility with kcenon::logger::log_level
    explicit exact_level_filter(kcenon::common::interfaces::log_level level)
        : level_(static_cast<logger_system::log_level>(static_cast<int>(level))) {}

    bool should_log(const log_entry& entry) const override {
        return entry.level == level_;
    }

    std::string get_name() const override {
        return "exact_level_filter";
    }
};

/**
 * @brief Regex-based log filter
 */
class regex_filter : public log_filter_interface {
private:
    std::regex pattern_;
    bool include_matches_;

public:
    regex_filter(const std::string& pattern, bool include_matches = true)
        : pattern_(pattern), include_matches_(include_matches) {}

    bool should_log(const log_entry& entry) const override {
        bool matches = std::regex_search(entry.message.to_string(), pattern_);
        return include_matches_ ? matches : !matches;
    }

    std::string get_name() const override {
        return "regex_filter";
    }
};

/**
 * @brief Composite filter with AND/OR logic
 */
class composite_filter : public log_filter_interface {
public:
    enum class logic_type { AND, OR };

private:
    std::vector<std::unique_ptr<log_filter_interface>> filters_;
    logic_type logic_;

public:
    explicit composite_filter(logic_type logic) : logic_(logic) {}

    void add_filter(std::unique_ptr<log_filter_interface> filter) {
        filters_.push_back(std::move(filter));
    }

    bool should_log(const log_entry& entry) const override {
        if (filters_.empty()) return true;

        if (logic_ == logic_type::AND) {
            for (const auto& filter : filters_) {
                if (!filter->should_log(entry)) {
                    return false;
                }
            }
            return true;
        } else { // OR logic
            for (const auto& filter : filters_) {
                if (filter->should_log(entry)) {
                    return true;
                }
            }
            return false;
        }
    }

    std::string get_name() const override {
        return logic_ == logic_type::AND ? "composite_and_filter" : "composite_or_filter";
    }
};

/**
 * @brief Function-based filter
 */
class function_filter : public log_filter_interface {
private:
    std::function<bool(const log_entry&)> predicate_;

public:
    explicit function_filter(decltype(predicate_) predicate) : predicate_(std::move(predicate)) {}

    bool should_log(const log_entry& entry) const override {
        return predicate_(entry);
    }

    std::string get_name() const override {
        return "function_filter";
    }
};

/**
 * @brief Filter based on structured field presence
 *
 * Passes messages that have (or don't have) the specified field.
 * @since 3.4.0
 */
class field_exists_filter : public log_filter_interface {
private:
    std::string field_name_;
    bool require_exists_;

public:
    /**
     * @brief Constructor
     * @param field_name Name of the field to check
     * @param require_exists If true, passes entries with the field; if false, passes entries without it
     */
    explicit field_exists_filter(const std::string& field_name, bool require_exists = true)
        : field_name_(field_name), require_exists_(require_exists) {}

    bool should_log(const log_entry& entry) const override {
        bool has_field = entry.fields.has_value() &&
                         entry.fields->find(field_name_) != entry.fields->end();
        return require_exists_ ? has_field : !has_field;
    }

    std::string get_name() const override {
        return "field_exists_filter";
    }
};

/**
 * @brief Filter based on structured field value
 *
 * Passes messages where the specified field matches the expected value.
 * @since 3.4.0
 */
class field_value_filter : public log_filter_interface {
private:
    std::string field_name_;
    log_value expected_value_;
    bool negate_;

public:
    /**
     * @brief Constructor
     * @param field_name Name of the field to check
     * @param expected_value Expected value of the field
     * @param negate If true, passes entries where field does NOT match value
     */
    field_value_filter(const std::string& field_name, log_value expected_value, bool negate = false)
        : field_name_(field_name), expected_value_(std::move(expected_value)), negate_(negate) {}

    bool should_log(const log_entry& entry) const override {
        if (!entry.fields.has_value()) {
            return negate_;  // No fields: pass if negating
        }

        auto it = entry.fields->find(field_name_);
        if (it == entry.fields->end()) {
            return negate_;  // Field not found: pass if negating
        }

        bool matches = it->second == expected_value_;
        return negate_ ? !matches : matches;
    }

    std::string get_name() const override {
        return "field_value_filter";
    }
};

/**
 * @brief Filter based on structured field value range (for numeric types)
 *
 * Passes messages where the specified numeric field is within the given range.
 * @since 3.4.0
 */
class field_range_filter : public log_filter_interface {
private:
    std::string field_name_;
    double min_value_;
    double max_value_;
    bool inclusive_min_;
    bool inclusive_max_;

public:
    /**
     * @brief Constructor
     * @param field_name Name of the numeric field to check
     * @param min_value Minimum value (use -infinity for no lower bound)
     * @param max_value Maximum value (use +infinity for no upper bound)
     * @param inclusive_min If true, min_value is included in range
     * @param inclusive_max If true, max_value is included in range
     */
    field_range_filter(const std::string& field_name,
                       double min_value,
                       double max_value,
                       bool inclusive_min = true,
                       bool inclusive_max = true)
        : field_name_(field_name)
        , min_value_(min_value)
        , max_value_(max_value)
        , inclusive_min_(inclusive_min)
        , inclusive_max_(inclusive_max) {}

    bool should_log(const log_entry& entry) const override {
        if (!entry.fields.has_value()) {
            return false;
        }

        auto it = entry.fields->find(field_name_);
        if (it == entry.fields->end()) {
            return false;
        }

        // Extract numeric value
        double value = 0.0;
        if (std::holds_alternative<int64_t>(it->second)) {
            value = static_cast<double>(std::get<int64_t>(it->second));
        } else if (std::holds_alternative<double>(it->second)) {
            value = std::get<double>(it->second);
        } else {
            return false;  // Not a numeric field
        }

        // Check range
        bool above_min = inclusive_min_ ? (value >= min_value_) : (value > min_value_);
        bool below_max = inclusive_max_ ? (value <= max_value_) : (value < max_value_);
        return above_min && below_max;
    }

    std::string get_name() const override {
        return "field_range_filter";
    }
};

/**
 * @brief Filter based on string field pattern matching
 *
 * Passes messages where the specified string field matches a regex pattern.
 * @since 3.4.0
 */
class field_regex_filter : public log_filter_interface {
private:
    std::string field_name_;
    std::regex pattern_;
    bool include_matches_;

public:
    /**
     * @brief Constructor
     * @param field_name Name of the string field to check
     * @param pattern Regex pattern to match against
     * @param include_matches If true, passes matching entries; if false, passes non-matching
     */
    field_regex_filter(const std::string& field_name,
                       const std::string& pattern,
                       bool include_matches = true)
        : field_name_(field_name)
        , pattern_(pattern)
        , include_matches_(include_matches) {}

    bool should_log(const log_entry& entry) const override {
        if (!entry.fields.has_value()) {
            return !include_matches_;
        }

        auto it = entry.fields->find(field_name_);
        if (it == entry.fields->end()) {
            return !include_matches_;
        }

        // Only works on string fields
        if (!std::holds_alternative<std::string>(it->second)) {
            return !include_matches_;
        }

        const auto& str_value = std::get<std::string>(it->second);
        bool matches = std::regex_search(str_value, pattern_);
        return include_matches_ ? matches : !matches;
    }

    std::string get_name() const override {
        return "field_regex_filter";
    }
};

/**
 * @brief Filter based on category field
 *
 * Passes messages with matching category (from log_entry.category).
 * @since 3.4.0
 */
class category_filter : public log_filter_interface {
private:
    std::vector<std::string> categories_;
    bool include_;

public:
    /**
     * @brief Constructor
     * @param categories List of category names to match
     * @param include If true, passes matching categories; if false, excludes them
     */
    category_filter(std::vector<std::string> categories, bool include = true)
        : categories_(std::move(categories)), include_(include) {}

    bool should_log(const log_entry& entry) const override {
        if (!entry.category.has_value()) {
            return !include_;  // No category: pass if excluding
        }

        std::string cat = entry.category->to_string();
        bool found = std::find(categories_.begin(), categories_.end(), cat) != categories_.end();
        return include_ ? found : !found;
    }

    std::string get_name() const override {
        return "category_filter";
    }
};

} // namespace kcenon::logger::filters