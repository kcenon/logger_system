/**
 * @file monitoring_factory.h
 * @brief Factory for creating monitoring instances (Phase 2.2)
 */

#pragma once

#include <kcenon/common/interfaces/monitoring_interface.h>
#include <memory>
#include <unordered_map>
#include <mutex>
#include <atomic>

namespace kcenon::logger::monitoring {

// Forward compatibility aliases (monitoring_interface removed due to conflict with transition header)
using monitoring_metrics = common::interfaces::metrics_snapshot;

/**
 * @brief Simple monitoring implementation for logger_system
 */
class simple_monitor : public common::interfaces::IMonitor {
public:
    common::VoidResult record_metric(const std::string& name, double value) override {
        std::lock_guard lock(mutex_);
        metrics_[name].store(value, std::memory_order_relaxed);
        return {}; // Success (empty variant)
    }

    common::VoidResult record_metric(
        const std::string& name,
        double value,
        const std::unordered_map<std::string, std::string>& tags) override {
        (void)tags; // Tags not yet implemented
        return record_metric(name, value);
    }

    common::Result<common::interfaces::metrics_snapshot> get_metrics() override {
        std::lock_guard lock(mutex_);
        common::interfaces::metrics_snapshot snapshot;
        snapshot.source_id = "logger_system::simple_monitor";
        snapshot.capture_time = std::chrono::system_clock::now();

        for (const auto& [name, value] : metrics_) {
            snapshot.add_metric(name, value.load(std::memory_order_relaxed));
        }

        return snapshot;
    }

    common::Result<common::interfaces::health_check_result> check_health() override {
        common::interfaces::health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = common::interfaces::health_status::healthy;
        result.message = "Simple monitor operational";
        return result;
    }

    common::VoidResult reset() override {
        std::lock_guard lock(mutex_);
        metrics_.clear();
        return {}; // Success (empty variant)
    }

private:
    std::mutex mutex_;
    std::unordered_map<std::string, std::atomic<double>> metrics_;
};

/**
 * @brief Factory class for creating monitoring instances
 */
class monitoring_factory {
public:
    /**
     * @brief Monitor type enumeration
     */
    enum class monitor_type {
        automatic,
        basic,
        advanced
    };

    /**
     * @brief Create a default monitoring instance
     * @return Shared pointer to IMonitor
     */
    static std::shared_ptr<common::interfaces::IMonitor> create_default() {
        return std::make_shared<simple_monitor>();
    }

    /**
     * @brief Create a monitoring instance with specific configuration
     * @param config Configuration parameters for monitoring
     * @return Shared pointer to IMonitor
     */
    template<typename ConfigType>
    static std::shared_ptr<common::interfaces::IMonitor> create_with_config(const ConfigType& config) {
        (void)config; // Suppress unused parameter warning
        return create_default();
    }

    /**
     * @brief Get the global monitoring instance
     * @return Reference to the global monitoring instance
     */
    static common::interfaces::IMonitor& get_global_monitoring() {
        static simple_monitor global_instance;
        return global_instance;
    }

    /**
     * @brief Create a monitoring instance with specific health thresholds
     * @param error_rate_threshold Error rate threshold for warning status
     * @param queue_utilization_threshold Queue utilization threshold for critical status
     * @return Shared pointer to IMonitor
     */
    static std::shared_ptr<common::interfaces::IMonitor> create_with_thresholds(
        double error_rate_threshold = 0.1,
        double queue_utilization_threshold = 0.9) {

        auto monitor = create_default();
        monitor->record_metric("error_rate_threshold", error_rate_threshold);
        monitor->record_metric("queue_utilization_threshold", queue_utilization_threshold);
        return monitor;
    }
};

} // namespace kcenon::logger::monitoring