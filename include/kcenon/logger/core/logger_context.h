#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <memory>
#include <mutex>
#include <set>
#include <atomic>
#include <csignal>

/**
 * @file logger_context.h
 * @brief Dependency injection container for logger system components
 * @author kcenon
 * @since 2.0.0
 *
 * @details This context replaces global singletons with injected dependencies,
 * enabling better testability and supporting multiple independent logger instances.
 *
 * Key Features:
 * - Dependency injection for signal_manager and other components
 * - Support for mock injection in tests
 * - Thread-safe component management
 * - Multiple independent contexts possible (no global state)
 *
 * @example
 * @code
 * // Production usage
 * auto context = std::make_shared<logger_context>();
 * auto logger = std::make_shared<logger>(context);
 *
 * // Test usage with mocks
 * auto mock_signal_mgr = std::make_shared<mock_signal_manager>();
 * auto context = std::make_shared<logger_context>(mock_signal_mgr);
 * auto logger = std::make_shared<logger>(context);
 * @endcode
 */

namespace kcenon::logger {

// Forward declarations
namespace security {
    class signal_manager_interface;
}

namespace core {

/**
 * @class logger_context
 * @brief Dependency injection container for logger components
 *
 * @details This class manages the lifetime and dependencies of logger components.
 * It replaces singleton pattern with dependency injection, enabling:
 * - Testing with mock objects
 * - Multiple independent logger instances
 * - Better separation of concerns
 * - No hidden global state
 *
 * Thread Safety: All methods are thread-safe. Component access is protected
 * by internal synchronization.
 */
class logger_context {
public:
    /**
     * @brief Default constructor - creates default implementations
     *
     * @details Creates a context with default implementations:
     * - signal_manager (for crash-safe logging)
     * - Other components as needed
     */
    logger_context();

    /**
     * @brief Test constructor - allows injection of mock implementations
     * @param signal_mgr Custom signal manager implementation (e.g., mock)
     *
     * @details This constructor is primarily for testing. It allows injection
     * of mock implementations for unit testing.
     *
     * @example
     * @code
     * auto mock = std::make_shared<mock_signal_manager>();
     * auto context = std::make_shared<logger_context>(mock);
     * // Use context in tests
     * @endcode
     */
    explicit logger_context(
        std::shared_ptr<security::signal_manager_interface> signal_mgr
    );

    /**
     * @brief Destructor - ensures clean shutdown
     */
    ~logger_context();

    // Delete copy and move to prevent accidental sharing
    logger_context(const logger_context&) = delete;
    logger_context& operator=(const logger_context&) = delete;
    logger_context(logger_context&&) = delete;
    logger_context& operator=(logger_context&&) = delete;

    /**
     * @brief Get signal manager instance
     * @return Shared pointer to signal manager interface
     *
     * @details Returns the signal manager for this context. This can be either
     * a real implementation or a mock (depending on how context was constructed).
     */
    std::shared_ptr<security::signal_manager_interface> get_signal_manager() const {
        return signal_manager_;
    }

    /**
     * @brief Set signal manager (for dynamic reconfiguration)
     * @param signal_mgr New signal manager implementation
     *
     * @details Allows runtime replacement of signal manager. Primarily useful
     * for advanced scenarios or testing.
     */
    void set_signal_manager(std::shared_ptr<security::signal_manager_interface> signal_mgr) {
        std::lock_guard lock(mutex_);
        signal_manager_ = std::move(signal_mgr);
    }

    /**
     * @brief Check if context is initialized
     * @return true if all required components are initialized
     */
    bool is_initialized() const {
        return signal_manager_ != nullptr;
    }

private:
    /// Signal manager instance
    std::shared_ptr<security::signal_manager_interface> signal_manager_;

    /// Mutex for thread-safe access
    mutable std::mutex mutex_;
};

} // namespace core
} // namespace kcenon::logger
