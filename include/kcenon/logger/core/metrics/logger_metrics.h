/**
 * @file logger_metrics.h
 * @brief Logger performance metrics and monitoring
 */

#pragma once

#include <atomic>
#include <chrono>
#include <cstdint>
#include <map>
#include <string>

namespace kcenon::logger::metrics {

/**
 * @brief Performance statistics for logger operations
 */
struct logger_performance_stats {
    std::atomic<uint64_t> messages_logged{0};          ///< Total messages logged
    std::atomic<uint64_t> messages_dropped{0};         ///< Messages dropped due to queue full
    std::atomic<uint64_t> total_log_time_ns{0};        ///< Total time spent logging (nanoseconds)
    std::atomic<uint64_t> queue_size{0};               ///< Current queue size
    std::atomic<uint64_t> max_queue_size{0};           ///< Maximum queue size reached
    std::atomic<uint64_t> writer_errors{0};            ///< Number of writer errors
    std::atomic<uint64_t> flush_operations{0};         ///< Number of flush operations

    // Compatibility counters for legacy integrations
    std::atomic<uint64_t> messages_enqueued{0};        ///< Legacy alias for enqueued messages
    std::atomic<uint64_t> messages_processed{0};       ///< Legacy alias for processed messages

    struct writer_stats_compat {
        uint64_t messages_written{0};
        uint64_t bytes_written{0};
        uint64_t write_failures{0};

        double get_avg_write_time_us() const { return 0.0; }
    };

    std::map<std::string, writer_stats_compat> writer_stats; ///< Legacy per-writer statistics

    // Copy constructor to allow copying atomic values
    logger_performance_stats() = default;
    logger_performance_stats(const logger_performance_stats& other)
        : messages_logged(other.messages_logged.load()),
          messages_dropped(other.messages_dropped.load()),
          total_log_time_ns(other.total_log_time_ns.load()),
          queue_size(other.queue_size.load()),
          max_queue_size(other.max_queue_size.load()),
          writer_errors(other.writer_errors.load()),
          flush_operations(other.flush_operations.load()),
          messages_enqueued(other.messages_enqueued.load()),
          messages_processed(other.messages_processed.load()),
          writer_stats(other.writer_stats) {}

    logger_performance_stats& operator=(const logger_performance_stats& other) {
        if (this != &other) {
            messages_logged.store(other.messages_logged.load());
            messages_dropped.store(other.messages_dropped.load());
            total_log_time_ns.store(other.total_log_time_ns.load());
            queue_size.store(other.queue_size.load());
            max_queue_size.store(other.max_queue_size.load());
            writer_errors.store(other.writer_errors.load());
            flush_operations.store(other.flush_operations.load());
            messages_enqueued.store(other.messages_enqueued.load());
            messages_processed.store(other.messages_processed.load());
            writer_stats = other.writer_stats;
        }
        return *this;
    }

    // Move constructor
    logger_performance_stats(logger_performance_stats&& other) noexcept
        : messages_logged(other.messages_logged.load()),
          messages_dropped(other.messages_dropped.load()),
          total_log_time_ns(other.total_log_time_ns.load()),
          queue_size(other.queue_size.load()),
          max_queue_size(other.max_queue_size.load()),
          writer_errors(other.writer_errors.load()),
          flush_operations(other.flush_operations.load()),
          messages_enqueued(other.messages_enqueued.load()),
          messages_processed(other.messages_processed.load()),
          writer_stats(std::move(other.writer_stats)) {}

    /**
     * @brief Get messages per second
     */
    double get_messages_per_second() const {
        auto msgs = messages_logged.load();
        auto time_ns = total_log_time_ns.load();
        if (time_ns == 0) return 0.0;
        return static_cast<double>(msgs) * 1'000'000'000.0 / static_cast<double>(time_ns);
    }

    /**
     * @brief Get average enqueue time in nanoseconds
     */
    uint64_t get_avg_enqueue_time_ns() const {
        auto msgs = messages_logged.load();
        auto time_ns = total_log_time_ns.load();
        if (msgs == 0) return 0;
        return time_ns / msgs;
    }

    /**
     * @brief Get queue utilization percentage
     */
    double get_queue_utilization_percent() const {
        auto current = queue_size.load();
        auto max_size = max_queue_size.load();
        if (max_size == 0) return 0.0;
        return static_cast<double>(current) * 100.0 / static_cast<double>(max_size);
    }

    double get_drop_rate_percent() const {
        auto enqueued = messages_enqueued.load();
        if (enqueued == 0) {
            return 0.0;
        }
        auto dropped = messages_dropped.load();
        return static_cast<double>(dropped) * 100.0 / static_cast<double>(enqueued);
    }

    double get_bytes_per_second() const {
        return 0.0;
    }

    /**
     * @brief Reset all statistics
     */
    void reset() {
        messages_logged.store(0);
        messages_dropped.store(0);
        total_log_time_ns.store(0);
        queue_size.store(0);
        max_queue_size.store(0);
        writer_errors.store(0);
        flush_operations.store(0);
        messages_enqueued.store(0);
        messages_processed.store(0);
        writer_stats.clear();
    }
};

/**
 * @brief Global logger metrics instance
 */
extern logger_performance_stats g_logger_stats;

/**
 * @brief Record a logged message
 */
inline void record_message_logged(uint64_t time_ns) {
    g_logger_stats.messages_logged.fetch_add(1);
    g_logger_stats.messages_enqueued.fetch_add(1);
    g_logger_stats.messages_processed.fetch_add(1);
    g_logger_stats.total_log_time_ns.fetch_add(time_ns);
}

/**
 * @brief Record a dropped message
 */
inline void record_message_dropped() {
    g_logger_stats.messages_dropped.fetch_add(1);
    g_logger_stats.messages_enqueued.fetch_add(1);
}

/**
 * @brief Update queue size metrics
 */
inline void update_queue_size(uint64_t current_size) {
    g_logger_stats.queue_size.store(current_size);

    // Update max queue size if necessary
    uint64_t current_max = g_logger_stats.max_queue_size.load();
    while (current_size > current_max) {
        if (g_logger_stats.max_queue_size.compare_exchange_weak(current_max, current_size)) {
            break;
        }
    }
}

/**
 * @brief Record a writer error
 */
inline void record_writer_error() {
    g_logger_stats.writer_errors.fetch_add(1);
}

/**
 * @brief Record a flush operation
 */
inline void record_flush_operation() {
    g_logger_stats.flush_operations.fetch_add(1);
}

} // namespace kcenon::logger::metrics
