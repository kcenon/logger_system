#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file critical_writer.h
 * @brief Synchronous wrapper for critical log messages to prevent loss
 * @author kcenon
 * @since 1.1.0
 *
 * @details This writer ensures critical logs are immediately written to disk
 * and flushed, preventing message loss during crashes or abnormal termination.
 * It wraps any base_writer and forces synchronous operation for critical/fatal
 * messages while allowing async operation for lower severity levels.
 *
 * Key Features:
 * - Immediate flush for critical/fatal messages (no queuing)
 * - Signal handler registration for graceful shutdown
 * - Dual-mode operation: sync for critical, async for others
 * - Write-ahead logging option for maximum durability
 * - Crash recovery support
 *
 * @example
 * @code
 * // Wrap a file writer to ensure critical logs are never lost
 * auto file_writer = std::make_unique<file_writer>("app.log");
 * auto critical_wrapper = std::make_unique<critical_writer>(
 *     std::move(file_writer),
 *     critical_writer_config{
 *         .force_flush_on_critical = true,
 *         .enable_signal_handlers = true,
 *         .write_ahead_log = true
 *     }
 * );
 *
 * logger.add_writer(std::move(critical_wrapper));
 *
 * // Critical logs are guaranteed to be written immediately
 * logger.log(log_level::critical, "Fatal error - shutting down");
 * @endcode
 */

#include "base_writer.h"
#include <atomic>
#include <memory>
#include <mutex>
#include <fstream>
#include <csignal>
#include <functional>

namespace kcenon::logger {

/**
 * @struct critical_writer_config
 * @brief Configuration for critical log writer
 */
struct critical_writer_config {
    /// Force immediate flush for critical/fatal messages (default: true)
    bool force_flush_on_critical = true;

    /// Force immediate flush for error messages (default: false)
    bool force_flush_on_error = false;

    /// Enable signal handlers for graceful shutdown (default: false)
    /// @deprecated Signal handling should be managed by logger and signal_manager (DI pattern)
    /// @since 2.0.0 Changed default from true to false
    /// @note This field is deprecated. Use logger_context for signal handling instead.
    bool enable_signal_handlers = false;

    /// Enable write-ahead logging for maximum durability (default: false)
    bool write_ahead_log = false;

    /// Path for write-ahead log file (only if write_ahead_log is true)
    std::string wal_path = "logs/.wal";

    /// Sync file descriptor after each critical write (default: true)
    bool sync_on_critical = true;

    /// Timeout for critical write operations in milliseconds (0 = no timeout)
    uint32_t critical_write_timeout_ms = 5000;
};

/**
 * @class critical_writer
 * @brief Synchronous wrapper ensuring critical logs are never lost
 *
 * @details This writer prevents message loss by:
 * 1. Bypassing async queue for critical/fatal messages
 * 2. Forcing immediate flush after critical writes
 * 3. Installing signal handlers to flush on abnormal termination
 * 4. Optional write-ahead logging for crash recovery
 * 5. File descriptor synchronization (fsync) for durability
 *
 * Thread Safety: All methods are thread-safe. Critical writes are serialized
 * to ensure ordering and prevent interleaving.
 */
class critical_writer : public base_writer {
public:
    /**
     * @brief Constructor
     * @param wrapped_writer The underlying writer to wrap
     * @param config Configuration options
     * @throws std::invalid_argument if wrapped_writer is null
     */
    explicit critical_writer(
        std::unique_ptr<base_writer> wrapped_writer,
        critical_writer_config config = {}
    );

    /**
     * @brief Destructor - ensures final flush
     */
    ~critical_writer() override;

    /**
     * @brief Write a log message with critical handling
     * @param level Log level
     * @param message Log message
     * @param file Source file
     * @param line Source line
     * @param function Function name
     * @param timestamp Timestamp
     * @return result_void indicating success or error
     *
     * @details For critical/fatal messages:
     * - Acquires exclusive lock
     * - Writes to WAL (if enabled)
     * - Writes to wrapped writer
     * - Forces immediate flush
     * - Syncs file descriptor (if configured)
     *
     * For non-critical messages, delegates to wrapped writer normally.
     */
    result_void write(
        logger_system::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function,
        const std::chrono::system_clock::time_point& timestamp
    ) override;

    /**
     * @brief Flush all pending messages
     * @return result_void indicating success or error
     */
    result_void flush() override;

