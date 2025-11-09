// Copyright (c) 2025, kcenon
// All rights reserved.

#pragma once

#include <memory>
#include <string>
#include <chrono>
#include <unordered_map>

// common_system integration
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>
#include <kcenon/common/adapters/typed_adapter.h>

#include "../core/logger.h"
#include "../interfaces/logger_interface.h"

namespace kcenon::logger::adapters {

/**
 * @brief Adapter to expose kcenon::logger as common::interfaces::ILogger
 *
 * This adapter allows kcenon::logger's logger to be used through
 * the standard common_system logger interface.
 *
 * Now inherits from typed_adapter for:
 * - Type safety and wrapper depth tracking
 * - Automatic prevention of infinite adapter chains (max depth: 2)
 * - Unwrap support to access underlying logger
 */
class common_system_logger_adapter
    : public ::common::adapters::typed_adapter<::common::interfaces::ILogger, logger> {
    using base_type = ::common::adapters::typed_adapter<::common::interfaces::ILogger, logger>;
public:
    /**
     * @brief Construct adapter with kcenon::logger logger
     * @param lg Shared pointer to kcenon::logger logger
     */
    explicit common_system_logger_adapter(std::shared_ptr<logger> lg)
        : base_type(lg) {}

    ~common_system_logger_adapter() override = default;

    /**
     * @brief Log a message with specified level
     */
    ::common::VoidResult log(
        ::common::interfaces::log_level level,
        const std::string& message) override {
        if (!this->impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            this->impl_->log(logger_level, message);
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
        if (!this->impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            // Create formatted message with location info
            std::string formatted = "[" + file + ":" + std::to_string(line) +
                                   " in " + function + "] " + message;
            this->impl_->log(logger_level, formatted);
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
        if (!this->impl_) {
            return false;
        }
        auto logger_level = convert_level_from_common(level);
        return this->impl_->should_log(logger_level);
    }

    /**
     * @brief Set the minimum log level
     */
    ::common::VoidResult set_level(
        ::common::interfaces::log_level level) override {
        if (!this->impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            this->impl_->set_level(logger_level);
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
        if (!this->impl_) {
            return ::common::interfaces::log_level::info;
        }
        auto logger_level = this->impl_->get_level();
        return convert_level_to_common(logger_level);
    }

    /**
     * @brief Flush any buffered log messages
     */
    ::common::VoidResult flush() override {
        if (!this->impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            this->impl_->flush();
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

private:

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
};

} // namespace kcenon::logger::adapters