#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file async_writer.h
 * @brief Asynchronous wrapper for log writers
 * 
 * This file provides an asynchronous wrapper that can be used with any
 * base_writer implementation to make it asynchronous.
 */

#include "base_writer.h"
#include "../interfaces/log_entry.h"
#include "../interfaces/writer_category.h"
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <memory>
#include <future>
#include <iostream>

namespace kcenon::logger {

/**
 * @class async_writer
 * @brief Asynchronous wrapper for log writers
 *
 * This class wraps any base_writer implementation and provides
 * asynchronous writing capabilities using a background thread.
 *
 * Category: Asynchronous (non-blocking), Decorator (wraps another writer)
 *
 * @since 1.4.0 Added async_writer_tag and decorator_writer_tag for classification
 */
class async_writer : public base_writer, public async_writer_tag, public decorator_writer_tag {
public:
    /**
     * @brief Constructor
     * @param wrapped_writer The writer to wrap with async functionality
     * @param queue_size Maximum queue size for pending messages
     * @param flush_timeout Maximum time to wait for flush operation (in seconds)
     */
    explicit async_writer(std::unique_ptr<base_writer> wrapped_writer,
                         std::size_t queue_size = 10000,
                         std::chrono::seconds flush_timeout = std::chrono::seconds(5))
        : wrapped_writer_(std::move(wrapped_writer))
        , max_queue_size_(queue_size)
        , flush_timeout_(flush_timeout)
        , running_(false) {
        if (!wrapped_writer_) {
            throw std::invalid_argument("Wrapped writer cannot be null");
        }
    }
    
    /**
     * @brief Destructor
     */
    ~async_writer() override {
        stop();
    }
    
    /**
     * @brief Start the async writer thread
     */
    void start() {
        // Use compare_exchange to safely check and set running_ flag
        bool expected = false;
        if (!running_.compare_exchange_strong(expected, true)) {
            return; // Already running
        }

        // Try to create the worker thread with proper error handling
        try {
            worker_thread_ = std::thread([this]() {
                process_messages();
            });
        } catch (const std::exception&) {
            // Thread creation failed, rollback the running_ flag
            running_.store(false);
            throw; // Re-throw to notify caller
        }
    }
    
    /**
     * @brief Stop the async writer thread
     * @param force_flush If true, process all remaining messages before stopping
     */
    void stop(bool force_flush = true) {
        if (!running_.exchange(false)) {
            return; // Already stopped
        }

        // Process remaining messages if requested
        if (force_flush) {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            size_t remaining_count = message_queue_.size();
            if (remaining_count > 0) {
                std::cerr << "[async_writer] Info: Processing " << remaining_count
                          << " remaining messages before shutdown.\n";
            }
        }

        // Signal the worker thread to stop
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);
            queue_cv_.notify_all();
        }

        // Wait for the worker thread to finish (infinite wait - shutdown prioritizes safety)
        if (worker_thread_.joinable()) {
            worker_thread_.join();
        }

        // Verify all messages were processed
        if (!message_queue_.empty()) {
            std::cerr << "[async_writer] Warning: " << message_queue_.size()
                      << " messages were not processed during shutdown.\n";
        }
    }
    
    /**
     * @brief Write a log entry asynchronously
     * @param entry The log entry to write
     * @return common::VoidResult indicating success or error
     * @since 3.5.0 Changed to use log_entry directly
     */
    common::VoidResult write(const log_entry& entry) override {
        if (!running_) {
            // If not running, write directly
            return wrapped_writer_->write(entry);
        }

        // Queue the message
        {
            std::lock_guard<std::mutex> lock(queue_mutex_);

            // Check queue size
            if (message_queue_.size() >= max_queue_size_) {
                // Queue is full, drop the message or handle overflow
                return make_logger_void_result(logger_error_code::queue_full, "Async writer queue is full");
            }

            // Create a copy of entry fields since log_entry is move-only
            if (entry.location) {
                message_queue_.emplace(entry.level,
                                      entry.message.to_string(),
                                      entry.location->file.to_string(),
                                      entry.location->line,
                                      entry.location->function.to_string(),
                                      entry.timestamp);
            } else {
                message_queue_.emplace(entry.level,
                                      entry.message.to_string(),
                                      entry.timestamp);
            }
            queue_cv_.notify_one();
        }

        return common::ok();
    }

    /**
     * @brief Flush all pending messages
     * @return common::VoidResult indicating success or error
     */
    common::VoidResult flush() override {
        if (!running_) {
            return wrapped_writer_->flush();
        }

        // Wait for the queue to be empty with a timeout to prevent indefinite blocking
        std::unique_lock<std::mutex> lock(queue_mutex_);
        bool flushed = flush_cv_.wait_for(lock, flush_timeout_, [this]() {
            return message_queue_.empty();
        });

        if (!flushed) {
            // Timeout occurred - worker thread may have exited or is blocked
            return make_logger_void_result(logger_error_code::flush_timeout,
                                           "Flush operation timed out after " +
                                           std::to_string(flush_timeout_.count()) + " seconds");
        }

        // Flush the wrapped writer
        return wrapped_writer_->flush();
    }
    
    /**
     * @brief Check if the writer is healthy
     * @return true if healthy, false otherwise
     */
    bool is_healthy() const override {
        return wrapped_writer_->is_healthy() && running_;
    }
    
    /**
     * @brief Get the name of this writer
     * @return Writer name
     */
    std::string get_name() const override {
        return "async_" + wrapped_writer_->get_name();
    }
    
    /**
     * @brief Set whether to use color output
     * @param use_color Enable/disable color output
     */
    void set_use_color(bool use_color) override {
        wrapped_writer_->set_use_color(use_color);
    }
    
private:
    /**
     * @brief Process messages from the queue
     */
    void process_messages() {
        while (running_) {
            std::unique_lock<std::mutex> lock(queue_mutex_);

            // Wait for messages or stop signal
            queue_cv_.wait(lock, [this]() {
                return !message_queue_.empty() || !running_;
            });

            // Process all available messages
            while (!message_queue_.empty()) {
                auto entry = std::move(message_queue_.front());
                message_queue_.pop();

                // Unlock while writing
                lock.unlock();
                wrapped_writer_->write(entry);
                lock.lock();
            }

            // Notify flush waiters
            flush_cv_.notify_all();
        }
    }

    /**
     * @brief Flush any remaining messages after stopping
     */
    void flush_remaining() {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        while (!message_queue_.empty()) {
            auto entry = std::move(message_queue_.front());
            message_queue_.pop();
            wrapped_writer_->write(entry);
        }
        wrapped_writer_->flush();
    }

    std::unique_ptr<base_writer> wrapped_writer_;
    std::size_t max_queue_size_;
    std::chrono::seconds flush_timeout_;  // Configurable flush timeout

    std::queue<log_entry> message_queue_;
    mutable std::mutex queue_mutex_;
    std::condition_variable queue_cv_;
    std::condition_variable flush_cv_;

    std::atomic<bool> running_;
    std::thread worker_thread_;
};

} // namespace kcenon::logger