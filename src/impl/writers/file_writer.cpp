/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include <filesystem>
#include <iostream>

namespace kcenon::logger {

file_writer::file_writer(const std::string& filename,
                        bool append,
                        std::unique_ptr<log_formatter_interface> formatter)
    : base_writer(formatter ? std::move(formatter) : std::make_unique<timestamp_formatter>())
    , filename_(filename)
    , append_mode_(append) {
    std::lock_guard<std::mutex> lock(mutex_);
    open_internal();
}

file_writer::~file_writer() {
    std::lock_guard<std::mutex> lock(mutex_);
    close_internal();
}

common::VoidResult file_writer::write(const log_entry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);

    return utils::try_write_operation([&]() -> common::VoidResult {
        // Check precondition
        if (!is_open_) {
            return make_logger_void_result(logger_error_code::file_write_failed, "File is not open");
        }

        // Format and write
        std::string formatted = format_entry(entry);
        file_stream_ << formatted << '\n';
        bytes_written_.fetch_add(formatted.size() + 1);  // +1 for newline

        // Verify stream state
        return utils::check_stream_state(file_stream_, "write");
    });
}

common::VoidResult file_writer::flush() {
    std::lock_guard<std::mutex> lock(mutex_);

    return utils::try_write_operation([&]() -> common::VoidResult {
        if (is_open_) {
            file_stream_.flush();
            return utils::check_stream_state(file_stream_, "flush");
        }
        return common::ok();
    }, logger_error_code::flush_timeout);
}

common::VoidResult file_writer::close() {
    std::lock_guard<std::mutex> lock(mutex_);
    close_internal();
    return common::ok();
}

bool file_writer::is_healthy() const {
    return is_open_ && file_stream_.good();
}

std::string file_writer::format_entry(const log_entry& entry) const {
    return format_log_entry(entry);
}

common::VoidResult file_writer::open_internal() {
    // IMPORTANT: Caller must hold the mutex before calling this method

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

        // Get current file size if appending
        if (append_mode_) {
            file_stream_.seekp(0, std::ios::end);
            bytes_written_ = file_stream_.tellp();
        } else {
            bytes_written_ = 0;
        }

        is_open_ = true;
        return common::ok();
    });
}

void file_writer::close_internal() {
    // IMPORTANT: Caller must hold the mutex before calling this method

    if (is_open_) {
        if (file_stream_.is_open()) {
            file_stream_.flush();
            file_stream_.close();
        }
        is_open_ = false;
    }
}

} // namespace kcenon::logger