#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file filter_factory.h
 * @brief Factory for creating log filter instances
 * @since 2.0.0
 *
 * @details Provides unified factory methods for creating all types of log filters.
 * Supports both static factory methods and a registry pattern for custom filters.
 */

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "../interfaces/log_filter_interface.h"
#include "../filters/log_filter.h"

namespace kcenon::logger {

/**
 * @class filter_factory
 * @brief Factory for creating log filter instances
 *
 * @details Provides static factory methods for all built-in filter types
 * and a registry pattern for custom filter types.
 *
 * @since 2.0.0
 */
class filter_factory {
public:
    using creator_fn = std::function<std::unique_ptr<log_filter_interface>()>;

    //========================================================================
    // Static Factory Methods
    //========================================================================

    /**
     * @brief Create a level filter
     * @param min_level Minimum log level to pass through
     * @return Unique pointer to level filter
     */
    static std::unique_ptr<log_filter_interface> create_level(
        logger_system::log_level min_level
    ) {
        return std::make_unique<filters::level_filter>(min_level);
    }

    /**
     * @brief Create a regex filter
     * @param pattern Regex pattern to match
     * @param include_matches true to include matches, false to exclude
     * @return Unique pointer to regex filter
     */
    static std::unique_ptr<log_filter_interface> create_regex(
        const std::string& pattern,
        bool include_matches = true
    ) {
        return std::make_unique<filters::regex_filter>(pattern, include_matches);
    }

    /**
     * @brief Create a function-based filter
     * @param predicate Filter function
     * @return Unique pointer to function filter
     */
    static std::unique_ptr<log_filter_interface> create_function(
        std::function<bool(const log_entry&)> predicate
    ) {
        return std::make_unique<filters::function_filter>(std::move(predicate));
    }

    /**
     * @brief Create a composite filter with AND logic
     * @return Unique pointer to composite filter
     */
    static std::unique_ptr<filters::composite_filter> create_composite_and() {
        return std::make_unique<filters::composite_filter>(
            filters::composite_filter::logic_type::AND
        );
    }

    /**
     * @brief Create a composite filter with OR logic
     * @return Unique pointer to composite filter
     */
    static std::unique_ptr<filters::composite_filter> create_composite_or() {
        return std::make_unique<filters::composite_filter>(
            filters::composite_filter::logic_type::OR
        );
    }

    //========================================================================
    // Preset Factory Methods
    //========================================================================

    /**
     * @brief Create a development filter
     * @return Filter that passes all messages (trace and above)
     */
    static std::unique_ptr<log_filter_interface> create_development() {
        return create_level(logger_system::log_level::trace);
    }

    /**
     * @brief Create a production filter
     * @return Composite filter: warning+ and no sensitive data
     */
    static std::unique_ptr<log_filter_interface> create_production() {
        auto composite = create_composite_and();
        composite->add_filter(create_level(logger_system::log_level::warn));
        composite->add_filter(create_regex("password|secret|token|api.?key", false));
        return composite;
    }

    /**
     * @brief Create an error-only filter
     * @return Filter that only passes error and fatal messages
     */
    static std::unique_ptr<log_filter_interface> create_errors_only() {
        return create_level(logger_system::log_level::error);
    }

    /**
     * @brief Create a debug filter for specific component
     * @param component Component name to filter
     * @return Filter that matches messages containing the component
     */
    static std::unique_ptr<log_filter_interface> create_component_filter(
        const std::string& component
    ) {
        return create_regex("\\[" + component + "\\]", true);
    }

    /**
     * @brief Create a sensitive data filter
     * @return Filter that excludes messages with sensitive patterns
     */
    static std::unique_ptr<log_filter_interface> create_sensitive_filter() {
        return create_regex(
            "password|secret|token|api.?key|credential|auth|bearer",
            false  // Exclude matches
        );
    }

    //========================================================================
    // Builder Pattern Helper
    //========================================================================

    /**
     * @brief Filter builder for complex filter construction
     */
    class builder {
    public:
        /**
         * @brief Set minimum log level
         */
        builder& with_min_level(logger_system::log_level level) {
            filters_.push_back(create_level(level));
            return *this;
        }

        /**
         * @brief Include messages matching pattern
         */
        builder& include_pattern(const std::string& pattern) {
            filters_.push_back(create_regex(pattern, true));
            return *this;
        }

        /**
         * @brief Exclude messages matching pattern
         */
        builder& exclude_pattern(const std::string& pattern) {
            filters_.push_back(create_regex(pattern, false));
            return *this;
        }

        /**
         * @brief Add custom filter function
         */
        builder& with_predicate(std::function<bool(const log_entry&)> pred) {
            filters_.push_back(create_function(std::move(pred)));
            return *this;
        }

        /**
         * @brief Build the composite filter
         * @return Unique pointer to composite filter
         */
        std::unique_ptr<log_filter_interface> build() {
            if (filters_.empty()) {
                return nullptr;
            }
            if (filters_.size() == 1) {
                return std::move(filters_[0]);
            }

            auto composite = create_composite_and();
            for (auto& filter : filters_) {
                composite->add_filter(std::move(filter));
            }
            return composite;
        }

    private:
        std::vector<std::unique_ptr<log_filter_interface>> filters_;
    };

    /**
     * @brief Start building a filter
     * @return Filter builder
     */
    static builder create_builder() {
        return builder{};
    }

    //========================================================================
    // Registry Pattern
    //========================================================================

    /**
     * @brief Register a custom filter type
     * @param name Type name for the filter
     * @param creator Factory function to create the filter
     */
    static void register_type(const std::string& name, creator_fn creator) {
        registry()[name] = std::move(creator);
    }

    /**
     * @brief Create a filter by registered name
     * @param name Registered type name
     * @return Unique pointer to filter, or nullptr if not found
     */
    static std::unique_ptr<log_filter_interface> create(const std::string& name) {
        auto it = registry().find(name);
        if (it == registry().end()) {
            return nullptr;
        }
        return it->second();
    }

    /**
     * @brief Check if a filter type is registered
     * @param name Type name to check
     * @return true if registered
     */
    static bool has_type(const std::string& name) {
        return registry().find(name) != registry().end();
    }

private:
    static std::unordered_map<std::string, creator_fn>& registry() {
        static std::unordered_map<std::string, creator_fn> instance;
        return instance;
    }
};

} // namespace kcenon::logger
