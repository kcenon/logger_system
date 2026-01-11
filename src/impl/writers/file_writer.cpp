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

common::VoidResult file_writer::write_entry_impl(const log_entry& entry) {
    // Note: Mutex is already held by thread_safe_writer::write()
    return utils::try_write_operation([&]() -> common::VoidResult {
        // Check precondition
        if (!file_stream_.is_open()) {
            return make_logger_void_result(logger_error_code::file_write_failed, "File stream is not open");
        }

        // Format and write - preserves all structured fields
        std::string formatted = format_log_entry(entry);
        file_stream_ << formatted << '\n';
        bytes_written_.fetch_add(formatted.size() + 1);  // +1 for newline

        // Verify stream state
        return utils::check_stream_state(file_stream_, "write");
    });
}

common::VoidResult file_writer::write_impl(logger_system::log_level level,
                                           const std::string& message,
                                           const std::string& file,
                                           int line,
                                           const std::string& function,
                                           const std::chrono::system_clock::time_point& timestamp) {
    // Legacy API: create a log_entry and delegate to write_entry_impl
    log_entry entry = (!file.empty() || line != 0 || !function.empty())
        ? log_entry(level, message, file, line, function, timestamp)
        : log_entry(level, message, timestamp);

    return write_entry_impl(entry);
}

common::VoidResult file_writer::flush_impl() {
    // Note: Mutex is already held by thread_safe_writer::flush()
    return utils::try_write_operation([&]() -> common::VoidResult {
        if (file_stream_.is_open()) {
            file_stream_.flush();
            return utils::check_stream_state(file_stream_, "flush");
        }
        return common::ok();
    }, logger_error_code::flush_timeout);
}

common::VoidResult file_writer::reopen() {
    std::lock_guard<std::mutex> lock(get_mutex());
    close();
    return open();
}

void file_writer::close() {
    // IMPORTANT: Caller must hold the mutex before calling this method
    // This ensures thread safety with concurrent write() operations

    if (file_stream_.is_open()) {
        file_stream_.flush();
        file_stream_.close();
    }
}

common::VoidResult file_writer::open() {
    // IMPORTANT: Caller must hold the mutex before calling this method
    // This ensures thread safety with concurrent operations

    return utils::try_open_operation([&]() -> common::VoidResult {
        // Create directory if it doesn't exist
        std::filesystem::path file_path(filename_);
        std::filesystem::path dir = file_path.parent_path();

        auto dir_result = utils::ensure_directory_exists(dir);
        if (dir_result.is_err()) return dir_result;

        // Open file
        auto mode = append_mode_ ? std::ios::app : std::ios::trunc;
        file_stream_.open(filename_, std::ios::out | mode);

        // Check if file opened successfully
        auto check = utils::check_condition(
            file_stream_.is_open(),
            logger_error_code::file_open_failed,
            "Failed to open file: " + filename_
        );
        if (check.is_err()) return check;

        // Set buffer
        file_stream_.rdbuf()->pubsetbuf(buffer_.get(), buffer_size_);

        // Get current file size if appending
        if (append_mode_) {
            file_stream_.seekp(0, std::ios::end);
            bytes_written_ = file_stream_.tellp();
        } else {
            bytes_written_ = 0;
        }

        return common::ok();
    });
}

} // namespace kcenon::logger