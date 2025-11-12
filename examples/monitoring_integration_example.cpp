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
 * @file monitoring_integration_example.cpp
 * @brief Advanced monitoring integration example for Phase 4
 *
 * Demonstrates integration between logger_system and monitoring_system
 * using only common_system interfaces (no circular dependencies).
 */

#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/interfaces/monitoring_interface.h>
#include <iostream>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <unordered_map>

using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;

/**
 * @brief Aggregating monitor that collects metrics from multiple sources
 */
class aggregating_monitor : public ci::IMonitor,
                            public ci::IMonitorProvider,
                            public std::enable_shared_from_this<aggregating_monitor> {
private:
    std::vector<std::shared_ptr<ci::IMonitorable>> monitored_components_;
    std::unordered_map<std::string, double> aggregated_metrics_;
    mutable std::mutex mutex_;

public:
    void register_component(std::shared_ptr<ci::IMonitorable> component) {
        std::lock_guard<std::mutex> lock(mutex_);
        monitored_components_.push_back(component);
        std::cout << "[AggregatingMonitor] Registered component: "
                  << component->get_component_name() << std::endl;
    }

    kcenon::common::VoidResult record_metric(
        const std::string& name,
        double value) override
    {
        std::lock_guard<std::mutex> lock(mutex_);
        aggregated_metrics_[name] = value;
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult record_metric(
        const std::string& name,
        double value,
        const std::unordered_map<std::string, std::string>& tags) override
    {
        std::string tagged_name = name;
        for (const auto& [k, v] : tags) {
            tagged_name += "." + k + ":" + v;
        }
        return record_metric(tagged_name, value);
    }

    kcenon::common::Result<ci::metrics_snapshot> get_metrics() override {
        std::lock_guard<std::mutex> lock(mutex_);

        ci::metrics_snapshot snapshot;
        snapshot.source_id = "aggregating_monitor";
        snapshot.capture_time = std::chrono::system_clock::now();

        // Add own metrics
        for (const auto& [name, value] : aggregated_metrics_) {
            snapshot.add_metric(name, value);
        }

        // Collect metrics from all registered components
        for (const auto& component : monitored_components_) {
            auto comp_data = component->get_monitoring_data();
            if (kcenon::common::is_ok(comp_data)) {
                const auto& component_metrics = kcenon::common::get_value(comp_data);
                for (const auto& metric : component_metrics.metrics) {
                    snapshot.metrics.push_back(metric);
                }
            } else {
                snapshot.add_metric("component_error_" + component->get_component_name(), 1.0);
            }
        }

        return kcenon::common::Result<ci::metrics_snapshot>::ok(std::move(snapshot));
    }

    kcenon::common::Result<ci::health_check_result> check_health() override {
        std::lock_guard<std::mutex> lock(mutex_);

        ci::health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = ci::health_status::healthy;
        result.message = "Aggregating monitor operational";

        // Check health of all components
        for (const auto& component : monitored_components_) {
            auto comp_health = component->health_check();
            const auto component_name = component->get_component_name();

            if (kcenon::common::is_ok(comp_health)) {
                const auto& component_result = kcenon::common::get_value(comp_health);
                result.metadata["component_status:" + component_name] = ci::to_string(component_result.status);

                if (component_result.status == ci::health_status::unhealthy) {
                    result.status = ci::health_status::unhealthy;
                    result.message = "One or more components unhealthy";
                } else if (component_result.status == ci::health_status::degraded &&
                           result.status == ci::health_status::healthy) {
                    result.status = ci::health_status::degraded;
                    result.message = "One or more components degraded";
                }
            } else {
                const auto& error = kcenon::common::get_error(comp_health);
                result.metadata["component_status:" + component_name] = "error:" + error.message;
                if (result.status == ci::health_status::healthy) {
                    result.status = ci::health_status::degraded;
                    result.message = "Component health check failed";
                }
            }
        }

        return kcenon::common::Result<ci::health_check_result>::ok(std::move(result));
    }

    kcenon::common::VoidResult reset() override {
        std::lock_guard<std::mutex> lock(mutex_);
        aggregated_metrics_.clear();
        return kcenon::common::ok();
    }

    // IMonitorProvider implementation
    std::shared_ptr<ci::IMonitor> get_monitor() override {
        return shared_from_this();
    }

    std::shared_ptr<ci::IMonitor> create_monitor(const std::string& name) override {
        // For this example, return self
        return shared_from_this();
    }

    size_t get_component_count() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return monitored_components_.size();
    }
};

