// BSD 3-Clause License
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#pragma once

/**
 * @file monitoring_integration_detector.h
 * @brief Automatically toggles monitoring_system integration macros when headers are available.
 */

#if !defined(LOGGER_MONITORING_INTEGRATION_DETECTED)
#define LOGGER_MONITORING_INTEGRATION_DETECTED

// Detect monitoring_system integration based on header availability
#if !defined(USE_MONITORING_SYSTEM)
#  if __has_include(<kcenon/monitoring/interfaces/monitoring_interface.h>)
#    define USE_MONITORING_SYSTEM 1
#  endif
#endif

#if !defined(ENABLE_MONITORING_INTEGRATION) && defined(USE_MONITORING_SYSTEM)
#  if __has_include(<kcenon/monitoring/interfaces/monitoring_interface.h>)
#    define ENABLE_MONITORING_INTEGRATION 1
#  endif
#endif

#endif // LOGGER_MONITORING_INTEGRATION_DETECTED
