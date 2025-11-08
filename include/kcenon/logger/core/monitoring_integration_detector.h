#pragma once

/**
 * @file monitoring_integration_detector.h
 * @brief Automatically toggles monitoring_system integration macros when headers are available.
 */

#if !defined(LOGGER_MONITORING_INTEGRATION_DETECTED)
#define LOGGER_MONITORING_INTEGRATION_DETECTED

#if !defined(USE_MONITORING_SYSTEM) && !defined(LOGGER_STANDALONE_MODE)
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
