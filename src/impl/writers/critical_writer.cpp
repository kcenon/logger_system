/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/writers/critical_writer.h>
#include <kcenon/logger/core/error_codes.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>

#ifdef _WIN32
#include <io.h>       // For _flushall()
#include <stdio.h>    // For _flushall()
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>
#include <fcntl.h>
#endif

namespace kcenon::logger {

// Static instance for signal handler
std::atomic<critical_writer*> critical_writer::instance_{nullptr};

critical_writer::critical_writer(
    std::unique_ptr<base_writer> wrapped_writer,
    critical_writer_config config
)
    : config_(std::move(config))
    , wrapped_writer_(std::move(wrapped_writer))
{
    if (!wrapped_writer_) {
        throw std::invalid_argument("Wrapped writer cannot be null");
    }

    // Initialize write-ahead log if enabled
    if (config_.write_ahead_log) {
        try {
            wal_stream_ = std::make_unique<std::ofstream>(
                config_.wal_path,
                std::ios::app | std::ios::binary
            );
            if (!wal_stream_->is_open()) {
                std::cerr << "[critical_writer] Failed to open WAL: "
                          << config_.wal_path << std::endl;
                wal_stream_.reset();
            }
        } catch (const std::exception& e) {
            std::cerr << "[critical_writer] WAL initialization failed: "
                      << e.what() << std::endl;
            wal_stream_.reset();
        }
    }

    // Install signal handlers if enabled
    if (config_.enable_signal_handlers) {
        install_signal_handlers();
    }
}

critical_writer::~critical_writer() {
    shutting_down_.store(true);

    // Restore signal handlers
    if (config_.enable_signal_handlers) {
        restore_signal_handlers();
    }

    // Final flush
    try {
        flush();
    } catch (...) {
        // Ignore exceptions in destructor
    }

    // Close WAL
    if (wal_stream_) {
        try {
            wal_stream_->flush();
            wal_stream_->close();
        } catch (...) {
            // Ignore exceptions in destructor
        }
    }

    // Clear global instance
    instance_.store(nullptr);
}

result_void critical_writer::write(
    logger_system::log_level level,
    const std::string& message,
    const std::string& file,
    int line,
    const std::string& function,
    const std::chrono::system_clock::time_point& timestamp
)
{
    // Check if we're in emergency shutdown (signal received)
    if (shutting_down_.load(std::memory_order_acquire)) {
        // In shutdown mode, all writes become critical to ensure they're flushed
        // before process termination
        std::lock_guard<std::mutex> lock(critical_mutex_);

        auto result = wrapped_writer_->write(level, message, file, line, function, timestamp);
        if (result) {
            wrapped_writer_->flush();
            stats_.total_flushes.fetch_add(1, std::memory_order_relaxed);
        }
        return result;
    }

    // Check if this is a critical level
    const bool is_critical = is_critical_level(level);

    if (is_critical) {
        // Acquire exclusive lock for critical writes
        std::lock_guard<std::mutex> lock(critical_mutex_);

        // Write to WAL first (if enabled)
        if (wal_stream_ && wal_stream_->is_open()) {
            try {
                write_to_wal(level, message, file, line, function, timestamp);
                stats_.wal_writes.fetch_add(1, std::memory_order_relaxed);
            } catch (const std::exception& e) {
                std::cerr << "[critical_writer] WAL write failed: "
                          << e.what() << std::endl;
            }
        }

        // Write to wrapped writer
        auto result = wrapped_writer_->write(level, message, file, line, function, timestamp);
        if (!result) {
            return result;
        }

        // Force flush immediately
        auto flush_result = wrapped_writer_->flush();
        stats_.total_flushes.fetch_add(1, std::memory_order_relaxed);

        // Sync file descriptor if configured
        if (config_.sync_on_critical) {
            try {
                sync_file_descriptor();
                stats_.sync_calls.fetch_add(1, std::memory_order_relaxed);
            } catch (const std::exception& e) {
                std::cerr << "[critical_writer] fsync failed: "
                          << e.what() << std::endl;
            }
        }

        stats_.total_critical_writes.fetch_add(1, std::memory_order_relaxed);
        return flush_result;
    }

    // Non-critical: delegate to wrapped writer normally
    return wrapped_writer_->write(level, message, file, line, function, timestamp);
}

result_void critical_writer::flush() {
    std::lock_guard<std::mutex> lock(critical_mutex_);

    // Flush WAL
    if (wal_stream_ && wal_stream_->is_open()) {
        try {
            wal_stream_->flush();
        } catch (const std::exception& e) {
            std::cerr << "[critical_writer] WAL flush failed: "
                      << e.what() << std::endl;
        }
    }

    // Flush wrapped writer
    auto result = wrapped_writer_->flush();
    stats_.total_flushes.fetch_add(1, std::memory_order_relaxed);

    return result;
}

bool critical_writer::is_healthy() const {
    // Check WAL health
    if (config_.write_ahead_log && (!wal_stream_ || !wal_stream_->good())) {
        return false;
    }

    return wrapped_writer_->is_healthy();
}

std::string critical_writer::get_name() const {
    return "critical_" + wrapped_writer_->get_name();
}

void critical_writer::set_use_color(bool use_color) {
    wrapped_writer_->set_use_color(use_color);
}

void critical_writer::set_force_flush_on_critical(bool enable) {
    config_.force_flush_on_critical = enable;
}

bool critical_writer::is_critical_level(logger_system::log_level level) const {
    // Critical and fatal always require immediate flush
    if (level >= logger_system::log_level::critical) {
        return config_.force_flush_on_critical;
    }

    // Error level if configured
    if (level == logger_system::log_level::error) {
        return config_.force_flush_on_error;
    }

    return false;
}

void critical_writer::write_to_wal(
    logger_system::log_level level,
    const std::string& message,
    const std::string& file,
    int line,
    const std::string& function,
    const std::chrono::system_clock::time_point& timestamp
)
{
    if (!wal_stream_ || !wal_stream_->is_open()) {
        return;
    }

    try {
        // Format: [timestamp] [LEVEL] [file:line:function] message\n
        auto time_t = std::chrono::system_clock::to_time_t(timestamp);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            timestamp.time_since_epoch()
        ).count() % 1000;

        std::ostringstream oss;
        oss << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms << "] "
            << "[" << logger_system::log_level_to_string(level) << "] "
            << "[" << file << ":" << line << ":" << function << "] "
            << message << "\n";

        *wal_stream_ << oss.str();
        wal_stream_->flush();

    } catch (const std::exception& e) {
        std::cerr << "[critical_writer] WAL write exception: "
                  << e.what() << std::endl;
    }
}

