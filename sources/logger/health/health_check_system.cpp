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

#include "health_check_system.h"
#include "../writers/base_writer.h"
#include "../core/log_collector.h"
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <condition_variable>

namespace logger_module {

class health_check_system::impl {
public:
    explicit impl(const health_check_criteria& criteria)
        : criteria_(criteria)
        , monitoring_active_(false)
        , check_writers_(true)
        , check_buffers_(true)
        , check_queues_(true) {
    }
    
    ~impl() {
        stop_monitoring();
    }
    
    void register_writer(const std::string& name, base_writer* writer) {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        
        writer_health_info info;
        info.writer_name = name;
        info.status = health_status::healthy;
        info.last_write_time = std::chrono::system_clock::now();
        
        writers_[name] = writer;
        writer_health_[name] = info;
    }
    
    void unregister_writer(const std::string& name) {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        writers_.erase(name);
        writer_health_.erase(name);
    }
    
    void register_collector(log_collector* collector) {
        std::lock_guard<std::mutex> lock(collector_mutex_);
        collector_ = collector;
    }
    
    health_check_result perform_health_check() {
        health_check_result result;
        std::vector<health_status> all_statuses;
        
        // Check writers
        if (check_writers_) {
            auto writer_statuses = check_all_writers();
            for (const auto& [name, info] : writer_statuses) {
                all_statuses.push_back(info.status);
                if (info.status != health_status::healthy) {
                    result.add_issue("Writer '" + name + "': " + 
                                   format_writer_issues(info));
                }
            }
        }
        
        // Check buffers
        if (check_buffers_) {
            auto buffer_info = check_buffer_health_internal();
            all_statuses.push_back(buffer_info.status);
            if (buffer_info.status != health_status::healthy) {
                result.add_issue("Buffer: " + format_buffer_issues(buffer_info));
            }
        }
        
        // Check queues
        if (check_queues_) {
            auto queue_info = check_queue_health_internal();
            all_statuses.push_back(queue_info.status);
            if (queue_info.status != health_status::healthy) {
                result.add_issue("Queue: " + format_queue_issues(queue_info));
            }
        }
        
        // Run custom checks
        for (const auto& [name, check] : custom_checks_) {
            health_status status = check();
            all_statuses.push_back(status);
            if (status != health_status::healthy) {
                result.add_issue("Custom check '" + name + "' failed");
            }
        }
        
        // Determine overall health
        if (!all_statuses.empty()) {
            result.set_status(health_check_utils::aggregate_health_status(all_statuses));
        }
        
        if (result.get_status() == health_status::healthy) {
            result.set_message("All systems healthy");
        } else if (result.get_status() == health_status::degraded) {
            result.set_message("System degraded - check issues");
        } else {
            result.set_message("System unhealthy - immediate attention required");
        }
        
        return result;
    }
    
    result<writer_health_info> check_writer_health(const std::string& name) {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        
        auto it = writer_health_.find(name);
        if (it == writer_health_.end()) {
            return error_code::component_not_found;
        }
        
        return it->second;
    }
    
    buffer_health_info check_buffer_health_internal() {
        std::lock_guard<std::mutex> lock(buffer_mutex_);
        
        buffer_health_.usage_percent = buffer_health_.total_capacity > 0 ?
            (static_cast<double>(buffer_health_.used_capacity) / 
             buffer_health_.total_capacity * 100.0) : 0.0;
        
        buffer_health_.available_capacity = 
            buffer_health_.total_capacity - buffer_health_.used_capacity;
        
        // Determine health status
        if (buffer_health_.usage_percent > criteria_.max_buffer_usage_percent ||
            buffer_health_.available_capacity < criteria_.min_available_buffer_size) {
            buffer_health_.status = health_status::unhealthy;
        } else if (buffer_health_.usage_percent > criteria_.max_buffer_usage_percent * 0.8) {
            buffer_health_.status = health_status::degraded;
        } else {
            buffer_health_.status = health_status::healthy;
        }
        
        return buffer_health_;
    }
    
    queue_health_info check_queue_health_internal() {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        
        if (collector_) {
            auto metrics = collector_->get_queue_metrics();
            queue_health_.current_size = metrics.first;
            queue_health_.max_size = metrics.second;
        }
        
        queue_health_.usage_percent = queue_health_.max_size > 0 ?
            (static_cast<double>(queue_health_.current_size) / 
             queue_health_.max_size * 100.0) : 0.0;
        
        // Determine health status
        if (queue_health_.usage_percent > criteria_.max_queue_usage_percent ||
            queue_health_.current_size > criteria_.max_queue_size ||
            queue_health_.max_wait_time > criteria_.max_queue_wait_time) {
            queue_health_.status = health_status::unhealthy;
        } else if (queue_health_.usage_percent > criteria_.max_queue_usage_percent * 0.8 ||
                   queue_health_.dropped_messages > 0) {
            queue_health_.status = health_status::degraded;
        } else {
            queue_health_.status = health_status::healthy;
        }
        
        return queue_health_;
    }
    
