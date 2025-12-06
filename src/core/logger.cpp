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
#include <kcenon/logger/core/log_collector.h>
#include <kcenon/logger/core/thread_integration_detector.h>
#include <kcenon/logger/backends/standalone_backend.h>

// Conditionally include thread_system_backend when integration is enabled
#if defined(USE_THREAD_SYSTEM_INTEGRATION)
#include <kcenon/logger/backends/thread_system_backend.h>
#endif

#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/logger_types.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/interfaces/log_entry.h>

#include <atomic>
#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <type_traits>
#include <vector>

namespace kcenon::logger {

namespace {
/**
 * @brief Check if a log level meets the minimum threshold
 *
 * logger_system::log_level uses ascending severity:
 * trace(0) < debug(1) < info(2) < warn(3) < error(4) < fatal(5)
 * → Use >= comparison (higher number = more severe)
 *
 * @param level The log level to check
 * @param minimum The minimum threshold level
 * @return true if the message should be logged
 *
 * @example if minimum=warn(3), log warn(3), error(4), fatal(5)
 *          but skip trace(0), debug(1), info(2)
 */
template <typename Level>
bool meets_threshold(Level level, Level minimum) {
    using underlying_type = std::underlying_type_t<Level>;
    // logger_system::log_level: ascending severity (0 = least severe)
    return static_cast<underlying_type>(level) >= static_cast<underlying_type>(minimum);
}
} // namespace

// Simple implementation class for logger PIMPL
class logger::impl {
public:
    bool async_mode_;
    std::size_t buffer_size_;
    bool running_;
    bool metrics_enabled_;
    std::atomic<logger_system::log_level> min_level_;
    std::vector<std::shared_ptr<base_writer>> writers_;
    std::shared_mutex writers_mutex_;  // Protects writers_ vector from concurrent modification
    std::unique_ptr<backends::integration_backend> backend_;  // Integration backend
    std::unique_ptr<log_collector> collector_;  // Async log collector for async mode
    std::unique_ptr<log_filter_interface> filter_;  // Global filter for log entries
    mutable std::shared_mutex filter_mutex_;  // Protects filter_ from concurrent access

    // Emergency flush support (for signal handlers)
    static constexpr size_t emergency_buffer_size_ = 8192;
    mutable char emergency_buffer_[emergency_buffer_size_];  // Static buffer for emergency use
    std::atomic<size_t> emergency_buffer_used_{0};  // Track how much is used
    std::atomic<int> emergency_fd_{-1};  // File descriptor for emergency writes