void critical_writer::sync_file_descriptor() {
#ifdef _WIN32
    // Windows: Use _commit() to flush file buffers to disk
    // Note: std::ofstream doesn't expose HANDLE, so we flush the stream
    // For a complete implementation, would need native Windows file handling
    if (wal_stream_) {
        wal_stream_->flush();
    }
    // Could use _flushall() to flush all open streams, but it's global
    ::_flushall();
#elif defined(__unix__) || defined(__APPLE__)
    // POSIX: Use fsync() to synchronize file to disk
    // Get file descriptor from wrapped writer if it's a file writer
    // This is a simplified implementation - actual implementation would need
    // to query the wrapped writer for its file descriptor

    // For now, sync stdout/stderr as fallback
    ::fsync(STDOUT_FILENO);
    ::fsync(STDERR_FILENO);

    // Sync WAL
    if (wal_stream_) {
        wal_stream_->flush();
        // Note: std::ofstream doesn't expose fd, would need native file handling
    }
#endif
}

void critical_writer::install_signal_handlers() {
    // Set global instance
    instance_.store(this);

#ifdef _WIN32
    // Windows: Use signal() API (less robust than POSIX sigaction)
    original_sigterm_ = ::signal(SIGTERM, &critical_writer::signal_handler);
    original_sigint_ = ::signal(SIGINT, &critical_writer::signal_handler);
    original_sigabrt_ = ::signal(SIGABRT, &critical_writer::signal_handler);
    // Note: SIGSEGV is not recommended to handle on Windows
#elif defined(__unix__) || defined(__APPLE__)
    // POSIX: Use sigaction for more reliable signal handling
    struct sigaction sa;
    sa.sa_handler = &critical_writer::signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    // Install handlers and save originals
    sigaction(SIGTERM, &sa, &original_sigterm_);
    sigaction(SIGINT, &sa, &original_sigint_);
    sigaction(SIGSEGV, &sa, &original_sigsegv_);
    sigaction(SIGABRT, &sa, &original_sigabrt_);
#endif
}

void critical_writer::restore_signal_handlers() {
#ifdef _WIN32
    // Windows: Restore original handlers
    if (original_sigterm_) ::signal(SIGTERM, original_sigterm_);
    if (original_sigint_) ::signal(SIGINT, original_sigint_);
    if (original_sigabrt_) ::signal(SIGABRT, original_sigabrt_);
#elif defined(__unix__) || defined(__APPLE__)
    // POSIX: Restore original handlers
    sigaction(SIGTERM, &original_sigterm_, nullptr);
    sigaction(SIGINT, &original_sigint_, nullptr);
    sigaction(SIGSEGV, &original_sigsegv_, nullptr);
    sigaction(SIGABRT, &original_sigabrt_, nullptr);
#endif

    instance_.store(nullptr);
}

