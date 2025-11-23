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
 * @file monitoring_interface_transition.h
 * @brief Transition header for migrating from logger::monitoring to common::interfaces
 *
 * @deprecated This entire file is deprecated and scheduled for removal in version 3.0.0.
 *
 * ## Migration Guide
 *
 * Replace the following types:
 * | Old Type (Deprecated)                          | New Type                                    |
 * |------------------------------------------------|---------------------------------------------|
 * | kcenon::logger::monitoring::health_status      | common::interfaces::health_status           |
 * | kcenon::logger::monitoring::metric_value       | common::interfaces::metric_value            |
 * | kcenon::logger::monitoring::monitoring_data    | common::interfaces::metrics_snapshot        |
 * | kcenon::logger::monitoring::health_check_result| common::interfaces::health_check_result     |
 * | kcenon::logger::monitoring::monitoring_interface| common::interfaces::IMonitor               |
 * | kcenon::logger::monitoring::basic_monitoring   | (Create custom IMonitor implementation)     |
 *
 * ## Example Migration
 *
 * Before (deprecated):
 * ```cpp
 * #include <kcenon/logger/core/monitoring/monitoring_interface_transition.h>
 * using kcenon::logger::monitoring::basic_monitoring;
 * basic_monitoring monitor;
 * monitor.set_metric("count", 42);
 * ```
 *
 * After (recommended):
 * ```cpp
 * #include <kcenon/common/interfaces/monitoring_interface.h>
 * // Use common::interfaces::IMonitor implementations
 * ```
 *
 * ## Timeline
 * - v2.x: Deprecation warnings enabled (current)
 * - v3.0: This file will be removed
 */

#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <chrono>
#include <vector>

// Only include common interfaces when explicitly using them
#ifdef LOGGER_USING_COMMON_INTERFACES
#include <kcenon/common/interfaces/monitoring_interface.h>
#endif

// Deprecation warning macros
#if defined(__GNUC__) || defined(__clang__)
#define LOGGER_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER)
#define LOGGER_DEPRECATED __declspec(deprecated)
#else
#define LOGGER_DEPRECATED
#endif

namespace kcenon::logger::monitoring {

#ifdef LOGGER_USING_COMMON_INTERFACES
// When using common interfaces, provide type aliases

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

#else
// Standalone mode: provide minimal legacy definitions

/**
 * @brief Health status enumeration (standalone mode)
 */
enum class health_status {
    healthy,
    degraded,
    unhealthy,
    unknown
};

/**
 * @brief Metric value structure (standalone mode)
 */
struct metric_value {
    std::string name;
    double value;
    std::chrono::system_clock::time_point timestamp;
    std::unordered_map<std::string, std::string> tags;

    metric_value(const std::string& n = "", double v = 0.0)
        : name(n), value(v), timestamp(std::chrono::system_clock::now()) {}
};

/**
 * @brief Monitoring data structure (standalone mode)
 */
struct monitoring_data {
    std::vector<metric_value> metrics;
    std::chrono::system_clock::time_point capture_time;
    std::string source_id;
    health_status status{health_status::unknown};
    std::string status_message;

    monitoring_data() : capture_time(std::chrono::system_clock::now()) {}

    void add_metric(const std::string& name, double value) {
        metrics.emplace_back(name, value);
    }
};

/**
 * @brief Health check result (standalone mode)
 */
struct health_check_result {
    health_status status = health_status::unknown;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::chrono::milliseconds check_duration{0};

    health_check_result() : timestamp(std::chrono::system_clock::now()) {}

