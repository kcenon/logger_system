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

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/backends/standalone_backend.h>
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/logger/backends/thread_system_backend.h>
#endif
#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/logger_types.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <type_traits>
#include <atomic>

namespace kcenon::logger {

namespace {
template <typename Level>
bool meets_threshold(Level level, Level minimum) {
    using underlying_type = std::underlying_type_t<Level>;
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    // thread::log_level uses descending severity: critical(0) > error(1) > warning(2) > info(3)
    // So we need level <= minimum to allow more severe messages
    return static_cast<underlying_type>(level) <= static_cast<underlying_type>(minimum);
#else
    // logger_system::log_level uses ascending severity: trace(0) < debug(1) < info(2) < warn(3)
    // So we need level >= minimum to allow more severe messages
    return static_cast<underlying_type>(level) >= static_cast<underlying_type>(minimum);
#endif
}
} // namespace

#ifdef USE_THREAD_SYSTEM_INTEGRATION
// Helper function to convert kcenon::thread::log_level to logger_system::log_level
logger_system::log_level convert_log_level(kcenon::thread::log_level level) {
    switch (level) {
        case kcenon::thread::log_level::critical: return logger_system::log_level::fatal;
        case kcenon::thread::log_level::error: return logger_system::log_level::error;
        case kcenon::thread::log_level::warning: return logger_system::log_level::warn;
        case kcenon::thread::log_level::info: return logger_system::log_level::info;
        case kcenon::thread::log_level::debug: return logger_system::log_level::debug;
        case kcenon::thread::log_level::trace: return logger_system::log_level::trace;
        default: return logger_system::log_level::info;
    }
}
#else
// In standalone mode, no conversion needed - both types are the same
logger_system::log_level convert_log_level(logger_system::log_level level) {
    return level;
}
#endif

// Simple implementation class for logger PIMPL
class logger::impl {
public:
    bool async_mode_;
    std::size_t buffer_size_;
    bool running_;
    bool metrics_enabled_;
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    std::atomic<kcenon::thread::log_level> min_level_;
#else
    std::atomic<logger_system::log_level> min_level_;
#endif
    std::vector<std::unique_ptr<base_writer>> writers_;
    std::mutex writers_mutex_;  // Protects writers_ vector from concurrent modification
    std::unique_ptr<backends::integration_backend> backend_;  // Integration backend

