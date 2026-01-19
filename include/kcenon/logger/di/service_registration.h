// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file service_registration.h
 * @brief Service container registration for logger_system services.
 *
 * This header provides functions to register logger_system services
 * with the unified service container from common_system.
 *
 * @see TICKET-103 for integration requirements.
 */

#pragma once

#include <memory>

#ifdef BUILD_WITH_COMMON_SYSTEM

#include <kcenon/common/di/service_container.h>
#include <kcenon/common/interfaces/logger_interface.h>

#include "../core/logger_builder.h"
#include "../core/logger.h"

namespace kcenon::logger::di {

// Type alias for log_level
using log_level = common::interfaces::log_level;

namespace detail {

/**
 * @brief ILogger adapter that correctly implements common::interfaces::ILogger
 */
class ilogger_adapter : public common::interfaces::ILogger {
public:
    explicit ilogger_adapter(std::shared_ptr<logger> lgr)
        : logger_(std::move(lgr))
        , level_(common::interfaces::log_level::info) {}

    ~ilogger_adapter() override = default;

    common::VoidResult log(log_level level,
                           const std::string& message) override {
        if (!logger_) {
            return common::make_error<std::monostate>(
                1, "Logger not initialized", "logger_system");
        }
        logger_->log(level, message);
        return common::VoidResult::ok({});
    }

    common::VoidResult log(log_level level,
                           const std::string& message,
                           const std::string& file,
                           int line,
                           const std::string& function) override {
        if (!logger_) {
            return common::make_error<std::monostate>(
                1, "Logger not initialized", "logger_system");
        }
        // Format message with location info
        std::string formatted = "[" + file + ":" + std::to_string(line) + ":" + function + "] " + message;
        logger_->log(level, formatted);
        return common::VoidResult::ok({});
    }

    common::VoidResult log(const common::interfaces::log_entry& entry) override {
        if (!logger_) {
            return common::make_error<std::monostate>(
                1, "Logger not initialized", "logger_system");
        }
        if (!entry.file.empty()) {
            std::string formatted = "[" + entry.file + ":" + std::to_string(entry.line) +
                                    ":" + entry.function + "] " + entry.message;
            logger_->log(entry.level, formatted);
        } else {
            logger_->log(entry.level, entry.message);
        }
        return common::VoidResult::ok({});
    }

    bool is_enabled(log_level level) const override {
        return level >= level_;
    }

    common::VoidResult set_level(log_level level) override {
        level_ = level;
        if (logger_) {
            logger_->set_level(level);
        }
        return common::VoidResult::ok({});
    }

    log_level get_level() const override {
        return level_;
    }

    common::VoidResult flush() override {
        if (logger_) {
            logger_->flush();
        }
        return common::VoidResult::ok({});
    }

private:
    std::shared_ptr<logger> logger_;
    log_level level_;
};

} // namespace detail

/**
 * @brief Default configuration for logger service registration
 */
struct logger_registration_config {
    /// Configuration template to use ("default", "production", "debug", etc.)
    std::string config_template = "default";

    /// Minimum log level
    log_level min_level = log_level::info;

    /// Enable async mode
    bool async = true;

    /// Enable metrics collection
    bool metrics = false;

    /// Service lifetime (typically singleton for loggers)
    common::di::service_lifetime lifetime = common::di::service_lifetime::singleton;
};

/**
 * @brief Register logger services with the service container.
 *
 * Registers ILogger implementation using logger_system's logger with
 * a sensible default configuration. The logger is registered as a singleton
 * by default.
 *
 * @param container The service container to register with
 * @param config Optional configuration for the logger
 * @return VoidResult indicating success or registration error
 *
 * @code
 * auto& container = common::di::service_container::global();
 *
 * // Register with default configuration
 * auto result = register_logger_services(container);
 *
 * // Or with custom configuration
 * logger_registration_config config;
 * config.config_template = "production";
 * config.min_level = log_level::warning;
 * auto result = register_logger_services(container, config);
 *
 * // Then resolve logger anywhere in the application
 * auto logger = container.resolve<common::interfaces::ILogger>().value();
 * logger->log(log_level::info, "Application started");
 * @endcode
 */
inline common::VoidResult register_logger_services(
    common::di::IServiceContainer& container,
    const logger_registration_config& config = {}) {

    // Check if already registered
    if (container.is_registered<common::interfaces::ILogger>()) {
        return common::make_error<std::monostate>(
            common::di::di_error_codes::already_registered,
            "ILogger is already registered",
            "logger_system::di"
        );
    }

    // Register logger factory
    return container.register_factory<common::interfaces::ILogger>(
        [config](common::di::IServiceContainer&) -> std::shared_ptr<common::interfaces::ILogger> {
            // Build logger with configuration
            auto builder = logger_builder()
                .use_template(config.config_template)
                .with_min_level(config.min_level)
                .with_async(config.async)
                .with_metrics(config.metrics);

            auto result = builder.build();
            if (!result) {
                // Return a null logger on failure - the caller should check
                // This should rarely happen with default configuration
                return nullptr;
            }

            // Create adapter that implements ILogger
            return std::make_shared<detail::ilogger_adapter>(
                std::shared_ptr<logger>(result.value().release())
            );
        },
        config.lifetime
    );
}

/**
 * @brief Register a pre-configured logger instance.
 *
 * Use this when you have already created a logger instance and want
 * to register it with the container.
 *
 * @param container The service container to register with
 * @param lgr The logger instance to register
 * @return VoidResult indicating success or registration error
 *
 * @code
 * // Create logger manually
 * auto logger = logger_builder()
 *     .use_template("high_performance")
 *     .with_file_output("/var/log/app", "myapp")
 *     .build()
 *     .value();
 *
 * // Register the instance
 * register_logger_instance(container, std::move(logger));
 * @endcode
 */
inline common::VoidResult register_logger_instance(
    common::di::IServiceContainer& container,
    std::unique_ptr<logger> lgr) {

    if (!lgr) {
        return common::make_error<std::monostate>(
            common::error_codes::INVALID_ARGUMENT,
            "Cannot register null logger instance",
            "logger_system::di"
        );
    }

    auto adapter = std::make_shared<detail::ilogger_adapter>(
        std::shared_ptr<logger>(lgr.release())
    );

    return container.register_instance<common::interfaces::ILogger>(adapter);
}

/**
 * @brief Unregister logger services from the container.
 *
 * @param container The service container to unregister from
 * @return VoidResult indicating success or error
 */
inline common::VoidResult unregister_logger_services(
    common::di::IServiceContainer& container) {

    return container.unregister<common::interfaces::ILogger>();
}

} // namespace kcenon::logger::di

#endif // BUILD_WITH_COMMON_SYSTEM
