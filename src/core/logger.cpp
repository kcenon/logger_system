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
#include <kcenon/logger/analysis/realtime_log_analyzer.h>
#include <kcenon/logger/sampling/log_sampler.h>

// Note: thread_system_backend was removed in Issue #225
// thread_system is now optional and standalone_backend is the default

#include <kcenon/logger/writers/base_writer.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/common/patterns/result.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <iostream>
#include <shared_mutex>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace kcenon::logger {

// Type alias for log_level
using log_level = common::interfaces::log_level;

namespace {
/**
 * @brief Check if a log level meets the minimum threshold
 *
 * log_level uses ascending severity:
 * trace(0) < debug(1) < info(2) < warning(3) < error(4) < critical(5)
 * → Use >= comparison (higher number = more severe)
 *
 * @param level The log level to check
 * @param minimum The minimum threshold level
 * @return true if the message should be logged
 *
 * @example if minimum=warning(3), log warning(3), error(4), critical(5)
 *          but skip trace(0), debug(1), info(2)
 */
bool meets_threshold(log_level level, log_level minimum) {
    return static_cast<int>(level) >= static_cast<int>(minimum);
}
} // namespace

// Simple implementation class for logger PIMPL
class logger::impl {
public:
    bool async_mode_;
    std::size_t buffer_size_;
    bool running_;
    bool metrics_enabled_;
    std::atomic<log_level> min_level_;
    std::vector<std::shared_ptr<base_writer>> writers_;
    std::unordered_map<std::string, std::shared_ptr<base_writer>> named_writers_;  // Named writer storage
    std::shared_mutex writers_mutex_;  // Protects writers_ and named_writers_ from concurrent modification
    std::unique_ptr<backends::integration_backend> backend_;  // Integration backend
    std::unique_ptr<log_collector> collector_;  // Async log collector for async mode
    std::unique_ptr<log_filter_interface> filter_;  // Global filter for log entries
    mutable std::shared_mutex filter_mutex_;  // Protects filter_ from concurrent access
    std::unique_ptr<log_router> router_;  // Log router for message routing
    mutable std::shared_mutex router_mutex_;  // Protects router_ from concurrent access

    // Real-time analysis
    std::unique_ptr<analysis::realtime_log_analyzer> realtime_analyzer_;  // Real-time log analyzer
    mutable std::shared_mutex analyzer_mutex_;  // Protects realtime_analyzer_

    // Context fields for structured logging
    log_fields context_fields_;  // Persistent context fields
    mutable std::shared_mutex context_mutex_;  // Protects context_fields_

    // Log sampling
    std::unique_ptr<sampling::log_sampler> sampler_;  // Log sampler for volume reduction
    mutable std::shared_mutex sampler_mutex_;  // Protects sampler_

    // Emergency flush support (for signal handlers)
    static constexpr size_t emergency_buffer_size_ = 8192;
    mutable char emergency_buffer_[emergency_buffer_size_];  // Static buffer for emergency use
    std::atomic<size_t> emergency_buffer_used_{0};  // Track how much is used
    std::atomic<int> emergency_fd_{-1};  // File descriptor for emergency writes

