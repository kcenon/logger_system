// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file realtime_log_analyzer.h
 * @brief Real-time log analysis with anomaly detection
 * @since 3.2.0
 *
 * @details This file provides real-time log analysis capabilities including:
 * - Error spike detection
 * - Pattern-based alerting
 * - Rate anomaly detection
 * - New error type tracking
 *
 * Unlike the post-hoc log_analyzer, this class is designed to be integrated
 * directly into the logging pipeline for real-time monitoring and alerting.
 *
 * @example Basic usage:
 * @code
 * auto analyzer = std::make_unique<realtime_log_analyzer>();
 *
 * // Configure detection rules
 * analyzer->set_error_spike_threshold(50);  // 50 errors/minute
 * analyzer->add_pattern_alert("Connection refused", log_level::error);
 * analyzer->add_pattern_alert("OOM", log_level::fatal);
 *
 * // Set callback for anomaly notifications
 * analyzer->set_anomaly_callback([](const anomaly_event& event) {
 *     switch (event.anomaly_type) {
 *         case anomaly_event::type::error_spike:
 *             send_pagerduty_alert(event.description);
 *             break;
 *         case anomaly_event::type::pattern_match:
 *             send_slack_notification(event.description);
 *             break;
 *     }
 * });
 * @endcode
 */

#pragma once

#include <kcenon/logger/interfaces/logger_types.h>
#include <kcenon/logger/analysis/log_analyzer.h>

#include <atomic>
#include <chrono>
#include <cstdint>
#include <deque>
#include <functional>
#include <memory>
#include <mutex>
#include <regex>
#include <shared_mutex>
#include <string>
#include <unordered_set>
#include <vector>

namespace kcenon::logger::analysis {

/**
 * @brief Represents an anomaly event detected during real-time analysis
 * @since 3.2.0
 */
struct anomaly_event {
    /**
     * @brief Type of anomaly detected
     */
    enum class type : std::uint8_t {
        error_spike,      ///< Sudden increase in errors
        pattern_match,    ///< Configured pattern detected
        rate_anomaly,     ///< Unusual log rate (too high or too low)
        new_error_type    ///< Previously unseen error message
    };

    type anomaly_type = type::error_spike;                   ///< Type of the anomaly
    std::chrono::system_clock::time_point detected_at;       ///< When the anomaly was detected
    std::string description;                                 ///< Human-readable description
    std::vector<analyzed_log_entry> related_entries;         ///< Log entries related to this anomaly
    std::string pattern;                                     ///< Pattern that triggered (for pattern_match)
    size_t current_count = 0;                                ///< Current count (for spike/rate anomalies)
    size_t threshold = 0;                                    ///< Threshold that was exceeded
};

/**
 * @brief Configuration for real-time log analysis
 * @since 3.2.0
 */
struct realtime_analysis_config {
    size_t error_spike_threshold = 100;                      ///< Errors per minute to trigger spike alert
    size_t rate_anomaly_high_threshold = 1000;               ///< Logs per minute considered high
    size_t rate_anomaly_low_threshold = 0;                   ///< Logs per minute considered low (0 = disabled)
    std::chrono::seconds window_duration{60};                ///< Sliding window duration for rate calculation
    std::chrono::seconds baseline_duration{300};             ///< Duration for baseline rate calculation
    bool track_new_errors = true;                            ///< Enable new error type detection
    bool enable_rate_anomaly_detection = true;               ///< Enable rate anomaly detection
    double rate_deviation_factor = 2.0;                      ///< Factor for dynamic rate anomaly detection
    size_t max_related_entries = 10;                         ///< Max entries to store per anomaly
};

/**
 * @brief Pattern alert configuration
 * @since 3.2.0
 */
struct pattern_alert {
    std::string pattern;                                     ///< Regex pattern to match
    logger_system::log_level min_level;                      ///< Minimum log level to trigger
    std::regex compiled_pattern;                             ///< Pre-compiled regex for efficiency

