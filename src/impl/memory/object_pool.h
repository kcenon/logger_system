/**
 * @file object_pool.h
 * @brief Object pool implementation for high-performance memory management
 *
 * This file provides a thread-safe object pool implementation for reducing
 * memory allocation overhead in high-frequency logging scenarios.
 */

#pragma once

#include <kcenon/logger/core/error_codes.h>
#include <memory>
#include <vector>
#include <mutex>
#include <atomic>
#include <queue>

namespace kcenon::logger::memory {

/**
 * @brief Thread-safe object pool for high-performance memory management
 * @tparam T The type of objects to pool
 */
template<typename T>
class object_pool {
public:
    /**
     * @brief Configuration for object pool
     */
    struct config {
        size_t initial_size{100};        ///< Initial pool size
        size_t max_size{10000};          ///< Maximum pool size
        bool allow_growth{true};         ///< Allow pool to grow beyond initial size

        config() = default;
    };

    /**
     * @brief Construct object pool with configuration
     * @param cfg Pool configuration
     */
    explicit object_pool(const config& cfg = config{})
        : config_(cfg), pool_size_(0) {
        initialize_pool();
    }

    /**
     * @brief Destructor
     */
    ~object_pool() = default;

    /**
     * @brief Get an object from the pool
     * @return Unique pointer to object, or nullptr if pool is empty and growth is disabled
     */
    std::unique_ptr<T> acquire() {
        std::lock_guard<std::mutex> lock(mutex_);

        if (!available_objects_.empty()) {
            auto obj = std::move(available_objects_.front());
            available_objects_.pop();
            return obj;
        }

        // If pool is empty and growth is allowed, create new object
        if (config_.allow_growth && pool_size_.load() < config_.max_size) {
            pool_size_.fetch_add(1);
            return std::make_unique<T>();
        }

        // Create temporary object if pool limits reached
        return std::make_unique<T>();
    }

    /**
     * @brief Return an object to the pool
     * @param obj Object to return
     */
    void release(std::unique_ptr<T> obj) {
        if (!obj) {
            return;
        }

        std::lock_guard<std::mutex> lock(mutex_);

        // Only return to pool if not exceeding max size
        if (available_objects_.size() < config_.max_size) {
            available_objects_.push(std::move(obj));
        }
        // If exceeding max size, object will be destroyed automatically
    }

    /**
     * @brief Get current pool statistics
     */
    struct statistics {
        size_t total_size;
        size_t available_count;
        size_t in_use_count;
    };

    /**
     * @brief Get pool statistics
     * @return Current pool statistics
     */
    statistics get_statistics() const {
        std::lock_guard<std::mutex> lock(mutex_);
        return {
            .total_size = pool_size_.load(),
            .available_count = available_objects_.size(),
            .in_use_count = pool_size_.load() - available_objects_.size()
        };
    }

    /**
     * @brief Clear all objects from pool
     */
    void clear() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!available_objects_.empty()) {
            available_objects_.pop();
        }
        pool_size_.store(0);
    }

private:
    /**
     * @brief Initialize pool with initial objects
     */
    void initialize_pool() {
        std::lock_guard<std::mutex> lock(mutex_);

        for (size_t i = 0; i < config_.initial_size; ++i) {
            available_objects_.push(std::make_unique<T>());
        }

        pool_size_.store(config_.initial_size);
    }

    config config_;                                    ///< Pool configuration
    mutable std::mutex mutex_;                         ///< Thread safety mutex
    std::queue<std::unique_ptr<T>> available_objects_; ///< Available objects
    std::atomic<size_t> pool_size_;                    ///< Current pool size
};

/**
 * @brief Thread-local cached object pool for improved multi-threaded performance
 * @tparam T The type of objects to pool
 *
 * @details This implementation uses thread-local caching to minimize lock contention
 * in multi-threaded environments. Each thread maintains a small local cache of objects,
 * only accessing the global pool when the local cache is empty or full.
 *
 * Performance characteristics:
 * - Single thread: Similar to object_pool
 * - Multi-threaded: 2-5x better than object_pool due to reduced lock contention
 *
 * Memory overhead:
 * - Each thread uses up to local_cache_size * sizeof(T*) additional memory
 * - Recommended local_cache_size: 16 (empirically optimal)
 *
 * @since 1.1.0
 */
template<typename T>
class thread_local_object_pool {
public:
    /**
     * @brief Configuration for thread-local object pool
     */
    struct config {
        size_t global_max{10000};        ///< Maximum total pool size
        size_t local_cache_size{16};     ///< Per-thread cache size
        size_t initial_size{100};        ///< Initial global pool size
        bool allow_growth{true};         ///< Allow pool to grow beyond initial size

        config() = default;
    };

    /**
     * @brief Construct thread-local object pool with configuration
     * @param cfg Pool configuration
     */
    explicit thread_local_object_pool(const config& cfg = config{})
        : config_(cfg)
        , global_size_(0) {
        initialize_pool();
    }

    /**
     * @brief Destructor
     */
    ~thread_local_object_pool() = default;

