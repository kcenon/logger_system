#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file object_pool.h
 * @brief High-performance object pooling system for log entries
 *
 * This file provides thread-local and global object pools to reduce
 * memory allocation overhead in logging operations.
 */

#include <memory>
#include <vector>
#include <stack>
#include <mutex>
#include <atomic>
#include <thread>
#include <unordered_map>
#include <type_traits>

namespace logger_module::memory {

/**
 * @brief Thread-safe object pool template
 * @tparam T Type of objects to pool
 */
template<typename T>
class object_pool {
public:
    /**
     * @brief Configuration for object pool
     */
    struct config {
        size_t initial_size{100};      ///< Initial pool size
        size_t max_size{1000};         ///< Maximum pool size
        size_t growth_factor{2};       ///< Growth factor when expanding
        bool thread_local_cache{true}; ///< Enable thread-local caching
        size_t thread_cache_size{50};  ///< Size of thread-local cache

        config() = default;
    };

    /**
     * @brief Pool statistics
     */
    struct pool_stats {
        std::atomic<uint64_t> total_allocations{0};
        std::atomic<uint64_t> total_deallocations{0};
        std::atomic<uint64_t> pool_hits{0};
        std::atomic<uint64_t> pool_misses{0};
        std::atomic<uint64_t> current_pool_size{0};
        std::atomic<uint64_t> peak_pool_size{0};

        double hit_ratio() const {
            const auto hits = pool_hits.load();
            const auto total = hits + pool_misses.load();
            return total > 0 ? static_cast<double>(hits) / total : 0.0;
        }

        void reset() {
            total_allocations = 0;
            total_deallocations = 0;
            pool_hits = 0;
            pool_misses = 0;
            current_pool_size = 0;
            peak_pool_size = 0;
        }
    };

    /**
     * @brief RAII wrapper for pooled objects
     */
    class pooled_object {
    public:
        pooled_object(T* obj, object_pool* pool)
            : object_(obj), pool_(pool) {}

        ~pooled_object() {
            if (object_ && pool_) {
                pool_->return_object(object_);
            }
        }

        // Move-only semantics
        pooled_object(const pooled_object&) = delete;
        pooled_object& operator=(const pooled_object&) = delete;

        pooled_object(pooled_object&& other) noexcept
            : object_(other.object_), pool_(other.pool_) {
            other.object_ = nullptr;
            other.pool_ = nullptr;
        }

        pooled_object& operator=(pooled_object&& other) noexcept {
            if (this != &other) {
                if (object_ && pool_) {
                    pool_->return_object(object_);
                }
                object_ = other.object_;
                pool_ = other.pool_;
                other.object_ = nullptr;
                other.pool_ = nullptr;
            }
            return *this;
        }

        T* get() const { return object_; }
        T& operator*() const { return *object_; }
        T* operator->() const { return object_; }

        explicit operator bool() const { return object_ != nullptr; }

        /**
         * @brief Release object back to pool explicitly
         */
        void reset() {
            if (object_ && pool_) {
                pool_->return_object(object_);
                object_ = nullptr;
                pool_ = nullptr;
            }
        }

    private:
        T* object_;
        object_pool* pool_;
    };

    /**
     * @brief Constructor
     * @param cfg Pool configuration
     */
    explicit object_pool(const config& cfg = config{})
        : config_(cfg)
        , stats_{} {
        // Pre-allocate initial objects
        std::lock_guard<std::mutex> lock(pool_mutex_);
        for (size_t i = 0; i < config_.initial_size; ++i) {
            pool_.push(create_object());
        }
        stats_.current_pool_size = config_.initial_size;
        stats_.peak_pool_size = config_.initial_size;
    }

    /**
     * @brief Destructor
     */
    ~object_pool() {
        clear();
    }

    /**
     * @brief Acquire an object from the pool
     * @param args Constructor arguments for new objects
     * @return RAII wrapper for pooled object
     */
    template<typename... Args>
    pooled_object acquire(Args&&... args) {
        T* obj = get_object();

        if (obj) {
            stats_.pool_hits.fetch_add(1, std::memory_order_relaxed);
            // Reset object to default state if needed
            if constexpr (std::is_constructible_v<T, Args...>) {
                *obj = T(std::forward<Args>(args)...);
            }
        } else {
            stats_.pool_misses.fetch_add(1, std::memory_order_relaxed);
            obj = create_object(std::forward<Args>(args)...);
        }

        stats_.total_allocations.fetch_add(1, std::memory_order_relaxed);
        return pooled_object(obj, this);
    }

    /**
     * @brief Get pool statistics
     * @return Reference to pool statistics
     */
    const pool_stats& get_stats() const { return stats_; }

    /**
     * @brief Reset pool statistics
     */
    void reset_stats() { stats_.reset(); }

    /**
     * @brief Clear the pool
     */
    void clear() {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        while (!pool_.empty()) {
            delete pool_.top();
            pool_.pop();
        }
        stats_.current_pool_size = 0;
    }

    /**
     * @brief Get current pool size
     * @return Number of objects in pool
     */
    size_t size() const {
        return stats_.current_pool_size.load(std::memory_order_relaxed);
    }

