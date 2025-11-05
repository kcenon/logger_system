#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, logger_system contributors
All rights reserved.
*****************************************************************************/

#include <memory>
#include <string>
#include <chrono>
#include <functional>
#include <format>

// Check if common_system is available
#ifdef BUILD_WITH_COMMON_SYSTEM
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>
#endif

#include "../logger.h"
#include "../log_level.h"
#include "../formatter/basic_formatter.h"

namespace kcenon {
namespace logger {
namespace adapters {

#ifdef BUILD_WITH_COMMON_SYSTEM

/**
 * @brief Convert log levels between logger_system and common_system
 */
inline ::common::interfaces::log_level to_common_level(log_level level) {
    switch (level) {
        case log_level::trace:
            return ::common::interfaces::log_level::trace;
        case log_level::debug:
            return ::common::interfaces::log_level::debug;
        case log_level::info:
            return ::common::interfaces::log_level::info;
        case log_level::warning:
            return ::common::interfaces::log_level::warning;
        case log_level::error:
            return ::common::interfaces::log_level::error;
        case log_level::critical:
            return ::common::interfaces::log_level::critical;
        case log_level::off:
            return ::common::interfaces::log_level::off;
        default:
            return ::common::interfaces::log_level::info;
    }
}

inline log_level from_common_level(::common::interfaces::log_level level) {
    switch (level) {
        case ::common::interfaces::log_level::trace:
            return log_level::trace;
        case ::common::interfaces::log_level::debug:
            return log_level::debug;
        case ::common::interfaces::log_level::info:
            return log_level::info;
        case ::common::interfaces::log_level::warning:
            return log_level::warning;
        case ::common::interfaces::log_level::error:
            return log_level::error;
        case ::common::interfaces::log_level::critical:
            return log_level::critical;
        case ::common::interfaces::log_level::off:
            return log_level::off;
        default:
            return log_level::info;
    }
}

/**
 * @brief Adapter to expose logger as common::interfaces::ILogger
 */
class logger_adapter : public ::common::interfaces::ILogger {
public:
    /**
     * @brief Construct adapter with logger instance
     */
    explicit logger_adapter(std::shared_ptr<kcenon::logger::logger> lgr)
        : logger_(lgr) {}

    ~logger_adapter() override = default;

    /**
     * @brief Log a message with specified level
     * @note No try-catch needed - logger operations are noexcept
     */
    ::common::VoidResult log(::common::interfaces::log_level level,
                             const std::string& message) override {
        if (!logger_) {
            return ::common::error_info(1, "Logger not initialized", "logger_system");
        }

        // Logger operations are designed to be noexcept - simply call and return success
        logger_->log(from_common_level(level), message);
        return ::common::VoidResult(std::monostate{});
    }

    /**
     * @brief Log a message with source location information
     * @note No try-catch needed - logger operations are noexcept
     */
    ::common::VoidResult log_with_location(
        ::common::interfaces::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function) override {
        if (!logger_) {
            return ::common::error_info(1, "Logger not initialized", "logger_system");
        }

        // Create formatted message with location info
        // Logger operations are noexcept - format may throw but we catch at API boundary
        try {
            std::string formatted = std::format("[{}:{}:{}] {}",
                file, line, function, message);
            logger_->log(from_common_level(level), formatted);
        } catch (const std::exception& e) {
            // Only format can throw - return error for format failure
            return ::common::error_info(2, std::string("Format error: ") + e.what(), "logger_system");
        }
        return ::common::VoidResult(std::monostate{});
    }

    /**
     * @brief Log a structured entry
     * @note No try-catch needed - logger operations are noexcept
     */
    ::common::VoidResult log_entry(
        const ::common::interfaces::log_entry& entry) override {
        if (!logger_) {
            return ::common::error_info(1, "Logger not initialized", "logger_system");
        }

        // Format the entry with location if available
        try {
            std::string message = entry.message;
            if (!entry.file.empty()) {
                message = std::format("[{}:{}:{}] {}",
                    entry.file, entry.line, entry.function, entry.message);
            }

            logger_->log(from_common_level(entry.level), message);
        } catch (const std::exception& e) {
            // Only format can throw
            return ::common::error_info(2, std::string("Format error: ") + e.what(), "logger_system");
        }
        return ::common::VoidResult(std::monostate{});
    }