    bool is_healthy() const {
        return status == health_status::healthy;
    }
};

/**
 * @brief Basic monitoring interface (standalone mode)
 */
class monitoring_interface {
public:
    virtual ~monitoring_interface() = default;
    virtual monitoring_data get_monitoring_data() const = 0;
    virtual bool is_healthy() const = 0;
    virtual health_status get_health_status() const = 0;
    virtual void set_metric(const std::string& name, double value) = 0;
    virtual double get_metric(const std::string& name) const = 0;
    virtual void reset() = 0;
};
#endif // LOGGER_USING_COMMON_INTERFACES

/**
 * @class basic_monitoring
 * @brief Legacy basic monitoring implementation for backward compatibility
 *
 * DEPRECATED: This class is provided for backward compatibility only.
 * New code should use common::interfaces::IMonitor implementations directly.
 * This class will be removed in version 3.0.0.
 */
#ifdef LOGGER_USING_COMMON_INTERFACES
class LOGGER_DEPRECATED basic_monitoring : public common::interfaces::IMonitor {
#else
class basic_monitoring : public monitoring_interface {
#endif
private:
#ifdef LOGGER_USING_COMMON_INTERFACES
    mutable common::interfaces::metrics_snapshot data_;
#else
    mutable monitoring_data data_;
#endif

public:
    basic_monitoring() {
        data_.source_id = "logger_system::basic_monitoring";
    }

#ifdef LOGGER_USING_COMMON_INTERFACES
    // IMonitor interface implementation
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

        return std::monostate{};
    }

    common::VoidResult record_metric(
        const std::string& name,
        double value,
        const std::unordered_map<std::string, std::string>& tags) override {

        common::interfaces::metric_value metric(name, value);
        metric.tags = tags;
        data_.metrics.push_back(metric);
        data_.capture_time = std::chrono::system_clock::now();

        return std::monostate{};
    }

    common::Result<common::interfaces::metrics_snapshot> get_metrics() override {
        data_.capture_time = std::chrono::system_clock::now();
        return data_;
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

        return result;
    }

    common::VoidResult reset() override {
        data_ = common::interfaces::metrics_snapshot();
        data_.source_id = "logger_system::basic_monitoring";
        return std::monostate{};
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
        if (std::holds_alternative<common::interfaces::health_check_result>(result)) {
            return std::get<common::interfaces::health_check_result>(result).is_healthy();
        }
        return false;
    }

    LOGGER_DEPRECATED
    health_status get_health_status() const {
        auto result = const_cast<basic_monitoring*>(this)->check_health();
        if (std::holds_alternative<common::interfaces::health_check_result>(result)) {
            return std::get<common::interfaces::health_check_result>(result).status;
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
#else
    // Standalone mode: implement legacy monitoring_interface
    monitoring_data get_monitoring_data() const override {
        auto copy = data_;
        copy.capture_time = std::chrono::system_clock::now();
        return copy;
    }

    bool is_healthy() const override {
        // Simple health check based on metrics
        for (const auto& metric : data_.metrics) {
            if (metric.name == "error_rate" && metric.value > 0.1) {
                return false;
            }
            if (metric.name == "queue_utilization" && metric.value > 0.9) {
                return false;
            }
        }
        return true;
    }

    health_status get_health_status() const override {
        for (const auto& metric : data_.metrics) {
            if (metric.name == "error_rate" && metric.value > 0.1) {
                return health_status::degraded;
            }
            if (metric.name == "queue_utilization" && metric.value > 0.9) {
                return health_status::unhealthy;
            }
        }
        return health_status::healthy;
    }

    void set_metric(const std::string& name, double value) override {
        data_.add_metric(name, value);
        data_.capture_time = std::chrono::system_clock::now();
    }

    double get_metric(const std::string& name) const override {
        for (const auto& metric : data_.metrics) {
            if (metric.name == name) {
                return metric.value;
            }
        }
        return 0.0;
    }

    void reset() override {
        data_ = monitoring_data();
        data_.source_id = "logger_system::basic_monitoring";
    }
#endif // LOGGER_USING_COMMON_INTERFACES
};

} // namespace kcenon::logger::monitoring

// Deprecation notice in compilation output
#pragma message("monitoring_interface_transition.h is deprecated. Use <kcenon/common/interfaces/monitoring_interface.h> instead.")