    /**
     * @brief Get an object from the pool
     * @return Unique pointer to object
     *
     * @details Attempts to acquire from thread-local cache first (no lock),
     * then from global pool in batches (single lock), and creates new if needed.
     */
    std::unique_ptr<T> acquire() {
        // Step 1: Check thread-local cache first (no lock!)
        auto& local_cache = get_local_cache();

        if (!local_cache.empty()) {
            local_cache_hits_.fetch_add(1, std::memory_order_relaxed);
            auto obj = std::move(local_cache.back());
            local_cache.pop_back();
            return obj;
        }

        // Step 2: Fetch batch from global pool (lock once)
        {
            std::lock_guard<std::mutex> lock(global_mutex_);

            // Batch size: half of local cache size
            size_t batch_size = std::min(
                config_.local_cache_size / 2,
                global_pool_.size()
            );

            for (size_t i = 0; i < batch_size; ++i) {
                if (!global_pool_.empty()) {
                    local_cache.push_back(std::move(global_pool_.back()));
                    global_pool_.pop_back();
                }
            }
        }

        // Step 3: Try local cache again after batch fetch
        if (!local_cache.empty()) {
            global_pool_hits_.fetch_add(1, std::memory_order_relaxed);
            auto obj = std::move(local_cache.back());
            local_cache.pop_back();
            return obj;
        }

        // Step 4: Create new object if pool is empty
        if (config_.allow_growth && global_size_.load(std::memory_order_relaxed) < config_.global_max) {
            global_size_.fetch_add(1, std::memory_order_relaxed);
            new_allocations_.fetch_add(1, std::memory_order_relaxed);
            return std::make_unique<T>();
        }

        // Fallback: create temporary object even if limit reached
        // (same behavior as original object_pool)
        return std::make_unique<T>();
    }

    /**
     * @brief Return an object to the pool
     * @param obj Object to return
     *
     * @details Adds to thread-local cache if space available (no lock),
     * otherwise transfers batch to global pool (single lock).
     */
    void release(std::unique_ptr<T> obj) {
        if (!obj) {
            return;
        }

        // Step 1: Add to thread-local cache (no lock!)
        auto& local_cache = get_local_cache();

        if (local_cache.size() < config_.local_cache_size) {
            local_cache.push_back(std::move(obj));
            return;
        }

        // Step 2: Transfer batch to global pool when local cache is full
        {
            std::lock_guard<std::mutex> lock(global_mutex_);

            // Move half of local cache to global
            size_t transfer_count = config_.local_cache_size / 2;

            for (size_t i = 0; i < transfer_count && !local_cache.empty(); ++i) {
                if (global_pool_.size() < config_.global_max) {
                    global_pool_.push_back(std::move(local_cache.back()));
                    local_cache.pop_back();
                }
            }

            // Add current object to global pool
            if (global_pool_.size() < config_.global_max) {
                global_pool_.push_back(std::move(obj));
            }
            // If exceeding max, object will be destroyed automatically
        }
    }

    /**
     * @brief Statistics for monitoring pool performance
     */
    struct statistics {
        size_t global_size;         ///< Total objects managed by pool
        size_t global_pool_size;    ///< Objects currently in global pool
        size_t local_cache_hits;    ///< Acquire from thread-local cache (fast path)
        size_t global_pool_hits;    ///< Acquire from global pool (batch fetch)
        size_t new_allocations;     ///< New objects created
    };

    /**
     * @brief Get pool statistics
     * @return Current pool statistics
     */
    statistics get_statistics() const {
        std::lock_guard<std::mutex> lock(global_mutex_);
        return statistics{
            .global_size = global_size_.load(std::memory_order_relaxed),
            .global_pool_size = global_pool_.size(),
            .local_cache_hits = local_cache_hits_.load(std::memory_order_relaxed),
            .global_pool_hits = global_pool_hits_.load(std::memory_order_relaxed),
            .new_allocations = new_allocations_.load(std::memory_order_relaxed)
        };
    }

    /**
     * @brief Clear all objects from pool
     * @warning This does not clear thread-local caches
     */
    void clear() {
        std::lock_guard<std::mutex> lock(global_mutex_);
        global_pool_.clear();
        global_size_.store(0, std::memory_order_relaxed);
        local_cache_hits_.store(0, std::memory_order_relaxed);
        global_pool_hits_.store(0, std::memory_order_relaxed);
        new_allocations_.store(0, std::memory_order_relaxed);
    }

private:
    /**
     * @brief Get thread-local cache
     * @return Reference to thread-local cache vector
     */
    static std::vector<std::unique_ptr<T>>& get_local_cache() {
        thread_local std::vector<std::unique_ptr<T>> cache;
        return cache;
    }

    /**
     * @brief Initialize pool with initial objects
     */
    void initialize_pool() {
        std::lock_guard<std::mutex> lock(global_mutex_);

        for (size_t i = 0; i < config_.initial_size; ++i) {
            global_pool_.push_back(std::make_unique<T>());
        }

        global_size_.store(config_.initial_size, std::memory_order_relaxed);
    }

    // Configuration
    config config_;

    // Global pool
    std::vector<std::unique_ptr<T>> global_pool_;
    mutable std::mutex global_mutex_;

    // Statistics (atomic for lock-free reads)
    std::atomic<size_t> global_size_{0};
    std::atomic<size_t> local_cache_hits_{0};
    std::atomic<size_t> global_pool_hits_{0};
    std::atomic<size_t> new_allocations_{0};
};

} // namespace kcenon::logger::memory