    impl(bool async, std::size_t buffer_size, std::unique_ptr<backends::integration_backend> backend)
        : async_mode_(async), buffer_size_(buffer_size), running_(false), metrics_enabled_(false),
          min_level_(logger_system::log_level::info), backend_(std::move(backend)) {
        // Reserve space to avoid reallocation during typical usage
        writers_.reserve(10);

        // Auto-detect backend if not provided
        // Users can provide thread_system_backend or other backends via constructor
        if (!backend_) {
            backend_ = std::make_unique<backends::standalone_backend>();
        }

        // Initialize backend if required
        if (backend_->requires_initialization()) {
            backend_->initialize();
        }

        // Create log collector for async mode
        if (async_mode_) {
            collector_ = std::make_unique<log_collector>(buffer_size_);
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

        // Start async collector if in async mode
        if (pimpl_->async_mode_ && pimpl_->collector_) {
            // Register all existing writers with the collector
            {
                std::shared_lock<std::shared_mutex> lock(pimpl_->writers_mutex_);
                for (const auto& writer : pimpl_->writers_) {
                    if (writer) {
                        pimpl_->collector_->add_writer(writer);
                    }
                }
            }

            // Start the background processing thread
            pimpl_->collector_->start();
        }
    }
    return result_void::success();
}

result_void logger::stop() {
    if (pimpl_ && pimpl_->running_) {
        // Stop async collector if in async mode
        // The collector's stop() drains the queue and flushes writers automatically
        if (pimpl_->async_mode_ && pimpl_->collector_) {
            pimpl_->collector_->stop();
        } else {
            // Synchronous mode: manually flush writers
            flush();
        }

        pimpl_->running_ = false;
    }
    return result_void::success();
}

bool logger::is_running() const {
    return pimpl_ && pimpl_->running_;
}

result_void logger::add_writer(std::unique_ptr<base_writer> writer) {
    if (pimpl_ && writer) {
        std::shared_ptr<base_writer> shared_writer(std::move(writer));

        {
            std::lock_guard<std::shared_mutex> lock(pimpl_->writers_mutex_);
            pimpl_->writers_.push_back(shared_writer);
        }

        // Register with collector if in async mode and running
        if (pimpl_->async_mode_ && pimpl_->collector_ && pimpl_->running_) {
            pimpl_->collector_->add_writer(shared_writer);
        }
    }
    return result_void::success();
}

void logger::add_writer(const std::string& /*name*/, std::unique_ptr<base_writer> writer) {
    if (pimpl_ && writer) {
        std::shared_ptr<base_writer> shared_writer(std::move(writer));

        {
            std::lock_guard<std::shared_mutex> lock(pimpl_->writers_mutex_);
            pimpl_->writers_.push_back(shared_writer);
        }

        // Register with collector if in async mode and running
        if (pimpl_->async_mode_ && pimpl_->collector_ && pimpl_->running_) {
            pimpl_->collector_->add_writer(shared_writer);
        }
    }
}

result_void logger::clear_writers() {
    if (pimpl_) {
        {
            std::lock_guard<std::shared_mutex> lock(pimpl_->writers_mutex_);
            pimpl_->writers_.clear();
        }

        // Clear collectors writers if in async mode
        if (pimpl_->async_mode_ && pimpl_->collector_) {
            pimpl_->collector_->clear_writers();
        }
    }
    return result_void::success();
}

void logger::set_min_level(log_level level) {
    if (pimpl_) {
        pimpl_->min_level_.store(level);
    }
}

log_level logger::get_min_level() const {
    if (pimpl_) {
        return pimpl_->min_level_.load();
    }
    return logger_system::log_level::info;
}

void logger::log(log_level level, const std::string& message) {
    if (!pimpl_ || !meets_threshold(level, pimpl_->min_level_.load())) {
        return;
    }

    // Apply filter if set
    {
        std::shared_lock<std::shared_mutex> filter_lock(pimpl_->filter_mutex_);
        if (pimpl_->filter_) {
            log_entry entry(level, message);
            if (!pimpl_->filter_->should_log(entry)) {
                return;  // Message filtered out
            }
        }
    }

    // Record metrics if enabled
    auto start_time = std::chrono::high_resolution_clock::now();

    // Convert level using backend
    auto converted_level = pimpl_->backend_->normalize_level(static_cast<int>(level));

    // Copy writer pointers under lock to minimize lock hold time
    std::vector<std::shared_ptr<base_writer>> local_writers;
    {
        std::shared_lock<std::shared_mutex> lock(pimpl_->writers_mutex_);
        local_writers = pimpl_->writers_;  // Copy shared_ptrs (cheap, just reference counting)
    }

    // Write without holding lock to avoid I/O under lock
    for (auto& writer : local_writers) {
        if (writer) {
            auto now = std::chrono::system_clock::now();
            writer->write(converted_level, message, "", 0, "", now);
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

    // Apply filter if set
    {
        std::shared_lock<std::shared_mutex> filter_lock(pimpl_->filter_mutex_);
        if (pimpl_->filter_) {
            log_entry entry(level, message, file, line, function);
            if (!pimpl_->filter_->should_log(entry)) {
                return;  // Message filtered out
            }
        }
    }

    // Record metrics if enabled
    auto start_time = std::chrono::high_resolution_clock::now();

    // Convert level using backend
    auto converted_level = pimpl_->backend_->normalize_level(static_cast<int>(level));

    // Use async path if in async mode
    if (pimpl_->async_mode_ && pimpl_->collector_) {
        // Enqueue to collector for background processing
        auto now = std::chrono::system_clock::now();
        pimpl_->collector_->enqueue(converted_level, message, file, line, function, now);

        // Update metrics if enabled (async path is much faster)
        if (pimpl_->metrics_enabled_) {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            metrics::record_message_logged(duration.count());
        }
        return;
    }

    // Synchronous path: Copy writer pointers under lock to minimize lock hold time
    std::vector<std::shared_ptr<base_writer>> local_writers;
    {
        std::shared_lock<std::shared_mutex> lock(pimpl_->writers_mutex_);
        local_writers = pimpl_->writers_;  // Copy shared_ptrs (cheap, just reference counting)
    }

    // Write without holding lock to avoid I/O under lock
    for (auto& writer : local_writers) {
        if (writer) {
            auto now = std::chrono::system_clock::now();
            writer->write(converted_level, message, file, line, function, now);
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
                const core::log_context& context) {
    log(level, message, std::string(context.file), context.line, std::string(context.function));
}

bool logger::is_enabled(log_level level) const {
    return pimpl_ && meets_threshold(level, pimpl_->min_level_.load());
}

void logger::flush() {
    if (pimpl_) {
        if (pimpl_->async_mode_ && pimpl_->collector_) {
            // Async mode: collector handles both queue draining and writer flushing
            pimpl_->collector_->flush();
        } else {
            // Synchronous mode: manually flush writers
            std::vector<std::shared_ptr<base_writer>> local_writers;
            {
                std::shared_lock<std::shared_mutex> lock(pimpl_->writers_mutex_);
                local_writers = pimpl_->writers_;
            }

            for (auto& writer : local_writers) {
                if (writer) {
                    writer->flush();
                }
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

// Emergency flush support implementation (critical_logger_interface)

int logger::get_emergency_fd() const {
    if (!pimpl_) {
        return -1;
    }
    return pimpl_->emergency_fd_.load(std::memory_order_acquire);
}

const char* logger::get_emergency_buffer() const {
    if (!pimpl_) {
        return nullptr;
    }
    return pimpl_->emergency_buffer_;
}

size_t logger::get_emergency_buffer_size() const {
    if (!pimpl_) {
        return 0;
    }
    return pimpl_->emergency_buffer_used_.load(std::memory_order_acquire);
}

// Filter support implementation

void logger::set_filter(std::unique_ptr<log_filter_interface> filter) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->filter_mutex_);
        pimpl_->filter_ = std::move(filter);
    }
}

bool logger::has_filter() const {
    if (!pimpl_) {
        return false;
    }
    std::shared_lock<std::shared_mutex> lock(pimpl_->filter_mutex_);
    return pimpl_->filter_ != nullptr;
}

} // namespace kcenon::logger
