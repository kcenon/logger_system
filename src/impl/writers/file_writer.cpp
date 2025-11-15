/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include <filesystem>
#include <ctime>
#include <iostream>

namespace kcenon::logger {

file_writer::file_writer(const std::string& filename, bool append, size_t buffer_size)
    : filename_(filename)
    , append_mode_(append)
    , buffer_size_(buffer_size)
    , buffer_(std::make_unique<char[]>(buffer_size)) {
    open();
}

file_writer::~file_writer() {
    close();
}

result_void file_writer::write(logger_system::log_level level,
                               const std::string& message,
                               const std::string& file,
                               int line,
                               const std::string& function,
                               const std::chrono::system_clock::time_point& timestamp) {
    std::lock_guard<std::mutex> lock(write_mutex_);

    return utils::try_write_operation([&]() -> result_void {
        // Check precondition
        auto check = utils::check_condition(
            file_stream_.is_open(),
            logger_error_code::file_write_failed,
            "File stream is not open"
        );
        if (!check) return check;

        // Create log_entry for new API
        log_entry entry = (!file.empty() || line != 0 || !function.empty())
            ? log_entry(level, message, file, line, function, timestamp)
            : log_entry(level, message, timestamp);

        // Format and write
        std::string formatted = format_log_entry(entry);
        file_stream_ << formatted << '\n';
        bytes_written_.fetch_add(formatted.size() + 1);  // +1 for newline

        // Verify stream state
        return utils::check_stream_state(file_stream_, "write");
    });
}

result_void file_writer::flush() {
    std::lock_guard<std::mutex> lock(write_mutex_);

    return utils::try_write_operation([&]() -> result_void {
        if (file_stream_.is_open()) {
            file_stream_.flush();
            return utils::check_stream_state(file_stream_, "flush");
        }
        return {}; // Success - no-op if file is not open
    }, logger_error_code::flush_timeout);
}

result_void file_writer::reopen() {
    std::lock_guard<std::mutex> lock(write_mutex_);
    close();
    return open();
}

void file_writer::close() {
    // IMPORTANT: Caller must hold write_mutex_ before calling this method
    // This ensures thread safety with concurrent write() operations
    // In debug builds, consider adding: assert(write_mutex_.try_lock() == false)

    if (file_stream_.is_open()) {
        file_stream_.flush();
        file_stream_.close();
    }
}

result_void file_writer::open() {
    // IMPORTANT: Caller must hold write_mutex_ before calling this method
    // This ensures thread safety with concurrent operations
    // In debug builds, consider adding: assert(write_mutex_.try_lock() == false)

    return utils::try_open_operation([&]() -> result_void {
        // Create directory if it doesn't exist
        std::filesystem::path file_path(filename_);
        std::filesystem::path dir = file_path.parent_path();

        auto dir_result = utils::ensure_directory_exists(dir);
        if (!dir_result) return dir_result;

        // Open file
        auto mode = append_mode_ ? std::ios::app : std::ios::trunc;
        file_stream_.open(filename_, std::ios::out | mode);

        // Check if file opened successfully
        auto check = utils::check_condition(
            file_stream_.is_open(),
            logger_error_code::file_open_failed,
            "Failed to open file: " + filename_
        );
        if (!check) return check;

        // Set buffer
        file_stream_.rdbuf()->pubsetbuf(buffer_.get(), buffer_size_);

        // Get current file size if appending
        if (append_mode_) {
            file_stream_.seekp(0, std::ios::end);
            bytes_written_ = file_stream_.tellp();
        } else {
            bytes_written_ = 0;
        }

        return {}; // Success
    });
}

} // namespace kcenon::logger