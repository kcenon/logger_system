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

#include "../monitoring/monitoring_interface.h"
#include "../error_codes.h"
#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>
#include <atomic>
#include <mutex>
#include <functional>
#include <chrono>
#include <optional>
#include <string>

namespace logger_module {

// Forward declarations
class base_writer;
class log_collector;

/**
 * @brief Health check criteria for different components
 */
struct health_check_criteria {
    // Writer health criteria
    size_t max_consecutive_write_failures = 5;
    std::chrono::milliseconds max_write_latency{1000};
    
    // Buffer health criteria  
    double max_buffer_usage_percent = 90.0;
    size_t min_available_buffer_size = 100;
    
    // Queue health criteria
    double max_queue_usage_percent = 85.0;
    size_t max_queue_size = 10000;
    std::chrono::milliseconds max_queue_wait_time{500};
    
    // System health criteria
    double max_memory_usage_mb = 500.0;
    double max_cpu_usage_percent = 80.0;
};

/**
 * @brief Writer health information
 */
struct writer_health_info {
    std::string writer_name;
    health_status status = health_status::healthy;
    size_t total_writes = 0;
    size_t failed_writes = 0;
    size_t consecutive_failures = 0;
    std::chrono::milliseconds avg_write_latency{0};
    std::chrono::milliseconds max_write_latency{0};
    std::chrono::system_clock::time_point last_write_time;
    std::chrono::system_clock::time_point last_failure_time;
    std::vector<std::string> recent_errors;
    
    double get_failure_rate() const {
        return total_writes > 0 ? 
            static_cast<double>(failed_writes) / total_writes * 100.0 : 0.0;
    }
};

/**
 * @brief Buffer health information
 */
struct buffer_health_info {
    size_t total_capacity = 0;
    size_t used_capacity = 0;
    size_t available_capacity = 0;
    double usage_percent = 0.0;
    size_t total_allocations = 0;
    size_t failed_allocations = 0;
    health_status status = health_status::healthy;
};

/**
 * @brief Queue health information
 */
struct queue_health_info {
    size_t max_size = 0;
    size_t current_size = 0;
    size_t total_enqueued = 0;
    size_t total_dequeued = 0;
    size_t dropped_messages = 0;
    double usage_percent = 0.0;
    std::chrono::milliseconds avg_wait_time{0};
    std::chrono::milliseconds max_wait_time{0};
    health_status status = health_status::healthy;
};

/**
 * @brief Comprehensive health check system for logger components
 * 
 * Monitors health of:
 * - Individual writers (file, console, network, etc.)
 * - Buffer systems
 * - Queue systems
 * - Overall system resources
 */
class health_check_system {
public:
    /**
     * @brief Constructor
     * @param criteria Health check criteria to use
     */
    explicit health_check_system(const health_check_criteria& criteria = {});
    
    /**
     * @brief Destructor
     */
    ~health_check_system();
    
    /**
     * @brief Register a writer for health monitoring
     * @param name Unique name for the writer
     * @param writer Pointer to the writer
     */
    void register_writer(const std::string& name, base_writer* writer);
    
    /**
     * @brief Unregister a writer from health monitoring
     * @param name Name of the writer to unregister
     */
    void unregister_writer(const std::string& name);
    
    /**
     * @brief Register a log collector for queue health monitoring
     * @param collector Pointer to the log collector
     */
    void register_collector(log_collector* collector);
    
    /**
     * @brief Perform comprehensive health check
     * @return Health check result with detailed information
     */
    health_check_result perform_health_check();
    
    /**
     * @brief Check health of a specific writer
     * @param name Name of the writer
     * @return Result containing writer health information
     */
    result<writer_health_info> check_writer_health(const std::string& name);
    
    /**
     * @brief Check buffer system health
     * @return Buffer health information
     */
    buffer_health_info check_buffer_health();
    
    /**
     * @brief Check queue system health
     * @return Queue health information
     */
    queue_health_info check_queue_health();
    
