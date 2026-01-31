#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file queued_writer_base.h
 * @brief Base template for queue-based log writers
 *
 * This file provides a common base class for writers that use a queue
 * to accumulate log entries before processing (async_writer, batch_writer).
 */

#include "base_writer.h"
#include "../interfaces/log_entry.h"
#include "../interfaces/writer_category.h"
#include <mutex>
#include <memory>
#include <atomic>
#include <queue>
#include <vector>

namespace kcenon::logger {

/**
 * @brief Creates a copy of a log_entry for queue storage
 * @details Since log_entry is move-only, this helper function creates
 * a new log_entry by copying the essential fields from the source.
 * @param entry The source log entry to copy from
 * @return A new log_entry with copied data
 */
inline log_entry copy_log_entry(const log_entry& entry) {
    if (entry.location) {
        return log_entry(entry.level,
                        entry.message.to_string(),
                        entry.location->file.to_string(),
                        entry.location->line,
                        entry.location->function.to_string(),
                        entry.timestamp);
    }
    return log_entry(entry.level,
                    entry.message.to_string(),
                    entry.timestamp);
}

/**
 * @class queued_writer_base
 * @brief Abstract base class for queue-based log writers
 *
 * This template provides common functionality for writers that accumulate
 * log entries in a queue before processing. Derived classes must implement
 * the queue processing strategy (immediate async vs batched).
 *
 * Template Parameter:
 * - Container: The container type to use for storing entries (e.g., std::queue, std::vector)
 *
 * Category: Asynchronous (non-blocking), Decorator (wraps another writer)
 */
template <typename Container>
class queued_writer_base : public base_writer, public async_writer_tag, public decorator_writer_tag {
public:
    /**
     * @brief Constructor accepting base_writer (legacy compatibility)
     * @param wrapped_writer The underlying writer to delegate writes to
     * @param max_queue_size Maximum number of entries in the queue
     * @throw std::invalid_argument if wrapped_writer is null
     */
    explicit queued_writer_base(std::unique_ptr<base_writer> wrapped_writer,
                                std::size_t max_queue_size)
        : wrapped_writer_(static_cast<std::unique_ptr<log_writer_interface>>(std::move(wrapped_writer)))
        , max_queue_size_(max_queue_size)
        , shutting_down_(false) {
        if (!wrapped_writer_) {
            throw std::invalid_argument("Wrapped writer cannot be null");
        }
    }

    /**
     * @brief Constructor accepting log_writer_interface (Decorator pattern)
     * @param wrapped_writer The underlying writer to delegate writes to
     * @param max_queue_size Maximum number of entries in the queue
     * @throw std::invalid_argument if wrapped_writer is null
     */
    explicit queued_writer_base(std::unique_ptr<log_writer_interface> wrapped_writer,
                                std::size_t max_queue_size)
        : wrapped_writer_(std::move(wrapped_writer))
        , max_queue_size_(max_queue_size)
        , shutting_down_(false) {
        if (!wrapped_writer_) {
            throw std::invalid_argument("Wrapped writer cannot be null");
        }
    }

    /**
     * @brief Virtual destructor
     */
    ~queued_writer_base() override = default;

    /**
     * @brief Check if the writer is healthy
     * @return true if healthy, false otherwise
     */
    bool is_healthy() const override {
        return !shutting_down_ && wrapped_writer_->is_healthy();
    }

    /**
     * @brief Set whether to use color output
     * @param use_color Enable/disable color output
     */
    void set_use_color(bool use_color) override {
        // Only call if wrapped_writer is a base_writer
        if (auto* base = dynamic_cast<base_writer*>(wrapped_writer_.get())) {
            base->set_use_color(use_color);
        }
    }

    /**
     * @brief Get the current queue size
     * @return Number of entries currently in the queue
     */
    std::size_t get_queue_size() const {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        return get_container_size(queue_);
    }

    /**
     * @brief Get maximum queue size
     * @return Maximum allowed queue size
     */
    std::size_t get_max_queue_size() const {
        return max_queue_size_;
    }

protected:
    /**
     * @brief Try to enqueue an entry with overflow protection
     * @param entry The log entry to enqueue
     * @return common::VoidResult indicating success or queue_full error
     */
    common::VoidResult try_enqueue(const log_entry& entry) {
        std::lock_guard<std::mutex> lock(queue_mutex_);

        if (get_container_size(queue_) >= max_queue_size_) {
            return handle_overflow(entry);
        }

        enqueue_entry(queue_, entry);
        on_entry_enqueued();
        return common::ok();
    }

    /**
     * @brief Handle queue overflow condition
     * @param entry The entry that couldn't be enqueued
     * @return Error result indicating queue full
     * @note Derived classes can override to implement custom overflow policies
     */
    virtual common::VoidResult handle_overflow(const log_entry& /*entry*/) {
        return make_logger_void_result(logger_error_code::queue_full, "Queue is full");
    }

    /**
     * @brief Called after an entry is successfully enqueued
     * @note Derived classes can override to implement notifications (e.g., condition variable)
     */
    virtual void on_entry_enqueued() {}

    /**
     * @brief Get container size (specialization for std::queue)
     */
    template <typename T>
    static std::size_t get_container_size(const std::queue<T>& container) {
        return container.size();
    }

    /**
     * @brief Get container size (specialization for std::vector)
     */
    template <typename T>
    static std::size_t get_container_size(const std::vector<T>& container) {
        return container.size();
    }

    /**
     * @brief Enqueue entry (specialization for std::queue)
     */
    template <typename T>
    static void enqueue_entry(std::queue<T>& container, const log_entry& entry) {
        container.push(copy_log_entry(entry));
    }

    /**
     * @brief Enqueue entry (specialization for std::vector)
     */
    template <typename T>
    static void enqueue_entry(std::vector<T>& container, const log_entry& entry) {
        container.push_back(copy_log_entry(entry));
    }

    std::unique_ptr<log_writer_interface> wrapped_writer_;
    std::size_t max_queue_size_;

    mutable std::mutex queue_mutex_;
    Container queue_;

    std::atomic<bool> shutting_down_;
};

} // namespace kcenon::logger
