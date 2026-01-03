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
 * @file analysis.cppm
 * @brief Analysis partition for kcenon.logger module
 * @author kcenon
 * @since 3.0.0
 *
 * @details This partition exports log analysis utilities:
 * - analyzed_log_entry: Structured log entry for analysis
 * - analysis_stats: Statistical analysis results
 * - log_analyzer: Log analysis engine
 * - analyzer_factory: Factory for creating analyzers
 *
 * The analysis module provides functionality for:
 * - Filtering logs by level, time range, or content
 * - Computing statistics (counts, rates, distributions)
 * - Detecting error patterns
 * - Generating summary reports
 */

export module kcenon.logger:analysis;

// Standard library imports
import <chrono>;
import <memory>;
import <string>;
import <unordered_map>;
import <vector>;

// Import core partition for types
import :core;

export namespace kcenon::logger::analysis {

/**
 * @brief Log entry structure for analysis
 *
 * @details Contains all information about a log entry that can be analyzed.
 */
struct analyzed_log_entry {
    logger_system::log_level level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::string source_file;
    int source_line;
    std::string function_name;
};

/**
 * @brief Analysis result statistics
 *
 * @details Contains aggregated statistics from log analysis.
 */
struct analysis_stats {
    size_t total_entries = 0;
    std::unordered_map<logger_system::log_level, size_t> level_counts;
    std::chrono::system_clock::time_point earliest_timestamp;
    std::chrono::system_clock::time_point latest_timestamp;
    std::vector<std::string> most_frequent_messages;
    std::unordered_map<std::string, size_t> error_patterns;
};

/**
 * @brief Log analyzer for processing and analyzing log data
 *
 * @details Provides functionality to:
 * - Add and store log entries
 * - Filter entries by various criteria
 * - Compute statistics and rates
 * - Generate summary reports
 */
class log_analyzer {
private:
    std::vector<analyzed_log_entry> entries_;
    analysis_stats cached_stats_;
    bool stats_dirty_ = true;

public:
    /**
     * @brief Add a log entry for analysis
     * @param entry The entry to add
     */
    void add_entry(const analyzed_log_entry& entry) {
        entries_.push_back(entry);
        stats_dirty_ = true;
    }

    /**
     * @brief Add multiple log entries
     * @param entries Vector of entries to add
     */
    void add_entries(const std::vector<analyzed_log_entry>& entries) {
        entries_.insert(entries_.end(), entries.begin(), entries.end());
        stats_dirty_ = true;
    }

    /**
     * @brief Clear all entries
     */
    void clear() {
        entries_.clear();
        cached_stats_ = analysis_stats{};
        stats_dirty_ = true;
    }

    /**
     * @brief Get analysis statistics
     * @return Reference to computed statistics
     */
    const analysis_stats& get_stats() {
        if (stats_dirty_) {
            update_stats();
            stats_dirty_ = false;
        }
        return cached_stats_;
    }

    /**
     * @brief Filter entries by log level
     * @param level The level to filter for
     * @return Vector of matching entries
     */
    std::vector<analyzed_log_entry> filter_by_level(logger_system::log_level level) const {
        std::vector<analyzed_log_entry> filtered;
        for (const auto& entry : entries_) {
            if (entry.level == level) {
                filtered.push_back(entry);
            }
        }
        return filtered;
    }

    /**
     * @brief Filter entries by time range
     * @param start Start of time range
     * @param end End of time range
     * @return Vector of entries within the time range
     */
    std::vector<analyzed_log_entry> filter_by_time_range(
        const std::chrono::system_clock::time_point& start,
        const std::chrono::system_clock::time_point& end) const {

        std::vector<analyzed_log_entry> filtered;
        for (const auto& entry : entries_) {
            if (entry.timestamp >= start && entry.timestamp <= end) {
                filtered.push_back(entry);
            }
        }
        return filtered;
    }