    impl(bool async, std::size_t buffer_size, std::unique_ptr<backends::integration_backend> backend)
#ifdef USE_THREAD_SYSTEM_INTEGRATION
        : async_mode_(async), buffer_size_(buffer_size), running_(false), metrics_enabled_(false),
          min_level_(kcenon::thread::log_level::info), backend_(std::move(backend)) {
#else
        : async_mode_(async), buffer_size_(buffer_size), running_(false), metrics_enabled_(false),
          min_level_(logger_system::log_level::info), backend_(std::move(backend)) {
#endif
        // Reserve space to avoid reallocation during typical usage
        writers_.reserve(10);

        // Auto-detect backend if not provided
        if (!backend_) {
#ifdef USE_THREAD_SYSTEM_INTEGRATION
            backend_ = std::make_unique<backends::thread_system_backend>();
#else
            backend_ = std::make_unique<backends::standalone_backend>();
#endif
        }

        // Initialize backend if required
        if (backend_->requires_initialization()) {
            backend_->initialize();
        }
    }

    ~impl() {
        if (backend_) {
            backend_->shutdown();
        }
    }
};

logger::logger(bool async, std::size_t buffer_size, std::unique_ptr<backends::integration_backend> backend)
    : pimpl_(std::make_unique<impl>(async, buffer_size, std::move(backend))) {
    // Initialize logger with configuration
}

logger::~logger() {
    if (pimpl_ && pimpl_->running_) {
        stop();
    }
}

result_void logger::start() {
    if (pimpl_ && !pimpl_->running_) {
        pimpl_->running_ = true;
        // Initialize async processing if needed
    }
    return result_void::success();
}

result_void logger::stop() {
    if (pimpl_ && pimpl_->running_) {
        flush();
        pimpl_->running_ = false;
    }
    return result_void::success();
}

bool logger::is_running() const {
    return pimpl_ && pimpl_->running_;
}

result_void logger::add_writer(std::unique_ptr<base_writer> writer) {
    if (pimpl_ && writer) {
        std::lock_guard<std::mutex> lock(pimpl_->writers_mutex_);
        pimpl_->writers_.push_back(std::move(writer));
    }
    return result_void::success();
}

void logger::add_writer(const std::string& /*name*/, std::unique_ptr<base_writer> writer) {
    if (pimpl_ && writer) {
        std::lock_guard<std::mutex> lock(pimpl_->writers_mutex_);
        pimpl_->writers_.push_back(std::move(writer));
    }
}

result_void logger::clear_writers() {
    if (pimpl_) {
        std::lock_guard<std::mutex> lock(pimpl_->writers_mutex_);
        pimpl_->writers_.clear();
    }
    return result_void::success();
}

#ifdef BUILD_WITH_COMMON_SYSTEM
void logger::set_monitor(std::unique_ptr<common::interfaces::IMonitor> monitor) {
    monitor_ = std::move(monitor);
    if (monitor_) {
        enable_metrics_collection(true);
    }
}
#endif

void logger::set_min_level(log_level level) {
    if (pimpl_) {
        pimpl_->min_level_.store(level);
    }
}

log_level logger::get_min_level() const {
    if (pimpl_) {
        return pimpl_->min_level_.load();
    }
#ifdef USE_THREAD_SYSTEM_INTEGRATION
    return kcenon::thread::log_level::info;
#else
    return logger_system::log_level::info;
#endif
}

void logger::log(log_level level, const std::string& message) {
    if (!pimpl_ || !meets_threshold(level, pimpl_->min_level_.load())) {
        return;
    }

    // Record metrics if enabled
    auto start_time = std::chrono::high_resolution_clock::now();

    // Convert level using backend
    auto converted_level = pimpl_->backend_->normalize_level(static_cast<int>(level));

    {
        // Lock writers_ vector to prevent concurrent modification during iteration
        std::lock_guard<std::mutex> lock(pimpl_->writers_mutex_);
        for (auto& writer : pimpl_->writers_) {
            if (writer) {
                // Create a simple log entry and write it
                auto now = std::chrono::system_clock::now();
                writer->write(converted_level, message, "", 0, "", now);
            }
        }
    }

    // Update metrics after logging
    if (pimpl_->metrics_enabled_) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        metrics::record_message_logged(duration.count());
    }
}

void logger::log(log_level level,
                const std::string& message,
                const std::string& file,
                int line,
                const std::string& function) {
    if (!pimpl_ || !meets_threshold(level, pimpl_->min_level_.load())) {
        return;
    }

    // Record metrics if enabled
    auto start_time = std::chrono::high_resolution_clock::now();

    // Convert level using backend
    auto converted_level = pimpl_->backend_->normalize_level(static_cast<int>(level));

    {
        // Lock writers_ vector to prevent concurrent modification during iteration
        std::lock_guard<std::mutex> lock(pimpl_->writers_mutex_);
        for (auto& writer : pimpl_->writers_) {
            if (writer) {
                // Create a log entry with source location
                auto now = std::chrono::system_clock::now();
                writer->write(converted_level, message, file, line, function, now);
            }
        }
    }

    // Update metrics after logging
    if (pimpl_->metrics_enabled_) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        metrics::record_message_logged(duration.count());
    }
}

bool logger::is_enabled(log_level level) const {
    return pimpl_ && meets_threshold(level, pimpl_->min_level_.load());
}

void logger::flush() {
    if (pimpl_) {
        std::lock_guard<std::mutex> lock(pimpl_->writers_mutex_);
        for (auto& writer : pimpl_->writers_) {
            if (writer) {
                writer->flush();
            }
        }
    }
}

result_void logger::enable_metrics_collection(bool enable) {
    if (!pimpl_) {
        return make_logger_error(logger_error_code::invalid_argument, "Logger not initialized");
    }

    pimpl_->metrics_enabled_ = enable;
    if (!enable) {
        metrics::g_logger_stats.reset();
    }

    return result_void::success();
}

bool logger::is_metrics_collection_enabled() const {
    return pimpl_ && pimpl_->metrics_enabled_;
}

