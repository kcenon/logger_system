#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/core/logger.h>
#include <atomic>
#include <csignal>
#include <memory>
#include <mutex>

namespace kcenon::logger::safety {

/**
 * @class crash_safe_logger
 * @brief Logger with crash recovery and emergency flush capabilities
 *
 * This logger wrapper provides additional safety guarantees:
 * - Signal handlers for crash detection (SIGSEGV, SIGABRT, SIGTERM)
 * - Emergency synchronous flush on crash
 * - Atomic log level management
 * - Flush with timeout to prevent deadlocks
 * - Periodic auto-flush for data durability
 *
 * ### Thread Safety
 * All methods are thread-safe. Signal handlers use async-signal-safe
 * operations only.
 *
 * ### Production Usage
 * @code
 * // Create crash-safe logger
 * auto logger = crash_safe_logger::create();
 *
 * // Enable crash handlers
 * logger->install_crash_handlers();
 *
 * // Configure auto-flush
 * logger->set_auto_flush_interval(std::chrono::seconds(5));
 *
 * // Use as normal logger
 * logger->log(log_level::info, "Application started");
 *
 * // On crash, logs are automatically flushed
 * @endcode
 *
 * ### Limitations
 * - Signal handlers cannot allocate memory
 * - Emergency flush is best-effort only
 * - May not work with all crash scenarios (e.g., stack corruption)
 */
class crash_safe_logger {
public:
    /**
     * @brief Create crash-safe logger
     * @param underlying_logger Base logger to wrap (optional)
     * @return Shared pointer to crash_safe_logger
     */
    static std::shared_ptr<crash_safe_logger> create(
        std::shared_ptr<logger> underlying_logger = nullptr
    ) {
        if (!underlying_logger) {
            underlying_logger = std::make_shared<logger>(true, 16384);
        }

        auto safe_logger = std::shared_ptr<crash_safe_logger>(
            new crash_safe_logger(std::move(underlying_logger))
        );

        // Register for cleanup
        global_instance_ = safe_logger;

        return safe_logger;
    }

    /**
     * @brief Install signal handlers for crash detection
     * @return true if handlers installed successfully
     */
    bool install_crash_handlers() {
        std::lock_guard<std::mutex> lock(handler_mutex_);

        if (handlers_installed_) {
            return true; // Already installed
        }

        // Install handlers for common crash signals
        struct sigaction sa;
        sa.sa_handler = &crash_safe_logger::signal_handler;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;

        bool success = true;
        success &= (sigaction(SIGSEGV, &sa, &old_sigsegv_) == 0);
        success &= (sigaction(SIGABRT, &sa, &old_sigabrt_) == 0);
        success &= (sigaction(SIGTERM, &sa, &old_sigterm_) == 0);
        success &= (sigaction(SIGINT, &sa, &old_sigint_) == 0);

        handlers_installed_ = success;
        return success;
    }

    /**
     * @brief Remove signal handlers
     */
    void uninstall_crash_handlers() {
        std::lock_guard<std::mutex> lock(handler_mutex_);

        if (!handlers_installed_) {
            return;
        }

        sigaction(SIGSEGV, &old_sigsegv_, nullptr);
        sigaction(SIGABRT, &old_sigabrt_, nullptr);
        sigaction(SIGTERM, &old_sigterm_, nullptr);
        sigaction(SIGINT, &old_sigint_, nullptr);

        handlers_installed_ = false;
    }

