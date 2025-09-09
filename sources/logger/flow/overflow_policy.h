#pragma once

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

#include "../error_codes.h"
#include <memory>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <atomic>
#include <functional>

// Conditional include based on build configuration
#ifdef USE_THREAD_SYSTEM
    #include <interfaces/logger_interface.h>
#else
    #include "../logger_interface.h"
#endif

namespace logger_module {

// Forward declaration
struct log_entry;

/**
 * @brief Overflow policy types
 */
enum class overflow_policy_type {
    drop_oldest,    ///< Drop oldest messages when queue is full
    drop_newest,    ///< Drop newest messages when queue is full
    block,          ///< Block until space is available
    grow,           ///< Dynamically grow the queue
    custom          ///< Custom policy implementation
};

/**
 * @brief Statistics for overflow handling
 */
struct overflow_stats {
    std::atomic<size_t> total_messages{0};
    std::atomic<size_t> dropped_messages{0};
    std::atomic<size_t> blocked_count{0};
    std::atomic<size_t> grow_count{0};
    std::atomic<size_t> current_size{0};
    std::atomic<size_t> max_size_reached{0};
    std::chrono::milliseconds total_block_time{0};
    std::chrono::milliseconds max_block_time{0};
    
    // Default constructor
    overflow_stats() = default;
    
    // Copy constructor (needed because of atomic members)
    overflow_stats(const overflow_stats& other) 
        : total_messages(other.total_messages.load()),
          dropped_messages(other.dropped_messages.load()),
          blocked_count(other.blocked_count.load()),
          grow_count(other.grow_count.load()),
          current_size(other.current_size.load()),
          max_size_reached(other.max_size_reached.load()),
          total_block_time(other.total_block_time),
          max_block_time(other.max_block_time) {}
    
    // Copy assignment operator
    overflow_stats& operator=(const overflow_stats& other) {
        if (this != &other) {
            total_messages = other.total_messages.load();
            dropped_messages = other.dropped_messages.load();
            blocked_count = other.blocked_count.load();
            grow_count = other.grow_count.load();
            current_size = other.current_size.load();
            max_size_reached = other.max_size_reached.load();
            total_block_time = other.total_block_time;
            max_block_time = other.max_block_time;
        }
        return *this;
    }
    
    double get_drop_rate() const {
        size_t total = total_messages.load();
        return total > 0 ? 
            static_cast<double>(dropped_messages.load()) / total * 100.0 : 0.0;
    }
    
    void reset() {
        total_messages = 0;
        dropped_messages = 0;
        blocked_count = 0;
        grow_count = 0;
        current_size = 0;
        max_size_reached = 0;
        total_block_time = std::chrono::milliseconds{0};
        max_block_time = std::chrono::milliseconds{0};
    }
};

/**
 * @brief Base class for overflow policies
 */
class overflow_policy_base {
public:
    virtual ~overflow_policy_base() = default;
    
    /**
     * @brief Handle overflow condition
     * @param entry The log entry trying to be added
     * @param queue Current queue (for manipulation if needed)
     * @param max_size Maximum queue size
     * @return true if entry was handled, false if should be dropped
     */
    virtual bool handle_overflow(const log_entry& entry,
                                 std::queue<log_entry>& queue,
                                 size_t max_size) = 0;
    
    /**
     * @brief Get policy type
     * @return The overflow policy type
     */
    virtual overflow_policy_type get_type() const = 0;
    
    /**
     * @brief Get overflow statistics
     * @return Current statistics
     */
    virtual overflow_stats get_stats() const { 
        overflow_stats copy;
        copy.total_messages = stats_.total_messages.load();
        copy.dropped_messages = stats_.dropped_messages.load();
        copy.blocked_count = stats_.blocked_count.load();
        copy.grow_count = stats_.grow_count.load();
        copy.current_size = stats_.current_size.load();
        copy.max_size_reached = stats_.max_size_reached.load();
        copy.total_block_time = stats_.total_block_time;
        copy.max_block_time = stats_.max_block_time;
        return copy;
    }
    
