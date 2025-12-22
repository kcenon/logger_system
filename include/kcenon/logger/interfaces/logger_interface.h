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

/**
 * @file logger_interface.h
 * @brief Logger interface for standalone mode
 *
 * @deprecated This header is deprecated in favor of common_system's standard interface.
 *             Use kcenon/common/interfaces/logger_interface.h instead.
 *             - Use common::interfaces::ILogger instead of logger_system::logger_interface
 *             - Use common::interfaces::log_level instead of logger_system::log_level
 *             This header will be removed in v3.0.0.
 *
 * @note Migration guide:
 *       - Replace logger_interface with common::interfaces::ILogger
 *       - Replace log(log_level, const std::string&) with
 *         log(common::interfaces::log_level, const std::string&)
 *       - Use the new source_location-based log() method where possible
 *
 * @see kcenon/common/interfaces/logger_interface.h for the standard interface
 */

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
 *
 * @deprecated Use common::interfaces::ILogger instead.
 *             This interface is maintained for backward compatibility only.
 *             Will be removed in v3.0.0.
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

} // namespace logger_system

// =============================================================================
// Convenience macros for logging (standalone mode)
// =============================================================================
//
// @deprecated These macros are deprecated in favor of common_system's LOG_* macros.
//             Use #include <kcenon/common/logging/log_macros.h> and LOG_INFO("message") instead.
//             These macros will be removed in v3.0.0.
//
// Migration: Simply replace THREAD_LOG_* with LOG_* and include the common_system header:
//   Before: THREAD_LOG_INFO("message");
//   After:  #include <kcenon/common/logging/log_macros.h>
//           LOG_INFO("message");

// Include common_system's log macros for the redirected implementation
#if __has_include(<kcenon/common/logging/log_macros.h>)
#include <kcenon/common/logging/log_macros.h>
#define LOGGER_SYSTEM_HAS_COMMON_LOG_MACROS 1
#else
#define LOGGER_SYSTEM_HAS_COMMON_LOG_MACROS 0
#endif

#if LOGGER_SYSTEM_HAS_COMMON_LOG_MACROS
// When common_system is available, THREAD_LOG_* macros are already defined
// in log_macros.h as deprecated aliases to LOG_* macros.
// No need to redefine them here.

#else
// Fallback for standalone mode when common_system is not available

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

#endif // LOGGER_SYSTEM_HAS_COMMON_LOG_MACROS