    void update_writer_stats(const std::string& name, 
                            bool success, 
                            std::chrono::milliseconds latency) {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        
        auto it = writer_health_.find(name);
        if (it == writer_health_.end()) {
            return;
        }
        
        auto& info = it->second;
        info.total_writes++;
        info.last_write_time = std::chrono::system_clock::now();
        
        if (success) {
            info.consecutive_failures = 0;
            
            // Update average latency
            if (info.avg_write_latency.count() == 0) {
                info.avg_write_latency = latency;
            } else {
                auto total_latency = info.avg_write_latency.count() * (info.total_writes - 1);
                info.avg_write_latency = std::chrono::milliseconds(
                    (total_latency + latency.count()) / info.total_writes
                );
            }
            
            // Update max latency
            if (latency > info.max_write_latency) {
                info.max_write_latency = latency;
            }
        } else {
            info.failed_writes++;
            info.consecutive_failures++;
            info.last_failure_time = std::chrono::system_clock::now();
        }
        
        // Update health status based on criteria
        if (info.consecutive_failures >= criteria_.max_consecutive_write_failures ||
            info.max_write_latency > criteria_.max_write_latency) {
            info.status = health_status::unhealthy;
        } else if (info.consecutive_failures > 0 ||
                   info.get_failure_rate() > 5.0) {
            info.status = health_status::degraded;
        } else {
            info.status = health_status::healthy;
        }
    }
    
    void update_buffer_stats(size_t used, size_t total, bool allocation_failed) {
        std::lock_guard<std::mutex> lock(buffer_mutex_);
        
        buffer_health_.used_capacity = used;
        buffer_health_.total_capacity = total;
        buffer_health_.total_allocations++;
        
        if (allocation_failed) {
            buffer_health_.failed_allocations++;
        }
    }
    
    void update_queue_stats(size_t current_size, 
                           size_t max_size,
                           bool dropped,
                           std::chrono::milliseconds wait_time) {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        
        queue_health_.current_size = current_size;
        queue_health_.max_size = max_size;
        
        if (dropped) {
            queue_health_.dropped_messages++;
        } else {
            queue_health_.total_enqueued++;
        }
        
        if (wait_time.count() > 0) {
            queue_health_.total_dequeued++;
            
            // Update average wait time
            if (queue_health_.avg_wait_time.count() == 0) {
                queue_health_.avg_wait_time = wait_time;
            } else {
                auto total_wait = queue_health_.avg_wait_time.count() * 
                                 (queue_health_.total_dequeued - 1);
                queue_health_.avg_wait_time = std::chrono::milliseconds(
                    (total_wait + wait_time.count()) / queue_health_.total_dequeued
                );
            }
            
            // Update max wait time
            if (wait_time > queue_health_.max_wait_time) {
                queue_health_.max_wait_time = wait_time;
            }
        }
    }
    
    void start_monitoring(std::chrono::seconds interval,
                         std::function<void(const health_check_result&)> callback) {
        if (monitoring_active_.exchange(true)) {
            return; // Already monitoring
        }
        
        monitoring_thread_ = std::thread([this, interval, callback]() {
            while (monitoring_active_) {
                std::unique_lock<std::mutex> lock(monitoring_mutex_);
                if (monitoring_cv_.wait_for(lock, interval, 
                    [this]() { return !monitoring_active_.load(); })) {
                    break; // Monitoring stopped
                }
                
                auto result = perform_health_check();
                if (callback) {
                    callback(result);
                }
            }
        });
    }
    
    void stop_monitoring() {
        if (!monitoring_active_.exchange(false)) {
            return; // Not monitoring
        }
        
        monitoring_cv_.notify_all();
        
        if (monitoring_thread_.joinable()) {
            monitoring_thread_.join();
        }
    }
    
    bool is_monitoring() const {
        return monitoring_active_.load();
    }
    
    std::unordered_map<std::string, writer_health_info> get_all_writer_health() const {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        return writer_health_;
    }
    
    void reset_stats() {
        {
            std::lock_guard<std::mutex> lock(writers_mutex_);
            for (auto& [name, info] : writer_health_) {
                info.total_writes = 0;
                info.failed_writes = 0;
                info.consecutive_failures = 0;
                info.avg_write_latency = std::chrono::milliseconds{0};
                info.max_write_latency = std::chrono::milliseconds{0};
                info.recent_errors.clear();
                info.status = health_status::healthy;
            }
        }
        
        {
            std::lock_guard<std::mutex> lock(buffer_mutex_);
            buffer_health_ = buffer_health_info{};
        }
        
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            queue_health_ = queue_health_info{};
        }
    }
    
