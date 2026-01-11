/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include <filesystem>
#include <algorithm>
#include <regex>
#include <ctime>
#include <iostream>

namespace kcenon::logger {

rotating_file_writer::rotating_file_writer(const std::string& filename,
                                         size_t max_size,
                                         size_t max_files,
                                         size_t check_interval)
    : file_writer(filename, true)
    , rotation_type_(rotation_type::size)
    , max_size_(max_size)
    , max_files_(max_files)
    , check_interval_(check_interval)
    , last_rotation_time_(std::chrono::system_clock::now())
    , current_period_start_(std::chrono::system_clock::now()) {

    // Extract base filename and extension
    std::filesystem::path path(filename);
    base_filename_ = path.stem().string();
    file_extension_ = path.extension().string();
    if (file_extension_.empty()) {
        file_extension_ = ".log";
    }
}

rotating_file_writer::rotating_file_writer(const std::string& filename,
                                         rotation_type type,
                                         size_t max_files,
                                         size_t check_interval)
    : file_writer(filename, true)
    , rotation_type_(type)
    , max_size_(0)
    , max_files_(max_files)
    , check_interval_(check_interval)
    , last_rotation_time_(std::chrono::system_clock::now())
    , current_period_start_(std::chrono::system_clock::now()) {

    // Extract base filename and extension
    std::filesystem::path path(filename);
    base_filename_ = path.stem().string();
    file_extension_ = path.extension().string();
    if (file_extension_.empty()) {
        file_extension_ = ".log";
    }
}

rotating_file_writer::rotating_file_writer(const std::string& filename,
                                         rotation_type type,
                                         size_t max_size,
                                         size_t max_files,
                                         size_t check_interval)
    : file_writer(filename, true)
    , rotation_type_(type)
    , max_size_(max_size)
    , max_files_(max_files)
    , check_interval_(check_interval)
    , last_rotation_time_(std::chrono::system_clock::now())
    , current_period_start_(std::chrono::system_clock::now()) {

    if (type != rotation_type::size_and_time) {
        throw std::invalid_argument("This constructor is only for size_and_time rotation");
    }

    // Extract base filename and extension
    std::filesystem::path path(filename);
    base_filename_ = path.stem().string();
    file_extension_ = path.extension().string();
    if (file_extension_.empty()) {
        file_extension_ = ".log";
    }
}

common::VoidResult rotating_file_writer::write_entry_impl(const log_entry& entry) {
    // Note: Mutex is already held by thread_safe_writer::write()
    // Check precondition
    if (!file_stream_.is_open()) {
        return make_logger_void_result(logger_error_code::file_write_failed, "File stream is not open");
    }

    // Format and write - preserves all structured fields
    std::string formatted = format_log_entry(entry);
    file_stream_ << formatted << '\n';
    bytes_written_.fetch_add(formatted.size() + 1);

    // Verify stream state
    if (file_stream_.fail()) {
        return make_logger_void_result(logger_error_code::file_write_failed, "Write failed");
    }

    // Periodic rotation check optimization (Phase 2)
    ++writes_since_check_;

    if (writes_since_check_ >= check_interval_) {
        if (should_rotate()) {
            perform_rotation();
        }
        writes_since_check_ = 0;
    }

    return common::ok();
}

common::VoidResult rotating_file_writer::write_impl(logger_system::log_level level,
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

void rotating_file_writer::rotate() {
    // Public API for manual rotation
    std::lock_guard<std::mutex> lock(get_mutex());
    perform_rotation();
}

bool rotating_file_writer::should_rotate() const {
    switch (rotation_type_) {
        case rotation_type::size:
            return get_file_size() >= max_size_;
            
        case rotation_type::daily:
        case rotation_type::hourly:
            return should_rotate_by_time();
            
        case rotation_type::size_and_time:
            return get_file_size() >= max_size_ || should_rotate_by_time();
            
        default:
            return false;
    }
}

void rotating_file_writer::perform_rotation() {
    // IMPORTANT: Caller must hold the mutex before calling this method
    // This ensures thread safety for all file operations and mutable state modifications

    // Close current file
    if (file_stream_.is_open()) {
        file_stream_.flush();
        file_stream_.close();
    }

    // Generate new filename for the current log
    std::string rotated_name = generate_rotated_filename();

    // Rename current file (with error handling)
    auto rename_result = utils::try_write_operation([&]() -> common::VoidResult {
        if (std::filesystem::exists(filename_)) {
            std::filesystem::rename(filename_, rotated_name);
        }
        return common::ok();
    }, logger_error_code::file_rotation_failed);

    if (rename_result.is_err()) {
        std::cerr << "Failed to rotate log file: " << rename_result.error().message << std::endl;
    }

    // Clean up old files
    cleanup_old_files();

    // Open new file (with error handling)
    auto open_result = utils::try_open_operation([&]() -> common::VoidResult {
        std::filesystem::path file_path(filename_);
        std::filesystem::path dir = file_path.parent_path();

        auto dir_result = utils::ensure_directory_exists(dir);
        if (dir_result.is_err()) return dir_result;

        auto mode = append_mode_ ? std::ios::app : std::ios::trunc;
        file_stream_.open(filename_, std::ios::out | mode);

        if (file_stream_.is_open()) {
            file_stream_.rdbuf()->pubsetbuf(buffer_.get(), buffer_size_);
            bytes_written_ = 0;
        }

        return common::ok();
    });

    if (open_result.is_err()) {
        std::cerr << "Failed to open new log file: " << open_result.error().message << std::endl;
    }

    // Update rotation time - protected by mutex
    last_rotation_time_ = std::chrono::system_clock::now();
    current_period_start_ = last_rotation_time_;

    // Reset write counter after rotation
    writes_since_check_ = 0;
}

std::string rotating_file_writer::generate_rotated_filename(int index) const {
    std::ostringstream oss;
    std::filesystem::path dir = std::filesystem::path(filename_).parent_path();

    if (!dir.empty()) {
        oss << dir.string() << "/";
    }

    // Build: base + extension + separator + timestamp/index
    // Example: "test.log.1" instead of "test.1.log"
    oss << base_filename_ << file_extension_;

    // Add timestamp or index
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);

    // Use thread-safe time conversion
    std::tm tm_buf{};
#ifdef _WIN32
    localtime_s(&tm_buf, &time_t);  // Windows thread-safe version
#else
    localtime_r(&time_t, &tm_buf);  // POSIX thread-safe version
#endif

    switch (rotation_type_) {
        case rotation_type::size:
            if (index >= 0) {
                oss << "." << index;
            } else {
                // Find next available index
                std::string base_with_ext = oss.str();
                int next_index = 1;
                while (std::filesystem::exists(base_with_ext + "." + std::to_string(next_index))) {
                    next_index++;
                }
                oss << "." << next_index;
            }
            break;

        case rotation_type::daily:
            oss << "." << std::put_time(&tm_buf, "%Y%m%d");
            break;

        case rotation_type::hourly:
            oss << "." << std::put_time(&tm_buf, "%Y%m%d_%H");
            break;

        case rotation_type::size_and_time:
            oss << "." << std::put_time(&tm_buf, "%Y%m%d_%H%M%S");
            break;
    }

    return oss.str();
}

void rotating_file_writer::cleanup_old_files() {
    auto backup_files = get_backup_files();

    if (backup_files.size() > max_files_) {
        // Sort by modification time (oldest first)
        std::sort(backup_files.begin(), backup_files.end(),
                 [](const std::string& a, const std::string& b) {
                     return std::filesystem::last_write_time(a) <
                            std::filesystem::last_write_time(b);
                 });

        // Remove oldest files
        size_t files_to_remove = backup_files.size() - max_files_;
        for (size_t i = 0; i < files_to_remove; ++i) {
            auto remove_result = utils::try_write_operation([&]() -> common::VoidResult {
                std::filesystem::remove(backup_files[i]);
                return common::ok();
            }, logger_error_code::file_rotation_failed);

            if (remove_result.is_err()) {
                std::cerr << "Failed to remove old log file: " << remove_result.error().message << std::endl;
            }
        }
    }
}

std::vector<std::string> rotating_file_writer::get_backup_files() const {
    std::vector<std::string> files;
    std::filesystem::path dir = std::filesystem::path(filename_).parent_path();
    if (dir.empty()) {
        dir = ".";
    }

    // Create regex pattern for backup files
    // Pattern: base_filename + file_extension + "." + (number or timestamp)
    // Example: "test_rotating.log.1" or "test.log.20250108"
    std::string escaped_ext = std::regex_replace(file_extension_, std::regex(R"(\.)"), R"(\\.)");
    std::string pattern = base_filename_ + escaped_ext + R"(\.(\d+|\d{8}|\d{8}_\d{2}|\d{8}_\d{6}))";
    std::regex backup_regex(pattern);

    // Use error handling utility for directory iteration
    auto result = utils::try_write_operation([&]() -> common::VoidResult {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (std::regex_match(filename, backup_regex)) {
                    files.push_back(entry.path().string());
                }
            }
        }
        return common::ok();
    }, logger_error_code::file_rotation_failed);

    if (result.is_err()) {
        std::cerr << "Error listing backup files: " << result.error().message << std::endl;
    }

    return files;
}

