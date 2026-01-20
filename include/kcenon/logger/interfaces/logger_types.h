/**
 * @file logger_types.h
 * @brief Common types and enumerations for logger system
 * @date 2025-09-09
 *
 * BSD 3-Clause License
 * Copyright (c) 2025, kcenon
 * All rights reserved.
 *
 * @note For log_level, use common::interfaces::log_level from
 *       kcenon/common/interfaces/logger_interface.h
 */

#pragma once

#include <chrono>
#include <string>

namespace logger_system {

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

} // namespace logger_system