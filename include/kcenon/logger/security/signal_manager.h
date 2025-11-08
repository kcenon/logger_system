#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "signal_manager_interface.h"
#include <csignal>
#include <set>
#include <mutex>
#include <atomic>
#include <unistd.h>

namespace kcenon::logger::security {

/**
 * @class signal_manager
 * @brief Manager for safe signal handler installation
 *
 * Features:
 * - Centralized signal handler management
 * - Thread-safe logger registration
 * - Uses only signal-safe functions in handlers
 * - Prevents global state conflicts when multiple loggers exist
 * - Implements signal_manager_interface for dependency injection
 *
 * @since 2.0.0 - Converted from singleton to DI pattern
 */
class signal_manager : public signal_manager_interface {
public:
    /**
     * @brief Default constructor
     */
    signal_manager() = default;

    /**
     * @brief Destructor
     */
    ~signal_manager() override = default;

    /**
     * @brief Register a logger to receive signal notifications
     * @param log Logger interface for emergency flushing
     *
     * Signal handlers are installed when the first logger is registered.
     */
    void register_logger(critical_logger_interface* log) override {
        std::lock_guard lock(mutex_);

        loggers_.insert(log);

        // Install handlers only on first registration
        if (loggers_.size() == 1 && !handlers_installed_.load()) {
            install_handlers();
        }
    }

    /**
     * @brief Unregister a logger
     * @param log Logger to unregister
     *
     * Signal handlers are removed when the last logger is unregistered.
     */
    void unregister_logger(critical_logger_interface* log) override {
        std::lock_guard lock(mutex_);

        loggers_.erase(log);

        // Remove handlers when last logger is unregistered
        if (loggers_.empty() && handlers_installed_.load()) {
            uninstall_handlers();
        }
    }

    /**
     * @brief Check if signal handlers are installed
     */
    bool are_handlers_installed() const override {
        return handlers_installed_.load();
    }

    /**
     * @brief Get number of registered loggers
     */
    size_t logger_count() const override {
        std::lock_guard lock(mutex_);
        return loggers_.size();
    }

    // Prevent copying and moving
    signal_manager(const signal_manager&) = delete;
    signal_manager& operator=(const signal_manager&) = delete;
    signal_manager(signal_manager&&) = delete;
    signal_manager& operator=(signal_manager&&) = delete;

private:

    /**
     * @brief Install signal handlers
     */
    void install_handlers() {
        // Set current instance for signal handler
        current_instance_.store(this, std::memory_order_release);

        // Save original handlers
        original_sigsegv_ = std::signal(SIGSEGV, signal_handler);
        original_sigabrt_ = std::signal(SIGABRT, signal_handler);
        original_sigterm_ = std::signal(SIGTERM, signal_handler);
        original_sigint_ = std::signal(SIGINT, signal_handler);

        handlers_installed_.store(true);
    }

    /**
     * @brief Uninstall signal handlers
     */
    void uninstall_handlers() {
        // Restore original handlers
        std::signal(SIGSEGV, original_sigsegv_);
        std::signal(SIGABRT, original_sigabrt_);
        std::signal(SIGTERM, original_sigterm_);
        std::signal(SIGINT, original_sigint_);

        handlers_installed_.store(false);

        // Clear current instance
        current_instance_.store(nullptr, std::memory_order_release);
    }

