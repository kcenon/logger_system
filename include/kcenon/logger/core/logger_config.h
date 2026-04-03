// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file logger_config.h
 * @brief Configuration structure for logger with validation.
 *
 * @see logger_config_builder.h For fluent construction
 */

#pragma once

#include <chrono>
#include <string>
#include <limits>
#include <kcenon/logger/core/thread_integration_detector.h>
#include <kcenon/logger/core/error_codes.h>

// Use common_system's standard interface
#include <kcenon/common/interfaces/logger_interface.h>

namespace kcenon::logger {

// Type alias for log_level using common::interfaces::log_level as the canonical type
using log_level = common::interfaces::log_level;

/**
 * @struct logger_config
 * @brief Configuration structure for logger with validation
 * 
 * This structure holds all configuration parameters for the logger
 * and provides validation to ensure configuration correctness.
 */
struct logger_config {
    /// @name Basic settings
    /// @{
    bool async = true;                              ///< Enable asynchronous logging.
    std::size_t buffer_size = 8192;                 ///< Internal buffer size in bytes.
    log_level min_level = log_level::info;           ///< Minimum log level to process.
    /// @}

    /// @name Performance settings
    /// @{
    std::size_t batch_size = 100;                   ///< Number of messages per batch write.
    std::chrono::milliseconds flush_interval{1000}; ///< Interval between automatic flushes.
    bool use_lock_free = false;                     ///< Use lock-free queue implementation.
    std::size_t max_writers = 10;                   ///< Maximum number of concurrent writers.
    bool enable_batch_writing = false;              ///< Enable batch writing mode.
    /// @}

    /// @name Feature flags
    /// @{
    bool enable_metrics = false;                    ///< Enable performance metrics collection.
    bool enable_crash_handler = false;              ///< Enable crash signal handler.
    bool enable_structured_logging = false;         ///< Enable structured (JSON) log output.
    bool enable_color_output = true;                ///< Enable ANSI color output.
    bool enable_timestamp = true;                   ///< Include timestamp in log entries.
    bool enable_source_location = false;            ///< Include source file/line in log entries.
    /// @}

    /// @name Queue settings
    /// @{
    std::size_t max_queue_size = 10000;             ///< Maximum number of queued messages.
    /**
     * @enum overflow_policy
     * @brief Policy for handling queue overflow when max_queue_size is reached.
     */
    enum class overflow_policy {
        drop_oldest,    ///< Drop oldest messages when queue is full.
        drop_newest,    ///< Drop new messages when queue is full (default).
        block,          ///< Block until space is available.
        grow            ///< Dynamically grow the queue (use with caution).
    };
    overflow_policy queue_overflow_policy = overflow_policy::drop_newest; ///< Active overflow policy.
    /// @}

    /// @name File output settings
    /// @{
    std::size_t max_file_size = 100 * 1024 * 1024;  ///< Maximum file size in bytes (default: 100MB).
    std::size_t max_file_count = 5;                  ///< Maximum number of rotating log files.
    std::string log_directory = "./logs";             ///< Directory for log file output.
    std::string log_file_prefix = "app";             ///< Prefix for log file names.
    /// @}

    /// @name Network settings
    /// @{
    std::string remote_host = "";                    ///< Remote log collector hostname.
    uint16_t remote_port = 0;                        ///< Remote log collector port.
    std::chrono::milliseconds network_timeout{5000}; ///< Network operation timeout.
    std::size_t network_retry_count = 3;             ///< Number of network retry attempts.
    /// @}

    /// @name Performance tuning
    /// @{
    std::size_t writer_thread_count = 1;             ///< Number of dedicated writer threads.
    bool enable_compression = false;                 ///< Enable log compression.
    
    /**
     * @brief Validate the configuration
     * @return common::VoidResult indicating success or specific validation error
     */
    common::VoidResult validate() const {
        // Validate buffer size
        if (buffer_size == 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Buffer size must be greater than 0");
        }

        if (buffer_size > std::numeric_limits<std::size_t>::max() / 2) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Buffer size is too large");
        }

        // Validate batch size
        if (batch_size == 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Batch size must be greater than 0");
        }