    pattern_alert(const std::string& p, logger_system::log_level level)
        : pattern(p), min_level(level), compiled_pattern(p, std::regex::optimize) {}
};

/**
 * @brief Real-time log analyzer with anomaly detection
 *
 * @details The realtime_log_analyzer class provides real-time analysis of log entries
 * during the logging process. Unlike the post-hoc log_analyzer, this class is designed
 * to detect anomalies as they occur and trigger callbacks for immediate alerting.
 *
 * Key features:
 * - Sliding window for rate calculation
 * - Error spike detection
 * - Pattern-based alerting with regex support
 * - Rate anomaly detection (high/low rate alerts)
 * - New error type tracking
 * - Thread-safe for concurrent logging
 *
 * @note This class is designed to have minimal performance impact (< 5% overhead)
 * on the logging pipeline.
 *
 * @since 3.2.0
 */
class realtime_log_analyzer {
public:
    /**
     * @brief Callback type for anomaly notifications
     */
    using anomaly_callback = std::function<void(const anomaly_event&)>;

    /**
     * @brief Default constructor
     */
    realtime_log_analyzer() = default;

    /**
     * @brief Constructor with configuration
     * @param config Analysis configuration
     */
    explicit realtime_log_analyzer(const realtime_analysis_config& config)
        : config_(config) {}

    /**
     * @brief Set the anomaly callback
     * @param cb Callback function to invoke when anomaly is detected
     *
     * @details The callback is invoked synchronously when an anomaly is detected.
     * For non-blocking operation, the callback should dispatch to a separate thread.
     */
    void set_anomaly_callback(anomaly_callback cb) {
        std::unique_lock lock(callback_mutex_);
        callback_ = std::move(cb);
    }

    /**
     * @brief Analyze a log entry in real-time
     * @param entry The log entry to analyze
     *
     * @details This method should be called for each log entry during logging.
     * It performs all configured detection checks and may trigger the anomaly callback.
     *
     * Thread-safe: Multiple threads can call this method concurrently.
     */
    void analyze(const analyzed_log_entry& entry) {
        auto now = std::chrono::system_clock::now();

        // Add to sliding window
        add_to_window(entry, now);

        // Check for error spike
        if (entry.level == logger_system::log_level::error ||
            entry.level == logger_system::log_level::fatal) {
            check_error_spike(entry, now);
        }

        // Check pattern alerts
        check_pattern_alerts(entry, now);

        // Check rate anomaly
        if (config_.enable_rate_anomaly_detection) {
            check_rate_anomaly(now);
        }

        // Track new error types
        if (config_.track_new_errors &&
            (entry.level == logger_system::log_level::error ||
             entry.level == logger_system::log_level::fatal)) {
            check_new_error_type(entry, now);
        }
    }

    /**
     * @brief Set error spike threshold
     * @param errors_per_minute Number of errors per minute to trigger alert
     */
    void set_error_spike_threshold(size_t errors_per_minute) {
        config_.error_spike_threshold = errors_per_minute;
    }

    /**
     * @brief Add a pattern-based alert
     * @param pattern Regex pattern to match against log messages
     * @param min_level Minimum log level for this pattern to trigger
     */
    void add_pattern_alert(const std::string& pattern, logger_system::log_level min_level) {
        std::unique_lock lock(patterns_mutex_);
        patterns_.emplace_back(pattern, min_level);
    }

    /**
     * @brief Remove a pattern alert
     * @param pattern The pattern to remove
     * @return true if pattern was found and removed
     */
    bool remove_pattern_alert(const std::string& pattern) {
        std::unique_lock lock(patterns_mutex_);
        auto it = std::remove_if(patterns_.begin(), patterns_.end(),
            [&pattern](const pattern_alert& alert) {
                return alert.pattern == pattern;
            });
        if (it != patterns_.end()) {
            patterns_.erase(it, patterns_.end());
            return true;
        }
        return false;
    }

    /**
     * @brief Clear all pattern alerts
     */
    void clear_pattern_alerts() {
        std::unique_lock lock(patterns_mutex_);
        patterns_.clear();
    }

    /**
     * @brief Set rate anomaly thresholds
     * @param high_threshold High rate threshold (logs per minute)
     * @param low_threshold Low rate threshold (logs per minute), 0 to disable
     */
    void set_rate_thresholds(size_t high_threshold, size_t low_threshold = 0) {
        config_.rate_anomaly_high_threshold = high_threshold;
        config_.rate_anomaly_low_threshold = low_threshold;
    }

    /**
     * @brief Enable or disable new error tracking
     * @param enable true to enable, false to disable
     */
    void set_track_new_errors(bool enable) {
        config_.track_new_errors = enable;
    }

    /**
     * @brief Get current error rate (errors per minute)
     * @return Current error rate
     */
    double get_error_rate() const {
        std::shared_lock lock(window_mutex_);
        return calculate_rate(error_window_);
    }

