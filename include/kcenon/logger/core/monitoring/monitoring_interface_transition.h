/**
 * @file monitoring_interface_transition.h
 * @brief Transition header for migrating from logger::monitoring to common::interfaces
 *
 * DEPRECATED: This file provides compatibility during migration to common_system interfaces.
 * It will be removed in version 3.0.0. Please migrate to using common::interfaces::IMonitor
 * and common::interfaces::IMonitorable directly.
 */

#pragma once

#include <kcenon/common/interfaces/monitoring_interface.h>
#include <memory>

// Deprecation warning macros
#if defined(__GNUC__) || defined(__clang__)
#define LOGGER_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define LOGGER_DEPRECATED __declspec(deprecated)
#else
#define LOGGER_DEPRECATED
#endif

namespace kcenon::logger::monitoring {

// DEPRECATED: Use common::interfaces::health_status instead
using health_status LOGGER_DEPRECATED = common::interfaces::health_status;

// DEPRECATED: Use common::interfaces::metric_value instead
using metric_value LOGGER_DEPRECATED = common::interfaces::metric_value;

// DEPRECATED: Use common::interfaces::metrics_snapshot instead
using monitoring_data LOGGER_DEPRECATED = common::interfaces::metrics_snapshot;

// DEPRECATED: Use common::interfaces::health_check_result instead
using health_check_result LOGGER_DEPRECATED = common::interfaces::health_check_result;

// DEPRECATED: Use common::interfaces::IMonitor instead
using monitoring_interface LOGGER_DEPRECATED = common::interfaces::IMonitor;

/**
 * @class basic_monitoring
 * @brief Legacy basic monitoring implementation for backward compatibility
 *
 * DEPRECATED: This class is provided for backward compatibility only.
 * New code should use common::interfaces::IMonitor implementations directly.
 * This class will be removed in version 3.0.0.
 */
class LOGGER_DEPRECATED basic_monitoring : public common::interfaces::IMonitor {
private:
    mutable common::interfaces::metrics_snapshot data_;

public:
    basic_monitoring() {
        data_.source_id = "logger_system::basic_monitoring";
    }

    common::VoidResult record_metric(const std::string& name, double value) override {
        data_.add_metric(name, value);

        // Auto-update health status based on metrics (legacy behavior)
        if (name == "error_rate" && value > 0.1) {
            // Mark as degraded in metadata
            data_.capture_time = std::chrono::system_clock::now();
        } else if (name == "queue_utilization" && value > 0.9) {
            // Mark as critical in metadata
            data_.capture_time = std::chrono::system_clock::now();
        }

        return common::VoidResult::success();
    }

    common::VoidResult record_metric(
        const std::string& name,
        double value,
        const std::unordered_map<std::string, std::string>& tags) override {

        common::interfaces::metric_value metric(name, value);
        metric.tags = tags;
        data_.metrics.push_back(metric);
        data_.capture_time = std::chrono::system_clock::now();

        return common::VoidResult::success();
    }

    common::Result<common::interfaces::metrics_snapshot> get_metrics() override {
        data_.capture_time = std::chrono::system_clock::now();
        return common::Result<common::interfaces::metrics_snapshot>::success(data_);
    }

    common::Result<common::interfaces::health_check_result> check_health() override {
        common::interfaces::health_check_result result;
        result.timestamp = std::chrono::system_clock::now();
        result.status = common::interfaces::health_status::healthy;
        result.message = "Basic monitoring operational";

        // Check metrics for health indicators
        for (const auto& metric : data_.metrics) {
            if (metric.name == "error_rate" && metric.value > 0.1) {
                result.status = common::interfaces::health_status::degraded;
                result.message = "High error rate detected";
            } else if (metric.name == "queue_utilization" && metric.value > 0.9) {
                result.status = common::interfaces::health_status::unhealthy;
                result.message = "Queue utilization critical";
            }
        }

        return common::Result<common::interfaces::health_check_result>::success(result);
    }

    common::VoidResult reset() override {
        data_ = common::interfaces::metrics_snapshot();
        data_.source_id = "logger_system::basic_monitoring";
        return common::VoidResult::success();
    }

    // Legacy methods for backward compatibility
    LOGGER_DEPRECATED
    monitoring_data get_monitoring_data() const {
        auto copy = data_;
        copy.capture_time = std::chrono::system_clock::now();
        return copy;
    }

    LOGGER_DEPRECATED
    bool is_healthy() const {
        auto result = const_cast<basic_monitoring*>(this)->check_health();
        return result && result.value().is_healthy();
    }

    LOGGER_DEPRECATED
    health_status get_health_status() const {
        auto result = const_cast<basic_monitoring*>(this)->check_health();
        if (result) {
            return result.value().status;
        }
        return health_status::unknown;
    }

    LOGGER_DEPRECATED
    void set_metric(const std::string& name, double value) {
        record_metric(name, value);
    }

    LOGGER_DEPRECATED
    double get_metric(const std::string& name) const {
        for (const auto& metric : data_.metrics) {
            if (metric.name == name) {
                return metric.value;
            }
        }
        return 0.0;
    }
};

} // namespace kcenon::logger::monitoring

// Deprecation notice in compilation output
#pragma message("monitoring_interface_transition.h is deprecated. Use <kcenon/common/interfaces/monitoring_interface.h> instead.")