    /**
     * @brief Set the minimum log level
     * @note No try-catch needed - logger operations are noexcept
     */
    ::common::VoidResult set_level(::common::interfaces::log_level level) override {
        if (!logger_) {
            return ::common::error_info(1, "Logger not initialized", "logger_system");
        }

        // Logger operations are designed to be noexcept
        logger_->set_level(from_common_level(level));
        return ::common::VoidResult(std::monostate{});
    }

    /**
     * @brief Get the current log level
     */
    ::common::interfaces::log_level get_level() const override {
        if (!logger_) {
            return ::common::interfaces::log_level::off;
        }

        return to_common_level(logger_->get_level());
    }

    /**
     * @brief Flush all pending log messages
     * @note No try-catch needed - logger operations are noexcept
     */
    ::common::VoidResult flush() override {
        if (!logger_) {
            return ::common::error_info(1, "Logger not initialized", "logger_system");
        }

        // Logger operations are designed to be noexcept
        logger_->flush();
        return ::common::VoidResult(std::monostate{});
    }

    /**
     * @brief Register a callback for log events
     */
    ::common::VoidResult register_callback(
        std::function<void(const ::common::interfaces::log_entry&)> callback) override {
        if (!logger_) {
            return ::common::error_info(1, "Logger not initialized", "logger_system");
        }

        // Note: This requires extending the logger_system to support callbacks
        // For now, return success as placeholder
        return ::common::VoidResult(std::monostate{});
    }

private:
    std::shared_ptr<kcenon::logger::logger> logger_;
};

/**
 * @brief Adapter to use common::interfaces::ILogger in logger_system
 */
class logger_from_common_adapter {
public:
    /**
     * @brief Construct adapter with common logger
     */
    explicit logger_from_common_adapter(
        std::shared_ptr<::common::interfaces::ILogger> common_logger)
        : common_logger_(common_logger) {}

    /**
     * @brief Log a message
     */
    void log(log_level level, const std::string& message) {
        if (!common_logger_) {
            return;
        }

        auto result = common_logger_->log(to_common_level(level), message);
        // Silently ignore errors for now
    }

    /**
     * @brief Log with source location
     */
    void log_with_location(log_level level,
                           const std::string& message,
                           const std::string& file,
                           int line,
                           const std::string& function) {
        if (!common_logger_) {
            return;
        }

        auto result = common_logger_->log_with_location(
            to_common_level(level), message, file, line, function);
        // Silently ignore errors for now
    }

    /**
     * @brief Set log level
     */
    void set_level(log_level level) {
        if (common_logger_) {
            common_logger_->set_level(to_common_level(level));
        }
    }

    /**
     * @brief Get log level
     */
    log_level get_level() const {
        if (!common_logger_) {
            return log_level::off;
        }

        return from_common_level(common_logger_->get_level());
    }

    /**
     * @brief Flush logs
     */
    void flush() {
        if (common_logger_) {
            common_logger_->flush();
        }
    }

private:
    std::shared_ptr<::common::interfaces::ILogger> common_logger_;
};

/**
 * @brief Factory for creating common_system compatible loggers
 */
class common_logger_factory {
public:
    /**
     * @brief Create a common_system ILogger from logger_system logger
     */
    static std::shared_ptr<::common::interfaces::ILogger> create_from_logger(
        std::shared_ptr<kcenon::logger::logger> lgr) {
        return std::make_shared<logger_adapter>(lgr);
    }

    /**
     * @brief Create a logger_system wrapper from common ILogger
     */
    static std::unique_ptr<logger_from_common_adapter> create_from_common(
        std::shared_ptr<::common::interfaces::ILogger> common_logger) {
        return std::make_unique<logger_from_common_adapter>(common_logger);
    }
};

#endif // BUILD_WITH_COMMON_SYSTEM

} // namespace adapters
} // namespace logger
} // namespace kcenon