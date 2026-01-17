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

/**
 * @file thread_integration_detector.h
 * @brief Conditionally enables thread_system integration when available
 * @since 1.3.0 - thread_system is now OPTIONAL (Issue #222)
 * @since 1.5.0 - IExecutor interface support (Issue #253)
 *
 * @details This header detects whether thread_system integration should be
 * enabled based on:
 * 1. CMake option LOGGER_USE_THREAD_SYSTEM (sets USE_THREAD_SYSTEM macro)
 * 2. Header availability via __has_include
 * 3. IExecutor interface availability from common_system
 *
 * When thread_system is not available, logger_system uses its standalone
 * std::jthread implementation for async operations.
 *
 * Integration Modes (Issue #253):
 * - LEGACY: Direct thread_pool dependency (LOGGER_HAS_THREAD_SYSTEM)
 * - EXECUTOR: IExecutor interface from common_system (LOGGER_HAS_IEXECUTOR)
 * - STANDALONE: Built-in async_worker with std::jthread
 */

#if !defined(LOGGER_THREAD_INTEGRATION_DETECTED)
#define LOGGER_THREAD_INTEGRATION_DETECTED

// ============================================================================
// IExecutor Interface Detection (Issue #253)
// ============================================================================
// IExecutor is the preferred abstraction layer from common_system.
// When available, logger_system can accept any IExecutor implementation
// without compile-time dependency on thread_system.

#if !defined(LOGGER_HAS_IEXECUTOR)
#  if __has_include(<kcenon/common/interfaces/executor_interface.h>)
#    define LOGGER_HAS_IEXECUTOR 1
#  elif __has_include(<common/interfaces/executor_interface.h>)
#    define LOGGER_HAS_IEXECUTOR 1
#  else
#    define LOGGER_HAS_IEXECUTOR 0
#  endif
#endif

// ============================================================================
// Legacy thread_system Integration Detection
// ============================================================================
// thread_system integration is now OPTIONAL (Issue #222)
// USE_THREAD_SYSTEM is defined by CMake when LOGGER_USE_THREAD_SYSTEM=ON
// and thread_system is found

// Auto-detect thread_system availability via header presence
// Note: thread_system v3.0+ uses common_system interfaces instead of
// thread-specific logger_interface.h. Detection now uses umbrella headers.
#if defined(USE_THREAD_SYSTEM)
#  if !defined(USE_THREAD_SYSTEM_INTEGRATION)
#    if __has_include(<kcenon/thread/thread_pool.h>)
#      define USE_THREAD_SYSTEM_INTEGRATION 1
#    endif
#  endif
#endif

// ============================================================================
// Compile-time Detection Functions
// ============================================================================

namespace kcenon::logger::detail {

/**
 * @brief Compile-time constant indicating IExecutor interface availability
 * @return true if IExecutor interface is available from common_system
 * @since 1.5.0
 */
constexpr bool has_iexecutor_interface() noexcept {
#if LOGGER_HAS_IEXECUTOR
    return true;
#else
    return false;
#endif
}

/**
 * @brief Compile-time constant indicating any async executor is available
 * @return true if IExecutor is available
 * @since 1.5.0
 * @note has_thread_system_integration() was removed in v3.0.0.
 *       Use has_iexecutor_interface() instead.
 */
constexpr bool has_any_executor_support() noexcept {
    return has_iexecutor_interface();
}

} // namespace kcenon::logger::detail

#endif // LOGGER_THREAD_INTEGRATION_DETECTED