result<logger_metrics> logger::get_current_metrics() const {
    if (!pimpl_) {
        return make_logger_error<logger_metrics>(
            logger_error_code::invalid_argument,
            "Logger not initialized");
    }

    if (!pimpl_->metrics_enabled_) {
        return make_logger_error<logger_metrics>(
            logger_error_code::invalid_argument,
            "Metrics collection is not enabled");
    }

    // Return a copy of the current global metrics (thread-safe because of copy constructor)
    // Use static factory method to avoid constructor ambiguity
    return result<logger_metrics>::ok_value(metrics::g_logger_stats);
}

#ifdef BUILD_WITH_COMMON_SYSTEM
// IMonitorable interface implementation (Phase 2.2)

common::Result<common::interfaces::metrics_snapshot> logger::get_monitoring_data() {
    common::interfaces::metrics_snapshot snapshot;
    snapshot.source_id = "logger_system::logger";
    snapshot.capture_time = std::chrono::system_clock::now();

    if (!pimpl_) {
        return common::error_info(
            static_cast<int>(logger_error_code::invalid_argument),
            "Logger not initialized");
    }

    // Add basic statistics
    snapshot.add_metric("writers_count", static_cast<double>(pimpl_->writers_.size()));
    snapshot.add_metric("async_mode", pimpl_->async_mode_ ? 1.0 : 0.0);

    // Add metrics from internal monitor if available
    if (monitor_) {
        auto monitor_result = monitor_->get_metrics();
        if (monitor_result.is_ok()) {
            const auto& monitor_metrics = monitor_result.value();
            // Merge monitor metrics into snapshot
            for (const auto& metric : monitor_metrics.metrics) {
                snapshot.metrics.push_back(metric);
            }
        }
    }

    // Add performance metrics if enabled
    if (pimpl_->metrics_enabled_) {
        auto& stats = metrics::g_logger_stats;
        snapshot.add_metric("messages_logged", static_cast<double>(stats.messages_logged.load()));
        snapshot.add_metric("messages_dropped", static_cast<double>(stats.messages_dropped.load()));
        snapshot.add_metric("queue_size", static_cast<double>(stats.queue_size.load()));
        snapshot.add_metric("max_queue_size", static_cast<double>(stats.max_queue_size.load()));
    }

    return snapshot;
}

common::Result<common::interfaces::health_check_result> logger::health_check() {
    common::interfaces::health_check_result result;
    result.timestamp = std::chrono::system_clock::now();
    auto start_time = std::chrono::high_resolution_clock::now();

    if (!pimpl_) {
        result.status = common::interfaces::health_status::unhealthy;
        result.message = "Logger not initialized";
        auto end_time = std::chrono::high_resolution_clock::now();
        result.check_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time);
        return result;
    }

    // Check if logger is running
    if (!pimpl_->running_) {
        result.status = common::interfaces::health_status::degraded;
        result.message = "Logger is not running";
        result.metadata["running"] = "false";
    } else {
        result.status = common::interfaces::health_status::healthy;
        result.message = "Logger operational";
        result.metadata["running"] = "true";
    }

    // Check writers
    if (pimpl_->writers_.empty()) {
        result.status = common::interfaces::health_status::degraded;
        result.message = "No writers configured";
        result.metadata["writers_count"] = "0";
    } else {
        result.metadata["writers_count"] = std::to_string(pimpl_->writers_.size());
    }

    // Check metrics if enabled
    if (pimpl_->metrics_enabled_) {
        auto& stats = metrics::g_logger_stats;
        auto dropped = stats.messages_dropped.load();
        auto logged = stats.messages_logged.load();

        if (logged > 0) {
            double drop_rate = static_cast<double>(dropped) / static_cast<double>(logged + dropped);
            result.metadata["drop_rate"] = std::to_string(drop_rate);

            if (drop_rate > 0.1) {
                result.status = common::interfaces::health_status::degraded;
                result.message = "High message drop rate detected";
            } else if (drop_rate > 0.5) {
                result.status = common::interfaces::health_status::unhealthy;
                result.message = "Critical message drop rate";
            }
        }

        result.metadata["queue_size"] = std::to_string(stats.queue_size.load());
    }

    // Add async mode info
    result.metadata["async_mode"] = pimpl_->async_mode_ ? "true" : "false";

    auto end_time = std::chrono::high_resolution_clock::now();
    result.check_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);

    return result;
}

std::string logger::get_component_name() const {
    return "logger_system::logger";
}
#endif // BUILD_WITH_COMMON_SYSTEM

} // namespace kcenon::logger