    void register_custom_check(const std::string& name,
                              std::function<health_status()> check) {
        std::lock_guard<std::mutex> lock(custom_checks_mutex_);
        custom_checks_[name] = check;
    }
    
    void configure_checks(bool check_writers, bool check_buffers, bool check_queues) {
        check_writers_ = check_writers;
        check_buffers_ = check_buffers;
        check_queues_ = check_queues;
    }
    
    health_check_criteria criteria_;
    
private:
    std::unordered_map<std::string, writer_health_info> check_all_writers() {
        std::lock_guard<std::mutex> lock(writers_mutex_);
        return writer_health_;
    }
    
    std::string format_writer_issues(const writer_health_info& info) {
        std::ostringstream oss;
        
        if (info.consecutive_failures > 0) {
            oss << info.consecutive_failures << " consecutive failures";
        }
        
        if (info.max_write_latency > criteria_.max_write_latency) {
            if (oss.tellp() > 0) oss << ", ";
            oss << "high latency (" << info.max_write_latency.count() << "ms)";
        }
        
        if (info.get_failure_rate() > 5.0) {
            if (oss.tellp() > 0) oss << ", ";
            oss << "failure rate " << std::fixed << std::setprecision(1) 
                << info.get_failure_rate() << "%";
        }
        
        return oss.str();
    }
    
    std::string format_buffer_issues(const buffer_health_info& info) {
        std::ostringstream oss;
        
        if (info.usage_percent > criteria_.max_buffer_usage_percent) {
            oss << "usage " << std::fixed << std::setprecision(1) 
                << info.usage_percent << "%";
        }
        
        if (info.available_capacity < criteria_.min_available_buffer_size) {
            if (oss.tellp() > 0) oss << ", ";
            oss << "only " << info.available_capacity << " bytes available";
        }
        
        if (info.failed_allocations > 0) {
            if (oss.tellp() > 0) oss << ", ";
            oss << info.failed_allocations << " failed allocations";
        }
        
        return oss.str();
    }
    
    std::string format_queue_issues(const queue_health_info& info) {
        std::ostringstream oss;
        
        if (info.usage_percent > criteria_.max_queue_usage_percent) {
            oss << "usage " << std::fixed << std::setprecision(1) 
                << info.usage_percent << "%";
        }
        
        if (info.current_size > criteria_.max_queue_size) {
            if (oss.tellp() > 0) oss << ", ";
            oss << "size " << info.current_size;
        }
        
        if (info.max_wait_time > criteria_.max_queue_wait_time) {
            if (oss.tellp() > 0) oss << ", ";
            oss << "max wait " << info.max_wait_time.count() << "ms";
        }
        
        if (info.dropped_messages > 0) {
            if (oss.tellp() > 0) oss << ", ";
            oss << info.dropped_messages << " dropped messages";
        }
        
        return oss.str();
    }
    
    // Writers
    mutable std::mutex writers_mutex_;
    std::unordered_map<std::string, base_writer*> writers_;
    std::unordered_map<std::string, writer_health_info> writer_health_;
    
    // Collector
    std::mutex collector_mutex_;
    log_collector* collector_ = nullptr;
    
    // Buffer health
    mutable std::mutex buffer_mutex_;
    buffer_health_info buffer_health_;
    
    // Queue health
    mutable std::mutex queue_mutex_;
    queue_health_info queue_health_;
    
    // Custom checks
    std::mutex custom_checks_mutex_;
    std::unordered_map<std::string, std::function<health_status()>> custom_checks_;
    
    // Monitoring
    std::atomic<bool> monitoring_active_;
    std::thread monitoring_thread_;
    std::mutex monitoring_mutex_;
    std::condition_variable monitoring_cv_;
    
    // Configuration
    std::atomic<bool> check_writers_;
    std::atomic<bool> check_buffers_;
    std::atomic<bool> check_queues_;
};

// Public interface implementation

health_check_system::health_check_system(const health_check_criteria& criteria)
    : pimpl_(std::make_unique<impl>(criteria)) {
}

health_check_system::~health_check_system() = default;

void health_check_system::register_writer(const std::string& name, base_writer* writer) {
    pimpl_->register_writer(name, writer);
}

void health_check_system::unregister_writer(const std::string& name) {
    pimpl_->unregister_writer(name);
}

void health_check_system::register_collector(log_collector* collector) {
    pimpl_->register_collector(collector);
}

health_check_result health_check_system::perform_health_check() {
    return pimpl_->perform_health_check();
}

result<writer_health_info> health_check_system::check_writer_health(const std::string& name) {
    return pimpl_->check_writer_health(name);
}

buffer_health_info health_check_system::check_buffer_health() {
    return pimpl_->check_buffer_health_internal();
}

