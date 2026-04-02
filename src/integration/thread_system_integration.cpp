// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file thread_system_integration.cpp
 * @brief Implementation of optional thread_system integration
 * @author kcenon
 * @since 1.4.0
 */

#include <kcenon/logger/integration/thread_system_integration.h>

#include <thread>

#ifdef LOGGER_HAS_THREAD_SYSTEM

#include <kcenon/thread/core/callback_job.h>

namespace kcenon::logger::integration {

// Static member initialization
std::atomic<async_backend_type> thread_system_integration::current_backend_{
    async_backend_type::standalone
};
std::shared_ptr<kcenon::thread::thread_pool> thread_system_integration::thread_pool_{nullptr};
std::mutex thread_system_integration::pool_mutex_{};

void thread_system_integration::enable(std::shared_ptr<kcenon::thread::thread_pool> pool) {
    std::lock_guard<std::mutex> lock(pool_mutex_);

    if (pool) {
        thread_pool_ = std::move(pool);
    } else if (!thread_pool_) {
        // Create default pool if none provided and none exists
        thread_pool_ = create_default_pool();
    }

    current_backend_.store(async_backend_type::thread_pool, std::memory_order_release);
}

void thread_system_integration::disable() {
    std::lock_guard<std::mutex> lock(pool_mutex_);

    current_backend_.store(async_backend_type::standalone, std::memory_order_release);
    // Note: We don't stop the pool here - let it be managed by its owner
    // Just release our reference
    thread_pool_.reset();
}

bool thread_system_integration::is_enabled() noexcept {
    return current_backend_.load(std::memory_order_acquire) == async_backend_type::thread_pool;
}

async_backend_type thread_system_integration::get_backend() noexcept {
    return current_backend_.load(std::memory_order_acquire);
}

void thread_system_integration::set_thread_pool(
    std::shared_ptr<kcenon::thread::thread_pool> pool) {
    std::lock_guard<std::mutex> lock(pool_mutex_);

    if (pool) {
        thread_pool_ = std::move(pool);
        current_backend_.store(async_backend_type::thread_pool, std::memory_order_release);
    } else {
        thread_pool_.reset();
        current_backend_.store(async_backend_type::standalone, std::memory_order_release);
    }
}

std::shared_ptr<kcenon::thread::thread_pool> thread_system_integration::get_thread_pool() noexcept {
    std::lock_guard<std::mutex> lock(pool_mutex_);
    return thread_pool_;
}

bool thread_system_integration::submit_task(std::function<void()> task) {
    if (!is_enabled()) {
        return false;
    }

    std::shared_ptr<kcenon::thread::thread_pool> pool;
    {
        std::lock_guard<std::mutex> lock(pool_mutex_);
        pool = thread_pool_;
    }

    if (!pool || !pool->is_running()) {
        return false;
    }

    // Create a callback_job that wraps the task
    auto job = std::make_unique<kcenon::thread::callback_job>(
        [task = std::move(task)]() -> kcenon::common::VoidResult {
            task();
            return kcenon::common::ok();
        },
        "logger_async_task"
    );

    auto result = pool->enqueue(std::move(job));
    return result.is_ok();
}

std::string thread_system_integration::get_backend_name() noexcept {
    return is_enabled() ? "thread_pool" : "standalone";
}

std::shared_ptr<kcenon::thread::thread_pool> thread_system_integration::create_default_pool() {
    auto pool = std::make_shared<kcenon::thread::thread_pool>(
        "logger_async_pool"
    );

    // Start the pool
    auto result = pool->start();
    if (!result.is_ok()) {
        // Failed to start pool - return nullptr to signal failure
        return nullptr;
    }

    return pool;
}

} // namespace kcenon::logger::integration

#endif // LOGGER_HAS_THREAD_SYSTEM