    /**
     * @brief Reset statistics
     */
    virtual void reset_stats() { stats_.reset(); }
    
protected:
    overflow_stats stats_;
};

/**
 * @brief Drop oldest messages when queue is full
 */
class drop_oldest_policy : public overflow_policy_base {
public:
    bool handle_overflow(const log_entry& entry,
                        std::queue<log_entry>& queue,
                        size_t max_size) override;
    
    overflow_policy_type get_type() const override {
        return overflow_policy_type::drop_oldest;
    }
};

/**
 * @brief Drop newest messages when queue is full
 */
class drop_newest_policy : public overflow_policy_base {
public:
    bool handle_overflow(const log_entry& entry,
                        std::queue<log_entry>& queue,
                        size_t max_size) override;
    
    overflow_policy_type get_type() const override {
        return overflow_policy_type::drop_newest;
    }
};

/**
 * @brief Block until space is available
 */
class block_policy : public overflow_policy_base {
public:
    explicit block_policy(std::chrono::milliseconds timeout = std::chrono::milliseconds{1000})
        : timeout_(timeout) {}
    
    bool handle_overflow(const log_entry& entry,
                        std::queue<log_entry>& queue,
                        size_t max_size) override;
    
    overflow_policy_type get_type() const override {
        return overflow_policy_type::block;
    }
    
    /**
     * @brief Set blocking timeout
     * @param timeout Maximum time to block
     */
    void set_timeout(std::chrono::milliseconds timeout) {
        timeout_ = timeout;
    }
    
    /**
     * @brief Notify that space is available
     */
    void notify_space_available() {
        cv_.notify_all();
    }
    
private:
    std::chrono::milliseconds timeout_;
    std::mutex mutex_;
    std::condition_variable cv_;
};

/**
 * @brief Dynamically grow the queue
 */
class grow_policy : public overflow_policy_base {
public:
    explicit grow_policy(size_t growth_factor = 2, size_t max_growth = 100000)
        : growth_factor_(growth_factor), max_total_size_(max_growth) {}
    
    bool handle_overflow(const log_entry& entry,
                        std::queue<log_entry>& queue,
                        size_t max_size) override;
    
    overflow_policy_type get_type() const override {
        return overflow_policy_type::grow;
    }
    
    /**
     * @brief Get current growth size
     * @return Current additional capacity
     */
    size_t get_current_growth() const {
        return current_growth_.load();
    }
    
private:
    size_t growth_factor_;
    size_t max_total_size_;
    std::atomic<size_t> current_growth_{0};
};

/**
 * @brief Adaptive backpressure system
 * 
 * Dynamically adjusts batch sizes and flush intervals based on system load
 */
class adaptive_backpressure {
public:
    /**
     * @brief Configuration for adaptive backpressure
     */
    struct config {
        // Batch size limits
        size_t min_batch_size = 10;
        size_t max_batch_size = 1000;
        size_t initial_batch_size = 100;
        
        // Flush interval limits (milliseconds)
        std::chrono::milliseconds min_flush_interval{10};
        std::chrono::milliseconds max_flush_interval{1000};
        std::chrono::milliseconds initial_flush_interval{100};
        
        // Adaptation parameters
        double load_threshold_low = 0.3;   // Below this, decrease pressure
        double load_threshold_high = 0.7;  // Above this, increase pressure
        double adaptation_rate = 0.1;      // How quickly to adapt (0.0 - 1.0)
        
        // Metrics window
        std::chrono::seconds metrics_window{10};
        size_t sample_count = 100;
        
        // Default constructor
        config() = default;
    };
    
    adaptive_backpressure();
    explicit adaptive_backpressure(const config& cfg);
    ~adaptive_backpressure();
    
    /**
     * @brief Update metrics with current load
     * @param queue_usage Queue usage percentage (0.0 - 1.0)
     * @param processing_time Time to process last batch
     */
    void update_metrics(double queue_usage, 
                       std::chrono::milliseconds processing_time);
    
    /**
     * @brief Get current batch size
     * @return Adaptive batch size
     */
    size_t get_batch_size() const;
    
    /**
     * @brief Get current flush interval
     * @return Adaptive flush interval
     */
    std::chrono::milliseconds get_flush_interval() const;
    
