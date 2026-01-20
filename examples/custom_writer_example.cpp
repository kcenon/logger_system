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
 * @file custom_writer_example.cpp
 * @brief Example demonstrating how to create custom writers using thread_safe_writer
 *
 * This example shows:
 * 1. How to inherit from thread_safe_writer for automatic thread-safety
 * 2. Implementing write_impl() and flush_impl() methods
 * 3. Using the writer with the logger system
 *
 * Since v1.3.0, thread_safe_writer is the recommended base class for custom writers.
 * It eliminates boilerplate mutex code and ensures consistent thread-safety.
 */

#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/thread_safe_writer.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <iostream>
#include <vector>
#include <map>

using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;

/**
 * @class memory_writer
 * @brief A custom writer that stores log entries in memory
 *
 * This example writer demonstrates the thread_safe_writer pattern.
 * Log entries are stored in a vector and can be retrieved later.
 *
 * Benefits of using thread_safe_writer:
 * - No need to manage mutex manually
 * - write_impl() and flush_impl() are called while holding the lock
 * - Thread-safety is guaranteed by the base class
 */
class memory_writer : public thread_safe_writer {
public:
    memory_writer() = default;

    std::string get_name() const override {
        return "memory";
    }

    /**
     * @brief Get all stored log entries
     * @return Vector of formatted log strings
     *
     * Note: This method uses get_mutex() to ensure thread-safe access
     * to the internal buffer when reading.
     */
    std::vector<std::string> get_entries() const {
        std::lock_guard<std::mutex> lock(get_mutex());
        return entries_;
    }

    /**
     * @brief Get count of stored entries
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(get_mutex());
        return entries_.size();
    }

    /**
     * @brief Clear all stored entries
     */
    void clear() {
        std::lock_guard<std::mutex> lock(get_mutex());
        entries_.clear();
    }

protected:
    /**
     * @brief Implementation of write operation
     *
     * This method is called by thread_safe_writer::write() while holding the mutex.
     * No need to acquire locks here - the base class handles synchronization.
     *
     * @warning Do not call public methods on 'this' inside *_impl methods
     *          as it would cause deadlock.
     */
    kcenon::common::VoidResult write_entry_impl(const log_entry& entry) override
    {
        std::string formatted = format_log_entry(entry);

        // Store in memory - already protected by base class mutex
        entries_.push_back(formatted);

        return kcenon::common::ok();
    }

    /**
     * @brief Implementation of flush operation
     *
     * For a memory writer, flush is a no-op since data is already in memory.
     * Called by thread_safe_writer::flush() while holding the mutex.
     */
    kcenon::common::VoidResult flush_impl() override {
        // Nothing to flush for memory storage
        return kcenon::common::ok();
    }

private:
    std::vector<std::string> entries_;
};

/**
 * @class counted_console_writer
 * @brief A console writer that counts messages per log level
 *
 * Demonstrates extending thread_safe_writer for console output with statistics.
 */
class counted_console_writer : public thread_safe_writer {
public:
    counted_console_writer() {
        // Initialize counters for each log level
        for (int i = 0; i <= static_cast<int>(kcenon::common::interfaces::log_level::trace); ++i) {
            counts_[static_cast<kcenon::common::interfaces::log_level>(i)] = 0;
        }
    }

    std::string get_name() const override {
        return "counted_console";
    }

    /**
     * @brief Get count for a specific log level
     */
    size_t get_count(kcenon::common::interfaces::log_level level) const {
        std::lock_guard<std::mutex> lock(get_mutex());
        auto it = counts_.find(level);
        return (it != counts_.end()) ? it->second : 0;
    }

    /**
     * @brief Get total message count
     */
    size_t total_count() const {
        std::lock_guard<std::mutex> lock(get_mutex());
        size_t total = 0;
        for (const auto& [level, count] : counts_) {
            total += count;
        }
        return total;
    }

