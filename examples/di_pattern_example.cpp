// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file di_pattern_example.cpp
 * @brief Dependency Injection pattern example for logger_system Phase 4
 *
 * This example demonstrates the Dependency Injection (DI) pattern with
 * common_system interfaces for loose coupling between logger and monitor systems.
 */

#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/interfaces/monitoring_interface.h>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <mutex>
#include <unordered_map>
#include <sstream>

using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;

namespace {

log_level to_logger_level(ci::log_level level) {
    switch (level) {
        case ci::log_level::trace: return log_level::trace;
        case ci::log_level::debug: return log_level::debug;
        case ci::log_level::info: return log_level::info;
        case ci::log_level::warning: return log_level::warning;
        case ci::log_level::error: return log_level::error;
        case ci::log_level::critical: return log_level::critical;
        case ci::log_level::off: return log_level::critical;
        default: return log_level::info;
    }
}

ci::log_level to_common_level(log_level level) {
    switch (level) {
        case log_level::trace: return ci::log_level::trace;
        case log_level::debug: return ci::log_level::debug;
        case log_level::info: return ci::log_level::info;
        case log_level::warning: return ci::log_level::warning;
        case log_level::error: return ci::log_level::error;
        case log_level::critical: return ci::log_level::critical;
        default: return ci::log_level::info;
    }
}

kcenon::common::VoidResult make_adapter_error(const std::string& message) {
    return kcenon::common::VoidResult(kcenon::common::error_info{1, message, "logger_system"});
}

class logger_interface_adapter : public ci::ILogger {
public:
    explicit logger_interface_adapter(std::shared_ptr<logger> logger)
        : logger_(std::move(logger)) {}

    kcenon::common::VoidResult log(ci::log_level level, const std::string& message) override {
        if (!logger_) {
            return make_adapter_error("Logger not initialized");
        }
        logger_->log(to_logger_level(level), message);
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult log(ci::log_level level,
                           std::string_view message,
                           const kcenon::common::source_location& loc) override {
        if (!logger_) {
            return make_adapter_error("Logger not initialized");
        }
        std::ostringstream oss;
        oss << "[" << loc.file_name() << ':' << loc.line() << ':' << loc.function_name() << "] " << message;
        logger_->log(to_logger_level(level), oss.str());
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult log(const ci::log_entry& entry) override {
        if (!logger_) {
            return make_adapter_error("Logger not initialized");
        }
        std::ostringstream oss;
        if (!entry.file.empty()) {
            oss << "[" << entry.file << ':' << entry.line << ':' << entry.function << "] ";
        }
        oss << entry.message;
        logger_->log(to_logger_level(entry.level), oss.str());
        return kcenon::common::ok();
    }

    bool is_enabled(ci::log_level level) const override {
        return logger_ && logger_->is_enabled(to_logger_level(level));
    }

    kcenon::common::VoidResult set_level(ci::log_level level) override {
        if (!logger_) {
            return make_adapter_error("Logger not initialized");
        }
        logger_->set_min_level(to_logger_level(level));
        return kcenon::common::ok();
    }

    ci::log_level get_level() const override {
        if (!logger_) {
            return ci::log_level::off;
        }
        return to_common_level(logger_->get_min_level());
    }

    kcenon::common::VoidResult flush() override {
        if (!logger_) {
            return make_adapter_error("Logger not initialized");
        }
        logger_->flush();
        return kcenon::common::ok();
    }

private:
    std::shared_ptr<logger> logger_;
};

} // namespace

/**
 * @brief Example monitor implementation demonstrating IMonitor interface
 *
 * This is a simple in-memory monitor that tracks metrics without
 * requiring a full monitoring_system dependency.
 */
class example_monitor : public ci::IMonitor {
private:
    std::unordered_map<std::string, double> metrics_;
    mutable std::mutex metrics_mutex_;
    size_t metric_count_ = 0;

public:
    kcenon::common::VoidResult record_metric(
        const std::string& name,
        double value) override
    {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        metrics_[name] = value;
        metric_count_++;

        std::cout << "[Monitor] Recorded metric: " << name
                  << " = " << value << std::endl;

        return kcenon::common::ok();
    }