    /**
     * @brief Manually trigger adaptation
     */
    void adapt();
    
    /**
     * @brief Get current load estimate
     * @return Load factor (0.0 - 1.0)
     */
    double get_current_load() const;
    
    /**
     * @brief Reset to initial values
     */
    void reset();
    
    /**
     * @brief Enable or disable adaptation
     * @param enabled Whether adaptation is enabled
     */
    void set_enabled(bool enabled);
    
    /**
     * @brief Check if adaptation is enabled
     * @return true if enabled
     */
    bool is_enabled() const;
    
    /**
     * @brief Get adaptation statistics
     */
    struct adaptation_stats {
        size_t current_batch_size;
        std::chrono::milliseconds current_flush_interval;
        double current_load;
        size_t adaptation_count;
        size_t increase_count;
        size_t decrease_count;
    };
    
    adaptation_stats get_stats() const;
    
private:
    class impl;
    std::unique_ptr<impl> pimpl_;
};

/**
 * @brief Factory for creating overflow policies
 */
class overflow_policy_factory {
public:
    /**
     * @brief Create an overflow policy
     * @param type Policy type
     * @return Unique pointer to the policy
     */
    static std::unique_ptr<overflow_policy_base> create(overflow_policy_type type);
    
    /**
     * @brief Create a custom overflow policy
     * @param handler Custom handler function
     * @return Unique pointer to the custom policy
     */
    static std::unique_ptr<overflow_policy_base> create_custom(
        std::function<bool(const log_entry&, std::queue<log_entry>&, size_t)> handler);
};

/**
 * @brief Queue with overflow policy support
 */
template<typename T>
class overflow_queue {
public:
    explicit overflow_queue(size_t max_size, 
                           std::unique_ptr<overflow_policy_base> policy = nullptr)
        : max_size_(max_size)
        , policy_(std::move(policy)) {
        if (!policy_) {
            policy_ = overflow_policy_factory::create(overflow_policy_type::drop_oldest);
        }
    }
    
    /**
     * @brief Push an item to the queue
     * @param item Item to push
     * @return true if item was added, false if dropped
     */
    bool push(T&& item) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        if (queue_.size() >= max_size_) {
            if (!policy_->handle_overflow(item, queue_, max_size_)) {
                return false;
            }
        }
        
        queue_.push(std::move(item));
        cv_.notify_one();
        return true;
    }
    
    /**
     * @brief Pop an item from the queue
     * @param item Output parameter for the popped item
     * @param timeout Maximum time to wait
     * @return true if item was popped, false on timeout
     */
    bool pop(T& item, std::chrono::milliseconds timeout = std::chrono::milliseconds{100}) {
        std::unique_lock<std::mutex> lock(mutex_);
        
        if (!cv_.wait_for(lock, timeout, [this] { return !queue_.empty() || stopped_; })) {
            return false;
        }
        
        if (stopped_) {
            return false;
        }
        
        if (queue_.empty()) {
            return false;
        }
        
        item = std::move(queue_.front());
        queue_.pop();
        
        // Notify block policy if space is available
        if (auto* block = dynamic_cast<block_policy*>(policy_.get())) {
            block->notify_space_available();
        }
        
        return true;
    }
    
    /**
     * @brief Get current queue size
     * @return Number of items in queue
     */
    size_t size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }
    
    /**
     * @brief Check if queue is empty
     * @return true if empty
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.empty();
    }
    
    /**
     * @brief Stop the queue
     */
    void stop() {
        std::lock_guard<std::mutex> lock(mutex_);
        stopped_ = true;
        cv_.notify_all();
    }
    
    /**
     * @brief Change overflow policy
     * @param policy New policy to use
     */
    void set_policy(std::unique_ptr<overflow_policy_base> policy) {
        std::lock_guard<std::mutex> lock(mutex_);
        policy_ = std::move(policy);
    }
    
    /**
     * @brief Get overflow statistics
     * @return Current statistics
     */
    overflow_stats get_stats() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return policy_->get_stats();
    }
    
private:
    size_t max_size_;
    std::queue<T> queue_;
    std::unique_ptr<overflow_policy_base> policy_;
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> stopped_{false};
};

} // namespace logger_module