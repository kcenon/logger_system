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

#include "overflow_policy.h"
#include <algorithm>
#include <deque>
#include <numeric>

namespace logger_module {

// Simple log_entry structure for compilation
struct log_entry {
    thread_module::log_level level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
};

// Drop oldest policy implementation
bool drop_oldest_policy::handle_overflow(const log_entry& /* entry */,
                                        std::queue<log_entry>& queue,
                                        size_t /* max_size */) {
    stats_.total_messages++;
    stats_.dropped_messages++;
    
    // Remove oldest entry
    if (!queue.empty()) {
        queue.pop();
    }
    
    // Return true to indicate the new entry can be added
    return true;
}

// Drop newest policy implementation
bool drop_newest_policy::handle_overflow(const log_entry& /* entry */,
                                        std::queue<log_entry>& /* queue */,
                                        size_t /* max_size */) {
    stats_.total_messages++;
    stats_.dropped_messages++;
    
    // Return false to indicate the new entry should be dropped
    return false;
}

// Block policy implementation
bool block_policy::handle_overflow(const log_entry& /* entry */,
                                  std::queue<log_entry>& queue,
                                  size_t max_size) {
    stats_.total_messages++;
    stats_.blocked_count++;
    
    auto start_time = std::chrono::steady_clock::now();
    
    std::unique_lock<std::mutex> lock(mutex_);
    bool space_available = cv_.wait_for(lock, timeout_, [&queue, max_size] {
        return queue.size() < max_size;
    });
    
    auto block_duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - start_time);
    
    stats_.total_block_time += block_duration;
    if (block_duration > stats_.max_block_time) {
        stats_.max_block_time = block_duration;
    }
    
    if (!space_available) {
        stats_.dropped_messages++;
        return false;
    }
    
    return true;
}

// Grow policy implementation
bool grow_policy::handle_overflow(const log_entry& /* entry */,
                                 std::queue<log_entry>& /* queue */,
                                 size_t max_size) {
    stats_.total_messages++;
    
    size_t current_total = max_size + current_growth_.load();
    
    if (current_total >= max_total_size_) {
        // Cannot grow anymore, drop the message
        stats_.dropped_messages++;
        return false;
    }
    
    // Calculate new growth
    size_t new_growth = std::min(
        max_size * growth_factor_,
        max_total_size_ - max_size
    );
    
    current_growth_ = new_growth;
    stats_.grow_count++;
    
    size_t new_max = max_size + new_growth;
    if (new_max > stats_.max_size_reached) {
        stats_.max_size_reached = new_max;
    }
    
    return true;
}

// Adaptive backpressure implementation
class adaptive_backpressure::impl {
public:
    explicit impl(const config& cfg)
        : config_(cfg)
        , current_batch_size_(cfg.initial_batch_size)
        , current_flush_interval_(cfg.initial_flush_interval)
        , enabled_(true)
        , adaptation_count_(0)
        , increase_count_(0)
        , decrease_count_(0) {
        load_samples_.reserve(cfg.sample_count);
        processing_times_.reserve(cfg.sample_count);
    }
    
    void update_metrics(double queue_usage, std::chrono::milliseconds processing_time) {
        std::lock_guard<std::mutex> lock(mutex_);
        
        // Add new samples
        auto now = std::chrono::steady_clock::now();
        load_samples_.push_back({queue_usage, now});
        processing_times_.push_back({processing_time, now});
        
        // Remove old samples outside the window
        auto window_start = now - config_.metrics_window;
        
        load_samples_.erase(
            std::remove_if(load_samples_.begin(), load_samples_.end(),
                [window_start](const auto& sample) {
                    return sample.timestamp < window_start;
                }),
            load_samples_.end()
        );
        
        processing_times_.erase(
            std::remove_if(processing_times_.begin(), processing_times_.end(),
                [window_start](const auto& sample) {
                    return sample.timestamp < window_start;
                }),
            processing_times_.end()
        );
        
        // Limit to sample count
        if (load_samples_.size() > config_.sample_count) {
            load_samples_.erase(load_samples_.begin(), 
                load_samples_.begin() + (load_samples_.size() - config_.sample_count));
        }
        
        if (processing_times_.size() > config_.sample_count) {
            processing_times_.erase(processing_times_.begin(),
                processing_times_.begin() + (processing_times_.size() - config_.sample_count));
        }
        
        // Calculate current load
        if (!load_samples_.empty()) {
            double sum = std::accumulate(load_samples_.begin(), load_samples_.end(), 0.0,
                [](double acc, const auto& sample) {
                    return acc + sample.value;
                });
            current_load_ = sum / load_samples_.size();
        }
        
        // Trigger adaptation if enabled
        if (enabled_) {
            adapt_internal();
        }
    }
    
    void adapt_internal() {
        adaptation_count_++;
        
        if (current_load_ > config_.load_threshold_high) {
            // High load - increase batch size, decrease flush interval
            increase_pressure();
            increase_count_++;
        } else if (current_load_ < config_.load_threshold_low) {
            // Low load - decrease batch size, increase flush interval
            decrease_pressure();
            decrease_count_++;
        }
    }
    
    void increase_pressure() {
        // Increase batch size
        size_t new_batch_size = static_cast<size_t>(
            current_batch_size_ * (1.0 + config_.adaptation_rate)
        );
        current_batch_size_ = std::min(new_batch_size, config_.max_batch_size);
        
        // Decrease flush interval
        auto new_interval = std::chrono::milliseconds(static_cast<long>(
            current_flush_interval_.count() * (1.0 - config_.adaptation_rate)
        ));
        current_flush_interval_ = std::max(new_interval, config_.min_flush_interval);
    }
    