    kcenon::common::VoidResult record_metric(
        const std::string& name,
        double value,
        const std::unordered_map<std::string, std::string>& tags) override
    {
        std::lock_guard<std::mutex> lock(metrics_mutex_);

        // Create tagged metric name
        std::string tagged_name = name;
        for (const auto& [key, val] : tags) {
            tagged_name += "." + key + ":" + val;
        }

        metrics_[tagged_name] = value;
        metric_count_++;

        std::cout << "[Monitor] Recorded tagged metric: " << tagged_name
                  << " = " << value << std::endl;

        return kcenon::common::ok();
    }

    kcenon::common::Result<ci::metrics_snapshot> get_metrics() override {
        std::lock_guard<std::mutex> lock(metrics_mutex_);

        ci::metrics_snapshot snapshot;
        snapshot.source_id = "example_monitor";
        snapshot.capture_time = std::chrono::system_clock::now();

        for (const auto& [name, value] : metrics_) {
            snapshot.add_metric(name, value);
        }

        return kcenon::common::Result<ci::metrics_snapshot>::ok(std::move(snapshot));
    }

    kcenon::common::Result<ci::health_check_result> check_health() override {
        ci::health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = ci::health_status::healthy;
        result.message = "Example monitor operational";
        result.metadata["metrics_count"] = std::to_string(metric_count_);

        return kcenon::common::Result<ci::health_check_result>::ok(std::move(result));
    }

    kcenon::common::VoidResult reset() override {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        metrics_.clear();
        metric_count_ = 0;

        std::cout << "[Monitor] Metrics reset" << std::endl;
        return kcenon::common::ok();
    }

    size_t get_metric_count() const {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        return metric_count_;
    }
};

/**
 * @brief Example demonstrating DI pattern with logger and monitor
 */
void example_1_basic_di_pattern() {
    std::cout << "\n=== Example 1: Basic Dependency Injection ===" << std::endl;

    // Step 1: Create monitor instance (can be any IMonitor implementation)
    auto monitor = std::make_shared<example_monitor>();

    // Step 2: Inject monitor into logger via builder pattern
    auto logger_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor)  // DI: Inject monitor interface
        .with_min_level(log_level::debug)
        .build();

    if (!logger_result) {
        std::cerr << "Failed to create logger: "
                  << logger_result.error_message() << std::endl;
        return;
    }

    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    // Step 3: Use logger - metrics are automatically recorded to monitor
    logger_instance->log(log_level::info, "Application started");
    logger_instance->log(log_level::debug, "Debug message");
    logger_instance->log(log_level::warning, "Warning message");

    // Step 4: Verify metrics were recorded
    std::cout << "\nMonitor collected " << monitor->get_metric_count()
              << " metrics" << std::endl;