bool rotating_file_writer::should_rotate_by_time() const {
    auto now = std::chrono::system_clock::now();
    
    switch (rotation_type_) {
        case rotation_type::daily:
        case rotation_type::size_and_time: {
            // Check if we're in a new day
            auto now_time_t = std::chrono::system_clock::to_time_t(now);
            auto start_time_t = std::chrono::system_clock::to_time_t(current_period_start_);

            // Use thread-safe time conversion
            std::tm now_tm{};
            std::tm start_tm{};
#ifdef _WIN32
            localtime_s(&now_tm, &now_time_t);
            localtime_s(&start_tm, &start_time_t);
#else
            localtime_r(&now_time_t, &now_tm);
            localtime_r(&start_time_t, &start_tm);
#endif

            return now_tm.tm_year != start_tm.tm_year ||
                   now_tm.tm_mon != start_tm.tm_mon ||
                   now_tm.tm_mday != start_tm.tm_mday;
        }
        
        case rotation_type::hourly: {
            // Check if we're in a new hour
            auto duration = now - current_period_start_;
            return duration >= std::chrono::hours(1);
        }
        
        default:
            return false;
    }
}

std::size_t rotating_file_writer::get_file_size() const {
    // IMPORTANT: This method should only be called while holding the mutex
    // to avoid race conditions with concurrent writes and file rotation.
    //
    // Race condition example:
    // - Thread A: Calls should_rotate() -> get_file_size() reads filesystem
    // - Thread B: Simultaneously writing, bytes_written_ is updating
    // - Result: get_file_size() returns stale data, rotation check is incorrect
    //
    // Better approach: Always use bytes_written_.load() which is atomic and
    // thread-safe. Only call filesystem functions during actual rotation when
    // the caller already holds the mutex.

    if (!file_stream_.is_open()) {
        return 0;
    }

    // Prefer atomic counter for thread safety
    // Only fall back to filesystem if absolutely necessary
    std::size_t atomic_size = bytes_written_.load(std::memory_order_relaxed);

    // Optional: Validate with filesystem size for debugging
    // (Remove in production for performance)
    #ifdef DEBUG_FILE_SIZE_VALIDATION
    std::error_code ec;
    auto fs_size = std::filesystem::file_size(filename_, ec);
    if (!ec && fs_size != atomic_size) {
        // Log discrepancy for debugging
        std::cerr << "File size mismatch: atomic=" << atomic_size
                  << " filesystem=" << fs_size << std::endl;
    }
    #endif

    return atomic_size;
}

} // namespace kcenon::logger