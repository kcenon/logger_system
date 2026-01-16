/**
 * @file logger_types.h
 * @brief Common types and enumerations for logger system
 * @date 2025-09-09
 *
 * BSD 3-Clause License
 * Copyright (c) 2025, kcenon
 * All rights reserved.
 *
 * @deprecated This header is deprecated in favor of common_system's standard types.
 *             Use kcenon/common/interfaces/logger_interface.h instead.
 *             - Use common::interfaces::log_level instead of logger_system::log_level
 *             - Use common::interfaces::log_entry instead of custom log entry types
 *             This header will be removed in v3.0.0.
 *
 * @note Migration guide:
 *       - Replace #include <kcenon/logger/interfaces/logger_types.h>
 *         with #include <kcenon/common/interfaces/logger_interface.h>
 *       - Replace logger_system::log_level with common::interfaces::log_level
 *       - Note: log_level values are identical between both enums
 */

#pragma once

#include <chrono>
#include <string>

namespace logger_system {

/**
 * @brief Log levels enumeration
 * @deprecated Use common::interfaces::log_level instead.
 *             This enum is maintained for backward compatibility only.
 *             Will be removed in v3.0.0.
 */
enum class [[deprecated("Use common::interfaces::log_level instead. Will be removed in v3.0.0.")]] log_level {
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    warning = 3,  // Alias for warn for compatibility
    error = 4,
    fatal = 5,
    critical = 5, // Alias for fatal for compatibility
    off = 6
};

/**
 * @brief Overflow policy for when buffers are full
 */
enum class overflow_policy {
    block,          // Block until space is available
    drop_oldest,    // Drop oldest messages
    drop_newest,    // Drop newest messages  
    grow           // Dynamically grow buffer
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

/**
 * @brief Logger error codes
 */
enum class logger_error_code {
    success = 0,
    invalid_configuration,
    writer_not_found,
    writer_already_exists,
    write_failed,
    flush_failed,
    buffer_full,
    invalid_level,
    invalid_pattern,
    file_open_failed,
    network_error,
    encryption_error,
    compression_error,
    monitoring_failed,
    component_not_found,
    di_resolution_failed,
    health_check_failed,
    writer_closed,
    unknown_error
};

/**
 * @brief Convert log level to string
 * @deprecated Use common::interfaces::log_level_to_string instead. Will be removed in v3.0.0.
 */
[[deprecated("Use common::interfaces::log_level_to_string instead. Will be removed in v3.0.0.")]]
inline const char* log_level_to_string(log_level level) {
    switch (level) {
        case log_level::trace: return "TRACE";
        case log_level::debug: return "DEBUG";
        case log_level::info:  return "INFO";
        case log_level::warn:  return "WARN";
        case log_level::error: return "ERROR";
        case log_level::fatal: return "FATAL";
        case log_level::off:   return "OFF";
        default: return "UNKNOWN";
    }
}

/**
 * @brief Convert string to log level
 * @deprecated Use common::interfaces::string_to_log_level instead. Will be removed in v3.0.0.
 */
[[deprecated("Use common::interfaces::string_to_log_level instead. Will be removed in v3.0.0.")]]
inline log_level string_to_log_level(const std::string& str) {
    if (str == "TRACE" || str == "trace") return log_level::trace;
    if (str == "DEBUG" || str == "debug") return log_level::debug;
    if (str == "INFO" || str == "info") return log_level::info;
    if (str == "WARN" || str == "warn" || str == "WARNING" || str == "warning") return log_level::warn;
    if (str == "ERROR" || str == "error") return log_level::error;
    if (str == "FATAL" || str == "fatal") return log_level::fatal;
    if (str == "OFF" || str == "off") return log_level::off;
    return log_level::info;  // Default
}

} // namespace logger_system

// Note: Type aliases for kcenon::logger namespace are defined in logger.h
// to support conditional compilation based on integration mode.
// DO NOT add 'using namespace logger_system' here as it causes ambiguous
// reference errors when combined with logger.h's conditional type aliases.