#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "integration_backend.h"
#include <kcenon/logger/core/thread_integration_detector.h>

/**
 * @file thread_system_backend.h
 * @brief Thread system integration backend implementation
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 *
 * @details This backend is used when the logger integrates with thread_system.
 * It provides log level conversion from thread_system's descending level scheme
 * (critical=0) to logger_system's ascending scheme (trace=0).
 *
 * @note Part of Phase 3: Code Quality Improvements
 * @note Only available when USE_THREAD_SYSTEM_INTEGRATION is defined
 */

namespace kcenon::logger::backends {

#ifdef USE_THREAD_SYSTEM_INTEGRATION

#include <kcenon/thread/interfaces/logger_interface.h>

/**
 * @class thread_system_backend
 * @brief Integration backend for thread_system compatibility
 *
 * @details This backend converts log levels from thread_system's enumeration
 * to logger_system's enumeration. The thread_system uses a descending severity
 * scheme (critical=0, trace=5), while logger_system uses an ascending scheme
 * (trace=0, fatal=5).
 *
 * Level mapping:
 * - thread::log_level::critical (0) → logger_system::log_level::fatal (5)
 * - thread::log_level::error (1) → logger_system::log_level::error (4)
 * - thread::log_level::warning (2) → logger_system::log_level::warning (3)
 * - thread::log_level::info (3) → logger_system::log_level::info (2)
 * - thread::log_level::debug (4) → logger_system::log_level::debug (1)
 * - thread::log_level::trace (5) → logger_system::log_level::trace (0)
 *
 * Usage:
 * @code
 * auto backend = std::make_unique<thread_system_backend>();
 * auto logger = std::make_unique<logger>(std::move(backend));
 * @endcode
 *
 * @since 1.2.0
 */
class thread_system_backend : public integration_backend {
public:
    /**
     * @brief Default constructor
     */
    thread_system_backend() = default;

    /**
     * @brief Default destructor
     */
    ~thread_system_backend() override = default;

    /**
     * @brief Normalize thread_system log level to logger_system level
     * @param external_level thread_system log level as integer
     * @return Corresponding logger_system::log_level
     *
     * @details Converts from thread_system's descending level scheme to
     * logger_system's ascending scheme. Unknown levels default to info.
     *
     * @since 1.2.0
     */
    logger_system::log_level normalize_level(int external_level) const override {
        // thread_system uses descending levels (critical=0, trace=5)
        // logger_system uses ascending levels (trace=0, fatal=5)
        auto level = static_cast<kcenon::thread::log_level>(external_level);

        switch (level) {
            case kcenon::thread::log_level::critical:
                return logger_system::log_level::fatal;
            case kcenon::thread::log_level::error:
                return logger_system::log_level::error;
            case kcenon::thread::log_level::warning:
                return logger_system::log_level::warning;
            case kcenon::thread::log_level::info:
                return logger_system::log_level::info;
            case kcenon::thread::log_level::debug:
                return logger_system::log_level::debug;
            case kcenon::thread::log_level::trace:
                return logger_system::log_level::trace;
            default:
                // Unknown level - default to info
                return logger_system::log_level::info;
        }
    }

    /**
     * @brief Get backend name
     * @return "thread_system"
     *
     * @since 1.2.0
     */
    std::string get_backend_name() const override {
        return "thread_system";
    }

    /**
     * @brief Check if initialization is required
     * @return false (no special initialization needed)
     *
     * @note Future versions may require registration with thread_system
     *
     * @since 1.2.0
     */
    bool requires_initialization() const override {
        return false;
    }
};

#endif // USE_THREAD_SYSTEM_INTEGRATION

} // namespace kcenon::logger::backends
