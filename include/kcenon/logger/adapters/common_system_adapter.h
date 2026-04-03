// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file common_system_adapter.h
 * @brief Adapter exposing kcenon::logger as common::interfaces::ILogger.
 *
 * @see logger_adapter.h For standalone usage
 */

#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <chrono>
#include <unordered_map>

// common_system integration
#include <kcenon/common/interfaces/logger_interface.h>
#include <kcenon/common/patterns/result.h>
#include <kcenon/common/utils/source_location.h>

#include "../core/logger.h"

namespace kcenon::logger::adapters {

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
     * @param lg Shared pointer to kcenon::logger logger
     */
    explicit common_system_logger_adapter(std::shared_ptr<logger> lg)
        : impl_(std::move(lg)) {}

    ~common_system_logger_adapter() override = default;

    /**
     * @brief Log a message with specified level
     */
    ::common::VoidResult log(
        ::common::interfaces::log_level level,
        const std::string& message) override {
        if (!impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            impl_->log(logger_level, message);
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

    /**
     * @brief Log a message with source location information (C++20 style)
     */
    ::common::VoidResult log(
        ::common::interfaces::log_level level,
        std::string_view message,
        const ::kcenon::common::source_location& loc = ::kcenon::common::source_location::current()) override {
        if (!impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            impl_->log(logger_level, message, loc);
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
        if (!impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            // Delegate directly to logger's log_entry overload, which extracts
            // file/line/function from the entry without constructing a
            // std::source_location (not possible in C++20).
            return impl_->log(entry);
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

    /**
     * @brief Check if logging is enabled for the specified level
     */
    bool is_enabled(::common::interfaces::log_level level) const override {
        if (!impl_) {
            return false;
        }
        auto logger_level = convert_level_from_common(level);
        return impl_->is_enabled(logger_level);
    }

    /**
     * @brief Set the minimum log level
     */
    ::common::VoidResult set_level(
        ::common::interfaces::log_level level) override {
        if (!impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            auto logger_level = convert_level_from_common(level);
            impl_->set_level(logger_level);
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
        if (!impl_) {
            return ::common::interfaces::log_level::info;
        }
        auto logger_level = impl_->get_level();
        return convert_level_to_common(logger_level);
    }

    /**
     * @brief Flush any buffered log messages
     */
    ::common::VoidResult flush() override {
        if (!impl_) {
            return ::common::VoidResult(
                ::common::error_info(1, "Logger not initialized", "kcenon::logger"));
        }

        try {
            impl_->flush();
            return ::common::VoidResult(std::monostate{});
        } catch (const std::exception& e) {
            return ::common::VoidResult(
                ::common::error_info(2, e.what(), "kcenon::logger"));
        }
    }

    /**
     * @brief Get access to the underlying logger
     * @return Shared pointer to the logger
     */
    std::shared_ptr<logger> get_impl() const { return impl_; }

private:
    std::shared_ptr<logger> impl_;

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