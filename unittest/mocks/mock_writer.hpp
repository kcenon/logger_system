/**
 * @file mock_writer.hpp
 * @brief Mock writer implementation for testing
 * @date 2025-09-09
 *
 * BSD 3-Clause License
 * Copyright (c) 2025, kcenon
 * All rights reserved.
 */

#pragma once

#include "../../sources/logger/writers/base_writer.h"
#include <atomic>
#include <vector>
#include <mutex>
#include <chrono>

namespace logger_system::testing {

/**
 * @brief Mock writer for unit testing
 * 
 * Provides controllable behavior for testing various scenarios
 * including success, failure, delays, and tracking write operations.
 */
class mock_writer : public base_writer {
public:
    struct write_record {
        log_entry entry;
        std::chrono::steady_clock::time_point timestamp;
    };

private:
    mutable std::mutex mutex_;
    std::vector<write_record> written_entries_;
    std::atomic<size_t> write_count_{0};
    std::atomic<size_t> flush_count_{0};
    std::atomic<bool> should_fail_{false};
    std::atomic<bool> is_open_{true};
    std::chrono::milliseconds write_delay_{0};
    error_code failure_error_{error_code::write_failed};

public:
    mock_writer() = default;
    ~mock_writer() override = default;

    // base_writer interface implementation
    result_void write(const log_entry& entry) override {
        if (should_fail_.load()) {
            return failure_error_;
        }

        if (!is_open_.load()) {
            return error_code::writer_closed;
        }

        if (write_delay_.count() > 0) {
            std::this_thread::sleep_for(write_delay_);
        }

        {
            std::lock_guard<std::mutex> lock(mutex_);
            written_entries_.push_back({entry, std::chrono::steady_clock::now()});
        }
        
        write_count_.fetch_add(1);
        return {};
    }

    result_void flush() override {
        if (should_fail_.load()) {
            return failure_error_;
        }

        flush_count_.fetch_add(1);
        return {};
    }

    result_void open() override {
        is_open_.store(true);
        return {};
    }

    result_void close() override {
        is_open_.store(false);
        return {};
    }

    bool is_thread_safe() const override {
        return true;
    }

    // Mock control methods
    void set_should_fail(bool fail, error_code error = error_code::write_failed) {
        should_fail_.store(fail);
        failure_error_ = error;
    }

    void set_write_delay(std::chrono::milliseconds delay) {
        write_delay_ = delay;
    }

    void reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        written_entries_.clear();
        write_count_.store(0);
        flush_count_.store(0);
        should_fail_.store(false);
        is_open_.store(true);
        write_delay_ = std::chrono::milliseconds(0);
    }

    // Inspection methods
    size_t get_write_count() const {
        return write_count_.load();
    }

    size_t get_flush_count() const {
        return flush_count_.load();
    }

    std::vector<write_record> get_written_entries() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return written_entries_;
    }

    bool has_entry_with_message(const std::string& message) const {
        std::lock_guard<std::mutex> lock(mutex_);
        return std::any_of(written_entries_.begin(), written_entries_.end(),
            [&message](const write_record& record) {
                return record.entry.message == message;
            });
    }

    log_entry get_last_entry() const {
        std::lock_guard<std::mutex> lock(mutex_);
        if (written_entries_.empty()) {
            throw std::runtime_error("No entries written");
        }
        return written_entries_.back().entry;
    }
};

/**
 * @brief Factory for creating mock writers
 */
class mock_writer_factory {
private:
    std::vector<std::shared_ptr<mock_writer>> created_writers_;

public:
    std::shared_ptr<mock_writer> create_writer() {
        auto writer = std::make_shared<mock_writer>();
        created_writers_.push_back(writer);
        return writer;
    }

    std::vector<std::shared_ptr<mock_writer>> get_all_writers() const {
        return created_writers_;
    }

    void reset_all() {
        for (auto& writer : created_writers_) {
            writer->reset();
        }
    }

    size_t get_total_write_count() const {
        size_t total = 0;
        for (const auto& writer : created_writers_) {
            total += writer->get_write_count();
        }
        return total;
    }
};

} // namespace logger_system::testing