    /**
     * @brief Signal handler (must use only signal-safe functions)
     * @param sig Signal number
     *
     * Signal-safe functions (POSIX):
     * - write(), fsync(), _exit()
     * - Most syscalls
     *
     * NOT signal-safe:
     * - malloc(), free()
     * - printf(), iostream
     * - std::mutex (use atomic flags instead)
     */
    static void signal_handler(int sig) {
        // Write signal message using only signal-safe functions
        const char* msg = nullptr;
        size_t msg_len = 0;

        switch (sig) {
            case SIGSEGV:
                msg = "\n[CRITICAL] SIGSEGV received - emergency flush\n";
                msg_len = 45;
                break;
            case SIGABRT:
                msg = "\n[CRITICAL] SIGABRT received - emergency flush\n";
                msg_len = 45;
                break;
            case SIGTERM:
                msg = "\n[CRITICAL] SIGTERM received - emergency flush\n";
                msg_len = 45;
                break;
            case SIGINT:
                msg = "\n[CRITICAL] SIGINT received - emergency flush\n";
                msg_len = 44;
                break;
            default:
                msg = "\n[CRITICAL] Unknown signal - emergency flush\n";
                msg_len = 43;
                break;
        }

        // Write to stderr (signal-safe)
        write(STDERR_FILENO, msg, msg_len);

        // Emergency flush for all registered loggers
        // Note: We cannot safely lock mutex_ here (not signal-safe)
        // Instead, we access the loggers directly (risk of race condition,
        // but in a crash scenario, this is acceptable)
        signal_manager* mgr = current_instance_.load(std::memory_order_acquire);
        if (!mgr) {
            // No instance available, nothing to flush
            _exit(128 + sig);
        }

        // Call emergency_flush on each logger
        // Note: This assumes the logger pointers are still valid
        // In production, consider using a signal-safe data structure
        for (auto* log : mgr->loggers_) {
            if (log) {
                emergency_flush(log);
            }
        }

        // Call original handler (or terminate)
        switch (sig) {
            case SIGSEGV:
                if (mgr->original_sigsegv_ != SIG_DFL &&
                    mgr->original_sigsegv_ != SIG_IGN) {
                    mgr->original_sigsegv_(sig);
                } else {
                    std::signal(SIGSEGV, SIG_DFL);
                    std::raise(SIGSEGV);
                }
                break;
            case SIGABRT:
                if (mgr->original_sigabrt_ != SIG_DFL &&
                    mgr->original_sigabrt_ != SIG_IGN) {
                    mgr->original_sigabrt_(sig);
                } else {
                    std::signal(SIGABRT, SIG_DFL);
                    std::raise(SIGABRT);
                }
                break;
            case SIGTERM:
                if (mgr->original_sigterm_ != SIG_DFL &&
                    mgr->original_sigterm_ != SIG_IGN) {
                    mgr->original_sigterm_(sig);
                } else {
                    _exit(128 + sig);
                }
                break;
            case SIGINT:
                if (mgr->original_sigint_ != SIG_DFL &&
                    mgr->original_sigint_ != SIG_IGN) {
                    mgr->original_sigint_(sig);
                } else {
                    _exit(128 + sig);
                }
                break;
            default:
                _exit(128 + sig);
        }
    }

    /**
     * @brief Emergency flush for a logger (signal-safe)
     * @param log Logger interface
     */
    static void emergency_flush(critical_logger_interface* log);

    /// Current instance for signal handler (static due to C signal API limitation)
    /// Note: Only one instance can handle signals at a time
    static std::atomic<signal_manager*> current_instance_;

    mutable std::mutex mutex_;
    std::set<critical_logger_interface*> loggers_;
    std::atomic<bool> handlers_installed_{false};

    // Original signal handlers
    void (*original_sigsegv_)(int) = SIG_DFL;
    void (*original_sigabrt_)(int) = SIG_DFL;
    void (*original_sigterm_)(int) = SIG_DFL;
    void (*original_sigint_)(int) = SIG_DFL;
};

/**
 * @interface critical_logger_interface
 * @brief Interface for loggers that support emergency flushing
 */
class critical_logger_interface {
public:
    virtual ~critical_logger_interface() = default;

    /**
     * @brief Get file descriptor for emergency writing
     * @return File descriptor or -1 if not available
     *
     * This method must be signal-safe (no allocations, no locks)
     */
    virtual int get_emergency_fd() const = 0;

    /**
     * @brief Get emergency buffer pointer
     * @return Pointer to buffer or nullptr
     *
     * This method must be signal-safe
     */
    virtual const char* get_emergency_buffer() const = 0;

    /**
     * @brief Get emergency buffer size
     * @return Buffer size in bytes
     *
     * This method must be signal-safe
     */
    virtual size_t get_emergency_buffer_size() const = 0;
};

// Implementation of emergency_flush (signal-safe)
inline void signal_manager::emergency_flush(critical_logger_interface* log) {
    if (!log) {
        return;
    }

    // Get file descriptor
    int fd = log->get_emergency_fd();
    if (fd < 0) {
        return;
    }

    // Get buffer
    const char* buffer = log->get_emergency_buffer();
    size_t size = log->get_emergency_buffer_size();

    if (buffer && size > 0) {
        // Write using signal-safe write() syscall
        ssize_t written = 0;
        while (written < static_cast<ssize_t>(size)) {
            ssize_t n = write(fd, buffer + written, size - written);
            if (n <= 0) {
                break;  // Error or would block
            }
            written += n;
        }

        // Sync to disk (signal-safe)
        fsync(fd);
    }
}

} // namespace kcenon::logger::security
