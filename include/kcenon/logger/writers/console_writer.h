// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file console_writer.h
 * @brief Console writer for logging to stdout/stderr.
 *
 * @see log_writer_interface.h
 */

#pragma once

#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_formatter_interface.h"
#include "../interfaces/writer_category.h"

#include <kcenon/logger/logger_export.h>

#include <atomic>
#include <mutex>
#include <memory>

namespace kcenon::logger {

/**
 * @class console_writer
 * @brief Core console writer for logging to stdout/stderr
 *
 * Pure console I/O implementation with direct mutex management.
 * Designed to serve as the base layer in Decorator pattern compositions.
 *
 * Features:
 * - Color support for different log levels (if terminal supports it)
 * - Thread-safe console output with internal mutex synchronization
 * - Error levels go to stderr, others to stdout
 *
 * Category: Synchronous (blocking I/O to console)
 *
 * @since 1.0.0
 * @since 1.3.0 Refactored to use thread_safe_writer base class
 * @since 1.4.0 Added sync_writer_tag for category classification
 * @since 4.0.0 Refactored for Decorator pattern with simplified inheritance
 * @since 4.1.0 Directly implements log_writer_interface as core writer
 */
class LOGGER_SYSTEM_API console_writer : public log_writer_interface, public sync_writer_tag {
public:
    /**
     * @brief Constructor
     * @param use_stderr If true, all output goes to stderr (default: false)
     * @param auto_detect_color Auto-detect terminal color support (default: true)
     * @param formatter Custom log formatter (default: timestamp formatter)
     */
    explicit console_writer(bool use_stderr = false,
                           bool auto_detect_color = true,
                           std::unique_ptr<log_formatter_interface> formatter = nullptr);

    /**
     * @brief Destructor
     */
    ~console_writer() override;

    // Non-copyable and non-movable
    console_writer(const console_writer&) = delete;
    console_writer& operator=(const console_writer&) = delete;
    console_writer(console_writer&&) = delete;
    console_writer& operator=(console_writer&&) = delete;

    /**
     * @brief Write a log entry to console
     * @param entry The log entry to write
     * @return common::VoidResult Success or error code
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Flush console streams
     * @return common::VoidResult Success or error code
     */
    common::VoidResult flush() override;

    /**
     * @brief Get writer name
     */
    std::string get_name() const override { return "console"; }

    /**
     * @brief Check if writer is healthy
     */
    bool is_healthy() const override;

    /**
     * @brief Set whether to use stderr for all output
     * @param use_stderr Enable/disable stderr usage
     */
    void set_use_stderr(bool use_stderr);

    /**
     * @brief Set whether to use color output (if supported)
     * @param use_color true to enable color output, false to disable
     */
    void set_use_color(bool use_color);

    /**
     * @brief Get current color output setting
     * @return true if color output is enabled, false otherwise
     */
    bool use_color() const;

protected:
    /**
     * @brief Format a log entry using the current formatter
     */
    std::string format_entry(const log_entry& entry) const;

    /**
     * @brief Access the writer mutex for extended operations
     * @return Reference to the internal mutex
     */
    std::mutex& get_mutex() const { return mutex_; }

private:
    /**
     * @brief Check if terminal supports color
     * @return true if color is supported
     */
    bool is_color_supported() const;

private:
    bool use_stderr_;
    bool use_color_{true};
    std::unique_ptr<log_formatter_interface> formatter_;
    mutable std::mutex mutex_;
};

} // namespace kcenon::logger