#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/core/logger_config.h>
#include <kcenon/logger/core/error_codes.h>

namespace kcenon::logger {

/**
 * @class logger_config_builder
 * @brief Fluent builder for logger_config
 *
 * Provides a convenient and type-safe builder pattern for constructing
 * logger_config instances. Supports preset configurations and individual
 * parameter customization.
 *
 * Example usage:
 * @code
 * auto config = logger_config_builder()
 *     .use_production_defaults()
 *     .set_min_level(log_level::warn)
 *     .set_buffer_size(32768)
 *     .enable_metrics(true)
 *     .build();
 * @endcode
 *
 * @note This class is part of Sprint 4: Builder Pattern & Config initiative
 */
class logger_config_builder {
public:
    /**
     * @brief Default constructor - starts with default configuration
     */
    logger_config_builder() = default;

    // =========================================================================
    // Preset Configurations
    // =========================================================================

    /**
     * @brief Use default configuration as base
     * @return Reference to builder for chaining
     */
    logger_config_builder& use_default_config() {
        config_ = logger_config::default_config();
        return *this;
    }

    /**
     * @brief Use production configuration as base
     * @return Reference to builder for chaining
     */
    logger_config_builder& use_production_defaults() {
        config_ = logger_config::production();
        return *this;
    }

    /**
     * @brief Use debug configuration as base
     * @return Reference to builder for chaining
     */
    logger_config_builder& use_debug_defaults() {
        config_ = logger_config::debug_config();
        return *this;
    }

    /**
     * @brief Use high-performance configuration as base
     * @return Reference to builder for chaining
     */
    logger_config_builder& use_high_performance_defaults() {
        config_ = logger_config::high_performance();
        return *this;
    }

    /**
     * @brief Use low-latency configuration as base
     * @return Reference to builder for chaining
     */
    logger_config_builder& use_low_latency_defaults() {
        config_ = logger_config::low_latency();
        return *this;
    }

    // =========================================================================
    // Basic Settings
    // =========================================================================

    /**
     * @brief Set asynchronous logging mode
     * @param enabled true to enable async logging
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_async(bool enabled) {
        config_.async = enabled;
        return *this;
    }

    /**
     * @brief Set buffer size
     * @param size Buffer size in bytes
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_buffer_size(std::size_t size) {
        config_.buffer_size = size;
        return *this;
    }

    /**
     * @brief Set minimum log level
     * @param level Minimum log level to output
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_min_level(log_level level) {
        config_.min_level = level;
        return *this;
    }

    // =========================================================================
    // Performance Settings
    // =========================================================================

    /**
     * @brief Set batch size for batch writing
     * @param size Number of messages to batch together
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_batch_size(std::size_t size) {
        config_.batch_size = size;
        return *this;
    }

    /**
     * @brief Set flush interval
     * @param interval Time between automatic flushes
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_flush_interval(std::chrono::milliseconds interval) {
        config_.flush_interval = interval;
        return *this;
    }

    /**
     * @brief Enable/disable lock-free queue
     * @param enabled true to use lock-free queue
     * @return Reference to builder for chaining
     */
    logger_config_builder& use_lock_free(bool enabled) {
        config_.use_lock_free = enabled;
        return *this;
    }

