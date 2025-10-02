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

using namespace kcenon::logger;
using namespace kcenon::common::interfaces;

/**
 * @brief Aggregating monitor that collects metrics from multiple sources
 */
class aggregating_monitor : public IMonitor, public IMonitorProvider {
private:
    std::vector<std::shared_ptr<IMonitorable>> monitored_components_;
    std::unordered_map<std::string, double> aggregated_metrics_;
    mutable std::mutex mutex_;

public:
    void register_component(std::shared_ptr<IMonitorable> component) {
        std::lock_guard<std::mutex> lock(mutex_);
        monitored_components_.push_back(component);
        std::cout << "[AggregatingMonitor] Registered component: "
                  << component->get_component_name() << std::endl;
    }

    common::VoidResult record_metric(
        const std::string& name,
        double value) override
    {
        std::lock_guard<std::mutex> lock(mutex_);
        aggregated_metrics_[name] = value;
        return common::VoidResult::success();
    }

    common::VoidResult record_metric(
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

    common::Result<metrics_snapshot> get_metrics() override {
        std::lock_guard<std::mutex> lock(mutex_);

        metrics_snapshot snapshot;
        snapshot.source_id = "aggregating_monitor";
        snapshot.capture_time = std::chrono::system_clock::now();

        // Add own metrics
        for (const auto& [name, value] : aggregated_metrics_) {
            snapshot.add_metric(name, value);
        }

        // Collect metrics from all registered components
        for (const auto& component : monitored_components_) {
            auto comp_data = component->get_monitoring_data();
            if (comp_data) {
                for (const auto& metric : comp_data.value().metrics) {
                    snapshot.metrics.push_back(metric);
                }
            }
        }

        return common::Result<metrics_snapshot>::success(std::move(snapshot));
    }

    common::Result<health_check_result> check_health() override {
        std::lock_guard<std::mutex> lock(mutex_);

        health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = health_status::healthy;
        result.message = "Aggregating monitor operational";

        // Check health of all components
        for (const auto& component : monitored_components_) {
            auto comp_health = component->health_check();
            if (comp_health) {
                dependency_health dep;
                dep.name = component->get_component_name();
                dep.status = comp_health.value().status;
                result.dependencies.push_back(dep);

                // Degrade overall status if any component is unhealthy
                if (comp_health.value().status != health_status::healthy) {
                    result.status = health_status::degraded;
                    result.message = "One or more components degraded";
                }
            }
        }

        return common::Result<health_check_result>::success(std::move(result));
    }

    common::VoidResult reset() override {
        std::lock_guard<std::mutex> lock(mutex_);
        aggregated_metrics_.clear();
        return common::VoidResult::success();
    }

    // IMonitorProvider implementation
    std::shared_ptr<IMonitor> get_monitor() override {
        return shared_from_this();
    }

    std::shared_ptr<IMonitor> create_monitor(const std::string& name) override {
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
void print_metrics_snapshot(const metrics_snapshot& snapshot) {
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
void print_health_result(const health_check_result& health) {
    std::cout << "\n--- Health Check ---" << std::endl;
    std::cout << "Status: " << to_string(health.status) << std::endl;
    std::cout << "Message: " << health.message << std::endl;

    if (!health.dependencies.empty()) {
        std::cout << "Dependencies:" << std::endl;
        for (const auto& dep : health.dependencies) {
            std::cout << "  - " << dep.name << ": "
                      << to_string(dep.status) << std::endl;
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

    auto logger_instance = std::move(logger_result.value());

    // Register logger as monitored component
    monitor->register_component(logger_instance);

    // Perform logging operations
    for (int i = 0; i < 5; ++i) {
        logger_instance->log(log_level::info,
            "Log message " + std::to_string(i + 1));
    }

    // Get aggregated metrics
    auto metrics = monitor->get_metrics();
    if (metrics) {
        print_metrics_snapshot(metrics.value());
    }

    // Check aggregated health
    auto health = monitor->check_health();
    if (health) {
        print_health_result(health.value());
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

    auto logger1 = std::move(logger1_result.value());
    auto logger2 = std::move(logger2_result.value());

    // Register both loggers
    monitor->register_component(logger1);
    monitor->register_component(logger2);

    // Both loggers use the same monitor
    logger1->log(log_level::info, "Message from logger 1");
    logger2->log(log_level::warning, "Message from logger 2");
    logger1->log(log_level::error, "Error from logger 1");

    std::cout << "\nMonitor tracks " << monitor->get_component_count()
              << " components" << std::endl;

    // Get combined metrics
    auto metrics = monitor->get_metrics();
    if (metrics) {
        std::cout << "Combined metrics from all loggers:" << std::endl;
        print_metrics_snapshot(metrics.value());
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

    auto logger_instance = std::move(logger_result.value());

    // Cast to IMonitorable to demonstrate interface usage
    if (auto monitorable = std::dynamic_pointer_cast<IMonitorable>(logger_instance)) {
        std::cout << "Logger component name: "
                  << monitorable->get_component_name() << std::endl;

        // Perform operations
        logger_instance->log(log_level::info, "Test message 1");
        logger_instance->log(log_level::info, "Test message 2");

        // Get monitoring data directly from logger
        auto data = monitorable->get_monitoring_data();
        if (data) {
            std::cout << "\nDirect monitoring data from logger:" << std::endl;
            print_metrics_snapshot(data.value());
        }

        // Health check directly from logger
        auto health = monitorable->health_check();
        if (health) {
            std::cout << "\nDirect health check from logger:" << std::endl;
            print_health_result(health.value());
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
    std::shared_ptr<IMonitor> monitor = std::make_shared<aggregating_monitor>();

    // Logger receives monitor through DI
    auto logger_result = logger_builder()
        .with_async(true)  // Async mode for realistic scenario
        .with_monitoring(monitor)
        .build();

    if (!logger_result) return;

    auto logger_instance = std::move(logger_result.value());

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
    if (metrics) {
        std::cout << "Monitoring system received metrics:" << std::endl;
        print_metrics_snapshot(metrics.value());
    }

    // Monitoring system can check logger health through IMonitorable
    if (auto monitorable = std::dynamic_pointer_cast<IMonitorable>(logger_instance)) {
        auto health = monitorable->health_check();
        if (health) {
            std::cout << "\nLogger health status:" << std::endl;
            print_health_result(health.value());
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
