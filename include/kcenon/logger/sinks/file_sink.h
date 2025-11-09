#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file file_sink.h
 * @brief File output sink implementation
 * @since 1.3.0
 * @version Sprint 5-6 - Writer Refactoring
 */

#include <fstream>
#include <mutex>
#include <filesystem>
#include "../interfaces/output_sink_interface.h"

namespace kcenon::logger {

/**
 * @class file_sink
 * @brief Writes log messages to a file
 *
 * @details Thread-safe file output sink. Handles file I/O only,
 * no formatting logic.
 *
 * @thread_safety All methods are thread-safe.
 *
 * @since 1.3.0
 */
class file_sink : public output_sink_interface {
public:
    /**
     * @brief Construct a file sink
     * @param file_path Path to the output file
     * @param append If true, append to existing file; if false, truncate
     */
    explicit file_sink(const std::string& file_path, bool append = true)
        : file_path_(file_path)
        , is_healthy_(false)
    {
        auto mode = append ? (std::ios::out | std::ios::app) : std::ios::out;
        file_.open(file_path, mode);
        is_healthy_ = file_.is_open() && file_.good();
    }

    ~file_sink() override {
        if (file_.is_open()) {
            file_.flush();
            file_.close();
        }
    }

    result_void write_raw(std::string_view message) override {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!file_.is_open() || !file_.good()) {
            is_healthy_ = false;
            return logger_error_code::write_failed;
        }

        file_ << message;

        if (file_.fail()) {
            is_healthy_ = false;
            return logger_error_code::write_failed;
        }

        return {};
    }

    result_void flush() override {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!file_.is_open()) {
            return logger_error_code::write_failed;
        }

        file_.flush();

        if (file_.fail()) {
            is_healthy_ = false;
            return logger_error_code::write_failed;
        }

        return {};
    }

    bool is_healthy() const override {
        return is_healthy_ && file_.is_open() && file_.good();
    }

    std::string get_name() const override {
        return "file";
    }

    std::string get_info() const override {
        std::lock_guard<std::mutex> lock(mutex_);
        std::string info = "file: " + file_path_;
        if (file_.is_open()) {
            try {
                auto size = std::filesystem::file_size(file_path_);
                info += ", size: " + std::to_string(size / 1024) + "KB";
            } catch (...) {
                // Ignore file size error
            }
        }
        return info;
    }

    /**
     * @brief Get the file path
     * @return File path
     */
    std::string get_file_path() const {
        return file_path_;
    }

private:
    std::string file_path_;
    std::ofstream file_;
    mutable std::mutex mutex_;
    bool is_healthy_;
};

} // namespace kcenon::logger