    /**
     * @brief Set maximum number of writers
     * @param max Maximum writers
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_max_writers(std::size_t max) {
        config_.max_writers = max;
        return *this;
    }

    /**
     * @brief Enable/disable batch writing
     * @param enabled true to enable batch writing
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_batch_writing(bool enabled) {
        config_.enable_batch_writing = enabled;
        return *this;
    }

    /**
     * @brief Set writer thread count
     * @param count Number of writer threads
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_writer_thread_count(std::size_t count) {
        config_.writer_thread_count = count;
        return *this;
    }

    // =========================================================================
    // Feature Flags
    // =========================================================================

    /**
     * @brief Enable/disable metrics collection
     * @param enabled true to enable metrics
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_metrics(bool enabled) {
        config_.enable_metrics = enabled;
        return *this;
    }

    /**
     * @brief Enable/disable crash handler
     * @param enabled true to enable crash handler
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_crash_handler(bool enabled) {
        config_.enable_crash_handler = enabled;
        return *this;
    }

    /**
     * @brief Enable/disable structured logging
     * @param enabled true to enable structured logging
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_structured_logging(bool enabled) {
        config_.enable_structured_logging = enabled;
        return *this;
    }

    /**
     * @brief Enable/disable color output
     * @param enabled true to enable color output
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_color_output(bool enabled) {
        config_.enable_color_output = enabled;
        return *this;
    }

    /**
     * @brief Enable/disable timestamp in log messages
     * @param enabled true to enable timestamp
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_timestamp(bool enabled) {
        config_.enable_timestamp = enabled;
        return *this;
    }

    /**
     * @brief Enable/disable source location in log messages
     * @param enabled true to enable source location
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_source_location(bool enabled) {
        config_.enable_source_location = enabled;
        return *this;
    }

    /**
     * @brief Enable/disable compression
     * @param enabled true to enable compression
     * @return Reference to builder for chaining
     */
    logger_config_builder& enable_compression(bool enabled) {
        config_.enable_compression = enabled;
        return *this;
    }

    // =========================================================================
    // Queue Settings
    // =========================================================================

    /**
     * @brief Set maximum queue size
     * @param size Maximum queue size
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_max_queue_size(std::size_t size) {
        config_.max_queue_size = size;
        return *this;
    }

    /**
     * @brief Set queue overflow policy
     * @param policy Overflow policy to use
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_overflow_policy(logger_config::overflow_policy policy) {
        config_.queue_overflow_policy = policy;
        return *this;
    }

    // =========================================================================
    // File Output Settings
    // =========================================================================

    /**
     * @brief Set maximum file size for rotation
     * @param size Maximum file size in bytes
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_max_file_size(std::size_t size) {
        config_.max_file_size = size;
        return *this;
    }

    /**
     * @brief Set maximum number of rotated files
     * @param count Maximum file count
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_max_file_count(std::size_t count) {
        config_.max_file_count = count;
        return *this;
    }

    /**
     * @brief Set log directory
     * @param directory Directory path for log files
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_log_directory(const std::string& directory) {
        config_.log_directory = directory;
        return *this;
    }

    /**
     * @brief Set log file prefix
     * @param prefix Prefix for log file names
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_log_file_prefix(const std::string& prefix) {
        config_.log_file_prefix = prefix;
        return *this;
    }

    // =========================================================================
    // Network Settings
    // =========================================================================

    /**
     * @brief Set remote logging host
     * @param host Remote host address
     * @param port Remote port number
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_remote_logging(const std::string& host, uint16_t port) {
        config_.remote_host = host;
        config_.remote_port = port;
        return *this;
    }

    /**
     * @brief Set network timeout
     * @param timeout Timeout duration
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_network_timeout(std::chrono::milliseconds timeout) {
        config_.network_timeout = timeout;
        return *this;
    }

    /**
     * @brief Set network retry count
     * @param count Number of retries
     * @return Reference to builder for chaining
     */
    logger_config_builder& set_network_retry_count(std::size_t count) {
        config_.network_retry_count = count;
        return *this;
    }

    // =========================================================================
    // Build Methods
    // =========================================================================

    /**
     * @brief Build the logger_config
     * @return result<logger_config> containing the config or validation error
     *
     * Validates the configuration and returns it if valid.
     */
    result<logger_config> build() const {
        auto validation_result = config_.validate();
        if (!validation_result.is_ok()) {
            return result<logger_config>{
                logger_error_code::invalid_configuration,
                "Configuration validation failed: " + validation_result.error().message};
        }
        return result<logger_config>(config_);
    }

    /**
     * @brief Build the logger_config without validation
     * @return logger_config instance
     * @warning Use only if you are certain the configuration is valid
     */
    logger_config build_unchecked() const {
        return config_;
    }

    /**
     * @brief Reset to default configuration
     * @return Reference to builder for chaining
     */
    logger_config_builder& reset() {
        config_ = logger_config{};
        return *this;
    }

private:
    logger_config config_;
};

} // namespace kcenon::logger
