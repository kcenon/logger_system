// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

/**
 * @file log_collector.h
 * @brief Asynchronous log collector using C++20 std::jthread
 *  kcenon
 * @since 1.3.0 - Standalone implementation without thread_system dependency
 *
 * @details This collector uses C++20 std::jthread with std::stop_token for
 * cooperative cancellation, eliminating the need for external thread libraries.
 */

#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/logger/interfaces/log_writer_interface.h>
#include <kcenon/logger/logger_export.h>

#include <atomic>
#include <chrono>
#include <memory>
#include <vector>

namespace kcenon::logger {

/**
 * @brief Asynchronous log collector for high-performance logging
 * 
 * Collects log entries in a queue (mutex/condition-variable backed)
 * and processes them in a background thread to minimize logging overhead.
 */
class LOGGER_SYSTEM_API log_collector {
public:
    /**
     * @brief Constructor
     * @param buffer_size Size of the log buffer (max queued entries)
     * @param batch_size Number of entries to process in each batch (default: 100)
     *                   Higher values improve throughput but increase latency
     *                   Lower values reduce latency but may decrease throughput
     */
    explicit log_collector(std::size_t buffer_size = 8192, std::size_t batch_size = 100);
    
    /**
     * @brief Destructor
     */
    ~log_collector();
    
    /**
     * @brief Enqueue a log entry
     * @param level Log level
     * @param message Log message
     * @param file Source file
     * @param line Source line
     * @param function Function name
     * @param timestamp Log timestamp
     */
    bool enqueue(common::interfaces::log_level level,
                 const std::string& message,
                 const std::string& file,
                 int line,
                 const std::string& function,
                 const std::chrono::system_clock::time_point& timestamp);
    
    /**
     * @brief Add a writer
     * @param writer Shared pointer to writer
     *
     * The collector stores a weak reference to prevent circular dependencies
     * and ensure safe access even if the writer is destroyed.
     */
    void add_writer(std::shared_ptr<log_writer_interface> writer);
    
    /**
     * @brief Clear all writers
     */
    void clear_writers();
    
    /**
     * @brief Start the background processing thread
     */
    void start();
    
    /**
     * @brief Stop the background processing thread
     */
    void stop();
    
    /**
     * @brief Flush all pending log entries
     */
    void flush();
    
    /**
     * @brief Get queue metrics
     * @return Pair of (current_size, max_capacity)
     */
    std::pair<size_t, size_t> get_queue_metrics() const;
    
private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

} // namespace kcenon::logger