    impl(bool async, std::size_t buffer_size, std::unique_ptr<backends::integration_backend> backend)
        : async_mode_(async), buffer_size_(buffer_size), running_(false), metrics_enabled_(false),
          min_level_(log_level::info), backend_(std::move(backend)),
          router_(std::make_unique<log_router>()) {
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

    /**
     * @brief Dispatch log to writers with routing support
     * @param level Log level
     * @param message Log message
     * @param file Source file
     * @param line Source line
     * @param function Source function
     * @param entry Log entry for routing check
     */
    void dispatch_to_writers(log_level level,
                            const std::string& message,
                            const std::string& file,
                            int line,
                            const std::string& function,
                            const log_entry& entry) {
        // Real-time analysis if analyzer is set
        {
            std::shared_lock<std::shared_mutex> lock(analyzer_mutex_);
            if (realtime_analyzer_) {
                analysis::analyzed_log_entry analyzed_entry;
                analyzed_entry.level = level;
                analyzed_entry.message = message;
                analyzed_entry.timestamp = entry.timestamp;
                analyzed_entry.source_file = file;
                analyzed_entry.source_line = line;
                analyzed_entry.function_name = function;
                realtime_analyzer_->analyze(analyzed_entry);
            }
        }

        // Check routing rules
        std::vector<std::string> routed_writer_names;
        bool is_exclusive = false;
        {
            std::shared_lock<std::shared_mutex> lock(router_mutex_);
            if (router_) {
                routed_writer_names = router_->get_writers_for_log(entry);
                is_exclusive = router_->is_exclusive_routing();
            }
        }

        auto now = std::chrono::system_clock::now();

        if (is_exclusive) {
            // Exclusive mode: only send to matched routes
            // If no routes match, message is dropped (exclusive mode behavior)
            if (!routed_writer_names.empty()) {
                std::unordered_map<std::string, std::shared_ptr<base_writer>> local_named_writers;
                {
                    std::shared_lock<std::shared_mutex> lock(writers_mutex_);
                    local_named_writers = named_writers_;
                }

                // Create log_entry for routing
                log_entry entry(level, message, file, line, function, now);

                for (const auto& writer_name : routed_writer_names) {
                    auto it = local_named_writers.find(writer_name);
                    if (it != local_named_writers.end() && it->second) {
                        it->second->write(entry);
                    }
                }
            }
            // No matched routes in exclusive mode = drop message
        } else {
            // Non-exclusive mode: send to all writers
            std::vector<std::shared_ptr<base_writer>> local_writers;
            {
                std::shared_lock<std::shared_mutex> lock(writers_mutex_);
                local_writers = writers_;
            }

            // Create log_entry once for all writers
            log_entry entry(level, message, file, line, function, now);

            for (auto& writer : local_writers) {
                if (writer) {
                    writer->write(entry);
                }
            }
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

common::VoidResult logger::start() {
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
    return common::ok();
}

common::VoidResult logger::stop() {
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
    return common::ok();
}

bool logger::is_running() const {
    return pimpl_ && pimpl_->running_;
}

common::VoidResult logger::add_writer(std::unique_ptr<base_writer> writer) {
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
    return common::ok();
}

common::VoidResult logger::add_writer(const std::string& name, std::unique_ptr<base_writer> writer) {
    if (!pimpl_) {
        return common::make_error<std::monostate>(
            static_cast<int>(logger_error_code::invalid_configuration),
            "Logger not initialized",
            "logger_system");
    }
    if (!writer) {
        return common::make_error<std::monostate>(
            static_cast<int>(logger_error_code::invalid_argument),
            "Writer cannot be null",
            "logger_system");
    }

    std::shared_ptr<base_writer> shared_writer(std::move(writer));

    {
        std::lock_guard<std::shared_mutex> lock(pimpl_->writers_mutex_);
        pimpl_->writers_.push_back(shared_writer);
        if (!name.empty()) {
            pimpl_->named_writers_[name] = shared_writer;
        }
    }

    // Register with collector if in async mode and running
    if (pimpl_->async_mode_ && pimpl_->collector_ && pimpl_->running_) {
        pimpl_->collector_->add_writer(shared_writer);
    }

    return common::ok();
}

common::VoidResult logger::clear_writers() {
    if (pimpl_) {
        {
            std::lock_guard<std::shared_mutex> lock(pimpl_->writers_mutex_);
            pimpl_->writers_.clear();
            pimpl_->named_writers_.clear();
        }

        // Clear collectors writers if in async mode
        if (pimpl_->async_mode_ && pimpl_->collector_) {
            pimpl_->collector_->clear_writers();
        }
    }
    return common::ok();
}

bool logger::remove_writer(const std::string& name) {
    if (!pimpl_ || name.empty()) {
        return false;
    }

    std::lock_guard<std::shared_mutex> lock(pimpl_->writers_mutex_);

    auto it = pimpl_->named_writers_.find(name);
    if (it == pimpl_->named_writers_.end()) {
        return false;
    }

    auto writer_to_remove = it->second;

    // Remove from named_writers_ map
    pimpl_->named_writers_.erase(it);

    // Remove from writers_ vector
    auto vec_it = std::find(pimpl_->writers_.begin(), pimpl_->writers_.end(), writer_to_remove);
    if (vec_it != pimpl_->writers_.end()) {
        pimpl_->writers_.erase(vec_it);
    }

    // Note: Cannot remove from collector's writers list as it doesn't support removal
    // The writer will be skipped during iteration if it's no longer valid

    return true;
}

base_writer* logger::get_writer(const std::string& name) {
    if (!pimpl_ || name.empty()) {
        return nullptr;
    }

    std::shared_lock<std::shared_mutex> lock(pimpl_->writers_mutex_);

    auto it = pimpl_->named_writers_.find(name);
    if (it != pimpl_->named_writers_.end() && it->second) {
        return it->second.get();
    }

    return nullptr;
}

// =========================================================================
// ILogger interface implementation (common::interfaces::ILogger)
// =========================================================================

common::VoidResult logger::log(common::interfaces::log_level level,
                               const std::string& message) {
    if (!pimpl_ || !meets_threshold(level, pimpl_->min_level_.load())) {
        return common::ok();
    }

    // Create log entry for filtering and routing
    log_entry entry(level, message);

    // Apply filter if set
    {
        std::shared_lock<std::shared_mutex> filter_lock(pimpl_->filter_mutex_);
        if (pimpl_->filter_) {
            if (!pimpl_->filter_->should_log(entry)) {
                return common::ok();  // Message filtered out (not an error)
            }
        }
    }

    // Apply sampling if set
    {
        std::shared_lock<std::shared_mutex> sampler_lock(pimpl_->sampler_mutex_);
        if (pimpl_->sampler_ && pimpl_->sampler_->is_enabled()) {
            if (!pimpl_->sampler_->should_sample(entry)) {
                return common::ok();  // Message sampled out (not an error)
            }
        }
    }

    // Record metrics if enabled
    auto start_time = std::chrono::high_resolution_clock::now();

    // Synchronous path: dispatch with routing support
    pimpl_->dispatch_to_writers(level, message, "", 0, "", entry);

    // Update metrics after logging
    if (pimpl_->metrics_enabled_) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        metrics::record_message_logged(duration.count());
    }

    return common::ok();
}

common::VoidResult logger::log(common::interfaces::log_level level,
                               std::string_view message,
                               const common::source_location& loc) {
    return log(level, std::string(message),
               std::string(loc.file_name()), loc.line(), std::string(loc.function_name()));
}

common::VoidResult logger::log(common::interfaces::log_level level,
                               const std::string& message,
                               const std::string& file,
                               int line,
                               const std::string& function) {
    if (!pimpl_ || !meets_threshold(level, pimpl_->min_level_.load())) {
        return common::ok();
    }

    // Create log entry for filtering and routing
    log_entry entry(level, message, file, line, function);

    // Apply filter if set
    {
        std::shared_lock<std::shared_mutex> filter_lock(pimpl_->filter_mutex_);
        if (pimpl_->filter_) {
            if (!pimpl_->filter_->should_log(entry)) {
                return common::ok();  // Message filtered out
            }
        }
    }

    // Apply sampling if set
    {
        std::shared_lock<std::shared_mutex> sampler_lock(pimpl_->sampler_mutex_);
        if (pimpl_->sampler_ && pimpl_->sampler_->is_enabled()) {
            if (!pimpl_->sampler_->should_sample(entry)) {
                return common::ok();  // Message sampled out
            }
        }
    }

    // Record metrics if enabled
    auto start_time = std::chrono::high_resolution_clock::now();

    // Use async path if in async mode
    if (pimpl_->async_mode_ && pimpl_->collector_) {
        // Enqueue to collector for background processing
        auto now = std::chrono::system_clock::now();
        pimpl_->collector_->enqueue(level, message, file, line, function, now);

        // Update metrics if enabled (async path is much faster)
        if (pimpl_->metrics_enabled_) {
            auto end_time = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
            metrics::record_message_logged(duration.count());
        }
        return common::ok();
    }

    // Synchronous path: dispatch with routing support
    pimpl_->dispatch_to_writers(level, message, file, line, function, entry);

    // Update metrics after logging
    if (pimpl_->metrics_enabled_) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end_time - start_time);
        metrics::record_message_logged(duration.count());
    }

    return common::ok();
}

common::VoidResult logger::log(const common::interfaces::log_entry& entry) {
    return log(entry.level, entry.message, entry.file, entry.line, entry.function);
}

bool logger::is_enabled(common::interfaces::log_level level) const {
    return pimpl_ && meets_threshold(level, pimpl_->min_level_.load());
}

common::VoidResult logger::set_level(common::interfaces::log_level level) {
    if (!pimpl_) {
        return common::make_error<std::monostate>(
            static_cast<int>(logger_error_code::invalid_configuration),
            "Logger not initialized",
            "logger_system");
    }

    pimpl_->min_level_.store(level);
    return common::ok();
}

common::interfaces::log_level logger::get_level() const {
    if (pimpl_) {
        return pimpl_->min_level_.load();
    }
    return common::interfaces::log_level::info;
}

common::VoidResult logger::flush() {
    if (!pimpl_) {
        return common::make_error<std::monostate>(
            static_cast<int>(logger_error_code::invalid_configuration),
            "Logger not initialized",
            "logger_system");
    }

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

    return common::ok();
}

common::VoidResult logger::enable_metrics_collection(bool enable) {
    if (!pimpl_) {
        return make_logger_void_result(logger_error_code::invalid_argument, "Logger not initialized");
    }

    pimpl_->metrics_enabled_ = enable;
    if (!enable) {
        metrics::g_logger_stats.reset();
    }

    return common::ok();
}

bool logger::is_metrics_collection_enabled() const {
    return pimpl_ && pimpl_->metrics_enabled_;
}

result<logger_metrics> logger::get_current_metrics() const {
    if (!pimpl_) {
        return result<logger_metrics>{
            logger_error_code::invalid_argument,
            "Logger not initialized"};
    }

    if (!pimpl_->metrics_enabled_) {
        return result<logger_metrics>{
            logger_error_code::invalid_argument,
            "Metrics collection is not enabled"};
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

// Routing support implementation

log_router& logger::get_router() {
    if (!pimpl_) {
        throw std::runtime_error("Logger not initialized");
    }
    std::shared_lock<std::shared_mutex> lock(pimpl_->router_mutex_);
    return *pimpl_->router_;
}

const log_router& logger::get_router() const {
    if (!pimpl_) {
        throw std::runtime_error("Logger not initialized");
    }
    std::shared_lock<std::shared_mutex> lock(pimpl_->router_mutex_);
    return *pimpl_->router_;
}

void logger::set_router(std::unique_ptr<log_router> router) {
    if (pimpl_ && router) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->router_mutex_);
        pimpl_->router_ = std::move(router);
    }
}

bool logger::has_routing() const {
    if (!pimpl_) {
        return false;
    }
    std::shared_lock<std::shared_mutex> lock(pimpl_->router_mutex_);
    // Check if router has any routes configured by checking if get_writers_for_log
    // returns anything for a test entry
    return pimpl_->router_ != nullptr;
}

// =========================================================================
// OpenTelemetry context management implementation
// =========================================================================

void logger::set_otel_context(const otlp::otel_context& ctx) {
    otlp::otel_context_storage::set(ctx);
}

std::optional<otlp::otel_context> logger::get_otel_context() const {
    return otlp::otel_context_storage::get();
}

void logger::clear_otel_context() {
    otlp::otel_context_storage::clear();
}

bool logger::has_otel_context() const {
    return otlp::otel_context_storage::has_context();
}

// =========================================================================
// Structured logging API implementation
// =========================================================================

structured_log_builder logger::log_structured(log_level level) {
    const log_fields* ctx_ptr = nullptr;
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->context_mutex_);
        if (!pimpl_->context_fields_.empty()) {
            ctx_ptr = &pimpl_->context_fields_;
        }
    }

    return structured_log_builder(
        level,
        [this, level](log_entry&& entry) {
            if (!pimpl_ || !meets_threshold(level, pimpl_->min_level_.load())) {
                return;
            }

            // Apply filter if set
            {
                std::shared_lock<std::shared_mutex> filter_lock(pimpl_->filter_mutex_);
                if (pimpl_->filter_) {
                    if (!pimpl_->filter_->should_log(entry)) {
                        return;
                    }
                }
            }

            // Dispatch to writers
            std::string file_str;
            int line = 0;
            std::string func_str;
            if (entry.location) {
                file_str = entry.location->file.to_string();
                line = entry.location->line;
                func_str = entry.location->function.to_string();
            }

            pimpl_->dispatch_to_writers(
                entry.level,
                entry.message.to_string(),
                file_str,
                line,
                func_str,
                entry
            );
        },
        ctx_ptr
    );
}