/**
 * @brief Print metrics snapshot in formatted way
 */
void print_metrics_snapshot(const ci::metrics_snapshot& snapshot) {
    std::cout << "\n--- Metrics Snapshot ---" << std::endl;
    std::cout << "Source: " << snapshot.source_id << std::endl;
    std::cout << "Captured at: "
              << std::chrono::system_clock::to_time_t(snapshot.capture_time)
              << std::endl;
    std::cout << "Metrics:" << std::endl;

    for (const auto& metric : snapshot.metrics) {
        std::cout << "  " << std::setw(30) << std::left << metric.name
                  << ": " << metric.value << std::endl;
    }
}

/**
 * @brief Print health check result
 */
void print_health_result(const ci::health_check_result& health) {
    std::cout << "\n--- Health Check ---" << std::endl;
    std::cout << "Status: " << ci::to_string(health.status) << std::endl;
    std::cout << "Message: " << health.message << std::endl;

    if (!health.metadata.empty()) {
        std::cout << "Component Status:" << std::endl;
        for (const auto& [key, value] : health.metadata) {
            if (key.rfind("component_status:", 0) == 0) {
                std::cout << "  - " << key.substr(std::string("component_status:").size())
                          << ": " << value << std::endl;
            }
        }
    }

    std::cout << "Check duration: "
              << health.check_duration.count() << "ms" << std::endl;
}

/**
 * @brief Example 1: Basic monitor integration with logger
 */
void example_1_basic_integration() {
    std::cout << "\n=== Example 1: Basic Monitor Integration ===" << std::endl;

    auto monitor = std::make_shared<aggregating_monitor>();

    auto logger_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor)
        .build();

    if (!logger_result) {
        std::cerr << "Failed to create logger" << std::endl;
        return;
    }

    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    // Register logger as monitored component
    monitor->register_component(std::static_pointer_cast<ci::IMonitorable>(logger_instance));

    // Perform logging operations
    for (int i = 0; i < 5; ++i) {
        logger_instance->log(log_level::info,
            "Log message " + std::to_string(i + 1));
    }

    // Get aggregated metrics
    auto metrics = monitor->get_metrics();
    if (kcenon::common::is_ok(metrics)) {
        print_metrics_snapshot(kcenon::common::get_value(metrics));
    }

    // Check aggregated health
    auto health = monitor->check_health();
    if (kcenon::common::is_ok(health)) {
        print_health_result(kcenon::common::get_value(health));
    }
}

/**
 * @brief Example 2: Multiple loggers with single monitor
 */
void example_2_multiple_loggers() {
    std::cout << "\n=== Example 2: Multiple Loggers, Single Monitor ===" << std::endl;

    auto monitor = std::make_shared<aggregating_monitor>();

    // Create multiple logger instances
    auto logger1_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor)
        .build();

    auto logger2_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor)
        .build();

    if (!logger1_result || !logger2_result) {
        std::cerr << "Failed to create loggers" << std::endl;
        return;
    }

    auto logger1 = std::shared_ptr<logger>(std::move(logger1_result.value()));
    auto logger2 = std::shared_ptr<logger>(std::move(logger2_result.value()));

    // Register both loggers
    monitor->register_component(std::static_pointer_cast<ci::IMonitorable>(logger1));
    monitor->register_component(std::static_pointer_cast<ci::IMonitorable>(logger2));

    // Both loggers use the same monitor
    logger1->log(log_level::info, "Message from logger 1");
    logger2->log(log_level::warning, "Message from logger 2");
    logger1->log(log_level::error, "Error from logger 1");

    std::cout << "\nMonitor tracks " << monitor->get_component_count()
              << " components" << std::endl;

    // Get combined metrics
    auto metrics = monitor->get_metrics();
    if (kcenon::common::is_ok(metrics)) {
        std::cout << "Combined metrics from all loggers:" << std::endl;
        print_metrics_snapshot(kcenon::common::get_value(metrics));
    }
}

