#pragma once

/**
 * @file logger_export.h
 * @brief DLL export/import macros for logger_system shared library support
 *
 * @details Provides the LOGGER_SYSTEM_API macro for controlling symbol visibility.
 *
 * - Static builds: Define LOGGER_SYSTEM_STATIC (done automatically by CMake
 *   when BUILD_SHARED_LIBS=OFF). All symbols have default visibility.
 * - Shared library builds: LOGGER_SYSTEM_BUILDING is defined when compiling
 *   the library itself (exports symbols). Consumers import symbols.
 *
 * @since 0.2.0
 */

#ifdef LOGGER_SYSTEM_STATIC
#define LOGGER_SYSTEM_API
#elif defined(LOGGER_SYSTEM_BUILDING)
#ifdef _WIN32
#define LOGGER_SYSTEM_API __declspec(dllexport)
#else
#define LOGGER_SYSTEM_API __attribute__((visibility("default")))
#endif
#else
#ifdef _WIN32
#define LOGGER_SYSTEM_API __declspec(dllimport)
#else
#define LOGGER_SYSTEM_API
#endif
#endif
