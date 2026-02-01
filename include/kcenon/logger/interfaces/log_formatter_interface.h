#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/**
 * @file log_formatter_interface.h
 * @brief Interface for log message formatters (Strategy Pattern)
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 * @version Phase 3 - Code Quality Improvement
 *
 * @details This interface defines the contract for formatting log messages.
 * Implementing the Strategy pattern allows different formatting strategies
 * to be used interchangeably, eliminating code duplication across writers.
 */

#include <string>
#include <memory>
#include <functional>

namespace kcenon::logger {

// Forward declaration
struct log_entry;

/**
 * @struct format_options
 * @brief Configuration options for log formatting
 *
 * @details Controls which elements are included in the formatted output.
 * Different formatters may interpret these options differently based on
 * their specific output format requirements.
 *
 * @since 1.2.0
 */
struct format_options {
    /** Include timestamp in formatted output */
    bool include_timestamp = true;

    /** Include thread ID in formatted output */
    bool include_thread_id = true;

    /** Include source location (file, line, function) in formatted output when available */
    bool include_source_location = true;

    /** Use ANSI color codes (only applicable to terminal output) */
    bool use_colors = false;

    /** Include log level in formatted output */
    bool include_level = true;

    /** Pretty-print output (add indentation, line breaks for readability) */
    bool pretty_print = false;
};

/**
 * @class log_formatter_interface
 * @brief Abstract interface for log message formatters
 *
 * @details Defines the contract that all log formatters must implement.
 * Formatters are responsible for converting log_entry structures into
 * formatted strings ready for output.
 *
 * Thread-safety: Formatters should be thread-safe as they may be called
 * concurrently from multiple threads when used with async writers.
 *
 * @note Implementations should be stateless when possible to ensure thread safety.
 *
 * @since 1.2.0
 */
class log_formatter_interface {
public:
    virtual ~log_formatter_interface() = default;

    /**
     * @brief Format a log entry to a string
     * @param entry The log entry to format
     * @return Formatted string ready for output
     *
     * @details Converts the log_entry structure into a formatted string
     * according to the formatter's specific format (text, JSON, XML, etc.).
     *
     * @note This method must be thread-safe.
     *
     * @warning Implementations should handle empty/null fields gracefully.
     *
     * @since 1.2.0
     */
    virtual std::string format(const log_entry& entry) const = 0;

    /**
     * @brief Set formatting options
     * @param opts Configuration options for formatting
     *
     * @details Allows runtime configuration of which elements to include
     * in the formatted output. Not all formatters may support all options.
     *
     * @note Default implementation stores options in protected member.
     *
     * @since 1.2.0
     */
    virtual void set_options(const format_options& opts) {
        options_ = opts;
    }

    /**
     * @brief Get current formatting options
     * @return Current format options
     *
     * @since 1.2.0
     */
    virtual format_options get_options() const {
        return options_;
    }

    /**
     * @brief Get the name of this formatter
     * @return Human-readable formatter name
     *
     * @details Returns a name identifying this formatter type.
     * Useful for debugging, logging, and configuration.
     *
     * @example "timestamp_formatter", "json_formatter", "xml_formatter"
     *
     * @since 1.2.0
     */
    virtual std::string get_name() const = 0;

protected:
    /** Current formatting options */
    format_options options_;
};

/**
 * @brief Factory function type for creating formatters
 *
 * @details Can be used with dependency injection systems to create
 * formatters dynamically.
 *
 * @since 1.2.0
 * @since 4.2.0 Renamed from formatter_factory to formatter_creator_fn to avoid
 *              conflict with formatter_factory class in factories/formatter_factory.h
 */
using formatter_creator_fn = std::function<std::unique_ptr<log_formatter_interface>()>;

/**
 * @deprecated Use formatter_creator_fn instead
 * @since 1.2.0
 * @since 4.2.0 Deprecated in favor of formatter_creator_fn
 */
// Note: The old 'formatter_factory' type alias is removed to avoid conflict with
// the formatter_factory class. Use formatter_creator_fn for factory functions.

} // namespace kcenon::logger