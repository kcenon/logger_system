// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file logger_types.h
 * @brief Common types and enumerations for logger system
 * @date 2025-09-09
 * @note For log_level, use common::interfaces::log_level from
 *       kcenon/common/interfaces/logger_interface.h
 *
 * @note Canonical namespace is kcenon::logger. The legacy
 *       logger_system:: aliases are retained as [[deprecated]]
 *       forwarding aliases for one minor release; consumer code
 *       should migrate to the kcenon::logger:: spellings.
 */

#pragma once

#include <chrono>
#include <string>

namespace kcenon::logger {

/**
 * @brief Overflow policy for when buffers are full
 */
enum class overflow_policy {
    block,          // Block until space is available
    drop_oldest,    // Drop oldest messages
    drop_newest,    // Drop newest messages
    grow            // Dynamically grow buffer
};

/**
 * @brief Health status enumeration
 */
enum class health_status {
    healthy,        // System is operating normally
    degraded,       // System has some issues but still operational
    unhealthy,      // System has serious issues
    critical        // System is in critical state
};

} // namespace kcenon::logger

namespace logger_system {

/**
 * @brief Deprecated forwarding alias for kcenon::logger::overflow_policy.
 * @deprecated Use kcenon::logger::overflow_policy. This alias will be removed in the next minor release.
 */
using overflow_policy [[deprecated("Use kcenon::logger::overflow_policy; alias removed in next minor release")]]
    = kcenon::logger::overflow_policy;

/**
 * @brief Deprecated forwarding alias for kcenon::logger::health_status.
 * @deprecated Use kcenon::logger::health_status. This alias will be removed in the next minor release.
 */
using health_status [[deprecated("Use kcenon::logger::health_status; alias removed in next minor release")]]
    = kcenon::logger::health_status;

} // namespace logger_system
