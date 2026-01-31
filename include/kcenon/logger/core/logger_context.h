#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <memory>
#include <kcenon/logger/security/signal_manager_interface.h>
#include <kcenon/logger/core/signal_manager_context.h>
#include <kcenon/logger/core/logger_registry.h>

/**
 * @file logger_context.h
 * @brief Facade for logger system components following SRP
 * @author kcenon
 * @since 2.0.0
 *
 * @details This context acts as a facade coordinating specialized components,
 * following the Single Responsibility Principle. Each component handles one concern:
 * - signal_manager_context: Signal manager lifecycle
 * - logger_registry: Logger registration tracking
 *
 * This design enables better testability and supports multiple independent logger instances.
 *
 * Key Features:
 * - Dependency injection for signal_manager and other components
 * - Support for mock injection in tests
 * - Separation of concerns (SRP)
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
 *
 * // Access components directly
 * auto& signal_ctx = context->signal_context();
 * auto& registry = context->registry();
 * @endcode
 */

namespace kcenon::logger {

// Forward declarations
namespace security {
    class critical_logger_interface;
}

namespace core {

/**
 * @class logger_context
 * @brief Facade coordinating logger system components
 *
 * @details This class acts as a facade pattern, coordinating specialized components
 * following the Single Responsibility Principle:
 * - signal_manager_context: Signal manager lifecycle management
 * - logger_registry: Logger registration tracking
 *
 * Benefits:
 * - Clear separation of concerns
 * - Easier testing of individual components
 * - Reduced mutex contention (each component has its own lock)
 * - Maintains backward-compatible API
 *
 * Thread Safety: All methods are thread-safe. Each component provides its own
 * thread safety guarantees.
 */
class logger_context {
public:
    /**
     * @brief Default constructor - creates default implementations
     *
     * @details Creates a context with default implementations:
     * - signal_manager_context with default signal_manager
     * - Empty logger_registry
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

    // ============================================================
    // Component Access (SRP-compliant design)
    // ============================================================

    /**
     * @brief Access signal manager context component
     * @return Reference to signal_manager_context
     *
     * @details Provides direct access to signal manager management component.
     * Use this for advanced scenarios requiring fine-grained control.
     */
    auto signal_context() -> signal_manager_context& {
        return signal_context_;
    }

    /**
     * @brief Access signal manager context component (const)
     * @return Const reference to signal_manager_context
     */
    auto signal_context() const -> const signal_manager_context& {
        return signal_context_;
    }

    /**
     * @brief Access logger registry component
     * @return Reference to logger_registry
     *
     * @details Provides direct access to logger registration component.
     * Use this for querying registered loggers.
     */
    auto registry() -> logger_registry& {
        return registry_;
    }

    /**
     * @brief Access logger registry component (const)
     * @return Const reference to logger_registry
     */
    auto registry() const -> const logger_registry& {
        return registry_;
    }

    // ============================================================
    // Convenience Methods (Backward Compatibility)
    // ============================================================

    /**
     * @brief Get signal manager instance
     * @return Shared pointer to signal manager interface
     *
     * @details Convenience method that delegates to signal_context().
     * Maintained for backward compatibility.
     */
    std::shared_ptr<security::signal_manager_interface> get_signal_manager() const {
        return signal_context_.get_signal_manager();
    }

    /**
     * @brief Set signal manager (for dynamic reconfiguration)
     * @param signal_mgr New signal manager implementation
     *
     * @details Convenience method that delegates to signal_context().
     * Maintained for backward compatibility.
     */
    void set_signal_manager(std::shared_ptr<security::signal_manager_interface> signal_mgr) {
        signal_context_.set_signal_manager(std::move(signal_mgr));
    }

    /**
     * @brief Check if context is initialized
     * @return true if signal manager is initialized
     *
     * @details Convenience method that delegates to signal_context().
     * Maintained for backward compatibility.
     */
    bool is_initialized() const {
        return signal_context_.is_initialized();
    }

    /**
     * @brief Register a logger
     * @param log Logger implementing critical_logger_interface
     *
     * @details Registers the logger in both:
     * 1. Internal registry (for tracking)
     * 2. Signal manager (for emergency flush)
     *
     * Convenience method combining registry and signal manager operations.
     *
     * @since 2.0.0
     */
    void register_logger(security::critical_logger_interface* log) {
        // Register in local registry
        registry_.register_logger(log);

        // Register with signal manager
        auto signal_mgr = signal_context_.get_signal_manager();
        if (signal_mgr) {
            signal_mgr->register_logger(log);
        }
    }

    /**
     * @brief Unregister a logger
     * @param log Logger to unregister
     *
     * @details Unregisters the logger from both:
     * 1. Internal registry
     * 2. Signal manager
     *
     * Convenience method combining registry and signal manager operations.
     *
     * @since 2.0.0
     */
    void unregister_logger(security::critical_logger_interface* log) {
        // Unregister from local registry
        registry_.unregister_logger(log);

        // Unregister from signal manager
        auto signal_mgr = signal_context_.get_signal_manager();
        if (signal_mgr) {
            signal_mgr->unregister_logger(log);
        }
    }

private:
    /// Signal manager lifecycle management
    signal_manager_context signal_context_;

    /// Logger registration tracking
    logger_registry registry_;
};

} // namespace core
} // namespace kcenon::logger
