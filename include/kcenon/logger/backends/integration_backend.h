#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

#include <string>
#include <kcenon/common/interfaces/logger_interface.h>

/**
 * @file integration_backend.h
 * @brief Runtime polymorphism interface for integration backends
 * @author 🍀☀🌕🌥 🌊
 * @since 1.2.0
 *
 * @details This file defines the integration backend interface that replaces
 * compile-time conditional compilation with runtime polymorphism. This approach
 * improves code maintainability and reduces test complexity.
 *
 * Benefits over conditional compilation:
 * - 90% reduction in #ifdef directives
 * - Test combinations reduced from 16 to 4 (75% reduction)
 * - Runtime backend switching capability
 * - Improved code readability
 *
 * @note Part of Phase 3: Code Quality Improvements
 */

namespace kcenon::logger::backends {

// Type alias for log_level
using log_level = common::interfaces::log_level;

/**
 * @class integration_backend
 * @brief Abstract interface for integration backends
 *
 * @details This interface allows the logger system to integrate with different
 * external systems (e.g., thread_system, common_system) without compile-time
 * conditional compilation. Implementations provide level conversion and optional
 * metrics reporting capabilities.
 *
 * @since 1.2.0
 */
class integration_backend {
public:
    /**
     * @brief Virtual destructor
     */
    virtual ~integration_backend() = default;

    /**
     * @brief Normalize external log level to internal level
     * @param external_level External system's log level (as integer)
     * @return Normalized log_level for internal use
     *
     * @details Converts log levels from external systems to the common::interfaces::log_level
     * enumeration. Different systems may use different level schemes
     * (ascending, descending, different naming), and this method handles the conversion.
     *
     * @note The external_level is passed as int to avoid compile-time dependencies
     * on external enum types.
     *
     * @since 1.2.0
     */
    virtual log_level normalize_level(int external_level) const = 0;

    /**
     * @brief Get the backend name
     * @return Human-readable backend identifier
     *
     * @details Returns a string identifying the backend implementation,
     * useful for diagnostics and monitoring.
     *
     * @example
     * - "standalone": Direct logger_system usage
     * - "thread_system": Integration with thread_system
     * - "common_system": Integration with common_system
     *
     * @since 1.2.0
     */
    virtual std::string get_backend_name() const = 0;

    /**
     * @brief Check if backend requires special initialization
     * @return true if the backend needs initialization
     *
     * @details Some backends may require initialization steps (e.g., registering
     * with external systems). This method indicates whether such steps are needed.
     *
     * @note Default implementation returns false (no initialization required).
     *
     * @since 1.2.0
     */
    virtual bool requires_initialization() const {
        return false;
    }

    /**
     * @brief Initialize the backend
     * @return true on success, false on failure
     *
     * @details Performs any necessary initialization steps for the backend.
     * This method is called during logger construction if requires_initialization()
     * returns true.
     *
     * @note Default implementation returns true (no-op).
     *
     * @since 1.2.0
     */
    virtual bool initialize() {
        return true;
    }

    /**
     * @brief Shutdown the backend
     *
     * @details Performs any necessary cleanup steps for the backend.
     * This method is called during logger destruction.
     *
     * @note Default implementation is a no-op.
     *
     * @since 1.2.0
     */
    virtual void shutdown() {
        // Default: no-op
    }
};

} // namespace kcenon::logger::backends