queue_health_info health_check_system::check_queue_health() {
    return pimpl_->check_queue_health_internal();
}

void health_check_system::update_writer_stats(const std::string& name,
                                             bool success,
                                             std::chrono::milliseconds latency) {
    pimpl_->update_writer_stats(name, success, latency);
}

void health_check_system::update_buffer_stats(size_t used, size_t total, bool allocation_failed) {
    pimpl_->update_buffer_stats(used, total, allocation_failed);
}

void health_check_system::update_queue_stats(size_t current_size,
                                            size_t max_size,
                                            bool dropped,
                                            std::chrono::milliseconds wait_time) {
    pimpl_->update_queue_stats(current_size, max_size, dropped, wait_time);
}

void health_check_system::start_monitoring(std::chrono::seconds interval,
                                          std::function<void(const health_check_result&)> callback) {
    pimpl_->start_monitoring(interval, callback);
}

void health_check_system::stop_monitoring() {
    pimpl_->stop_monitoring();
}

bool health_check_system::is_monitoring() const {
    return pimpl_->is_monitoring();
}

std::unordered_map<std::string, writer_health_info> health_check_system::get_all_writer_health() const {
    return pimpl_->get_all_writer_health();
}

health_check_criteria health_check_system::get_criteria() const {
    return pimpl_->criteria_;
}

void health_check_system::set_criteria(const health_check_criteria& criteria) {
    pimpl_->criteria_ = criteria;
}

void health_check_system::reset_stats() {
    pimpl_->reset_stats();
}

void health_check_system::register_custom_check(const std::string& name,
                                               std::function<health_status()> check) {
    pimpl_->register_custom_check(name, check);
}

void health_check_system::configure_checks(bool check_writers,
                                          bool check_buffers,
                                          bool check_queues) {
    pimpl_->configure_checks(check_writers, check_buffers, check_queues);
}

// Utility functions implementation

std::string health_check_utils::health_status_to_string(health_status status) {
    switch (status) {
        case health_status::healthy:
            return "healthy";
        case health_status::degraded:
            return "degraded";
        case health_status::unhealthy:
            return "unhealthy";
        case health_status::unknown:
            return "unknown";
        default:
            return "unknown";
    }
}

health_status health_check_utils::aggregate_health_status(const std::vector<health_status>& statuses) {
    if (statuses.empty()) {
        return health_status::unknown;
    }
    
    // Return the worst status
    bool has_unhealthy = false;
    bool has_degraded = false;
    bool has_unknown = false;
    
    for (const auto& status : statuses) {
        if (status == health_status::unhealthy) {
            has_unhealthy = true;
        } else if (status == health_status::degraded) {
            has_degraded = true;
        } else if (status == health_status::unknown) {
            has_unknown = true;
        }
    }
    
    if (has_unhealthy) {
        return health_status::unhealthy;
    } else if (has_degraded) {
        return health_status::degraded;
    } else if (has_unknown) {
        return health_status::unknown;
    } else {
        return health_status::healthy;
    }
}

std::string health_check_utils::format_as_json(const health_check_result& result) {
    std::ostringstream json;
    json << "{\n";
    json << "  \"status\": \"" << health_status_to_string(result.get_status()) << "\",\n";
    json << "  \"message\": \"" << result.get_message() << "\",\n";
    json << "  \"issues\": [\n";
    
    const auto& issues = result.get_issues();
    for (size_t i = 0; i < issues.size(); ++i) {
        json << "    \"" << issues[i] << "\"";
        if (i < issues.size() - 1) {
            json << ",";
        }
        json << "\n";
    }
    
    json << "  ],\n";
    json << "  \"timestamp\": \"" 
         << std::chrono::system_clock::to_time_t(result.get_check_time()) 
         << "\"\n";
    json << "}";
    
    return json.str();
}

std::string health_check_utils::format_writer_health(const writer_health_info& info) {
    std::ostringstream oss;
    oss << "Writer: " << info.writer_name << "\n";
    oss << "  Status: " << health_status_to_string(info.status) << "\n";
    oss << "  Total writes: " << info.total_writes << "\n";
    oss << "  Failed writes: " << info.failed_writes << "\n";
    oss << "  Failure rate: " << std::fixed << std::setprecision(2) 
        << info.get_failure_rate() << "%\n";
    oss << "  Consecutive failures: " << info.consecutive_failures << "\n";
    oss << "  Avg latency: " << info.avg_write_latency.count() << "ms\n";
    oss << "  Max latency: " << info.max_write_latency.count() << "ms\n";
    
    if (!info.recent_errors.empty()) {
        oss << "  Recent errors:\n";
        for (const auto& error : info.recent_errors) {
            oss << "    - " << error << "\n";
        }
    }
    
    return oss.str();
}

} // namespace logger_module