    void decrease_pressure() {
        // Decrease batch size
        size_t new_batch_size = static_cast<size_t>(
            current_batch_size_ * (1.0 - config_.adaptation_rate)
        );
        current_batch_size_ = std::max(new_batch_size, config_.min_batch_size);
        
        // Increase flush interval
        auto new_interval = std::chrono::milliseconds(static_cast<long>(
            current_flush_interval_.count() * (1.0 + config_.adaptation_rate)
        ));
        current_flush_interval_ = std::min(new_interval, config_.max_flush_interval);
    }
    
    void reset() {
        std::lock_guard<std::mutex> lock(mutex_);
        
        current_batch_size_ = config_.initial_batch_size;
        current_flush_interval_ = config_.initial_flush_interval;
        current_load_ = 0.0;
        adaptation_count_ = 0;
        increase_count_ = 0;
        decrease_count_ = 0;
        
        load_samples_.clear();
        processing_times_.clear();
    }
    
    size_t get_batch_size() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return current_batch_size_;
    }
    
    std::chrono::milliseconds get_flush_interval() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return current_flush_interval_;
    }
    
    double get_current_load() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return current_load_;
    }
    
    void set_enabled(bool enabled) {
        std::lock_guard<std::mutex> lock(mutex_);
        enabled_ = enabled;
    }
    
    bool is_enabled() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return enabled_;
    }
    
    adaptive_backpressure::adaptation_stats get_stats() const {
        std::lock_guard<std::mutex> lock(mutex_);
        
        return {
            current_batch_size_,
            current_flush_interval_,
            current_load_,
            adaptation_count_,
            increase_count_,
            decrease_count_
        };
    }
    
private:
    template<typename T>
    struct timestamped_sample {
        T value;
        std::chrono::steady_clock::time_point timestamp;
    };
    
    config config_;
    mutable std::mutex mutex_;
    
    // Current adaptive values
    size_t current_batch_size_;
    std::chrono::milliseconds current_flush_interval_;
    double current_load_ = 0.0;
    
    // Metrics
    std::vector<timestamped_sample<double>> load_samples_;
    std::vector<timestamped_sample<std::chrono::milliseconds>> processing_times_;
    
    // State
    bool enabled_;
    size_t adaptation_count_;
    size_t increase_count_;
    size_t decrease_count_;
};

// Adaptive backpressure public interface
adaptive_backpressure::adaptive_backpressure()
    : pimpl_(std::make_unique<impl>(config{})) {
}

adaptive_backpressure::adaptive_backpressure(const config& cfg)
    : pimpl_(std::make_unique<impl>(cfg)) {
}

adaptive_backpressure::~adaptive_backpressure() = default;

void adaptive_backpressure::update_metrics(double queue_usage,
                                          std::chrono::milliseconds processing_time) {
    pimpl_->update_metrics(queue_usage, processing_time);
}

size_t adaptive_backpressure::get_batch_size() const {
    return pimpl_->get_batch_size();
}

std::chrono::milliseconds adaptive_backpressure::get_flush_interval() const {
    return pimpl_->get_flush_interval();
}

void adaptive_backpressure::adapt() {
    pimpl_->adapt_internal();
}

double adaptive_backpressure::get_current_load() const {
    return pimpl_->get_current_load();
}

void adaptive_backpressure::reset() {
    pimpl_->reset();
}

void adaptive_backpressure::set_enabled(bool enabled) {
    pimpl_->set_enabled(enabled);
}

bool adaptive_backpressure::is_enabled() const {
    return pimpl_->is_enabled();
}

adaptive_backpressure::adaptation_stats adaptive_backpressure::get_stats() const {
    return pimpl_->get_stats();
}

// Custom policy implementation
class custom_overflow_policy : public overflow_policy_base {
public:
    explicit custom_overflow_policy(
        std::function<bool(const log_entry&, std::queue<log_entry>&, size_t)> handler)
        : handler_(handler) {}
    
    bool handle_overflow(const log_entry& entry,
                        std::queue<log_entry>& queue,
                        size_t max_size) override {
        stats_.total_messages++;
        bool result = handler_(entry, queue, max_size);
        if (!result) {
            stats_.dropped_messages++;
        }
        return result;
    }
    
    overflow_policy_type get_type() const override {
        return overflow_policy_type::custom;
    }
    
private:
    std::function<bool(const log_entry&, std::queue<log_entry>&, size_t)> handler_;
};

// Factory implementation
std::unique_ptr<overflow_policy_base> overflow_policy_factory::create(overflow_policy_type type) {
    switch (type) {
        case overflow_policy_type::drop_oldest:
            return std::make_unique<drop_oldest_policy>();
        
        case overflow_policy_type::drop_newest:
            return std::make_unique<drop_newest_policy>();
        
        case overflow_policy_type::block:
            return std::make_unique<block_policy>();
        
        case overflow_policy_type::grow:
            return std::make_unique<grow_policy>();
        
        default:
            return std::make_unique<drop_oldest_policy>();
    }
}

std::unique_ptr<overflow_policy_base> overflow_policy_factory::create_custom(
    std::function<bool(const log_entry&, std::queue<log_entry>&, size_t)> handler) {
    return std::make_unique<custom_overflow_policy>(handler);
}

} // namespace logger_module