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

    bool should_log(logger_system::log_level level, const std::string&,
                   const std::string&, int, const std::string&) const override {
        return static_cast<int>(level) >= static_cast<int>(min_level_);
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

    bool should_log(logger_system::log_level, const std::string& message,
                   const std::string&, int, const std::string&) const override {
        bool matches = std::regex_search(message, pattern_);
        return include_matches_ ? matches : !matches;
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

    bool should_log(logger_system::log_level level, const std::string& message,
                   const std::string& file, int line, const std::string& function) const override {
        if (filters_.empty()) return true;

        if (logic_ == logic_type::AND) {
            for (const auto& filter : filters_) {
                if (!filter->should_log(level, message, file, line, function)) {
                    return false;
                }
            }
            return true;
        } else { // OR logic
            for (const auto& filter : filters_) {
                if (filter->should_log(level, message, file, line, function)) {
                    return true;
                }
            }
            return false;
        }
    }
};

/**
 * @brief Function-based filter
 */
class function_filter : public log_filter_interface {
private:
    std::function<bool(logger_system::log_level, const std::string&, const std::string&, int, const std::string&)> predicate_;

public:
    explicit function_filter(decltype(predicate_) predicate) : predicate_(std::move(predicate)) {}

    bool should_log(logger_system::log_level level, const std::string& message,
                   const std::string& file, int line, const std::string& function) const override {
        return predicate_(level, message, file, line, function);
    }
};

} // namespace kcenon::logger::filters