    /**
     * @brief Update writer statistics after a write operation
     * @param name Writer name
     * @param success Whether the write was successful
     * @param latency Write operation latency
     */
    void update_writer_stats(const std::string& name, 
                            bool success, 
                            std::chrono::milliseconds latency);
    
    /**
     * @brief Update buffer statistics
     * @param used Used capacity
     * @param total Total capacity
     * @param allocation_failed Whether an allocation failed
     */
    void update_buffer_stats(size_t used, size_t total, bool allocation_failed = false);
    
    /**
     * @brief Update queue statistics
     * @param current_size Current queue size
     * @param max_size Maximum queue size
     * @param dropped Whether a message was dropped
     * @param wait_time Time spent waiting in queue
     */
    void update_queue_stats(size_t current_size, 
                           size_t max_size,
                           bool dropped = false,
                           std::chrono::milliseconds wait_time = std::chrono::milliseconds{0});
    
    /**
     * @brief Start automatic health monitoring
     * @param interval Check interval
     * @param callback Callback for health check results
     */
    void start_monitoring(std::chrono::seconds interval,
                         std::function<void(const health_check_result&)> callback);
    
    /**
     * @brief Stop automatic health monitoring
     */
    void stop_monitoring();
    
    /**
     * @brief Check if monitoring is active
     * @return true if monitoring is running
     */
    bool is_monitoring() const;
    
    /**
     * @brief Get all writer health information
     * @return Map of writer names to health info
     */
    std::unordered_map<std::string, writer_health_info> get_all_writer_health() const;
    
    /**
     * @brief Get health check criteria
     * @return Current health check criteria
     */
    health_check_criteria get_criteria() const;
    
    /**
     * @brief Update health check criteria
     * @param criteria New criteria to use
     */
    void set_criteria(const health_check_criteria& criteria);
    
    /**
     * @brief Reset all health statistics
     */
    void reset_stats();
    
    /**
     * @brief Register a custom health check
     * @param name Name of the custom check
     * @param check Function that performs the check
     */
    void register_custom_check(const std::string& name,
                              std::function<health_status()> check);
    
    /**
     * @brief Enable or disable specific health checks
     * @param check_writers Enable writer health checks
     * @param check_buffers Enable buffer health checks
     * @param check_queues Enable queue health checks
     */
    void configure_checks(bool check_writers = true,
                         bool check_buffers = true,
                         bool check_queues = true);
    
private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

/**
 * @brief RAII helper for automatic health check registration
 */
class scoped_health_registration {
public:
    scoped_health_registration(health_check_system& system,
                              const std::string& name,
                              base_writer* writer)
        : system_(system), name_(name) {
        system_.register_writer(name, writer);
    }
    
    ~scoped_health_registration() {
        system_.unregister_writer(name_);
    }
    
    // Non-copyable, non-movable
    scoped_health_registration(const scoped_health_registration&) = delete;
    scoped_health_registration& operator=(const scoped_health_registration&) = delete;
    scoped_health_registration(scoped_health_registration&&) = delete;
    scoped_health_registration& operator=(scoped_health_registration&&) = delete;
    
private:
    health_check_system& system_;
    std::string name_;
};

/**
 * @brief Health check utilities
 */
class health_check_utils {
public:
    /**
     * @brief Convert health status to string
     * @param status Health status
     * @return String representation
     */
    static std::string health_status_to_string(health_status status);
    
    /**
     * @brief Determine overall health from multiple statuses
     * @param statuses Vector of health statuses
     * @return Worst health status
     */
    static health_status aggregate_health_status(const std::vector<health_status>& statuses);
    
    /**
     * @brief Format health check result as JSON
     * @param result Health check result
     * @return JSON string representation
     */
    static std::string format_as_json(const health_check_result& result);
    
    /**
     * @brief Format writer health info as string
     * @param info Writer health information
     * @return Formatted string
     */
    static std::string format_writer_health(const writer_health_info& info);
};

} // namespace logger_module