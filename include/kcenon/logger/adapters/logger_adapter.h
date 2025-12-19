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

#pragma once

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/thread_integration_detector.h>
#include <memory>
#include <sstream>
#include <string_view>

// For thread_system v3.0+ integration, use common_logger_adapter.h which provides
// adapters for kcenon::common::interfaces::ILogger

namespace kcenon::logger::adapters {

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
    void log(logger_system::log_level level, std::string_view message) {
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
    void set_level(logger_system::log_level level) {
        if (logger_) {
            logger_->set_level(level);
        }
    }

private:
    std::shared_ptr<logger> logger_;
    bool is_running_{false};
};

} // namespace kcenon::logger::adapters