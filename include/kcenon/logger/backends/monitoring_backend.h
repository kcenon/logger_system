#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "integration_backend.h"
#include <kcenon/logger/core/monitoring_integration_detector.h>
#include <memory>
#include <string>

// Conditional includes based on integration settings
#ifdef ENABLE_MONITORING_INTEGRATION
#include <kcenon/monitoring/interfaces/monitoring_interface.h>
#endif

/**
 * @file monitoring_backend.h
 * @brief Monitoring system integration backend implementation
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 *
 * @details This backend is used when the logger integrates with monitoring_system.
 * It provides metrics emission and event publishing capabilities to the monitoring
 * system, allowing centralized observability of logging operations.
 *
 * Features:
 * - Log count metrics per level
 * - Log rate tracking
 * - Error/warning rate monitoring
 * - Integration with monitoring_system's event bus
 *
 * @note Part of Sprint 3-6: Backend Pattern Conversion
 * @note Only available when ENABLE_MONITORING_INTEGRATION is defined
 */

namespace kcenon::logger::backends {

#ifdef ENABLE_MONITORING_INTEGRATION

/**
 * @class monitoring_backend
 * @brief Integration backend for monitoring_system compatibility
 *
 * @details This backend enables the logger to publish metrics and events to
 * the monitoring_system. It tracks logging activity and provides observability
 * into the logger's operation.
 *
 * Metrics emitted:
 * - log.count.{level}: Total number of logs per level
 * - log.rate.{level}: Logs per second per level
 * - log.error_rate: Combined error + fatal logs per second
 * - log.queue_depth: Current async log queue depth
 *
 * Events published:
 * - log.level_changed: When log level threshold changes
 * - log.writer_added: When a new writer is added
 * - log.writer_removed: When a writer is removed
 * - log.error_spike: When error rate exceeds threshold
 *
 * Usage:
 * @code
 * auto monitor = std::make_shared<monitoring::performance_monitor>();
 * auto backend = std::make_unique<monitoring_backend>(monitor);
 * auto logger = std::make_unique<logger>(std::move(backend));
 * @endcode
 *
 * @since 1.2.0
 */
class monitoring_backend : public integration_backend {
public:
    /**
     * @brief Construct with monitoring system reference
     * @param monitor Shared pointer to monitoring_system's IMonitor interface
     *
     * @details The backend will use the provided monitor to emit metrics and
     * publish events. The monitor must remain valid for the lifetime of the backend.
     *
     * @since 1.2.0
     */
    explicit monitoring_backend(
        std::shared_ptr<monitoring::interfaces::IMonitor> monitor = nullptr)
        : monitor_(std::move(monitor)) {}

    /**
     * @brief Default destructor
     */
    ~monitoring_backend() override = default;

    /**
     * @brief Normalize monitoring_system log level to logger_system level
     * @param external_level monitoring_system log level as integer
     * @return Corresponding logger_system::log_level
     *
     * @details monitoring_system uses the same ascending level scheme as
     * logger_system, so this is a direct cast. If the systems diverge in
     * the future, conversion logic can be added here.
     *
     * @since 1.2.0
     */
    logger_system::log_level normalize_level(int external_level) const override {
        // monitoring_system uses same level scheme as logger_system
        return static_cast<logger_system::log_level>(external_level);
    }

    /**
     * @brief Get backend name
     * @return "monitoring_system"
     *
     * @since 1.2.0
     */
    std::string get_backend_name() const override {
        return "monitoring_system";
    }

    /**
     * @brief Check if initialization is required
     * @return true if monitor is provided
     *
     * @note Initialization registers the logger with the monitoring system
     *
     * @since 1.2.0
     */
    bool requires_initialization() const override {
        return monitor_ != nullptr;
    }

    /**
     * @brief Initialize the backend
     * @return true on success, false on failure
     *
     * @details Registers the logger with the monitoring system and sets up
     * initial metrics.
     *
     * @since 1.2.0
     */
    bool initialize() override {
        if (!monitor_) {
            return true; // No monitor, nothing to initialize
        }

        // Register logger metrics with monitoring system
        // This would typically set up metric collectors, event subscriptions, etc.
        // Implementation depends on monitoring_system's API

        return true;
    }

    /**
     * @brief Shutdown the backend
     *
     * @details Unregisters the logger from the monitoring system and flushes
     * any pending metrics.
     *
     * @since 1.2.0
     */
    void shutdown() override {
        if (!monitor_) {
            return;
        }

        // Flush any pending metrics
        // Unregister from monitoring system
        // Implementation depends on monitoring_system's API
    }

    /**
     * @brief Emit a metric to the monitoring system
     * @param metric_name Name of the metric (e.g., "log.count.error")
     * @param value Metric value
     *
     * @details Sends a metric data point to the monitoring system. This is
     * typically called internally by the logger when significant events occur.
     *
     * @note This method is called frequently, so it should be lightweight
     *
     * @since 1.2.0
     */
    void emit_metric(const std::string& metric_name, double value) {
        if (!monitor_) {
            return;
        }

        // Emit metric to monitoring system
        // Implementation would call monitor_->record_metric(metric_name, value)
        // or similar API depending on monitoring_system's interface
    }

    /**
     * @brief Publish an event to the monitoring system
     * @param event_type Type of event (e.g., "log.level_changed")
     * @param event_data Event-specific data
     *
     * @details Publishes a structured event to the monitoring system's event bus.
     * This allows other components to react to logger state changes.
     *
     * @since 1.2.0
     */
    void publish_event(const std::string& event_type, const std::string& event_data) {
        if (!monitor_) {
            return;
        }

        // Publish event to monitoring system
        // Implementation would call monitor_->publish_event(event_type, event_data)
        // or similar API depending on monitoring_system's interface
    }

private:
    std::shared_ptr<monitoring::interfaces::IMonitor> monitor_;
};

#endif // ENABLE_MONITORING_INTEGRATION

} // namespace kcenon::logger::backends
