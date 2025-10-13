/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/writers/rotating_file_writer.h>
#include <filesystem>
#include <algorithm>
#include <regex>
#include <ctime>
#include <iostream>

namespace kcenon::logger {

rotating_file_writer::rotating_file_writer(const std::string& filename,
                                         size_t max_size,
                                         size_t max_files)
    : file_writer(filename, true)
    , rotation_type_(rotation_type::size)
    , max_size_(max_size)
    , max_files_(max_files)
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
                                         size_t max_files)
    : file_writer(filename, true)
    , rotation_type_(type)
    , max_size_(0)
    , max_files_(max_files)
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
                                         size_t max_files)
    : file_writer(filename, true)
    , rotation_type_(type)
    , max_size_(max_size)
    , max_files_(max_files)
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

result_void rotating_file_writer::write(logger_system::log_level level,
                                        const std::string& message,
                                        const std::string& file,
                                        int line,
                                        const std::string& function,
                                        const std::chrono::system_clock::time_point& timestamp) {
    // Lock the mutex first to ensure atomic check-and-rotate operation
    std::lock_guard<std::mutex> lock(write_mutex_);

    // Write to file with mutex already held
    if (!file_stream_.is_open()) {
        return make_logger_error(logger_error_code::file_write_failed,
                                "File stream is not open");
    }

    std::string formatted = format_log_entry(level, message, file, line, function, timestamp);

    try {
        file_stream_ << formatted << std::endl;
        bytes_written_.fetch_add(formatted.size() + 1);

        if (!file_stream_.good()) {
            return make_logger_error(logger_error_code::file_write_failed,
                                    "Failed to write to file stream");
        }

        // Check if rotation is needed after writing
        // This ensures we can accurately determine if the size threshold is exceeded
        if (should_rotate()) {
            perform_rotation();
        }

        return {};
    } catch (const std::exception& e) {
        return make_logger_error(logger_error_code::file_write_failed, e.what());
    }
}

void rotating_file_writer::rotate() {
    // Public API for manual rotation
    std::lock_guard<std::mutex> lock(write_mutex_);
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
    // IMPORTANT: Caller must hold write_mutex_ before calling this method
    // This ensures thread safety for all file operations and mutable state modifications

    // Close current file
    if (file_stream_.is_open()) {
        file_stream_.flush();
        file_stream_.close();
    }

    // Generate new filename for the current log
    std::string rotated_name = generate_rotated_filename();

    // Rename current file
    try {
        if (std::filesystem::exists(filename_)) {
            std::filesystem::rename(filename_, rotated_name);
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to rotate log file: " << e.what() << std::endl;
    }

    // Clean up old files
    cleanup_old_files();

    // Open new file
    try {
        std::filesystem::path file_path(filename_);
        std::filesystem::path dir = file_path.parent_path();
        if (!dir.empty() && !std::filesystem::exists(dir)) {
            std::filesystem::create_directories(dir);
        }

        auto mode = append_mode_ ? std::ios::app : std::ios::trunc;
        file_stream_.open(filename_, std::ios::out | mode);

        if (file_stream_.is_open()) {
            file_stream_.rdbuf()->pubsetbuf(buffer_.get(), buffer_size_);
            bytes_written_ = 0;
        }
    } catch (const std::exception& e) {
        std::cerr << "Failed to open new log file: " << e.what() << std::endl;
    }

    // Update rotation time - protected by mutex
    last_rotation_time_ = std::chrono::system_clock::now();
    current_period_start_ = last_rotation_time_;
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
            try {
                std::filesystem::remove(backup_files[i]);
            } catch (const std::exception& e) {
                std::cerr << "Failed to remove old log file: " << e.what() << std::endl;
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
    
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dir)) {
            if (entry.is_regular_file()) {
                std::string filename = entry.path().filename().string();
                if (std::regex_match(filename, backup_regex)) {
                    files.push_back(entry.path().string());
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error listing backup files: " << e.what() << std::endl;
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
    if (!file_stream_.is_open()) {
        return 0;
    }

    // Use filesystem to get actual file size
    std::error_code ec;
    auto size = std::filesystem::file_size(filename_, ec);
    if (ec) {
        // If filesystem::file_size fails, fall back to bytes_written atomic counter
        return bytes_written_.load();
    }

    return static_cast<std::size_t>(size);
}

} // namespace kcenon::logger