/**
 * @file monitoring_interface.h
 * @brief Interface for logger monitoring and health checks
 *
 * DEPRECATED: This file is deprecated and will be removed in version 3.0.0.
 * Please use <kcenon/common/interfaces/monitoring_interface.h> instead.
 *
 * Migration guide:
 * - Replace kcenon::logger::monitoring::monitoring_interface with common::interfaces::IMonitor
 * - Replace kcenon::logger::monitoring::health_status with common::interfaces::health_status
 * - Replace kcenon::logger::monitoring::monitoring_data with common::interfaces::metrics_snapshot
 * - For backward compatibility, include "monitoring_interface_transition.h" temporarily
 */

#pragma once

// Deprecation warning
#if defined(__GNUC__) || defined(__clang__)
#warning "logger/core/monitoring/monitoring_interface.h is deprecated. Use <kcenon/common/interfaces/monitoring_interface.h> instead."
#elif defined(_MSC_VER)
#pragma message("Warning: logger/core/monitoring/monitoring_interface.h is deprecated. Use <kcenon/common/interfaces/monitoring_interface.h> instead.")
#endif

// Forward to transition header for compatibility
#include "monitoring_interface_transition.h"

// NOTE: All definitions have been moved to monitoring_interface_transition.h
// This file is kept for backward compatibility and will be removed in v3.0.0