    /**
     * @brief Print statistics summary
     */
    void print_stats() const {
        std::lock_guard<std::mutex> lock(get_mutex());
        std::cout << "\n=== Log Statistics ===" << std::endl;
        std::cout << "Fatal:   " << counts_.at(kcenon::common::interfaces::log_level::fatal) << std::endl;
        std::cout << "Error:   " << counts_.at(kcenon::common::interfaces::log_level::error) << std::endl;
        std::cout << "Warning: " << counts_.at(kcenon::common::interfaces::log_level::warning) << std::endl;
        std::cout << "Info:    " << counts_.at(kcenon::common::interfaces::log_level::info) << std::endl;
        std::cout << "Debug:   " << counts_.at(kcenon::common::interfaces::log_level::debug) << std::endl;
        std::cout << "Trace:   " << counts_.at(kcenon::common::interfaces::log_level::trace) << std::endl;
    }

protected:
    kcenon::common::VoidResult write_entry_impl(const log_entry& entry) override
    {
        // Convert log_level from logger_system to common::interfaces
        auto level = static_cast<kcenon::common::interfaces::log_level>(static_cast<int>(entry.level));

        // Increment counter for this level
        counts_[level]++;

        // Format log entry
        std::string formatted = format_log_entry(entry);

        // Output to console with color based on level
        if (use_color()) {
            switch (level) {
                case kcenon::common::interfaces::log_level::fatal:
                case kcenon::common::interfaces::log_level::error:
                    std::cerr << "\033[31m" << formatted << "\033[0m" << std::endl;
                    break;
                case kcenon::common::interfaces::log_level::warning:
                    std::cout << "\033[33m" << formatted << "\033[0m" << std::endl;
                    break;
                default:
                    std::cout << formatted << std::endl;
                    break;
            }
        } else {
            if (level <= kcenon::common::interfaces::log_level::error) {
                std::cerr << formatted << std::endl;
            } else {
                std::cout << formatted << std::endl;
            }
        }

        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush_impl() override {
        std::cout.flush();
        std::cerr.flush();
        return kcenon::common::ok();
    }

private:
    mutable std::map<kcenon::common::interfaces::log_level, size_t> counts_;
};

int main() {
    std::cout << "=== Custom Writer Example (thread_safe_writer) ===" << std::endl;
    std::cout << std::endl;

    // Example 1: Memory Writer
    std::cout << "--- Example 1: Memory Writer ---" << std::endl;
    {
        auto result = logger_builder()
            .with_min_level(kcenon::common::interfaces::log_level::info)
            .add_writer("memory", std::make_unique<memory_writer>())
            .build();

        if (!result.has_value()) {
            std::cerr << "Failed to create logger" << std::endl;
            return 1;
        }

        auto logger = std::move(result.value());

        // Log some messages
        logger->log(ci::log_level::info, std::string("First message"));
        logger->log(ci::log_level::warning, std::string("Second message"));
        logger->log(ci::log_level::error, std::string("Third message"));

        std::cout << "Logged 3 messages to memory writer" << std::endl;
    }

    std::cout << std::endl;

    // Example 2: Counted Console Writer
    std::cout << "--- Example 2: Counted Console Writer ---" << std::endl;
    {
        auto counted_writer = std::make_unique<counted_console_writer>();
        auto* counted_ptr = counted_writer.get();  // Keep reference for stats

        auto result = logger_builder()
            .with_min_level(kcenon::common::interfaces::log_level::debug)
            .add_writer("counted", std::move(counted_writer))
            .build();

        if (!result.has_value()) {
            std::cerr << "Failed to create logger" << std::endl;
            return 1;
        }

        auto logger = std::move(result.value());

        // Log messages at different levels
        logger->log(ci::log_level::debug, std::string("Debug message 1"));
        logger->log(ci::log_level::debug, std::string("Debug message 2"));
        logger->log(ci::log_level::info, std::string("Info message"));
        logger->log(ci::log_level::warning, std::string("Warning message"));
        logger->log(ci::log_level::error, std::string("Error message"));

        // Print statistics
        counted_ptr->print_stats();
    }

    std::cout << std::endl;
    std::cout << "=== Custom Writer Example Complete ===" << std::endl;

    return 0;
}
