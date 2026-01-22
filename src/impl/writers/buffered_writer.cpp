/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/**
 * @file buffered_writer.cpp
 * @brief Implementation of buffered_writer decorator
 * @author kcenon
 * @since 4.0.0
 */

#include <kcenon/logger/writers/buffered_writer.h>

#include <stdexcept>

namespace kcenon::logger {

buffered_writer::buffered_writer(std::unique_ptr<log_writer_interface> wrapped,
                                 size_t max_entries,
                                 std::chrono::milliseconds flush_interval)
    : decorator_writer_base(std::move(wrapped), "buffered")
    , max_entries_(max_entries)
    , flush_interval_(flush_interval)
    , last_flush_time_(std::chrono::steady_clock::now()) {
    if (max_entries_ == 0) {
        throw std::invalid_argument("buffered_writer: max_entries must be greater than 0");
    }
    buffer_.reserve(max_entries_);
}

buffered_writer::~buffered_writer() {
    // Flush remaining entries - ignore errors as we can't report them
    std::lock_guard<std::mutex> lock(mutex_);
    if (!buffer_.empty()) {
        flush_buffer_unsafe();
    }
}

common::VoidResult buffered_writer::write(const log_entry& entry) {
    std::lock_guard<std::mutex> lock(mutex_);

    // Add entry to buffer
    buffer_.push_back(copy_entry(entry));
    stats_.total_entries_written.fetch_add(1, std::memory_order_relaxed);

    // Check if we need to flush
    bool flush_needed = false;
    bool flush_by_size = buffer_.size() >= max_entries_;
    bool flush_by_time = should_flush_by_time();

    if (flush_by_size) {
        stats_.flush_on_full.fetch_add(1, std::memory_order_relaxed);
        flush_needed = true;
    } else if (flush_by_time) {
        stats_.flush_on_interval.fetch_add(1, std::memory_order_relaxed);
        flush_needed = true;
    }

    if (flush_needed) {
        return flush_buffer_unsafe();
    }

    return common::ok();
}

common::VoidResult buffered_writer::flush() {
    std::lock_guard<std::mutex> lock(mutex_);

    if (!buffer_.empty()) {
        stats_.manual_flushes.fetch_add(1, std::memory_order_relaxed);
    }

    return flush_buffer_unsafe();
}

common::VoidResult buffered_writer::flush_buffer_unsafe() {
    // Write all buffered entries to wrapped writer
    for (const auto& entry : buffer_) {
        auto result = wrapped().write(entry);
        if (result.is_err()) {
            // Clear buffer even on error to avoid infinite retry
            buffer_.clear();
            last_flush_time_ = std::chrono::steady_clock::now();
            return result;
        }
    }

    buffer_.clear();
    last_flush_time_ = std::chrono::steady_clock::now();

    if (!buffer_.empty() || stats_.total_flushes > 0) {
        stats_.total_flushes.fetch_add(1, std::memory_order_relaxed);
    }

    // Delegate to wrapped writer's flush
    return wrapped().flush();
}

bool buffered_writer::should_flush_by_time() const {
    if (flush_interval_.count() == 0) {
        return false;  // Time-based flush disabled
    }

    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - last_flush_time_);
    return elapsed >= flush_interval_;
}

log_entry buffered_writer::copy_entry(const log_entry& entry) {
    if (entry.location) {
        return log_entry(entry.level,
                         entry.message.to_string(),
                         entry.location->file.to_string(),
                         entry.location->line,
                         entry.location->function.to_string(),
                         entry.timestamp);
    }
    return log_entry(entry.level, entry.message.to_string(), entry.timestamp);
}

size_t buffered_writer::get_buffer_count() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return buffer_.size();
}

size_t buffered_writer::get_max_entries() const noexcept {
    return max_entries_;
}

std::chrono::milliseconds buffered_writer::get_flush_interval() const noexcept {
    return flush_interval_;
}

const buffered_writer::stats& buffered_writer::get_stats() const noexcept {
    return stats_;
}

void buffered_writer::reset_stats() {
    stats_.total_entries_written.store(0, std::memory_order_relaxed);
    stats_.total_flushes.store(0, std::memory_order_relaxed);
    stats_.flush_on_full.store(0, std::memory_order_relaxed);
    stats_.flush_on_interval.store(0, std::memory_order_relaxed);
    stats_.manual_flushes.store(0, std::memory_order_relaxed);
}

std::unique_ptr<buffered_writer> make_buffered_writer(
    std::unique_ptr<log_writer_interface> writer,
    size_t max_entries,
    std::chrono::milliseconds flush_interval) {
    return std::make_unique<buffered_writer>(std::move(writer), max_entries, flush_interval);
}

}  // namespace kcenon::logger