    /**
     * @brief Get current log rate (logs per minute)
     * @return Current log rate
     */
    double get_log_rate() const {
        std::shared_lock lock(window_mutex_);
        return calculate_rate(log_window_);
    }

    /**
     * @brief Get current statistics
     * @return Statistics structure
     */
    struct statistics {
        size_t total_analyzed = 0;
        size_t total_errors = 0;
        size_t anomalies_detected = 0;
        size_t error_spikes = 0;
        size_t pattern_matches = 0;
        size_t rate_anomalies = 0;
        size_t new_error_types = 0;
        double current_log_rate = 0.0;
        double current_error_rate = 0.0;
    };

    statistics get_statistics() const {
        std::shared_lock lock(stats_mutex_);
        statistics stats;
        stats.total_analyzed = total_analyzed_.load();
        stats.total_errors = total_errors_.load();
        stats.anomalies_detected = anomalies_detected_.load();
        stats.error_spikes = error_spikes_.load();
        stats.pattern_matches = pattern_matches_.load();
        stats.rate_anomalies = rate_anomalies_.load();
        stats.new_error_types = new_error_types_.load();
        stats.current_log_rate = get_log_rate();
        stats.current_error_rate = get_error_rate();
        return stats;
    }

    /**
     * @brief Reset all statistics and tracked state
     */
    void reset() {
        {
            std::unique_lock lock(window_mutex_);
            log_window_.clear();
            error_window_.clear();
            baseline_rates_.clear();
        }
        {
            std::unique_lock lock(errors_mutex_);
            known_errors_.clear();
        }
        {
            std::unique_lock lock(stats_mutex_);
            total_analyzed_ = 0;
            total_errors_ = 0;
            anomalies_detected_ = 0;
            error_spikes_ = 0;
            pattern_matches_ = 0;
            rate_anomalies_ = 0;
            new_error_types_ = 0;
        }
        last_rate_check_ = std::chrono::system_clock::time_point{};
        last_spike_alert_ = std::chrono::system_clock::time_point{};
    }

    /**
     * @brief Get the configuration
     * @return Current configuration
     */
    const realtime_analysis_config& get_config() const {
        return config_;
    }

    /**
     * @brief Set the configuration
     * @param config New configuration
     */
    void set_config(const realtime_analysis_config& config) {
        config_ = config;
    }

private:
    struct timestamped_entry {
        std::chrono::system_clock::time_point timestamp;
        analyzed_log_entry entry;
    };

    void add_to_window(const analyzed_log_entry& entry,
                       std::chrono::system_clock::time_point now) {
        std::unique_lock lock(window_mutex_);

        // Add to log window
        log_window_.push_back({now, entry});

        // Add to error window if error/fatal
        if (entry.level == logger_system::log_level::error ||
            entry.level == logger_system::log_level::fatal) {
            error_window_.push_back({now, entry});
        }

        // Clean up old entries
        auto cutoff = now - config_.window_duration;
        cleanup_window(log_window_, cutoff);
        cleanup_window(error_window_, cutoff);

        // Update statistics
        total_analyzed_.fetch_add(1, std::memory_order_relaxed);
        if (entry.level == logger_system::log_level::error ||
            entry.level == logger_system::log_level::fatal) {
            total_errors_.fetch_add(1, std::memory_order_relaxed);
        }
    }

    static void cleanup_window(std::deque<timestamped_entry>& window,
                               std::chrono::system_clock::time_point cutoff) {
        while (!window.empty() && window.front().timestamp < cutoff) {
            window.pop_front();
        }
    }

    double calculate_rate(const std::deque<timestamped_entry>& window) const {
        if (window.empty()) return 0.0;

        auto duration = std::chrono::duration_cast<std::chrono::seconds>(
            config_.window_duration).count();
        if (duration == 0) duration = 60;

        return static_cast<double>(window.size()) * 60.0 / duration;
    }

