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

#include <kcenon/thread/interfaces/shared_interfaces.h>
#include <kcenon/logger/core/logger.h>
#include <memory>
#include <sstream>

namespace kcenon::logger::adapters {

/**
 * @brief Adapter to make logger compatible with ILogger interface
 */
class logger_adapter : public shared::ILogger, public shared::IService {
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

    // ILogger interface
    void log(shared::LogLevel level, std::string_view message) override {
        if (!logger_) {
            return;
        }

        // Convert shared::LogLevel to logger::log_level
        logger::log_level logger_level;
        switch (level) {
            case shared::LogLevel::Trace:
                logger_level = logger::log_level::trace;
                break;
            case shared::LogLevel::Debug:
                logger_level = logger::log_level::debug;
                break;
            case shared::LogLevel::Info:
                logger_level = logger::log_level::info;
                break;
            case shared::LogLevel::Warning:
                logger_level = logger::log_level::warning;
                break;
            case shared::LogLevel::Error:
                logger_level = logger::log_level::error;
                break;
            case shared::LogLevel::Critical:
                logger_level = logger::log_level::critical;
                break;
            default:
                logger_level = logger::log_level::info;
        }

        logger_->log(logger_level, std::string(message));
    }

    // IService interface
    bool initialize() override {
        if (logger_) {
            is_running_ = true;
            return true;
        }
        return false;
    }

    void shutdown() override {
        if (logger_) {
            logger_->flush();
        }
        is_running_ = false;
    }

    bool is_running() const override {
        return is_running_ && logger_ != nullptr;
    }

    std::string name() const override {
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
    void set_level(shared::LogLevel level) {
        if (logger_) {
            // Convert and set level
            logger::log_level logger_level;
            switch (level) {
                case shared::LogLevel::Trace:
                    logger_level = logger::log_level::trace;
                    break;
                case shared::LogLevel::Debug:
                    logger_level = logger::log_level::debug;
                    break;
                case shared::LogLevel::Info:
                    logger_level = logger::log_level::info;
                    break;
                case shared::LogLevel::Warning:
                    logger_level = logger::log_level::warning;
                    break;
                case shared::LogLevel::Error:
                    logger_level = logger::log_level::error;
                    break;
                case shared::LogLevel::Critical:
                    logger_level = logger::log_level::critical;
                    break;
                default:
                    logger_level = logger::log_level::info;
            }
            logger_->set_level(logger_level);
        }
    }

private:
    std::shared_ptr<logger> logger_;
    bool is_running_{false};
};

} // namespace kcenon::logger::adapters