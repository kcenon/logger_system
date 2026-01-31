#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "thread_safe_writer.h"
#include "../interfaces/writer_category.h"
#include <fstream>
#include <atomic>
#include <memory>

namespace kcenon::logger {

/**
 * @class file_writer
 * @brief Basic file writer for logging to files
 *
 * Thread-safe file output with automatic mutex management via thread_safe_writer.
 *
 * Category: Synchronous (blocking I/O to file)
 *
 * @since 1.0.0
 * @since 1.3.0 Refactored to use thread_safe_writer base class
 * @since 1.4.0 Added sync_writer_tag for category classification
 */
class file_writer : public thread_safe_writer, public sync_writer_tag {
public:
    /**
     * @brief Constructor
     * @param filename Path to the log file
     * @param append Whether to append to existing file (default: true)
     * @param buffer_size Output buffer size in bytes (default: 8192)
     */
    explicit file_writer(const std::string& filename,
                        bool append = true,
                        size_t buffer_size = 8192);
    
    /**
     * @brief Destructor
     */
    ~file_writer() override;
    
    /**
     * @brief Get writer name
     */
    std::string get_name() const override { return "file"; }

    /**
     * @brief Check if file is open
     */
    bool is_open() const override { return file_stream_.is_open(); }

    /**
     * @brief Get current file size
     */
    size_t get_file_size() const { return bytes_written_.load(); }

    /**
     * @brief Reopen the file (thread-safe)
     */
    common::VoidResult reopen();

protected:
    /**
     * @brief Implementation of structured write operation
     * @param entry The log entry to write (includes structured fields)
     * @note Called by thread_safe_writer::write(const log_entry&) while holding the mutex
     * @since 3.4.0
     * @since 3.5.0 This is now the only write implementation (legacy API removed)
     */
    common::VoidResult write_entry_impl(const log_entry& entry) override;

    /**
     * @brief Implementation of flush operation
     * @note Called by thread_safe_writer::flush() while holding the mutex
     */
    common::VoidResult flush_impl() override;

    /**
     * @brief Close the current file (internal)
     * @note Caller must hold the mutex
     */
    void close_internal();

    /**
     * @brief Open the file
     * @note Caller must hold the mutex
     */
    common::VoidResult open();

protected:
    std::string filename_;
    bool append_mode_;
    size_t buffer_size_;

    std::ofstream file_stream_;
    std::unique_ptr<char[]> buffer_;

    std::atomic<size_t> bytes_written_{0};
};

} // namespace kcenon::logger