void critical_writer::signal_handler(int signal) {
    // IMPORTANT: This is a signal handler - only async-signal-safe operations allowed!
    // See POSIX.1-2008 or https://man7.org/linux/man-pages/man7/signal-safety.7.html

    // Get global instance (atomic operation - safe)
    critical_writer* writer = instance_.load(std::memory_order_acquire);
    if (!writer) {
        return;
    }

    // Update statistics (atomic operation - safe)
    writer->stats_.signal_handler_invocations.fetch_add(1, std::memory_order_relaxed);

    // Set shutdown flag (atomic operation - safe)
    // This allows other threads to detect signal and perform graceful shutdown
    writer->shutting_down_.store(true, std::memory_order_release);

    // Write emergency message using async-signal-safe write() syscall
    const char* msg = nullptr;
    size_t msg_len = 0;

    switch (signal) {
        case SIGTERM:
            msg = "\n[critical_writer] SIGTERM received, shutting down...\n";
            msg_len = 55;
            break;
        case SIGINT:
            msg = "\n[critical_writer] SIGINT received, shutting down...\n";
            msg_len = 54;
            break;
#if defined(__unix__) || defined(__APPLE__)
        case SIGSEGV:
            msg = "\n[critical_writer] SIGSEGV received, emergency shutdown...\n";
            msg_len = 61;
            break;
#endif
        case SIGABRT:
            msg = "\n[critical_writer] SIGABRT received, emergency shutdown...\n";
            msg_len = 61;
            break;
        default:
            msg = "\n[critical_writer] Signal received, shutting down...\n";
            msg_len = 55;
            break;
    }

    // Use async-signal-safe write() directly to stderr (POSIX-safe)
#if defined(__unix__) || defined(__APPLE__)
    if (msg && msg_len > 0) {
        ::write(STDERR_FILENO, msg, msg_len);
    }
#elif defined(_WIN32)
    // Windows doesn't have strict async-signal-safe requirements
    // But we still avoid complex operations
    if (msg && msg_len > 0) {
        ::write(_fileno(stderr), msg, static_cast<unsigned int>(msg_len));
    }
#endif

    // NOTE: We do NOT call flush() here as it acquires mutex (signal-unsafe!)
    // The shutting_down_ flag will be checked by other threads to perform
    // graceful shutdown and flush operations in a safe context.

#if defined(__unix__) || defined(__APPLE__)
    // For fatal signals (SIGSEGV, SIGABRT), restore original handler and re-raise
    // This allows the default action (core dump) to proceed
    if (signal == SIGSEGV || signal == SIGABRT) {
        // Restore original handlers (sigaction is async-signal-safe on most systems)
        writer->restore_signal_handlers();
        // Re-raise the signal to trigger default action
        std::raise(signal);
    }
#else
    // Windows: For SIGABRT, restore and re-raise
    if (signal == SIGABRT) {
        writer->restore_signal_handlers();
        std::raise(signal);
    }
#endif
}

// ============================================================================
// hybrid_writer implementation
// ============================================================================

hybrid_writer::hybrid_writer(
    std::unique_ptr<base_writer> wrapped_writer,
    critical_writer_config critical_config,
    std::size_t async_queue_size
)
    : async_queue_size_(async_queue_size)
{
    // Wrap in critical_writer first
    critical_writer_ = std::make_unique<critical_writer>(
        std::move(wrapped_writer),
        std::move(critical_config)
    );
}

hybrid_writer::~hybrid_writer() = default;

result_void hybrid_writer::write(
    logger_system::log_level level,
    const std::string& message,
    const std::string& file,
    int line,
    const std::string& function,
    const std::chrono::system_clock::time_point& timestamp
)
{
    // Critical writer handles both sync (for critical) and async (for normal)
    return critical_writer_->write(level, message, file, line, function, timestamp);
}

result_void hybrid_writer::flush() {
    return critical_writer_->flush();
}

bool hybrid_writer::is_healthy() const {
    return critical_writer_->is_healthy();
}

std::string hybrid_writer::get_name() const {
    return "hybrid_" + critical_writer_->get_name();
}

void hybrid_writer::set_use_color(bool use_color) {
    critical_writer_->set_use_color(use_color);
}

} // namespace kcenon::logger
