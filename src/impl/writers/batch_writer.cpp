/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
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

#include <kcenon/logger/writers/batch_writer.h>
#include <kcenon/logger/utils/error_handling_utils.h>
#include <algorithm>

namespace kcenon::logger {

batch_writer::batch_writer(log_writer_ptr underlying_writer,
                          const config& cfg)
    : base_type(std::move(underlying_writer), cfg.max_batch_size, "batch")
    , config_(cfg)
    , last_flush_time_(std::chrono::steady_clock::now()) {

    // Reserve space for batch to avoid reallocations
    queue_.reserve(config_.max_batch_size);
}

batch_writer::~batch_writer() {
    shutting_down_ = true;

    // Flush any remaining entries with proper error handling
    if (!queue_.empty()) {
        utils::safe_destructor_result_operation("final_batch_flush", [this]() {
            return flush();
        });
    }
}

common::VoidResult batch_writer::write(const log_entry& entry) {

    if (shutting_down_) {
        return make_logger_void_result(logger_error_code::queue_stopped, "Batch writer is shutting down");
    }

    bool should_flush = false;

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        // Add entry to batch using shared helper
        queue_.push_back(copy_log_entry(entry));
        stats_.total_entries++;

        // Check if we should flush
        should_flush = should_flush_by_size();
        if (should_flush) {
            stats_.flush_on_size++;
        }
    }

    // Flush outside of lock to avoid blocking other writers
    if (should_flush) {
        return flush();
    }

    return common::ok();
}

common::VoidResult batch_writer::flush() {
    if (shutting_down_ && queue_.empty()) {
        return common::ok();
    }

    std::lock_guard<std::mutex> lock(queue_mutex_);

    if (!shutting_down_) {
        stats_.manual_flushes++;
    }

    return flush_batch_unsafe();
}

common::VoidResult batch_writer::flush_batch_unsafe() {
    if (queue_.empty()) {
        return common::ok();
    }

    // Write all entries in the batch
    common::VoidResult last_result = common::ok();

    for (const auto& entry : queue_) {
        auto result = wrapped().write(entry);

        if (result.is_err()) {
            last_result = result;
            stats_.dropped_entries++;
        }
    }

    // Flush the underlying writer
    auto flush_result = wrapped().flush();
    if (flush_result.is_err() && last_result.is_ok()) {
        last_result = flush_result;
    }

    // Update statistics
    stats_.total_batches++;

    // Clear the batch
    queue_.clear();
    last_flush_time_ = std::chrono::steady_clock::now();

    // Return the last error if any
    return last_result;
}

std::string batch_writer::get_name() const {
    return "batch_writer[" + wrapped().get_name() + "]";
}

bool batch_writer::is_healthy() const {
    return !shutting_down_ && wrapped().is_healthy();
}

size_t batch_writer::get_current_batch_size() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return queue_.size();
}

void batch_writer::reset_stats() {
    stats_.total_batches = 0;
    stats_.total_entries = 0;
    stats_.dropped_entries = 0;
    stats_.flush_on_size = 0;
    stats_.flush_on_timeout = 0;
    stats_.manual_flushes = 0;
}

bool batch_writer::should_flush_by_size() const {
    return queue_.size() >= config_.max_batch_size;
}

bool batch_writer::should_flush_by_time() const {
    auto now = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
        now - last_flush_time_
    );
    return elapsed >= config_.flush_interval;
}

// Factory function implementation
std::unique_ptr<batch_writer> make_batch_writer(
    log_writer_ptr writer,
    size_t batch_size,
    std::chrono::milliseconds flush_interval) {
    
    batch_writer::config cfg;
    cfg.max_batch_size = batch_size;
    cfg.flush_interval = flush_interval;
    
    return std::make_unique<batch_writer>(std::move(writer), cfg);
}

} // namespace kcenon::logger