    void check_error_spike(const analyzed_log_entry& entry,
                          std::chrono::system_clock::time_point now) {
        std::shared_lock lock(window_mutex_);

        double current_rate = calculate_rate(error_window_);

        if (current_rate >= static_cast<double>(config_.error_spike_threshold)) {
            // Rate limit: don't alert more than once per minute
            {
                std::shared_lock rate_lock(rate_limit_mutex_);
                if (now - last_spike_alert_ < std::chrono::minutes(1)) {
                    return;
                }
            }

            lock.unlock();  // Release before callback

            // Update last alert time
            {
                std::unique_lock rate_lock(rate_limit_mutex_);
                last_spike_alert_ = now;
            }

            anomaly_event event;
            event.anomaly_type = anomaly_event::type::error_spike;
            event.detected_at = now;
            event.description = "Error spike detected: " +
                std::to_string(static_cast<size_t>(current_rate)) +
                " errors/minute (threshold: " +
                std::to_string(config_.error_spike_threshold) + ")";
            event.current_count = static_cast<size_t>(current_rate);
            event.threshold = config_.error_spike_threshold;

            // Collect related entries
            {
                std::shared_lock entries_lock(window_mutex_);
                collect_related_entries(event, error_window_);
            }

            notify_anomaly(event);
            error_spikes_.fetch_add(1, std::memory_order_relaxed);
        }
    }

    void check_pattern_alerts(const analyzed_log_entry& entry,
                             std::chrono::system_clock::time_point now) {
        std::shared_lock lock(patterns_mutex_);

        for (const auto& alert : patterns_) {
            // Check level threshold
            if (static_cast<int>(entry.level) < static_cast<int>(alert.min_level)) {
                continue;
            }

            // Check pattern match
            if (std::regex_search(entry.message, alert.compiled_pattern)) {
                lock.unlock();  // Release before callback

                anomaly_event event;
                event.anomaly_type = anomaly_event::type::pattern_match;
                event.detected_at = now;
                event.pattern = alert.pattern;
                event.description = "Pattern '" + alert.pattern +
                    "' matched in log message: " + entry.message;
                event.related_entries.push_back(entry);

                notify_anomaly(event);
                pattern_matches_.fetch_add(1, std::memory_order_relaxed);
                return;  // Only report first match per entry
            }
        }
    }

    void check_rate_anomaly(std::chrono::system_clock::time_point now) {
        // Rate limit rate anomaly checks to once per 10 seconds
        {
            std::shared_lock rate_lock(rate_limit_mutex_);
            if (now - last_rate_check_ < std::chrono::seconds(10)) {
                return;
            }
        }
        {
            std::unique_lock rate_lock(rate_limit_mutex_);
            last_rate_check_ = now;
        }

        std::shared_lock lock(window_mutex_);
        double current_rate = calculate_rate(log_window_);
        lock.unlock();

        // Check high rate
        if (current_rate >= static_cast<double>(config_.rate_anomaly_high_threshold)) {
            anomaly_event event;
            event.anomaly_type = anomaly_event::type::rate_anomaly;
            event.detected_at = now;
            event.description = "High log rate detected: " +
                std::to_string(static_cast<size_t>(current_rate)) +
                " logs/minute (threshold: " +
                std::to_string(config_.rate_anomaly_high_threshold) + ")";
            event.current_count = static_cast<size_t>(current_rate);
            event.threshold = config_.rate_anomaly_high_threshold;

            notify_anomaly(event);
            rate_anomalies_.fetch_add(1, std::memory_order_relaxed);
        }
        // Check low rate (if enabled)
        else if (config_.rate_anomaly_low_threshold > 0 &&
                 current_rate < static_cast<double>(config_.rate_anomaly_low_threshold) &&
                 total_analyzed_.load() > 100) {  // Only after enough data
            anomaly_event event;
            event.anomaly_type = anomaly_event::type::rate_anomaly;
            event.detected_at = now;
            event.description = "Low log rate detected: " +
                std::to_string(static_cast<size_t>(current_rate)) +
                " logs/minute (threshold: " +
                std::to_string(config_.rate_anomaly_low_threshold) + ")";
            event.current_count = static_cast<size_t>(current_rate);
            event.threshold = config_.rate_anomaly_low_threshold;

            notify_anomaly(event);
            rate_anomalies_.fetch_add(1, std::memory_order_relaxed);
        }
    }

