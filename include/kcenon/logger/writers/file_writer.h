#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "../interfaces/log_writer_interface.h"
#include "../interfaces/log_formatter_interface.h"
#include "../interfaces/writer_category.h"
#include <fstream>
#include <atomic>
#include <memory>
#include <mutex>

namespace kcenon::logger {

/**
 * @class file_writer
 * @brief Core file writer for logging to files
 *
 * Pure file I/O implementation with direct mutex management.
 * Designed to serve as the base layer in Decorator pattern compositions.
 *
 * Thread-safe with internal mutex synchronization.
 *
 * Category: Synchronous (blocking I/O to file)
 *
 * @since 1.0.0
 * @since 1.3.0 Refactored to use thread_safe_writer base class
 * @since 1.4.0 Added sync_writer_tag for category classification
 * @since 4.0.0 Refactored for Decorator pattern with simplified inheritance
 * @since 4.1.0 Directly implements log_writer_interface as core writer
 */
class file_writer : public log_writer_interface, public sync_writer_tag {
public:
    /**
     * @brief Constructor
     * @param filename Path to the log file
     * @param append Whether to append to existing file (default: true)
     * @param formatter Custom log formatter (default: timestamp formatter)
     */
    explicit file_writer(const std::string& filename,
                        bool append = true,
                        std::unique_ptr<log_formatter_interface> formatter = nullptr);

    /**
     * @brief Destructor
     */
    ~file_writer() override;

    // Non-copyable and non-movable
    file_writer(const file_writer&) = delete;
    file_writer& operator=(const file_writer&) = delete;
    file_writer(file_writer&&) = delete;
    file_writer& operator=(file_writer&&) = delete;

    /**
     * @brief Write a log entry to file
     * @param entry The log entry to write
     * @return common::VoidResult Success or error code
     */
    common::VoidResult write(const log_entry& entry) override;

    /**
     * @brief Flush file stream
     * @return common::VoidResult Success or error code
     */
    common::VoidResult flush() override;

    /**
     * @brief Close the file
     * @return common::VoidResult Success or error code
     */
    common::VoidResult close() override;

    /**
     * @brief Get writer name
     */
    std::string get_name() const override { return "file"; }

    /**
     * @brief Check if file is open
     */
    [[nodiscard]] bool is_open() const override { return is_open_; }

    /**
     * @brief Check if writer is healthy
     */
    bool is_healthy() const override;

    /**
     * @brief Get current file size
     */
    size_t get_file_size() const { return bytes_written_.load(); }

protected:
    /**
     * @brief Format a log entry using the current formatter
     */
    std::string format_entry(const log_entry& entry) const;

    /**
     * @brief Open the file (internal, caller must hold mutex)
     */
    common::VoidResult open_internal();

    /**
     * @brief Close the file (internal, caller must hold mutex)
     */
    void close_internal();

    /**
     * @brief Access the writer mutex for extended operations
     * @return Reference to the internal mutex
     */
    std::mutex& get_mutex() const { return mutex_; }

protected:
    std::string filename_;
    bool append_mode_;

    std::ofstream file_stream_;
    std::atomic<bool> is_open_{false};
    std::atomic<size_t> bytes_written_{0};

    std::unique_ptr<log_formatter_interface> formatter_;
    mutable std::mutex mutex_;
};

} // namespace kcenon::logger