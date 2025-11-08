#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <cstddef>

namespace kcenon::logger::security {

// Forward declaration
class critical_logger_interface;

/**
 * @interface signal_manager_interface
 * @brief Interface for signal handler management
 *
 * @details This interface defines the contract for managing signal handlers
 * and logger registration. It enables dependency injection and mocking for tests.
 *
 * Key Responsibilities:
 * - Register/unregister loggers for signal notifications
 * - Install/manage signal handlers
 * - Provide signal-safe emergency flushing
 *
 * @since 2.0.0
 */
class signal_manager_interface {
public:
    virtual ~signal_manager_interface() = default;

    /**
     * @brief Register a logger to receive signal notifications
     * @param log Logger interface for emergency flushing
     *
     * @details Signal handlers are installed when the first logger is registered.
     * Thread-safe: yes
     */
    virtual void register_logger(critical_logger_interface* log) = 0;

    /**
     * @brief Unregister a logger
     * @param log Logger to unregister
     *
     * @details Signal handlers are removed when the last logger is unregistered.
     * Thread-safe: yes
     */
    virtual void unregister_logger(critical_logger_interface* log) = 0;

    /**
     * @brief Check if signal handlers are installed
     * @return true if handlers are installed, false otherwise
     *
     * Thread-safe: yes
     */
    virtual bool are_handlers_installed() const = 0;

    /**
     * @brief Get number of registered loggers
     * @return Count of registered loggers
     *
     * Thread-safe: yes
     */
    virtual size_t logger_count() const = 0;
};

} // namespace kcenon::logger::security
