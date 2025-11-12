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
#include <regex>
#include <functional>

namespace kcenon::logger::filters {

/**
 * @brief Level-based log filter
 */
class level_filter : public log_filter_interface {
private:
    logger_system::log_level min_level_;

public:
    explicit level_filter(logger_system::log_level min_level) : min_level_(min_level) {}

    bool should_log(const log_entry& entry) const override {
        return static_cast<int>(entry.level) >= static_cast<int>(min_level_);
    }

    std::string get_name() const override {
        return "level_filter";
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

} // namespace kcenon::logger::filters