    void check_new_error_type(const analyzed_log_entry& entry,
                             std::chrono::system_clock::time_point now) {
        // Normalize error message (remove numbers, timestamps, etc.)
        std::string normalized = normalize_error_message(entry.message);

        {
            std::shared_lock lock(errors_mutex_);
            if (known_errors_.contains(normalized)) {
                return;  // Already seen this error type
            }
        }

        // New error type detected
        {
            std::unique_lock lock(errors_mutex_);
            known_errors_.insert(normalized);
        }

        anomaly_event event;
        event.anomaly_type = anomaly_event::type::new_error_type;
        event.detected_at = now;
        event.description = "New error type detected: " + entry.message;
        event.related_entries.push_back(entry);

        notify_anomaly(event);
        new_error_types_.fetch_add(1, std::memory_order_relaxed);
    }

    static std::string normalize_error_message(const std::string& message) {
        // Remove numbers (IDs, timestamps, etc.)
        std::string normalized = std::regex_replace(message,
            std::regex(R"(\d+)"), "N");

        // Remove hex values
        normalized = std::regex_replace(normalized,
            std::regex(R"(0x[0-9a-fA-F]+)"), "HEX");

        // Remove UUIDs
        normalized = std::regex_replace(normalized,
            std::regex(R"([0-9a-fA-F]{8}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{12})"),
            "UUID");

        return normalized;
    }

    void collect_related_entries(anomaly_event& event,
                                const std::deque<timestamped_entry>& window) const {
        size_t count = 0;
        for (auto it = window.rbegin();
             it != window.rend() && count < config_.max_related_entries;
             ++it, ++count) {
            event.related_entries.push_back(it->entry);
        }
    }

    void notify_anomaly(const anomaly_event& event) {
        anomalies_detected_.fetch_add(1, std::memory_order_relaxed);

        std::shared_lock lock(callback_mutex_);
        if (callback_) {
            callback_(event);
        }
    }

    // Configuration
    realtime_analysis_config config_;

    // Callback
    anomaly_callback callback_;
    mutable std::shared_mutex callback_mutex_;

    // Sliding windows
    std::deque<timestamped_entry> log_window_;
    std::deque<timestamped_entry> error_window_;
    std::deque<double> baseline_rates_;
    mutable std::shared_mutex window_mutex_;

    // Pattern alerts
    std::vector<pattern_alert> patterns_;
    mutable std::shared_mutex patterns_mutex_;

    // Known error types for new error detection
    std::unordered_set<std::string> known_errors_;
    mutable std::shared_mutex errors_mutex_;

    // Rate limiting
    std::chrono::system_clock::time_point last_rate_check_;
    std::chrono::system_clock::time_point last_spike_alert_;
    mutable std::shared_mutex rate_limit_mutex_;

    // Statistics (atomic for lock-free reads)
    std::atomic<size_t> total_analyzed_{0};
    std::atomic<size_t> total_errors_{0};
    std::atomic<size_t> anomalies_detected_{0};
    std::atomic<size_t> error_spikes_{0};
    std::atomic<size_t> pattern_matches_{0};
    std::atomic<size_t> rate_anomalies_{0};
    std::atomic<size_t> new_error_types_{0};
    mutable std::shared_mutex stats_mutex_;
};

/**
 * @brief Factory for creating configured realtime log analyzers
 * @since 3.2.0
 */
class realtime_analyzer_factory {
public:
    /**
     * @brief Create a basic realtime analyzer with default settings
     */
    static std::unique_ptr<realtime_log_analyzer> create_basic() {
        return std::make_unique<realtime_log_analyzer>();
    }

    /**
     * @brief Create a realtime analyzer with custom configuration
     * @param config Analysis configuration
     */
    static std::unique_ptr<realtime_log_analyzer> create(
        const realtime_analysis_config& config) {
        return std::make_unique<realtime_log_analyzer>(config);
    }

    /**
     * @brief Create a production-ready analyzer with sensible defaults
     * @param error_threshold Errors per minute threshold (default: 50)
     * @param callback Anomaly callback function
     */
    static std::unique_ptr<realtime_log_analyzer> create_production(
        size_t error_threshold = 50,
        realtime_log_analyzer::anomaly_callback callback = nullptr) {

        realtime_analysis_config config;
        config.error_spike_threshold = error_threshold;
        config.rate_anomaly_high_threshold = 1000;
        config.rate_anomaly_low_threshold = 10;
        config.track_new_errors = true;
        config.enable_rate_anomaly_detection = true;

        auto analyzer = std::make_unique<realtime_log_analyzer>(config);
        if (callback) {
            analyzer->set_anomaly_callback(std::move(callback));
        }
        return analyzer;
    }
};

} // namespace kcenon::logger::analysis
