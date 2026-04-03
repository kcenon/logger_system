// BSD 3-Clause License
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file logger_adapter.h
 * @brief Standalone logger adapter for use without common_system DI.
 *
 */

#pragma once

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/thread_integration_detector.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <memory>
#include <sstream>
#include <string_view>

// For thread_system v3.0+ integration, use common_logger_adapter.h which provides
// adapters for kcenon::common::interfaces::ILogger

namespace kcenon::logger::adapters {

// Type alias for log_level
using log_level = common::interfaces::log_level;

/**
 * @brief Standalone logger adapter
 * @note For thread_system integration, use common_logger_adapter.h which provides
 *       adapters for kcenon::common::interfaces::ILogger (the unified interface
 *       used by thread_system v3.0+)
 */
class logger_adapter {
public:
    /**
     * @brief Constructor with logger instance
     * @param logger Logger instance to adapt
     */
    explicit logger_adapter(std::shared_ptr<logger> logger_instance)
        : logger_(std::move(logger_instance)) {
    }

    /**
     * @brief Default constructor - creates a default logger
     */
    logger_adapter() : logger_(std::make_shared<logger>()) {
    }

    /**
     * @brief Log a message with the specified level
     * @param level Log level
     * @param message Message to log
     */
    void log(log_level level, std::string_view message) {
        if (!logger_) {
            return;
        }
        logger_->log(level, std::string(message));
    }

    /**
     * @brief Initialize the adapter
     * @return true if initialization succeeded
     */
    bool initialize() {
        if (logger_) {
            is_running_ = true;
            return true;
        }
        return false;
    }

    /**
     * @brief Shutdown the adapter
     */
    void shutdown() {
        if (logger_) {
            logger_->flush();
        }
        is_running_ = false;
    }

    /**
     * @brief Check if the adapter is running
     * @return true if running
     */
    bool is_running() const {
        return is_running_ && logger_ != nullptr;
    }

    /**
     * @brief Get the adapter name
     * @return "LoggerAdapter"
     */
    std::string name() const {
        return "LoggerAdapter";
    }

    /**
     * @brief Get the underlying logger
     * @return Logger instance
     */
    std::shared_ptr<logger> get_logger() const {
        return logger_;
    }

    /**
     * @brief Set minimum log level
     * @param level Minimum level to log
     */
    void set_level(log_level level) {
        if (logger_) {
            logger_->set_level(level);
        }
    }

private:
    std::shared_ptr<logger> logger_;
    bool is_running_{false};
};

} // namespace kcenon::logger::adapters