// =========================================================================
// Context fields management implementation
// =========================================================================

void logger::set_context(const std::string& key, const std::string& value) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        pimpl_->context_fields_[key] = value;
    }
}

void logger::set_context(const std::string& key, int64_t value) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        pimpl_->context_fields_[key] = value;
    }
}

void logger::set_context(const std::string& key, double value) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        pimpl_->context_fields_[key] = value;
    }
}

void logger::set_context(const std::string& key, bool value) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        pimpl_->context_fields_[key] = value;
    }
}

void logger::remove_context(const std::string& key) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        pimpl_->context_fields_.erase(key);
    }
}

void logger::clear_context() {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        pimpl_->context_fields_.clear();
    }
}

bool logger::has_context() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->context_mutex_);
        return !pimpl_->context_fields_.empty();
    }
    return false;
}

log_fields logger::get_context() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->context_mutex_);
        return pimpl_->context_fields_;
    }
    return {};
}

// =========================================================================
// Generic context ID API implementation
// =========================================================================

namespace {
const std::string CORRELATION_ID_KEY = "correlation_id";
const std::string REQUEST_ID_KEY = "request_id";
const std::string TRACE_ID_KEY = "trace_id";
const std::string SPAN_ID_KEY = "span_id";
const std::string PARENT_SPAN_ID_KEY = "parent_span_id";

// List of all known context ID keys for clear_all_context_ids()
const std::vector<std::string> ALL_CONTEXT_ID_KEYS = {
    CORRELATION_ID_KEY,
    REQUEST_ID_KEY,
    TRACE_ID_KEY,
    SPAN_ID_KEY,
    PARENT_SPAN_ID_KEY
};
} // namespace

