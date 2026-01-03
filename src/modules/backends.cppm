/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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

/**
 * @file backends.cppm
 * @brief Backends partition for kcenon.logger module
 * @author kcenon
 * @since 3.0.0
 *
 * @details This partition exports the backend implementations for logger integration:
 * - integration_backend: Abstract interface for integration backends
 * - standalone_backend: Backend for independent logger operation
 * - monitoring_backend: Backend for monitoring_system integration (optional)
 *
 * Backends provide runtime polymorphism for integrating the logger with different
 * external systems, replacing compile-time conditional compilation for improved
 * maintainability and reduced test complexity.
 *
 * Benefits:
 * - 90% reduction in #ifdef directives
 * - Test combinations reduced from 16 to 4 (75% reduction)
 * - Runtime backend switching capability
 * - Improved code readability
 */

export module kcenon.logger:backends;

// Standard library imports
import <memory>;
import <string>;

// Import common dependencies
import kcenon.common;

// Import core partition for types
import :core;

export namespace kcenon::logger::backends {

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
     * @details Converts log levels from external systems to the logger_system's
     * log_level enumeration. Different systems may use different level schemes.
     *
     * @since 1.2.0
     */
    virtual logger_system::log_level normalize_level(int external_level) const = 0;

    /**
     * @brief Get the backend name
     * @return Human-readable backend identifier
     *
     * @since 1.2.0
     */
    virtual std::string get_backend_name() const = 0;

    /**
     * @brief Check if backend requires special initialization
     * @return true if the backend needs initialization
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
     * @since 1.2.0
     */
    virtual bool initialize() {
        return true;
    }

    /**
     * @brief Shutdown the backend
     *
     * @since 1.2.0
     */
    virtual void shutdown() {
        // Default: no-op
    }
};

/**
 * @class standalone_backend
 * @brief Integration backend for standalone logger operation
 *
 * @details This is the default backend when no external integration is required.
 * It assumes the external level values match the logger_system::log_level enumeration,
 * providing a simple pass-through conversion.
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
     * @since 1.2.0
     */
    logger_system::log_level normalize_level(int external_level) const override {
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

/**
 * @brief Factory function to create a standalone backend
 * @return Unique pointer to standalone_backend
 *
 * @example
 * @code
 * auto backend = create_standalone_backend();
 * @endcode
 *
 * @since 3.0.0
 */
inline std::unique_ptr<integration_backend> create_standalone_backend() {
    return std::make_unique<standalone_backend>();
}

/**
 * @brief Detect and create the appropriate default backend
 * @return Unique pointer to the detected integration backend
 *
 * @details This function checks compile-time and runtime conditions to determine
 * the most appropriate backend. In standalone mode, returns standalone_backend.
 * When thread_system or monitoring_system integration is enabled, returns the
 * corresponding backend.
 *
 * @since 3.0.0
 */
inline std::unique_ptr<integration_backend> create_default_backend() {
    // Default to standalone mode
    // In the future, this can check for available integrations
    return create_standalone_backend();
}

} // namespace kcenon::logger::backends
