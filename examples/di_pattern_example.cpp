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

using namespace kcenon::logger;
using namespace kcenon::common::interfaces;

/**
 * @brief Example monitor implementation demonstrating IMonitor interface
 *
 * This is a simple in-memory monitor that tracks metrics without
 * requiring a full monitoring_system dependency.
 */
class example_monitor : public IMonitor {
private:
    std::unordered_map<std::string, double> metrics_;
    mutable std::mutex metrics_mutex_;
    size_t metric_count_ = 0;

public:
    common::VoidResult record_metric(
        const std::string& name,
        double value) override
    {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        metrics_[name] = value;
        metric_count_++;

        std::cout << "[Monitor] Recorded metric: " << name
                  << " = " << value << std::endl;

        return common::VoidResult::success();
    }

    common::VoidResult record_metric(
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

        return common::VoidResult::success();
    }

    common::Result<metrics_snapshot> get_metrics() override {
        std::lock_guard<std::mutex> lock(metrics_mutex_);

        metrics_snapshot snapshot;
        snapshot.source_id = "example_monitor";
        snapshot.capture_time = std::chrono::system_clock::now();

        for (const auto& [name, value] : metrics_) {
            snapshot.add_metric(name, value);
        }

        return common::Result<metrics_snapshot>::success(std::move(snapshot));
    }

    common::Result<health_check_result> check_health() override {
        health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = health_status::healthy;
        result.message = "Example monitor operational";
        result.metadata["metrics_count"] = std::to_string(metric_count_);

        return common::Result<health_check_result>::success(std::move(result));
    }

    common::VoidResult reset() override {
        std::lock_guard<std::mutex> lock(metrics_mutex_);
        metrics_.clear();
        metric_count_ = 0;

        std::cout << "[Monitor] Metrics reset" << std::endl;
        return common::VoidResult::success();
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
                  << logger_result.error().message << std::endl;
        return;
    }

    auto logger_instance = std::move(logger_result.value());

    // Step 3: Use logger - metrics are automatically recorded to monitor
    logger_instance->log(log_level::info, "Application started");
    logger_instance->log(log_level::debug, "Debug message");
    logger_instance->log(log_level::warning, "Warning message");

    // Step 4: Verify metrics were recorded
    std::cout << "\nMonitor collected " << monitor->get_metric_count()
              << " metrics" << std::endl;

    // Step 5: Get health status from logger (IMonitorable interface)
    auto health = logger_instance->health_check();
    if (health) {
        std::cout << "Logger health: "
                  << to_string(health.value().status) << std::endl;
    }
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

    auto logger_instance = std::move(logger_result.value());

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

    auto logger_instance = std::move(logger_result.value());

    std::cout << "Phase 1: Operating without monitor" << std::endl;
    logger_instance->log(log_level::info, "Message 1 - no monitoring");

    // Inject monitor at runtime
    auto monitor = std::make_shared<example_monitor>();
    logger_instance->set_monitor(monitor);

    std::cout << "\nPhase 2: Monitor injected at runtime" << std::endl;
    logger_instance->log(log_level::info, "Message 2 - with monitoring");
    logger_instance->log(log_level::info, "Message 3 - with monitoring");

    std::cout << "\nMonitor recorded " << monitor->get_metric_count()
              << " metrics (only from Phase 2)" << std::endl;
}

/**
 * @brief Example demonstrating monitor swapping
 */
void example_4_monitor_swapping() {
    std::cout << "\n=== Example 4: Monitor Swapping ===" << std::endl;

    auto monitor1 = std::make_shared<example_monitor>();
    auto monitor2 = std::make_shared<example_monitor>();

    auto logger_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor1)
        .build();

    if (!logger_result) return;
    auto logger_instance = std::move(logger_result.value());

    std::cout << "Using Monitor 1" << std::endl;
    logger_instance->log(log_level::info, "Message to Monitor 1");
    logger_instance->log(log_level::info, "Another message to Monitor 1");

    // Swap monitors at runtime
    logger_instance->set_monitor(monitor2);

    std::cout << "\nSwapped to Monitor 2" << std::endl;
    logger_instance->log(log_level::info, "Message to Monitor 2");

    std::cout << "\nMonitor 1 metrics: " << monitor1->get_metric_count() << std::endl;
    std::cout << "Monitor 2 metrics: " << monitor2->get_metric_count() << std::endl;
}

/**
 * @brief Example demonstrating ILogger interface usage
 */
void use_logger_via_interface(std::shared_ptr<ILogger> logger) {
    // This function only depends on ILogger interface
    // Can work with any logger implementation

    logger->log(log_level::info, "Used via interface - loose coupling!");

    // Check if logger supports monitoring (IMonitorable)
    if (auto monitorable = std::dynamic_pointer_cast<IMonitorable>(logger)) {
        auto data = monitorable->get_monitoring_data();
        if (data) {
            std::cout << "Logger provides monitoring data from "
                      << data.value().source_id << std::endl;
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
    std::shared_ptr<ILogger> logger_interface = std::move(logger_result.value());

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
