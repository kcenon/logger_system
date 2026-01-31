#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <string>
#include <chrono>
#include <memory>
#include <kcenon/common/patterns/result.h>
#include <kcenon/logger/core/error_codes.h>

namespace kcenon::logger {

// Forward declaration
struct log_entry;

/**
 * @interface log_writer_interface
 * @brief Base interface for all log writers and decorators
 *
 * @details This interface defines the contract for all log writers.
 * It serves as the foundation for the Decorator pattern, enabling composable
 * log writer pipelines where writers can be wrapped with additional functionality
 * (e.g., buffering, encryption, async processing).
 *
 * Follows the Single Responsibility Principle - only responsible for writing logs.
 * All methods use Result-based error handling for consistent error propagation.
 *
 * @note Implementations should be thread-safe when used in concurrent contexts.
 *
 * Example: Implementations must override write(), flush(), close(), is_open(),
 * get_name(), and is_healthy() methods. See file_writer or console_writer for
 * concrete examples.
 *
 * Decorator pattern usage: Writers can be composed by wrapping a base writer
 * with decorators (e.g., buffered_writer wraps base writer, async_writer wraps
 * buffered_writer) to create composable log processing pipelines.
 *
 * @since 1.0.0
 * @since 4.0.0 Added close() and is_open() for Decorator pattern support
 */
class log_writer_interface {
public:
    virtual ~log_writer_interface() = default;

    /**
     * @brief Write a log entry
     * @param entry The log entry to write
     * @return common::VoidResult indicating success or failure
     *
     * @details This method should write the provided log entry to the underlying
     * output destination (file, console, network, etc.). The implementation may
     * buffer the data and defer actual I/O until flush() is called.
     *
     * @note Thread-safety is implementation-dependent. Consult specific writer
     * documentation for thread-safety guarantees.
     */
    virtual common::VoidResult write(const log_entry& entry) = 0;

    /**
     * @brief Flush any buffered data
     * @return common::VoidResult indicating success or failure
     *
     * @details Forces any buffered log entries to be written to the underlying
     * output destination immediately. This method should be called before
     * program termination or when immediate persistence is required.
     *
     * @note Some writers may perform synchronous I/O in this method, which
     * could block for an extended period.
     */
    virtual common::VoidResult flush() = 0;

    /**
     * @brief Close the writer and release resources
     * @return common::VoidResult indicating success or failure
     *
     * @details Closes the writer, flushing any remaining buffered data and
     * releasing all associated resources (file handles, network connections, etc.).
     * After calling close(), the writer should not be used for further operations.
     *
     * @note Implementations should be idempotent - calling close() multiple times
     * should not cause errors. Subsequent write() calls after close() may fail.
     *
     * Default implementation calls flush() and returns success. Subclasses with
     * resources (file handles, connections) should override to release them.
     *
     * @since 4.0.0
     */
    virtual common::VoidResult close() {
        return flush();
    }

    /**
     * @brief Check if writer is open and ready
     * @return true if the writer is open and ready to accept writes
     *
     * @details Returns whether the writer is in an operational state.
     * A writer is considered open if:
     * - Resources have been successfully initialized
     * - close() has not been called
     * - No fatal errors have occurred
     *
     * @note This method should be fast and non-blocking.
     *
     * Default implementation returns true (assumes always open). Subclasses with
     * closeable resources should override to check actual state.
     *
     * @since 4.0.0
     */
    [[nodiscard]] virtual auto is_open() const -> bool {
        return true;
    }

    /**
     * @brief Get the name of this writer
     * @return Writer name for identification
     *
     * @details Returns a human-readable name identifying this writer type.
     * Useful for debugging, logging, and configuration.
     *
     * @example "file", "console", "network", "async", "buffered"
     */
    virtual std::string get_name() const = 0;

    /**
     * @brief Check if the writer is healthy
     * @return true if the writer is operational
     *
     * @details Returns whether the writer is functioning correctly.
     * Unlike is_open(), this method may perform additional health checks
     * such as verifying disk space, network connectivity, or buffer status.
     *
     * @note This method may be slower than is_open() due to additional checks.
     */
    virtual bool is_healthy() const = 0;
};

/**
 * @brief Type alias for writer unique pointer
 * @details Convenience alias for managing writer lifetime and ownership
 * @since 4.0.0
 */
using log_writer_ptr = std::unique_ptr<log_writer_interface>;

} // namespace kcenon::logger