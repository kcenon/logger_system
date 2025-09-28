// Copyright (c) 2025, kcenon
// All rights reserved.

#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <unordered_map>

// Check if common_system is available
#ifdef BUILD_WITH_COMMON_SYSTEM
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>
#endif

#include "../core/logger.h"
#include "../interfaces/logger_interface.h"

namespace kcenon::logger::adapters {

#ifdef BUILD_WITH_COMMON_SYSTEM

/**
 * @brief Adapter to expose kcenon::logger as common::interfaces::ILogger
 *
 * This adapter allows kcenon::logger's logger to be used through
 * the standard common_system logger interface.
 */
class common_system_logger_adapter : public ::common::interfaces::ILogger {
public:
    /**
     * @brief Construct adapter with kcenon::logger logger
     * @param logger Shared pointer to kcenon::logger logger
     */
    explicit common_system_logger_adapter(std::shared_ptr<logger> lg)
        : logger_(lg) {}

    ~common_system_logger_adapter() override = default;

    /**
     * @brief Log a message with specified level
     */
    ::common::VoidResult log(
        ::common::interfaces::log_level level,
        const std::string& message) override {
        if (!logger_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            logger_->log(logger_level, message);
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

    /**
     * @brief Log a message with source location information
     */
    ::common::VoidResult log(
        ::common::interfaces::log_level level,
        const std::string& message,
        const std::string& file,
        int line,
        const std::string& function) override {
        if (!logger_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            // Create formatted message with location info
            std::string formatted = "[" + file + ":" + std::to_string(line) +
                                   " in " + function + "] " + message;
            logger_->log(logger_level, formatted);
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

    /**
     * @brief Log a structured entry
     */
    ::common::VoidResult log(
        const ::common::interfaces::log_entry& entry) override {
        return log(entry.level, entry.message, entry.file, entry.line, entry.function);
    }

    /**
     * @brief Check if logging is enabled for the specified level
     */
    bool is_enabled(::common::interfaces::log_level level) const override {
        if (!logger_) {
            return false;
        }
        auto logger_level = convert_level_from_common(level);
        return logger_->should_log(logger_level);
    }

    /**
     * @brief Set the minimum log level
     */
    ::common::VoidResult set_level(
        ::common::interfaces::log_level level) override {
        if (!logger_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            logger_->set_level(logger_level);
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

    /**
     * @brief Get the current minimum log level
     */
    ::common::interfaces::log_level get_level() const override {
        if (!logger_) {
            return ::common::interfaces::log_level::info;
        }
        auto logger_level = logger_->get_level();
        return convert_level_to_common(logger_level);
    }

    /**
     * @brief Flush any buffered log messages
     */
    ::common::VoidResult flush() override {
        if (!logger_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            logger_->flush();
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

private:
    std::shared_ptr<logger> logger_;

    /**
     * @brief Convert common log level to kcenon::logger log level
     */
    static log_level convert_level_from_common(::common::interfaces::log_level level) {
        switch(level) {
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
            default:
                return log_level::off;
        }
    }

    /**
     * @brief Convert kcenon::logger log level to common log level
     */
    static ::common::interfaces::log_level convert_level_to_common(log_level level) {
        switch(level) {
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
            default:
                return ::common::interfaces::log_level::off;
        }
    }
};

/**
 * @brief Adapter to use common::interfaces::ILogger in kcenon::logger
 *
 * This adapter allows a common_system logger to be used as
 * a kcenon::logger writer.
 */
class logger_from_common_adapter : public logger_interface {
public:
    /**
     * @brief Construct adapter with common logger
     * @param logger Shared pointer to common logger
     */
    explicit logger_from_common_adapter(
        std::shared_ptr<::common::interfaces::ILogger> common_logger)
        : common_logger_(common_logger) {}

    ~logger_from_common_adapter() override = default;

    /**
     * @brief Log a message with specified level
     */
    void log(log_level level, const std::string& message) override {
        if (!common_logger_) {
            return;
        }

        auto common_level = convert_level_to_common(level);
        common_logger_->log(common_level, message);
    }

    /**
     * @brief Log a message with source location information
     */
    void log(log_level level,
             const std::string& message,
             const std::string& file,
             int line,
             const std::string& function) override {
        if (!common_logger_) {
            return;
        }

        auto common_level = convert_level_to_common(level);
        common_logger_->log(common_level, message, file, line, function);
    }

    /**
     * @brief Check if logging is enabled for the specified level
     */
    bool is_enabled(log_level level) const override {
        if (!common_logger_) {
            return false;
        }
        auto common_level = convert_level_to_common(level);
        return common_logger_->is_enabled(common_level);
    }

    /**
     * @brief Flush any buffered log messages
     */
    void flush() override {
        if (common_logger_) {
            common_logger_->flush();
        }
    }

private:
    std::shared_ptr<::common::interfaces::ILogger> common_logger_;

    /**
     * @brief Convert kcenon::logger log level to common log level
     */
    static ::common::interfaces::log_level convert_level_to_common(log_level level) {
        switch(level) {
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
            default:
                return ::common::interfaces::log_level::off;
        }
    }
};

/**
 * @brief Factory for creating common_system compatible loggers
 */
class common_logger_factory {
public:
    /**
     * @brief Create a common_system ILogger from kcenon::logger logger
     */
    static std::shared_ptr<::common::interfaces::ILogger> create_common_logger(
        std::shared_ptr<logger> lg) {
        return std::make_shared<common_system_logger_adapter>(lg);
    }

    /**
     * @brief Create a kcenon::logger logger that wraps common ILogger
     */
    static std::shared_ptr<logger_interface> create_from_common(
        std::shared_ptr<::common::interfaces::ILogger> common_logger) {
        return std::make_shared<logger_from_common_adapter>(common_logger);
    }
};

#endif // BUILD_WITH_COMMON_SYSTEM

} // namespace kcenon::logger::adapters