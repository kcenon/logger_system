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

#pragma once

#include <kcenon/logger/interfaces/log_writer_interface.h>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

namespace integration_tests {

namespace fs = std::filesystem;

/**
 * @class ScopedTimer
 * @brief RAII timer for measuring execution time
 */
class ScopedTimer {
public:
    using clock_type = std::chrono::high_resolution_clock;
    using duration_type = std::chrono::nanoseconds;

    explicit ScopedTimer(std::function<void(duration_type)> callback = nullptr)
        : start_(clock_type::now()), callback_(std::move(callback)) {}

    ~ScopedTimer() {
        auto duration = std::chrono::duration_cast<duration_type>(clock_type::now() - start_);
        if (callback_) {
            callback_(duration);
        }
    }

    duration_type elapsed() const {
        return std::chrono::duration_cast<duration_type>(clock_type::now() - start_);
    }

private:
    clock_type::time_point start_;
    std::function<void(duration_type)> callback_;
};

/**
 * @class PerformanceMetrics
 * @brief Collects and calculates performance statistics
 */
class PerformanceMetrics {
public:
    void add_sample(std::chrono::nanoseconds duration) {
        samples_.push_back(duration.count());
    }

    void add_sample(int64_t nanoseconds) {
        samples_.push_back(nanoseconds);
    }

    double mean() const {
        if (samples_.empty())
            return 0.0;
        int64_t sum = 0;
        for (auto s : samples_) {
            sum += s;
        }
        return static_cast<double>(sum) / samples_.size();
    }

    int64_t min() const {
        if (samples_.empty())
            return 0;
        return *std::min_element(samples_.begin(), samples_.end());
    }

    int64_t max() const {
        if (samples_.empty())
            return 0;
        return *std::max_element(samples_.begin(), samples_.end());
    }

    int64_t p50() const {
        return percentile(50);
    }

    int64_t p95() const {
        return percentile(95);
    }

    int64_t p99() const {
        return percentile(99);
    }

    size_t count() const {
        return samples_.size();
    }

    void clear() {
        samples_.clear();
    }

private:
    int64_t percentile(int p) const {
        if (samples_.empty())
            return 0;
        auto sorted = samples_;
        std::sort(sorted.begin(), sorted.end());
        size_t index = (sorted.size() * p) / 100;
        if (index >= sorted.size())
            index = sorted.size() - 1;
        return sorted[index];
    }

    std::vector<int64_t> samples_;
};

/**
 * @class WorkSimulator
 * @brief Simulates CPU work for testing
 */
class WorkSimulator {
public:
    /**
     * @brief Simulate CPU work for specified duration
     */
    static void simulate_work(std::chrono::microseconds duration) {
        auto start = std::chrono::high_resolution_clock::now();
        volatile int64_t sum = 0;
        while (std::chrono::high_resolution_clock::now() - start < duration) {
            sum += 1;
        }
    }

    /**
     * @brief Simulate variable CPU work with random duration
     */
    static void simulate_variable_work(std::chrono::microseconds min_duration,
                                       std::chrono::microseconds max_duration) {
        static thread_local std::mt19937 gen(std::random_device{}());
        std::uniform_int_distribution<> dis(static_cast<int>(min_duration.count()),
                                            static_cast<int>(max_duration.count()));

        simulate_work(std::chrono::microseconds(dis(gen)));
    }
};

/**
 * @class BarrierSync
 * @brief Simple barrier synchronization for tests
 */
class BarrierSync {
public:
    explicit BarrierSync(size_t count) : threshold_(count), count_(count), generation_(0) {}

    void arrive_and_wait() {
        std::unique_lock<std::mutex> lock(mutex_);
        auto gen = generation_;
        if (--count_ == 0) {
            generation_++;
            count_ = threshold_;
            cv_.notify_all();
        } else {
            cv_.wait(lock, [this, gen] { return gen != generation_; });
        }
    }

private:
    const size_t threshold_;
    size_t count_;
    size_t generation_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

/**
 * @class RateLimiter
 * @brief Controls the rate of operations
 */
class RateLimiter {
public:
    explicit RateLimiter(size_t ops_per_second)
        : interval_(std::chrono::seconds(1) / ops_per_second)
        , last_op_(std::chrono::steady_clock::now()) {}

