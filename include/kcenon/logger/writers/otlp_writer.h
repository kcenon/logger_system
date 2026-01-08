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
 * @file otlp_writer.h
 * @brief OpenTelemetry Protocol (OTLP) log writer for observability
 * @author 🍀☀🌕🌥 🌊
 * @since 3.0.0
 *
 * @details This file defines the OTLP writer for exporting logs to
 * OpenTelemetry-compatible collectors. Supports both HTTP and gRPC
 * transport protocols with batch export for efficiency.
 *
 * @note Requires LOGGER_ENABLE_OTLP=ON and opentelemetry-cpp dependency.
 *
 * @example Basic usage:
 * @code
 * otlp_writer::config cfg{
 *     .endpoint = "http://otel-collector:4318/v1/logs",
 *     .protocol = otlp_writer::protocol_type::http,
 *     .service_name = "my-service",
 *     .service_version = "1.0.0"
 * };
 *
 * auto writer = std::make_unique<otlp_writer>(cfg);
 * logger->add_writer("otlp", std::move(writer));
 * @endcode
 */

#include "base_writer.h"
#include "../otlp/otel_context.h"

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

namespace kcenon::logger {

/**
 * @class otlp_writer
 * @brief OTLP log exporter for OpenTelemetry integration
 *
 * @details Exports log records to OpenTelemetry collectors using the OTLP
 * protocol. Supports:
 * - HTTP and gRPC transport
 * - Batch export for efficiency
 * - Automatic retry with exponential backoff
 * - Graceful degradation when collector unavailable
 *
 * @note This writer batches logs for network efficiency. Logs may be
 * delayed by up to flush_interval before being sent.
 *
 * @warning Requires OpenTelemetry collector endpoint to be available.
 * Logs are dropped if collector is unavailable and queue is full.
 *
 * @since 3.0.0
 */
class otlp_writer : public base_writer {
public:
    /**
     * @enum protocol_type
     * @brief Transport protocol for OTLP export
     */
    enum class protocol_type {
        http,   ///< OTLP/HTTP (port 4318 by default)
        grpc    ///< OTLP/gRPC (port 4317 by default)
    };

    /**
     * @struct config
     * @brief Configuration for OTLP writer
     */
    struct config {
        /**
         * @brief OTLP endpoint URL
         *
         * @details For HTTP: http://host:4318/v1/logs
         *          For gRPC: host:4317
         */
        std::string endpoint = "http://localhost:4318/v1/logs";

        /**
         * @brief Transport protocol (HTTP or gRPC)
         */
        protocol_type protocol = protocol_type::http;

        /**
         * @brief Connection timeout
         */
        std::chrono::milliseconds timeout{5000};

        /**
         * @brief Use TLS for connection
         */
        bool use_tls = false;

        /**
         * @brief Service name (required for resource attributes)
         */
        std::string service_name;

        /**
         * @brief Service version
         */
        std::string service_version;

        /**
         * @brief Service namespace
         */
        std::string service_namespace;

        /**
         * @brief Service instance ID
         */
        std::string service_instance_id;

        /**
         * @brief Custom resource attributes
         */
        std::unordered_map<std::string, std::string> resource_attributes;

        /**
         * @brief Maximum batch size before forced flush
         */
        std::size_t max_batch_size = 512;

        /**
         * @brief Maximum time to wait before flushing batch
         */
        std::chrono::milliseconds flush_interval{5000};

        /**
         * @brief Maximum queue size (logs dropped if exceeded)
         */
        std::size_t max_queue_size = 10000;

        /**
         * @brief Number of retry attempts on failure
         */
        std::size_t max_retries = 3;

        /**
         * @brief Initial retry delay (doubled on each retry)
         */
        std::chrono::milliseconds retry_delay{100};

        /**
         * @brief HTTP headers for authentication
         */
        std::unordered_map<std::string, std::string> headers;
    };

    /**
     * @struct export_stats
     * @brief Statistics snapshot for OTLP export (copyable)
     */
    struct export_stats {
        uint64_t logs_exported{0};
        uint64_t logs_dropped{0};
        uint64_t export_success{0};
        uint64_t export_failures{0};
        uint64_t retries{0};
        std::chrono::system_clock::time_point last_export;
        std::chrono::system_clock::time_point last_error;
    };

    /**
     * @struct internal_stats
     * @brief Internal atomic statistics (non-copyable)
     */
    struct internal_stats {
        std::atomic<uint64_t> logs_exported{0};
        std::atomic<uint64_t> logs_dropped{0};
        std::atomic<uint64_t> export_success{0};
        std::atomic<uint64_t> export_failures{0};
        std::atomic<uint64_t> retries{0};
        std::chrono::system_clock::time_point last_export;
        std::chrono::system_clock::time_point last_error;
    };

    /**
     * @brief Constructor with configuration
     * @param cfg Configuration for OTLP export
     */
    explicit otlp_writer(const config& cfg);

    /**
     * @brief Destructor - flushes remaining logs
     */
    ~otlp_writer() override;

    // Non-copyable, non-movable
    otlp_writer(const otlp_writer&) = delete;
    otlp_writer& operator=(const otlp_writer&) = delete;
    otlp_writer(otlp_writer&&) = delete;
    otlp_writer& operator=(otlp_writer&&) = delete;

    /**
     * @brief Write a log entry (base_writer interface)
     */
    common::VoidResult write(logger_system::log_level level,
                             const std::string& message,
                             const std::string& file,
                             int line,
                             const std::string& function,
                             const std::chrono::system_clock::time_point& timestamp) override;

    /**
     * @brief Write a log entry with OTEL context
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Flush pending logs
     */
    common::VoidResult flush() override;

    /**
     * @brief Get writer name
     */
    std::string get_name() const override { return "otlp"; }

    /**
     * @brief Check if writer is healthy
     */
    bool is_healthy() const override;

    /**
     * @brief Get export statistics
     */
    export_stats get_stats() const;

    /**
     * @brief Force immediate export of current batch
     */
    void force_export();

private:
    /**
     * @struct buffered_log
     * @brief Internal log entry for batching
     */
    struct buffered_log {
        logger_system::log_level level;
        std::string message;
        std::string file;
        int line;
        std::string function;
        std::chrono::system_clock::time_point timestamp;
        std::optional<otlp::otel_context> otel_ctx;
    };

    // Background export thread
    void export_thread_func();

    // Export batch to collector
    bool export_batch(const std::vector<buffered_log>& batch);

    // Convert log level to OTLP severity
    static int to_otlp_severity(logger_system::log_level level);

#ifdef LOGGER_HAS_OTLP
    // Export using OpenTelemetry SDK
    bool export_with_otel_sdk(const std::vector<buffered_log>& batch);
#else
    // Fallback: Export using HTTP directly
    bool export_with_http(const std::vector<buffered_log>& batch);

    // JSON string escaping helper
    static std::string escape_json(const std::string& str);
#endif

private:
    config config_;
    internal_stats stats_;

    // Queue management
    std::queue<buffered_log> queue_;
    mutable std::mutex queue_mutex_;
    std::condition_variable queue_cv_;

    // Background thread
    std::unique_ptr<std::jthread> export_thread_;
    std::atomic<bool> running_{false};
    std::atomic<bool> healthy_{true};

#ifdef LOGGER_HAS_OTLP
    // OpenTelemetry SDK components (forward declared, implemented in cpp)
    class otel_impl;
    std::unique_ptr<otel_impl> otel_impl_;
#endif
};

} // namespace kcenon::logger
