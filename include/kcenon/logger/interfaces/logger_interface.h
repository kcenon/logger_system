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

// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#pragma once

// Logger interface for standalone mode
// Note: Integration with thread_system is handled via backend pattern
#include <kcenon/logger/core/thread_integration_detector.h>

#include <memory>
#include <mutex>
#include <string>
#include "logger_types.h"

// Include Result pattern for exception-free error handling
#include <kcenon/common/patterns/result.h>

namespace logger_system {

// Log level is defined in logger_types.h

// Type alias for Result pattern
using VoidResult = kcenon::common::VoidResult;

/**
 * @brief Logger interface for standalone mode
 *
 * This interface provides a logging contract for standalone logger implementations.
 * All methods are noexcept and return VoidResult for exception-free error handling.
 */
class logger_interface {
public:
  virtual ~logger_interface() = default;

  /**
   * @brief Log a message with specified level
   * @param level Log level
   * @param message Log message
   * @return VoidResult indicating success or failure
   * @note This method is noexcept - all errors are returned via VoidResult
   */
  virtual VoidResult log(log_level level, const std::string& message) noexcept = 0;

  /**
   * @brief Log a message with source location information
   * @param level Log level
   * @param message Log message
   * @param file Source file name
   * @param line Source line number
   * @param function Function name
   * @return VoidResult indicating success or failure
   * @note This method is noexcept - all errors are returned via VoidResult
   */
  virtual VoidResult log(log_level level, const std::string& message,
                         const std::string& file, int line,
                         const std::string& function) noexcept = 0;

  /**
   * @brief Check if logging is enabled for the specified level
   * @param level Log level to check
   * @return true if logging is enabled for this level
   */
  virtual bool is_enabled(log_level level) const noexcept = 0;

  /**
   * @brief Flush any buffered log messages
   * @return VoidResult indicating success or failure
   * @note This method is noexcept - all errors are returned via VoidResult
   */
  virtual VoidResult flush() noexcept = 0;
};

/**
 * @brief Global logger registry (standalone mode)
 *
 * Thread-safe global registry to access a process-wide logger instance
 * without depending on thread_system. Inline definitions avoid the need
 * for a separate .cpp translation unit in standalone builds.
 */
class logger_registry {
public:
  static void set_logger(std::shared_ptr<logger_interface> logger) {
    std::lock_guard<std::mutex> lock(mutex_);
    logger_ = std::move(logger);
  }

  static std::shared_ptr<logger_interface> get_logger() {
    std::lock_guard<std::mutex> lock(mutex_);
    return logger_;
  }

  static void clear_logger() {
    std::lock_guard<std::mutex> lock(mutex_);
    logger_.reset();
  }

private:
  inline static std::shared_ptr<logger_interface> logger_{};
  inline static std::mutex mutex_{};
};

// Convenience macros for logging (standalone mode)
#define THREAD_LOG_IF_ENABLED(level, message)                                  \
  do {                                                                         \
    if (auto logger = ::logger_system::logger_registry::get_logger()) {        \
      if (logger->is_enabled(level)) {                                         \
        logger->log(level, message, __FILE__, __LINE__, __FUNCTION__);         \
      }                                                                         \
    }                                                                           \
  } while (0)

#define THREAD_LOG_CRITICAL(message)                                           \
  THREAD_LOG_IF_ENABLED(::logger_system::log_level::critical, message)
#define THREAD_LOG_ERROR(message)                                              \
  THREAD_LOG_IF_ENABLED(::logger_system::log_level::error, message)
#define THREAD_LOG_WARNING(message)                                            \
  THREAD_LOG_IF_ENABLED(::logger_system::log_level::warning, message)
#define THREAD_LOG_INFO(message)                                               \
  THREAD_LOG_IF_ENABLED(::logger_system::log_level::info, message)
#define THREAD_LOG_DEBUG(message)                                              \
  THREAD_LOG_IF_ENABLED(::logger_system::log_level::debug, message)
#define THREAD_LOG_TRACE(message)                                              \
  THREAD_LOG_IF_ENABLED(::logger_system::log_level::trace, message)

} // namespace logger_system