    void acquire() {
        std::unique_lock<std::mutex> lock(mutex_);
        auto now = std::chrono::steady_clock::now();
        auto elapsed = now - last_op_;

        if (elapsed < interval_) {
            std::this_thread::sleep_for(interval_ - elapsed);
        }

        last_op_ = std::chrono::steady_clock::now();
    }

private:
    const std::chrono::nanoseconds interval_;
    std::chrono::steady_clock::time_point last_op_;
    std::mutex mutex_;
};

/**
 * @class TempLogFile
 * @brief RAII wrapper for temporary log files
 */
class TempLogFile {
public:
    explicit TempLogFile(const std::string& prefix = "test_log")
        : path_(fs::temp_directory_path() /
                (prefix + "_" +
                 std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()) +
                 ".log")) {}

    ~TempLogFile() {
        if (fs::exists(path_)) {
            std::error_code ec;
            fs::remove(path_, ec);
        }
    }

    const std::string& path() const {
        return path_string_;
    }

    const fs::path& get_path() const {
        return path_;
    }

    std::string read() const {
        std::ifstream file(path_);
        if (!file.is_open()) {
            return "";
        }
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    size_t line_count() const {
        std::ifstream file(path_);
        if (!file.is_open()) {
            return 0;
        }
        size_t count = 0;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                count++;
            }
        }
        return count;
    }

private:
    fs::path path_;
    std::string path_string_{path_.string()};
};

/**
 * @class MockLogWriter
 * @brief Mock writer for testing log routing
 */
class MockLogWriter : public kcenon::logger::log_writer_interface {
public:
    MockLogWriter() = default;

    kcenon::common::VoidResult write(const kcenon::logger::log_entry& entry) override {
        write_count_.fetch_add(1);
        last_message_ = entry.message;
        return kcenon::common::ok();
    }

    kcenon::common::VoidResult flush() override {
        flush_count_.fetch_add(1);
        return kcenon::common::ok();
    }

    bool is_healthy() const override {
        return healthy_;
    }

    size_t get_write_count() const {
        return write_count_.load();
    }

    size_t get_flush_count() const {
        return flush_count_.load();
    }

    std::string get_last_message() const {
        return last_message_;
    }

    void set_healthy(bool healthy) {
        healthy_ = healthy;
    }

    void reset() {
        write_count_.store(0);
        flush_count_.store(0);
        last_message_.clear();
    }

private:
    std::atomic<size_t> write_count_{0};
    std::atomic<size_t> flush_count_{0};
    std::string last_message_;
    bool healthy_{true};
};

/**
 * @brief Helper to wait for atomic counter to reach expected value
 */
template <typename T>
bool WaitForAtomicValue(const std::atomic<T>& counter, T expected,
                        std::chrono::milliseconds timeout) {
    auto start = std::chrono::steady_clock::now();
    while (counter.load() < expected) {
        if (std::chrono::steady_clock::now() - start > timeout) {
            return false;
        }
        std::this_thread::yield();
    }
    return true;
}

/**
 * @brief Helper to measure throughput (operations per second)
 */
inline double CalculateThroughput(size_t operations, std::chrono::nanoseconds duration) {
    if (duration.count() == 0)
        return 0.0;
    return (static_cast<double>(operations) * 1'000'000'000.0) / duration.count();
}

/**
 * @brief Helper to format duration for display
 */
inline std::string FormatDuration(std::chrono::nanoseconds duration) {
    auto ns = duration.count();
    if (ns < 1'000) {
        return std::to_string(ns) + " ns";
    } else if (ns < 1'000'000) {
        return std::to_string(ns / 1'000) + " us";
    } else if (ns < 1'000'000'000) {
        return std::to_string(ns / 1'000'000) + " ms";
    } else {
        return std::to_string(ns / 1'000'000'000) + " s";
    }
}

/**
 * @brief Generate random string for testing
 */
inline std::string GenerateRandomString(size_t length) {
    static const char charset[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    static thread_local std::mt19937 gen(std::random_device{}());
    std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);

    std::string result;
    result.reserve(length);
    for (size_t i = 0; i < length; ++i) {
        result += charset[dis(gen)];
    }
    return result;
}

}  // namespace integration_tests
