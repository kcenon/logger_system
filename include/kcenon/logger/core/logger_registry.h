#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <vector>
#include <shared_mutex>

// Forward declaration to avoid circular dependencies
namespace kcenon::logger::security {
    class critical_logger_interface;
}

/**
 * @file logger_registry.h
 * @brief Manages logger registration and tracking
 * @author kcenon
 * @since 2.0.0
 *
 * @details Extracted from logger_context following Single Responsibility Principle.
 * This component is solely responsible for maintaining the list of registered loggers.
 */

namespace kcenon::logger::core {

/**
 * @class logger_registry
 * @brief Manages registration and unregistration of loggers
 *
 * @details This class handles a single responsibility: tracking registered loggers.
 * It provides thread-safe registration/unregistration and query operations.
 *
 * Thread Safety: All methods are thread-safe. Uses shared_mutex for reader-writer lock
 * optimization (multiple readers, single writer).
 */
class logger_registry {
public:
    /**
     * @brief Default constructor
     */
    logger_registry() = default;

    /**
     * @brief Register a logger for tracking
     * @param log Logger implementing critical_logger_interface
     *
     * @details Adds the logger to the internal registry.
     * If the logger is already registered, this is a no-op.
     */
    void register_logger(security::critical_logger_interface* log);

    /**
     * @brief Unregister a logger
     * @param log Logger to unregister
     *
     * @details Removes the logger from the internal registry.
     * If the logger is not registered, this is a no-op.
     */
    void unregister_logger(security::critical_logger_interface* log);

    /**
     * @brief Get all registered loggers
     * @return Vector of registered logger pointers
     *
     * @details Returns a snapshot of currently registered loggers.
     * The returned vector is a copy and safe to iterate without holding locks.
     */
    auto get_registered_loggers() const -> std::vector<security::critical_logger_interface*>;

    /**
     * @brief Check if registry is empty
     * @return true if no loggers are registered
     */
    bool empty() const;

    /**
     * @brief Get number of registered loggers
     * @return Count of registered loggers
     */
    size_t size() const;

private:
    /// Registered loggers (using vector to preserve registration order)
    std::vector<security::critical_logger_interface*> loggers_;

    /// Mutex for thread-safe access (shared for read, exclusive for write)
    mutable std::shared_mutex mutex_;
};

} // namespace kcenon::logger::core