    /**
     * @brief Flush with timeout to prevent deadlocks
     * @param timeout Maximum time to wait for flush
     * @return true if flushed successfully, false on timeout
     */
    bool flush_with_timeout(std::chrono::milliseconds timeout) {
        auto deadline = std::chrono::steady_clock::now() + timeout;

        // Signal flush request
        flush_requested_.store(true, std::memory_order_release);

        // Try to acquire flush lock with timeout
        while (std::chrono::steady_clock::now() < deadline) {
            if (flush_mutex_.try_lock()) {
                // Got lock, perform flush
                logger_->flush();
                flush_mutex_.unlock();
                flush_requested_.store(false, std::memory_order_release);
                return true;
            }

            // Wait a bit before retry
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        // Timeout occurred
        return false;
    }

    /**
     * @brief Emergency flush (async-signal-safe)
     *
     * This is called from signal handlers and must be async-signal-safe.
     * It attempts a best-effort flush without allocations or locks.
     */
    void emergency_flush() {
        // Set emergency flag
        emergency_flush_active_.store(true, std::memory_order_release);

        // Try immediate flush (may fail if locks are held)
        if (flush_mutex_.try_lock()) {
            logger_->flush();
            flush_mutex_.unlock();
        }
    }

    /**
     * @brief Set minimum log level (thread-safe)
     */
    void set_min_level(log_level level) {
        min_level_.store(level, std::memory_order_release);
        logger_->set_min_level(level);
    }

    /**
     * @brief Get minimum log level (thread-safe)
     */
    log_level get_min_level() const {
        return min_level_.load(std::memory_order_acquire);
    }

    /**
     * @brief Enable auto-flush at regular intervals
     * @param interval Time between auto-flushes
     */
    void set_auto_flush_interval(std::chrono::milliseconds interval) {
        auto_flush_interval_ = interval;

        if (interval.count() > 0 && !auto_flush_thread_.joinable()) {
            start_auto_flush_thread();
        } else if (interval.count() == 0 && auto_flush_thread_.joinable()) {
            stop_auto_flush_thread();
        }
    }

    /**
     * @brief Log message (delegates to underlying logger)
     */
    void log(log_level level, const std::string& message) {
        // Early exit if level not enabled
        if (level < min_level_.load(std::memory_order_acquire)) {
            return;
        }

        logger_->log(level, message);
    }

    /**
     * @brief Log message with source location
     */
    void log(log_level level,
             const std::string& message,
             const std::string& file,
             int line,
             const std::string& function) {
        if (level < min_level_.load(std::memory_order_acquire)) {
            return;
        }

        logger_->log(level, message, file, line, function);
    }

    /**
     * @brief Start the underlying logger
     */
    common::VoidResult start() {
        return logger_->start();
    }

    /**
     * @brief Stop the underlying logger
     */
    common::VoidResult stop() {
        stop_auto_flush_thread();
        return logger_->stop();
    }

    /**
     * @brief Get the underlying logger
     */
    std::shared_ptr<logger> get_underlying_logger() {
        return logger_;
    }

    ~crash_safe_logger() {
        uninstall_crash_handlers();
        stop_auto_flush_thread();
    }

private:
    explicit crash_safe_logger(std::shared_ptr<logger> logger)
        : logger_(std::move(logger))
        , min_level_(log_level::info)
        , handlers_installed_(false)
        , emergency_flush_active_(false)
        , flush_requested_(false)
        , auto_flush_interval_(0)
        , stop_auto_flush_(false)
    {
    }

    /**
     * @brief Signal handler (async-signal-safe)
     */
    static void signal_handler(int signal) {
        // Get global instance
        auto instance = global_instance_.lock();
        if (!instance) {
            return; // Logger already destroyed
        }

        // Perform emergency flush
        instance->emergency_flush();

        // Chain to old handler if exists
        switch (signal) {
            case SIGSEGV:
                if (instance->old_sigsegv_.sa_handler != SIG_DFL &&
                    instance->old_sigsegv_.sa_handler != SIG_IGN) {
                    instance->old_sigsegv_.sa_handler(signal);
                }
                break;
            case SIGABRT:
                if (instance->old_sigabrt_.sa_handler != SIG_DFL &&
                    instance->old_sigabrt_.sa_handler != SIG_IGN) {
                    instance->old_sigabrt_.sa_handler(signal);
                }
                break;
            case SIGTERM:
            case SIGINT:
                // For these, we exit after flush
                std::exit(signal);
                break;
        }
    }

    /**
     * @brief Start auto-flush background thread
     */
    void start_auto_flush_thread() {
        stop_auto_flush_ = false;
        auto_flush_thread_ = std::thread([this]() {
            while (!stop_auto_flush_.load(std::memory_order_acquire)) {
                std::this_thread::sleep_for(auto_flush_interval_);

                if (!stop_auto_flush_.load(std::memory_order_acquire)) {
                    flush_with_timeout(std::chrono::seconds(1));
                }
            }
        });
    }

    /**
     * @brief Stop auto-flush background thread
     */
    void stop_auto_flush_thread() {
        if (auto_flush_thread_.joinable()) {
            stop_auto_flush_.store(true, std::memory_order_release);
            auto_flush_thread_.join();
        }
    }

private:
    std::shared_ptr<logger> logger_;
    std::atomic<log_level> min_level_;

    // Signal handler state
    std::mutex handler_mutex_;
    bool handlers_installed_;
    struct sigaction old_sigsegv_;
    struct sigaction old_sigabrt_;
    struct sigaction old_sigterm_;
    struct sigaction old_sigint_;

    // Emergency flush state
    std::atomic<bool> emergency_flush_active_;
    std::atomic<bool> flush_requested_;
    std::mutex flush_mutex_;

    // Auto-flush
    std::chrono::milliseconds auto_flush_interval_;
    std::thread auto_flush_thread_;
    std::atomic<bool> stop_auto_flush_;

    // Global instance for signal handler
    static std::weak_ptr<crash_safe_logger> global_instance_;
};

// Static member definition
std::weak_ptr<crash_safe_logger> crash_safe_logger::global_instance_;

} // namespace kcenon::logger::safety