    // Step 5: Get health status from logger (IMonitorable interface)
    // TODO: Implement health_check() in logger class (Phase 2.2)
    // auto health = logger_instance->health_check();
    // if (kcenon::common::is_ok(health)) {
    //     std::cout << "Logger health: "
    //               << ci::to_string(kcenon::common::get_value(health).status) << std::endl;
    // }
    std::cout << "Logger health: [health_check() not yet implemented]" << std::endl;
}

/**
 * @brief Example demonstrating monitor-less operation
 */
void example_2_optional_monitor() {
    std::cout << "\n=== Example 2: Optional Monitor (No Monitor) ===" << std::endl;

    // Build logger without monitor - demonstrates optional dependency
    auto logger_result = logger_builder()
        .with_async(false)
        .with_min_level(log_level::info)
        .build();

    if (!logger_result) {
        std::cerr << "Failed to create logger" << std::endl;
        return;
    }

    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    // Logger works fine without monitor
    logger_instance->log(log_level::info, "Operating without monitor");
    logger_instance->log(log_level::warning, "Warning without monitoring");

    std::cout << "Logger operates successfully without monitor (DI optional)" << std::endl;
}

/**
 * @brief Example demonstrating runtime monitor injection
 */
void example_3_runtime_monitor_injection() {
    std::cout << "\n=== Example 3: Runtime Monitor Injection ===" << std::endl;

    // Create logger without monitor initially
    auto logger_result = logger_builder()
        .with_async(false)
        .build();

    if (!logger_result) {
        std::cerr << "Failed to create logger" << std::endl;
        return;
    }

    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    std::cout << "Phase 1: Operating without monitor" << std::endl;
    logger_instance->log(log_level::info, "Message 1 - no monitoring");

    // Inject monitor at runtime
    // TODO: Implement set_monitor() in logger class (Phase 2.2)
    auto monitor = std::make_unique<example_monitor>();
    auto* monitor_ref = monitor.get();
    // logger_instance->set_monitor(std::move(monitor));

    std::cout << "\nPhase 2: Monitor injected at runtime [set_monitor() not yet implemented]" << std::endl;
    logger_instance->log(log_level::info, "Message 2 - with monitoring");
    logger_instance->log(log_level::info, "Message 3 - with monitoring");

    std::cout << "\nMonitor recorded " << monitor_ref->get_metric_count()
              << " metrics (only from Phase 2)" << std::endl;
}

/**
 * @brief Example demonstrating monitor swapping
 */
void example_4_monitor_swapping() {
    std::cout << "\n=== Example 4: Monitor Swapping ===" << std::endl;

    auto logger_result = logger_builder()
        .with_async(false)
        .build();

    if (!logger_result) return;
    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    // TODO: Implement set_monitor() in logger class (Phase 2.2)
    auto monitor1 = std::make_unique<example_monitor>();
    auto* monitor1_ref = monitor1.get();
    // logger_instance->set_monitor(std::move(monitor1));

    std::cout << "Using Monitor 1 [set_monitor() not yet implemented]" << std::endl;
    logger_instance->log(log_level::info, "Message to Monitor 1");
    logger_instance->log(log_level::info, "Another message to Monitor 1");

    size_t monitor1_metrics = monitor1_ref->get_metric_count();

    // Swap monitors at runtime
    auto monitor2 = std::make_unique<example_monitor>();
    auto* monitor2_ref = monitor2.get();
    // logger_instance->set_monitor(std::move(monitor2));

    std::cout << "\nSwapped to Monitor 2 [set_monitor() not yet implemented]" << std::endl;
    logger_instance->log(log_level::info, "Message to Monitor 2");

    size_t monitor2_metrics = monitor2_ref->get_metric_count();

    std::cout << "\nMonitor 1 metrics before swap: " << monitor1_metrics << std::endl;
    std::cout << "Monitor 2 metrics after swap: " << monitor2_metrics << std::endl;
}

/**
 * @brief Example demonstrating ILogger interface usage
 */
void use_logger_via_interface(std::shared_ptr<ci::ILogger> logger) {
    // This function only depends on ILogger interface
    // Can work with any logger implementation

    // Use std::string explicitly to avoid ambiguity between log overloads
    logger->log(ci::log_level::info, std::string("Used via interface - loose coupling!"));

    // Check if logger supports monitoring (IMonitorable)
    if (auto monitorable = std::dynamic_pointer_cast<ci::IMonitorable>(logger)) {
        auto data = monitorable->get_monitoring_data();
        if (kcenon::common::is_ok(data)) {
            std::cout << "Logger provides monitoring data from "
                      << kcenon::common::get_value(data).source_id << std::endl;
        }
    }
}

void example_5_interface_based_usage() {
    std::cout << "\n=== Example 5: Interface-Based Usage ===" << std::endl;

    auto monitor = std::make_shared<example_monitor>();
    auto logger_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor)
        .build();

    if (!logger_result) return;

    // Use logger through ILogger interface pointer
    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));
    auto logger_adapter = std::make_shared<logger_interface_adapter>(logger_instance);
    std::shared_ptr<ci::ILogger> logger_interface = logger_adapter;

    use_logger_via_interface(logger_interface);

    std::cout << "Successfully used logger through interface abstraction" << std::endl;
}

int main() {
    std::cout << "==================================================" << std::endl;
    std::cout << "Logger System - Dependency Injection Pattern Demo" << std::endl;
    std::cout << "Phase 4: DI Pattern Implementation Examples" << std::endl;
    std::cout << "==================================================" << std::endl;

    try {
        example_1_basic_di_pattern();
        example_2_optional_monitor();
        example_3_runtime_monitor_injection();
        example_4_monitor_swapping();
        example_5_interface_based_usage();

        std::cout << "\n==================================================" << std::endl;
        std::cout << "All DI pattern examples completed successfully!" << std::endl;
        std::cout << "==================================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
