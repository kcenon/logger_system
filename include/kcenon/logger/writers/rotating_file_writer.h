#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "file_writer.h"
#include <chrono>
#include <vector>
#include <sstream>

namespace kcenon::logger {

/**
 * @enum rotation_type
 * @brief Determines when log rotation should occur
 */
enum class rotation_type {
    size,           ///< Rotate based on file size only
    daily,          ///< Rotate daily at midnight
    hourly,         ///< Rotate every hour
    size_and_time   ///< Rotate based on both size and time
};

/**
 * @class rotating_file_writer
 * @brief File writer with automatic log rotation support
 *
 * @details This writer extends file_writer to add automatic log rotation
 * based on file size, time intervals, or both. When rotation occurs,
 * the current log file is renamed with a timestamp or index, and a new
 * file is created.
 *
 * Rotation strategies:
 * - Size-based: Rotates when file reaches max_size bytes
 * - Time-based: Rotates daily or hourly
 * - Combined: Rotates when either condition is met
 *
 * Thread Safety:
 * - All public methods are thread-safe
 * - Uses mutex inherited from file_writer for write operations
 *
 * @since 1.0.0
 */
class rotating_file_writer : public file_writer {
public:
    /**
     * @brief Construct with size-based rotation
     * @param filename Path to the log file
     * @param max_size Maximum file size in bytes before rotation
     * @param max_files Maximum number of backup files to keep
     * @param check_interval Number of writes between rotation checks (default: 100)
     */
    rotating_file_writer(const std::string& filename,
                        size_t max_size,
                        size_t max_files,
                        size_t check_interval = 100);

    /**
     * @brief Construct with time-based rotation
     * @param filename Path to the log file
     * @param type Rotation type (daily or hourly)
     * @param max_files Maximum number of backup files to keep
     * @param check_interval Number of writes between rotation checks (default: 100)
     */
    rotating_file_writer(const std::string& filename,
                        rotation_type type,
                        size_t max_files,
                        size_t check_interval = 100);

    /**
     * @brief Construct with combined size and time rotation
     * @param filename Path to the log file
     * @param type Must be rotation_type::size_and_time
     * @param max_size Maximum file size in bytes before rotation
     * @param max_files Maximum number of backup files to keep
     * @param check_interval Number of writes between rotation checks (default: 100)
     * @throws std::invalid_argument if type is not size_and_time
     */
    rotating_file_writer(const std::string& filename,
                        rotation_type type,
                        size_t max_size,
                        size_t max_files,
                        size_t check_interval = 100);

    /**
     * @brief Write log entry with automatic rotation check
     */
    common::VoidResult write(logger_system::log_level level,
                             const std::string& message,
                             const std::string& file,
                             int line,
                             const std::string& function,
                             const std::chrono::system_clock::time_point& timestamp) override;

    /**
     * @brief Get writer name
     */
    std::string get_name() const override { return "rotating_file"; }

    /**
     * @brief Manually trigger log rotation
     */
    void rotate();

private:
    /**
     * @brief Check if rotation should occur
     */
    bool should_rotate() const;

    /**
     * @brief Perform the actual rotation operation
     * @note Caller must hold write_mutex_
     */
    void perform_rotation();

    /**
     * @brief Generate filename for rotated log
     * @param index Optional index for size-based rotation (-1 for auto)
     */
    std::string generate_rotated_filename(int index = -1) const;

    /**
     * @brief Remove old backup files beyond max_files limit
     */
    void cleanup_old_files();

    /**
     * @brief Get list of existing backup files
     */
    std::vector<std::string> get_backup_files() const;

    /**
     * @brief Check if time-based rotation should occur
     */
    bool should_rotate_by_time() const;

    /**
     * @brief Get current file size from filesystem
     */
    std::size_t get_file_size() const;

private:
    rotation_type rotation_type_;
    size_t max_size_;
    size_t max_files_;
    size_t check_interval_;           ///< Number of writes between rotation checks
    size_t writes_since_check_{0};    ///< Counter for writes since last check
    std::string base_filename_;
    std::string file_extension_;
    std::chrono::system_clock::time_point last_rotation_time_;
    std::chrono::system_clock::time_point current_period_start_;
};

} // namespace kcenon::logger
