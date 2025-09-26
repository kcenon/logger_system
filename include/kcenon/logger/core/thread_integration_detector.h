#pragma once

/**
 * @file thread_integration_detector.h
 * @brief Automatically toggles thread_system integration macros when headers are available.
 */

#if !defined(LOGGER_THREAD_INTEGRATION_DETECTED)
#define LOGGER_THREAD_INTEGRATION_DETECTED

#if !defined(USE_THREAD_SYSTEM) && !defined(LOGGER_STANDALONE_MODE)
#  if __has_include(<kcenon/thread/interfaces/logger_interface.h>)
#    define USE_THREAD_SYSTEM 1
#  endif
#endif

#if !defined(USE_THREAD_SYSTEM_INTEGRATION) && defined(USE_THREAD_SYSTEM)
#  if __has_include(<kcenon/thread/interfaces/logger_interface.h>)
#    define USE_THREAD_SYSTEM_INTEGRATION 1
#  endif
#endif

#endif // LOGGER_THREAD_INTEGRATION_DETECTED