        if (batch_size > buffer_size) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Batch size cannot exceed buffer size");
        }

        // Validate flush interval
        if (flush_interval.count() < 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Flush interval must be non-negative");
        }

        if (flush_interval.count() > 3600000) {  // 1 hour max
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Flush interval too large (max 1 hour)");
        }

        // Validate queue settings
        if (max_queue_size == 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Max queue size must be greater than 0");
        }

        if (max_queue_size < batch_size) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Max queue size must be at least as large as batch size");
        }

        // Validate file settings
        if (max_file_size < 1024) {  // Minimum 1KB
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Max file size too small (minimum 1KB)");
        }

        if (max_file_count == 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Max file count must be greater than 0");
        }

        if (max_file_count > 1000) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Max file count too large (max 1000)");
        }

        // Validate network settings if configured
        if (!remote_host.empty()) {
            if (remote_port == 0) {
                return make_logger_void_result(logger_error_code::invalid_configuration,
                                "Remote port must be specified when remote host is set");
            }

            if (network_timeout.count() <= 0) {
                return make_logger_void_result(logger_error_code::invalid_configuration,
                                "Network timeout must be positive");
            }

            if (network_retry_count > 100) {
                return make_logger_void_result(logger_error_code::invalid_configuration,
                                "Network retry count too large (max 100)");
            }
        }

        // Validate writer settings
        if (max_writers == 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Must allow at least one writer");
        }

        if (max_writers > 100) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Max writers too large (max 100)");
        }

        // Validate thread count
        if (writer_thread_count == 0) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Writer thread count must be at least 1");
        }

        if (writer_thread_count > 32) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Writer thread count too large (max 32)");
        }

        // Validate feature combinations
        if (use_lock_free && queue_overflow_policy == overflow_policy::grow) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Lock-free queue cannot use grow overflow policy");
        }

        if (!async && batch_size > 1) {
            return make_logger_void_result(logger_error_code::invalid_configuration,
                            "Batch processing requires async mode");
        }

        return common::ok();
    }
    
    /**
     * @brief Create a default configuration
     * @return Default logger configuration
     */
    static logger_config default_config() {
        return logger_config{};
    }
    
    /**
     * @brief Create a high-performance configuration
     * @return Configuration optimized for performance
     */
    static logger_config high_performance() {
        logger_config config;
        config.async = true;
        config.buffer_size = 65536;
        config.batch_size = 500;
        config.flush_interval = std::chrono::milliseconds(5000);
        config.use_lock_free = true;
        config.max_queue_size = 100000;
        config.writer_thread_count = 2;
        config.enable_compression = true;
        config.enable_batch_writing = true;
        return config;
    }
    
    /**
     * @brief Create a low-latency configuration
     * @return Configuration optimized for low latency
     */
    static logger_config low_latency() {
        logger_config config;
        config.async = true;
        config.buffer_size = 4096;
        config.batch_size = 10;
        config.flush_interval = std::chrono::milliseconds(10);
        config.use_lock_free = true;
        config.max_queue_size = 10000;
        config.queue_overflow_policy = overflow_policy::drop_oldest;
        return config;
    }
    
    /**
     * @brief Create a debug configuration
     * @return Configuration optimized for debugging
     */
    static logger_config debug_config() {
        logger_config config;
        config.async = false;  // Synchronous for immediate output
        config.min_level = log_level::trace;
        config.enable_metrics = true;
        config.enable_crash_handler = true;
        config.enable_color_output = true;
        config.batch_size = 1;
        config.flush_interval = std::chrono::milliseconds(0);
        return config;
    }
    
    /**
     * @brief Create a production configuration
     * @return Configuration optimized for production use
     */
    static logger_config production() {
        logger_config config;
        config.async = true;
        config.buffer_size = 16384;
        config.min_level = log_level::warning;
        config.enable_metrics = true;
        config.enable_crash_handler = true;
        config.enable_color_output = false;
        config.max_file_size = 500 * 1024 * 1024;  // 500MB
        config.max_file_count = 10;
        config.enable_compression = true;
        config.enable_batch_writing = true;
        config.batch_size = 200;
        return config;
    }
};

} // namespace kcenon::logger
