#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "integration_backend.h"

/**
 * @file standalone_backend.h
 * @brief Standalone integration backend implementation
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 *
 * @details This backend is used when the logger operates independently
 * without external system integration. It provides direct pass-through
 * level conversion since no external level mapping is needed.
 *
 * @note Part of Phase 3: Code Quality Improvements
 */

namespace kcenon::logger::backends {

/**
 * @class standalone_backend
 * @brief Integration backend for standalone logger operation
 *
 * @details This is the default backend when no external integration is required.
 * It assumes the external level values match the logger_system::log_level enumeration,
 * providing a simple pass-through conversion.
 *
 * Usage:
 * @code
 * auto backend = std::make_unique<standalone_backend>();
 * auto logger = std::make_unique<logger>(std::move(backend));
 * @endcode
 *
 * @since 1.2.0
 */
class standalone_backend : public integration_backend {
public:
    /**
     * @brief Default constructor
     */
    standalone_backend() = default;

    /**
     * @brief Default destructor
     */
    ~standalone_backend() override = default;

    /**
     * @brief Normalize external log level (pass-through)
     * @param external_level External log level as integer
     * @return The same level cast to logger_system::log_level
     *
     * @details In standalone mode, the external level is assumed to already
     * be in logger_system::log_level format, so this is a simple cast.
     *
     * @since 1.2.0
     */
    logger_system::log_level normalize_level(int external_level) const override {
        // Direct cast - assumes external level is already logger_system::log_level
        return static_cast<logger_system::log_level>(external_level);
    }

    /**
     * @brief Get backend name
     * @return "standalone"
     *
     * @since 1.2.0
     */
    std::string get_backend_name() const override {
        return "standalone";
    }

    /**
     * @brief Check if initialization is required
     * @return false (no initialization needed)
     *
     * @since 1.2.0
     */
    bool requires_initialization() const override {
        return false;
    }
};

} // namespace kcenon::logger::backends
