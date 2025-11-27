#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file formatter_factory.h
 * @brief Factory for creating log formatter instances
 * @since 2.0.0
 *
 * @details Provides unified factory methods for creating all types of log formatters.
 * Supports both static factory methods and a registry pattern for custom formatters.
 */

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>

#include "../interfaces/log_formatter_interface.h"
#include "../formatters/json_formatter.h"
#include "../formatters/timestamp_formatter.h"

namespace kcenon::logger {

/**
 * @enum format_type
 * @brief Pre-defined format types
 */
enum class format_type {
    plain,      ///< Plain text with timestamps
    json,       ///< JSON structured format
    compact     ///< Compact single-line format
};

/**
 * @class formatter_factory
 * @brief Factory for creating log formatter instances
 *
 * @details Provides static factory methods for all built-in formatter types
 * and a registry pattern for custom formatter types.
 *
 * @since 2.0.0
 */
class formatter_factory {
public:
    using creator_fn = std::function<std::unique_ptr<log_formatter_interface>()>;

    //========================================================================
    // Static Factory Methods
    //========================================================================

    /**
     * @brief Create a plain/timestamp formatter
     * @param opts Format options
     * @return Unique pointer to timestamp formatter
     */
    static std::unique_ptr<log_formatter_interface> create_plain(
        const format_options& opts = format_options{}
    ) {
        return std::make_unique<timestamp_formatter>(opts);
    }

    /**
     * @brief Create a JSON formatter
     * @param opts Format options
     * @return Unique pointer to JSON formatter
     */
    static std::unique_ptr<log_formatter_interface> create_json(
        const format_options& opts = format_options{}
    ) {
        return std::make_unique<json_formatter>(opts);
    }

    /**
     * @brief Create a compact formatter
     * @return Unique pointer to compact formatter
     *
     * @details Compact format: minimal overhead, single line
     */
    static std::unique_ptr<log_formatter_interface> create_compact() {
        format_options opts;
        opts.include_timestamp = true;
        opts.include_thread_id = false;
        opts.include_source_location = false;
        opts.include_level = true;
        opts.use_colors = false;
        return std::make_unique<timestamp_formatter>(opts);
    }

    /**
     * @brief Create a formatter by type enum
     * @param type Format type
     * @param opts Format options (for customization)
     * @return Unique pointer to formatter
     */
    static std::unique_ptr<log_formatter_interface> create(
        format_type type,
        const format_options& opts = format_options{}
    ) {
        switch (type) {
        case format_type::json:
            return create_json(opts);
        case format_type::compact:
            return create_compact();
        case format_type::plain:
        default:
            return create_plain(opts);
        }
    }

    //========================================================================
    // Preset Factory Methods
    //========================================================================

    /**
     * @brief Create a development formatter
     * @return Colored timestamp formatter with source location
     */
    static std::unique_ptr<log_formatter_interface> create_development() {
        format_options opts;
        opts.include_timestamp = true;
        opts.include_thread_id = true;
        opts.include_source_location = true;
        opts.include_level = true;
        opts.use_colors = true;
        return create_plain(opts);
    }

    /**
     * @brief Create a production formatter
     * @return JSON formatter for structured logging
     */
    static std::unique_ptr<log_formatter_interface> create_production() {
        format_options opts;
        opts.include_timestamp = true;
        opts.include_thread_id = true;
        opts.include_source_location = false;
        opts.include_level = true;
        opts.use_colors = false;
        return create_json(opts);
    }

    /**
     * @brief Create a minimal formatter
     * @return Compact formatter with minimal fields
     */
    static std::unique_ptr<log_formatter_interface> create_minimal() {
        format_options opts;
        opts.include_timestamp = false;
        opts.include_thread_id = false;
        opts.include_source_location = false;
        opts.include_level = true;
        opts.use_colors = false;
        return create_plain(opts);
    }

    //========================================================================
    // Registry Pattern
    //========================================================================

    /**
     * @brief Register a custom formatter type
     * @param name Type name for the formatter
     * @param creator Factory function to create the formatter
     */
    static void register_type(const std::string& name, creator_fn creator) {
        registry()[name] = std::move(creator);
    }

    /**
     * @brief Create a formatter by registered name
     * @param name Registered type name
     * @return Unique pointer to formatter, or nullptr if not found
     */
    static std::unique_ptr<log_formatter_interface> create(const std::string& name) {
        auto it = registry().find(name);
        if (it == registry().end()) {
            return nullptr;
        }
        return it->second();
    }

    /**
     * @brief Check if a formatter type is registered
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
