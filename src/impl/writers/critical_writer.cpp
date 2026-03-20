/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/writers/critical_writer.h>
#include <kcenon/logger/core/error_codes.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include <kcenon/logger/utils/string_utils.h>
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

critical_writer::critical_writer(
    log_writer_ptr wrapped_writer,
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
        auto wal_result = utils::try_open_operation([&]() -> common::VoidResult {
            wal_stream_ = std::make_unique<std::ofstream>(
                config_.wal_path,
                std::ios::app | std::ios::binary
            );

            auto check = utils::check_condition(
                wal_stream_->is_open(),
                logger_error_code::file_open_failed,
                "Failed to open WAL: " + config_.wal_path
            );

            if (check.is_err()) {
                wal_stream_.reset();
                return check;
            }

            return common::ok();
        });

        if (wal_result.is_err()) {
            std::cerr << "[critical_writer] WAL initialization failed: "
                      << wal_result.error().message << std::endl;
            wal_stream_.reset();
        }
    }

}

critical_writer::~critical_writer() {
    shutting_down_.store(true);

    // Final flush - use safe operation to log any errors
    utils::safe_destructor_result_operation("final_flush", [this]() {
        return flush();
    });

    // Close WAL with proper error logging
    if (wal_stream_) {
        utils::safe_destructor_operation("wal_close", [this]() {
            wal_stream_->flush();
            wal_stream_->close();
        });
    }
}

common::VoidResult critical_writer::write(const log_entry& entry) {
    // Convert log_level from logger_system to common::interfaces
    auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

    // Check if we're in emergency shutdown (signal received)
    if (shutting_down_.load(std::memory_order_acquire)) {
        // In shutdown mode, all writes become critical to ensure they're flushed
        // before process termination
        std::lock_guard<std::mutex> lock(critical_mutex_);

        auto result = wrapped_writer_->write(entry);
        if (result.is_ok()) {
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
            write_to_wal(entry);
            stats_.wal_writes.fetch_add(1, std::memory_order_relaxed);
        }

        // Write to wrapped writer
        auto result = wrapped_writer_->write(entry);
        if (result.is_err()) {
            return result;
        }

        // Force flush immediately
        auto flush_result = wrapped_writer_->flush();
        stats_.total_flushes.fetch_add(1, std::memory_order_relaxed);

        // Sync file descriptor if configured
        if (config_.sync_on_critical) {
            sync_file_descriptor();
            stats_.sync_calls.fetch_add(1, std::memory_order_relaxed);
        }

        stats_.total_critical_writes.fetch_add(1, std::memory_order_relaxed);
        return flush_result;
    }

    // Non-critical: delegate to wrapped writer normally
    return wrapped_writer_->write(entry);
}

common::VoidResult critical_writer::flush() {
    std::lock_guard<std::mutex> lock(critical_mutex_);

    // Flush WAL
    if (wal_stream_ && wal_stream_->is_open()) {
        wal_stream_->flush();
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
    // Color setting is now handled by formatter, not writer interface
    // This method is kept for backward compatibility but is deprecated
    (void)use_color;  // Suppress unused parameter warning
}

void critical_writer::set_force_flush_on_critical(bool enable) {
    config_.force_flush_on_critical = enable;
}

bool critical_writer::is_critical_level(common::interfaces::log_level level) const {
    // Critical and fatal always require immediate flush
    if (level >= common::interfaces::log_level::critical) {
        return config_.force_flush_on_critical;
    }

    // Error level if configured
    if (level == common::interfaces::log_level::error) {
        return config_.force_flush_on_error;
    }

    return false;
}

void critical_writer::write_to_wal(const log_entry& entry) {
    if (!wal_stream_ || !wal_stream_->is_open()) {
        return;
    }

    try {
        // Convert log_level
        auto level = static_cast<common::interfaces::log_level>(static_cast<int>(entry.level));

        // Extract source location
        std::string file = entry.location ? entry.location->file.to_string() : "";
        int line = entry.location ? entry.location->line : 0;
        std::string function = entry.location ? entry.location->function.to_string() : "";

        // Format: [timestamp] [LEVEL] [file:line:function] message\n
        auto time_t = std::chrono::system_clock::to_time_t(entry.timestamp);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            entry.timestamp.time_since_epoch()
        ).count() % 1000;

        std::ostringstream oss;
        oss << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms << "] "
            << "[" << utils::string_utils::level_to_string(level) << "] "
            << "[" << file << ":" << line << ":" << function << "] "
            << entry.message.to_string() << "\n";

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

// ============================================================================
// hybrid_writer implementation
// ============================================================================

hybrid_writer::hybrid_writer(
    log_writer_ptr wrapped_writer,
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

common::VoidResult hybrid_writer::write(const log_entry& entry) {
    // Critical writer handles both sync (for critical) and async (for normal)
    return critical_writer_->write(entry);
}

common::VoidResult hybrid_writer::flush() {
    return critical_writer_->flush();
}

bool hybrid_writer::is_healthy() const {
    return critical_writer_->is_healthy();
}

std::string hybrid_writer::get_name() const {
    return "hybrid_" + critical_writer_->get_name();
}

void hybrid_writer::set_use_color(bool use_color) {
    // Color setting is now handled by formatter, not writer interface
    // This method is kept for backward compatibility but is deprecated
    (void)use_color;  // Suppress unused parameter warning
}

} // namespace kcenon::logger