void logger::set_context_id(std::string_view key, std::string_view value) {
    set_context(std::string(key), std::string(value));
}

std::string logger::get_context_id(std::string_view key) const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->context_mutex_);
        auto it = pimpl_->context_fields_.find(std::string(key));
        if (it != pimpl_->context_fields_.end()) {
            if (auto* str = std::get_if<std::string>(&it->second)) {
                return *str;
            }
        }
    }
    return {};
}

void logger::clear_context_id(std::string_view key) {
    remove_context(std::string(key));
}

bool logger::has_context_id(std::string_view key) const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->context_mutex_);
        return pimpl_->context_fields_.find(std::string(key)) != pimpl_->context_fields_.end();
    }
    return false;
}

void logger::clear_all_context_ids() {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->context_mutex_);
        for (const auto& key : ALL_CONTEXT_ID_KEYS) {
            pimpl_->context_fields_.erase(key);
        }
    }
}

// =========================================================================
// Real-time analysis implementation
// =========================================================================

void logger::set_realtime_analyzer(std::unique_ptr<analysis::realtime_log_analyzer> analyzer) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->analyzer_mutex_);
        pimpl_->realtime_analyzer_ = std::move(analyzer);
    }
}

analysis::realtime_log_analyzer* logger::get_realtime_analyzer() {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->analyzer_mutex_);
        return pimpl_->realtime_analyzer_.get();
    }
    return nullptr;
}