    /**
     * @brief Check if the writer is healthy
     * @return true if healthy, false otherwise
     */
    bool is_healthy() const override;

    /**
     * @brief Get the name of this writer
     * @return Writer name
     */
    std::string get_name() const override;

    /**
     * @brief Set whether to use color output
     * @param use_color Enable/disable color output
     */
    void set_use_color(bool use_color) override;

    /**
     * @brief Enable/disable critical flushing at runtime
     * @param enable Enable or disable
     */
    void set_force_flush_on_critical(bool enable);

    /**
     * @brief Get current configuration
     * @return Current configuration
     */
    const critical_writer_config& get_config() const { return config_; }

    /**
     * @brief Get statistics about critical writes
     */
    struct critical_stats {
        std::atomic<uint64_t> total_critical_writes{0};
        std::atomic<uint64_t> total_flushes{0};
        std::atomic<uint64_t> wal_writes{0};
        std::atomic<uint64_t> sync_calls{0};
        std::atomic<uint64_t> signal_handler_invocations{0};
    };

    const critical_stats& get_stats() const { return stats_; }

private:
    /**
     * @brief Check if log level requires critical handling
     * @param level Log level to check
     * @return true if level requires immediate flush
     */
    bool is_critical_level(logger_system::log_level level) const;

    /**
     * @brief Write to write-ahead log
     * @param level Log level
     * @param message Message
     * @param file Source file
     * @param line Line number
     * @param function Function name
     * @param timestamp Timestamp
     */
    void write_to_wal(
        logger_system::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function,
        const std::chrono::system_clock::time_point& timestamp
    );

    /**
     * @brief Force sync of underlying file descriptor
     */
    void sync_file_descriptor();

    /**
     * @brief Install signal handlers for graceful shutdown
     */
    void install_signal_handlers();

    /**
     * @brief Restore original signal handlers
     */
    void restore_signal_handlers();

    /**
     * @brief Signal handler callback
     * @param signal Signal number
     */
    static void signal_handler(int signal);

    /// Configuration
    critical_writer_config config_;

    /// Wrapped writer
    std::unique_ptr<base_writer> wrapped_writer_;

    /// Mutex for critical section
    mutable std::mutex critical_mutex_;

    /// Write-ahead log stream
    std::unique_ptr<std::ofstream> wal_stream_;

    /// Statistics
    mutable critical_stats stats_;

    /// Flag for shutdown
    std::atomic<bool> shutting_down_{false};

    /// Global instance for signal handler (unfortunately needed for C signal API)
    static std::atomic<critical_writer*> instance_;

#ifdef _WIN32
    /// Original signal handlers (Windows)
    void (*original_sigterm_)(int) = nullptr;
    void (*original_sigint_)(int) = nullptr;
    void (*original_sigabrt_)(int) = nullptr;
#elif defined(__unix__) || defined(__APPLE__)
    /// Original signal handlers (POSIX)
    struct sigaction original_sigterm_;
    struct sigaction original_sigint_;
    struct sigaction original_sigsegv_;
    struct sigaction original_sigabrt_;
#endif
};

/**
 * @class hybrid_writer
 * @brief Combines async writer for normal logs with critical_writer for safety
 *
 * @details This is a convenience class that automatically configures:
 * - async_writer for normal/debug/info/warn logs (high performance)
 * - Synchronous immediate flush for error/critical/fatal logs (no loss)
 *
 * @example
 * @code
 * // Best of both worlds: performance + safety
 * auto hybrid = std::make_unique<hybrid_writer>(
 *     std::make_unique<rotating_file_writer>("app.log", 10_MB, 5)
 * );
 * logger.add_writer(std::move(hybrid));
 * @endcode
 */
class hybrid_writer : public base_writer {
public:
    explicit hybrid_writer(
        std::unique_ptr<base_writer> wrapped_writer,
        critical_writer_config critical_config = {},
        std::size_t async_queue_size = 10000
    );

    ~hybrid_writer() override;

    result_void write(
        logger_system::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function,
        const std::chrono::system_clock::time_point& timestamp
    ) override;

    result_void flush() override;
    bool is_healthy() const override;
    std::string get_name() const override;
    void set_use_color(bool use_color) override;

private:
    std::unique_ptr<base_writer> critical_writer_;
    std::size_t async_queue_size_;
};

} // namespace kcenon::logger