/**
 * @brief Example 3: Demonstrating IMonitorable interface
 */
void example_3_imonitorable_interface() {
    std::cout << "\n=== Example 3: IMonitorable Interface ===" << std::endl;

    auto monitor = std::make_shared<aggregating_monitor>();

    auto logger_result = logger_builder()
        .with_async(false)
        .with_monitoring(monitor)
        .build();

    if (!logger_result) return;

    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    // Cast to IMonitorable to demonstrate interface usage
    if (auto monitorable = std::dynamic_pointer_cast<ci::IMonitorable>(logger_instance)) {
        std::cout << "Logger component name: "
                  << monitorable->get_component_name() << std::endl;

        // Perform operations
        logger_instance->log(log_level::info, "Test message 1");
        logger_instance->log(log_level::info, "Test message 2");

        // Get monitoring data directly from logger
        auto data = monitorable->get_monitoring_data();
        if (kcenon::common::is_ok(data)) {
            std::cout << "\nDirect monitoring data from logger:" << std::endl;
            print_metrics_snapshot(kcenon::common::get_value(data));
        }

        // Health check directly from logger
        auto health = monitorable->health_check();
        if (kcenon::common::is_ok(health)) {
            std::cout << "\nDirect health check from logger:" << std::endl;
            print_health_result(kcenon::common::get_value(health));
        }
    }
}

/**
 * @brief Example 4: Simulating monitoring_system integration
 */
void example_4_monitoring_system_simulation() {
    std::cout << "\n=== Example 4: Monitoring System Integration Simulation ===" << std::endl;
    std::cout << "Note: This demonstrates how logger_system and monitoring_system" << std::endl;
    std::cout << "      interact via interfaces without circular dependencies" << std::endl;

    // Simulate monitoring_system providing a monitor
    std::shared_ptr<ci::IMonitor> monitor = std::make_shared<aggregating_monitor>();

    // Logger receives monitor through DI
    auto logger_result = logger_builder()
        .with_async(true)  // Async mode for realistic scenario
        .with_monitoring(monitor)
        .build();

    if (!logger_result) return;

    auto logger_instance = std::shared_ptr<logger>(std::move(logger_result.value()));

    std::cout << "\nPhase 1: Logger operates and reports to monitor" << std::endl;

    // Simulate application activity
    for (int i = 0; i < 10; ++i) {
        logger_instance->log(log_level::info,
            "Application event " + std::to_string(i + 1));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Give async logger time to flush
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::cout << "\nPhase 2: Monitoring system queries metrics" << std::endl;

    // Monitoring system can query the monitor
    auto metrics = monitor->get_metrics();
    if (kcenon::common::is_ok(metrics)) {
        std::cout << "Monitoring system received metrics:" << std::endl;
        print_metrics_snapshot(kcenon::common::get_value(metrics));
    }

    // Monitoring system can check logger health through IMonitorable
    if (auto monitorable = std::dynamic_pointer_cast<ci::IMonitorable>(logger_instance)) {
        auto health = monitorable->health_check();
        if (kcenon::common::is_ok(health)) {
            std::cout << "\nLogger health status:" << std::endl;
            print_health_result(kcenon::common::get_value(health));
        }
    }

    std::cout << "\n✓ Integration successful without circular dependencies" << std::endl;
}

int main() {
    std::cout << "============================================================" << std::endl;
    std::cout << "Logger-Monitor Integration Examples (Phase 4)" << std::endl;
    std::cout << "Demonstrating loose coupling via common_system interfaces" << std::endl;
    std::cout << "============================================================" << std::endl;

    try {
        example_1_basic_integration();
        example_2_multiple_loggers();
        example_3_imonitorable_interface();
        example_4_monitoring_system_simulation();

        std::cout << "\n============================================================" << std::endl;
        std::cout << "All integration examples completed successfully!" << std::endl;
        std::cout << "============================================================" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