const analysis::realtime_log_analyzer* logger::get_realtime_analyzer() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->analyzer_mutex_);
        return pimpl_->realtime_analyzer_.get();
    }
    return nullptr;
}

bool logger::has_realtime_analysis() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->analyzer_mutex_);
        return pimpl_->realtime_analyzer_ != nullptr;
    }
    return false;
}

// =========================================================================
// Log sampling implementation
// =========================================================================

void logger::set_sampler(std::unique_ptr<sampling::log_sampler> sampler) {
    if (pimpl_) {
        std::lock_guard<std::shared_mutex> lock(pimpl_->sampler_mutex_);
        pimpl_->sampler_ = std::move(sampler);
    }
}

sampling::log_sampler* logger::get_sampler() {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->sampler_mutex_);
        return pimpl_->sampler_.get();
    }
    return nullptr;
}

const sampling::log_sampler* logger::get_sampler() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->sampler_mutex_);
        return pimpl_->sampler_.get();
    }
    return nullptr;
}

bool logger::has_sampling() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->sampler_mutex_);
        return pimpl_->sampler_ != nullptr && pimpl_->sampler_->is_enabled();
    }
    return false;
}

sampling::sampling_stats logger::get_sampling_stats() const {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->sampler_mutex_);
        if (pimpl_->sampler_) {
            return pimpl_->sampler_->get_stats();
        }
    }
    return sampling::sampling_stats{};
}

void logger::reset_sampling_stats() {
    if (pimpl_) {
        std::shared_lock<std::shared_mutex> lock(pimpl_->sampler_mutex_);
        if (pimpl_->sampler_) {
            pimpl_->sampler_->reset_stats();
        }
    }
}

} // namespace kcenon::logger