    /**
     * @brief Check if pool is empty
     * @return true if pool has no objects
     */
    bool empty() const {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        return pool_.empty();
    }

private:
    /**
     * @brief Get an object from the pool
     * @return Pointer to object or nullptr if pool is empty
     */
    T* get_object() {
        // Try thread-local cache first if enabled
        if (config_.thread_local_cache) {
            if (auto* obj = get_from_thread_cache()) {
                return obj;
            }
        }

        // Fall back to global pool
        std::lock_guard<std::mutex> lock(pool_mutex_);
        if (pool_.empty()) {
            return nullptr;
        }

        T* obj = pool_.top();
        pool_.pop();
        stats_.current_pool_size.fetch_sub(1, std::memory_order_relaxed);
        return obj;
    }

    /**
     * @brief Return an object to the pool
     * @param obj Object to return
     */
    void return_object(T* obj) {
        if (!obj) return;

        stats_.total_deallocations.fetch_add(1, std::memory_order_relaxed);

        // Try thread-local cache first if enabled
        if (config_.thread_local_cache) {
            if (return_to_thread_cache(obj)) {
                return;
            }
        }

        // Return to global pool
        std::lock_guard<std::mutex> lock(pool_mutex_);
        if (pool_.size() < config_.max_size) {
            pool_.push(obj);
            const auto new_size = stats_.current_pool_size.fetch_add(1, std::memory_order_relaxed) + 1;

            // Update peak size
            auto current_peak = stats_.peak_pool_size.load(std::memory_order_relaxed);
            while (new_size > current_peak) {
                if (stats_.peak_pool_size.compare_exchange_weak(current_peak, new_size)) {
                    break;
                }
            }
        } else {
            // Pool is full, delete the object
            delete obj;
        }
    }

    /**
     * @brief Create a new object
     * @param args Constructor arguments
     * @return Pointer to new object
     */
    template<typename... Args>
    T* create_object(Args&&... args) {
        return new T(std::forward<Args>(args)...);
    }

    /**
     * @brief Get object from thread-local cache
     * @return Pointer to object or nullptr
     */
    T* get_from_thread_cache() {
        thread_local static std::stack<T*> cache;

        if (!cache.empty()) {
            T* obj = cache.top();
            cache.pop();
            return obj;
        }
        return nullptr;
    }

    /**
     * @brief Return object to thread-local cache
     * @param obj Object to cache
     * @return true if cached, false if cache is full
     */
    bool return_to_thread_cache(T* obj) {
        thread_local static std::stack<T*> cache;

        if (cache.size() < config_.thread_cache_size) {
            cache.push(obj);
            return true;
        }
        return false;
    }

    config config_;
    mutable std::mutex pool_mutex_;
    std::stack<T*> pool_;
    mutable pool_stats stats_;
};

/**
 * @brief Specialized object pool for log entries
 */
class log_entry_pool {
public:
    /**
     * @brief Log entry structure optimized for pooling
     */
    struct pooled_log_entry {
        std::string message;
        std::string file;
        std::string function;
        int level{0};
        int line{0};
        std::chrono::system_clock::time_point timestamp;

        // Reset to default state
        void reset() {
            message.clear();
            file.clear();
            function.clear();
            level = 0;
            line = 0;
            timestamp = {};
        }
    };

    using pool_type = object_pool<pooled_log_entry>;
    using pooled_entry = pool_type::pooled_object;

    /**
     * @brief Get global log entry pool instance
     * @return Reference to global pool
     */
    static pool_type& instance() {
        static pool_type pool{pool_type::config{
            .initial_size = 200,
            .max_size = 2000,
            .growth_factor = 2,
            .thread_local_cache = true,
            .thread_cache_size = 100
        }};
        return pool;
    }

    /**
     * @brief Acquire a pooled log entry
     * @return RAII wrapper for log entry
     */
    static pooled_entry acquire() {
        return instance().acquire();
    }
};

/**
 * @brief Auto-resizing object pool with adaptive sizing
 */
template<typename T>
class auto_resize_pool : public object_pool<T> {
public:
    using base_type = object_pool<T>;
    using config = typename base_type::config;

    /**
     * @brief Constructor with monitoring
     * @param cfg Pool configuration
     */
    explicit auto_resize_pool(const config& cfg = config{})
        : base_type(cfg)
        , monitor_thread_(&auto_resize_pool::monitor_loop, this) {
    }

    /**
     * @brief Destructor
     */
    ~auto_resize_pool() {
        stop_monitoring_ = true;
        if (monitor_thread_.joinable()) {
            monitor_thread_.join();
        }
    }

private:
    /**
     * @brief Monitor loop for adaptive resizing
     */
    void monitor_loop() {
        while (!stop_monitoring_) {
            std::this_thread::sleep_for(std::chrono::seconds(10));

            const auto& stats = this->get_stats();
            const double hit_ratio = stats.hit_ratio();

            // Adaptive resizing logic based on hit ratio
            if (hit_ratio < 0.8 && this->size() < this->config_.max_size) {
                // Low hit ratio, consider expanding
                expand_pool();
            } else if (hit_ratio > 0.95 && this->size() > this->config_.initial_size) {
                // Very high hit ratio but potential memory waste
                consider_shrinking();
            }
        }
    }

    void expand_pool() {
        // Implementation for pool expansion
        // Add more objects to improve hit ratio
    }

    void consider_shrinking() {
        // Implementation for pool shrinking
        // Remove excess objects if memory usage is high
    }

    std::atomic<bool> stop_monitoring_{false};
    std::thread monitor_thread_;
};

} // namespace logger_module::memory