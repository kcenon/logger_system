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

#ifdef __unix__
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
#ifdef __unix__
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
    // Windows would use _commit() or FlushFileBuffers()
}

void critical_writer::install_signal_handlers() {
    // Set global instance
    instance_.store(this);

    struct sigaction sa;
    sa.sa_handler = &critical_writer::signal_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;

    // Install handlers and save originals
    sigaction(SIGTERM, &sa, &original_sigterm_);
    sigaction(SIGINT, &sa, &original_sigint_);
    sigaction(SIGSEGV, &sa, &original_sigsegv_);
    sigaction(SIGABRT, &sa, &original_sigabrt_);
}

void critical_writer::restore_signal_handlers() {
    // Restore original handlers
    sigaction(SIGTERM, &original_sigterm_, nullptr);
    sigaction(SIGINT, &original_sigint_, nullptr);
    sigaction(SIGSEGV, &original_sigsegv_, nullptr);
    sigaction(SIGABRT, &original_sigabrt_, nullptr);

    instance_.store(nullptr);
}

void critical_writer::signal_handler(int signal) {
    // Get global instance
    critical_writer* writer = instance_.load();
    if (!writer) {
        return;
    }

    // Update statistics
    writer->stats_.signal_handler_invocations.fetch_add(1, std::memory_order_relaxed);

    // Emergency flush - do not allocate memory or throw exceptions
    try {
        // Write emergency message to stderr
        const char* signal_name = "UNKNOWN";
        switch (signal) {
            case SIGTERM: signal_name = "SIGTERM"; break;
            case SIGINT:  signal_name = "SIGINT"; break;
            case SIGSEGV: signal_name = "SIGSEGV"; break;
            case SIGABRT: signal_name = "SIGABRT"; break;
        }

        std::cerr << "\n[critical_writer] Signal " << signal_name
                  << " received, performing emergency flush...\n" << std::flush;

        // Flush wrapped writer
        if (writer->wrapped_writer_) {
            writer->wrapped_writer_->flush();
        }

        // Flush WAL
        if (writer->wal_stream_ && writer->wal_stream_->is_open()) {
            writer->wal_stream_->flush();
        }

        std::cerr << "[critical_writer] Emergency flush completed\n" << std::flush;

    } catch (...) {
        // Cannot do anything in signal handler
        std::cerr << "[critical_writer] Emergency flush failed\n" << std::flush;
    }

    // For SIGSEGV and SIGABRT, restore original handler and re-raise
    if (signal == SIGSEGV || signal == SIGABRT) {
        writer->restore_signal_handlers();
        std::raise(signal);
    }
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