    /**
     * @brief Find entries containing specific text
     * @param search_text Text to search for
     * @return Vector of matching entries
     */
    std::vector<analyzed_log_entry> search_messages(const std::string& search_text) const {
        std::vector<analyzed_log_entry> results;
        for (const auto& entry : entries_) {
            if (entry.message.find(search_text) != std::string::npos) {
                results.push_back(entry);
            }
        }
        return results;
    }

    /**
     * @brief Get error rate for a time window
     * @param window Time window to analyze (default: 60 minutes)
     * @return Error rate as a decimal (0.0 to 1.0)
     */
    double get_error_rate(const std::chrono::minutes& window = std::chrono::minutes(60)) const {
        auto now = std::chrono::system_clock::now();
        auto start_time = now - window;

        size_t total_in_window = 0;
        size_t errors_in_window = 0;

        for (const auto& entry : entries_) {
            if (entry.timestamp >= start_time) {
                total_in_window++;
                if (entry.level == logger_system::log_level::error ||
                    entry.level == logger_system::log_level::fatal) {
                    errors_in_window++;
                }
            }
        }

        return total_in_window > 0 ?
            static_cast<double>(errors_in_window) / static_cast<double>(total_in_window) : 0.0;
    }

    /**
     * @brief Generate summary report
     * @return Formatted summary string
     */
    std::string generate_summary_report() {
        const auto& stats = get_stats();

        std::string report = "=== Log Analysis Summary ===\n";
        report += "Total Entries: " + std::to_string(stats.total_entries) + "\n";
        report += "Level Distribution:\n";

        for (const auto& [level, count] : stats.level_counts) {
            report += "  " + level_to_string(level) + ": " + std::to_string(count) + "\n";
        }

        if (stats.total_entries > 0) {
            auto duration = std::chrono::duration_cast<std::chrono::minutes>(
                stats.latest_timestamp - stats.earliest_timestamp);
            report += "Time Range: " + std::to_string(duration.count()) + " minutes\n";
        }

        return report;
    }

    /**
     * @brief Get the total number of entries
     * @return Number of stored entries
     */
    size_t size() const {
        return entries_.size();
    }

    /**
     * @brief Check if the analyzer has no entries
     * @return true if empty
     */
    bool empty() const {
        return entries_.empty();
    }

private:
    void update_stats() {
        cached_stats_ = analysis_stats{};
        cached_stats_.total_entries = entries_.size();

        if (entries_.empty()) {
            return;
        }

        // Initialize timestamps
        cached_stats_.earliest_timestamp = entries_[0].timestamp;
        cached_stats_.latest_timestamp = entries_[0].timestamp;

        // Count levels and update timestamps
        for (const auto& entry : entries_) {
            cached_stats_.level_counts[entry.level]++;

            if (entry.timestamp < cached_stats_.earliest_timestamp) {
                cached_stats_.earliest_timestamp = entry.timestamp;
            }
            if (entry.timestamp > cached_stats_.latest_timestamp) {
                cached_stats_.latest_timestamp = entry.timestamp;
            }
        }
    }

    std::string level_to_string(logger_system::log_level level) const {
        switch (level) {
            case logger_system::log_level::trace: return "TRACE";
            case logger_system::log_level::debug: return "DEBUG";
            case logger_system::log_level::info: return "INFO";
            case logger_system::log_level::warn: return "WARN";
            case logger_system::log_level::error: return "ERROR";
            case logger_system::log_level::fatal: return "FATAL";
            case logger_system::log_level::off: return "OFF";
            default: return "UNKNOWN";
        }
    }
};

/**
 * @brief Factory for creating log analyzers
 */
class analyzer_factory {
public:
    /**
     * @brief Create a basic log analyzer
     * @return Unique pointer to a new log_analyzer
     */
    static std::unique_ptr<log_analyzer> create_basic() {
        return std::make_unique<log_analyzer>();
    }
};

} // namespace kcenon::logger::analysis
