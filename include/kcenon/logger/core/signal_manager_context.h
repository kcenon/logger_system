#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <memory>
#include <mutex>
#include <kcenon/logger/security/signal_manager_interface.h>

/**
 * @file signal_manager_context.h
 * @brief Manages signal manager lifecycle and access
 * @author kcenon
 * @since 2.0.0
 *
 * @details Extracted from logger_context following Single Responsibility Principle.
 * This component is solely responsible for signal manager lifecycle management.
 */

namespace kcenon::logger::core {

/**
 * @class signal_manager_context
 * @brief Manages signal manager instance with thread-safe access
 *
 * @details This class handles a single responsibility: managing the signal manager
 * instance. It provides thread-safe getter/setter and initialization checking.
 *
 * Thread Safety: All methods are thread-safe.
 */
class signal_manager_context {
public:
    /**
     * @brief Default constructor - creates null signal manager
     *
     * @details Use set_signal_manager() to initialize with actual instance.
     */
    signal_manager_context() = default;

    /**
     * @brief Constructor with signal manager
     * @param signal_mgr Initial signal manager instance
     */
    explicit signal_manager_context(
        std::shared_ptr<security::signal_manager_interface> signal_mgr
    );

    /**
     * @brief Get signal manager instance
     * @return Shared pointer to signal manager interface
     */
    auto get_signal_manager() const -> std::shared_ptr<security::signal_manager_interface>;

    /**
     * @brief Set signal manager instance
     * @param signal_mgr New signal manager implementation
     */
    void set_signal_manager(std::shared_ptr<security::signal_manager_interface> signal_mgr);

    /**
     * @brief Check if signal manager is initialized
     * @return true if signal manager is not null
     */
    bool is_initialized() const;

private:
    /// Signal manager instance
    std::shared_ptr<security::signal_manager_interface> signal_manager_;

    /// Mutex for thread-safe access
    mutable std::mutex mutex_;
};

} // namespace kcenon::logger::core
