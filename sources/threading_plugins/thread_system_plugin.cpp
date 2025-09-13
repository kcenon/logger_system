#include "thread_system_plugin.h"
#include <algorithm>
#include <stdexcept>

#ifdef USE_THREAD_SYSTEM
    // Include thread_system headers if available
    // This would be included when building with thread_system support
    // #include <thread_system/thread_pool.h>
#endif

namespace logger_system {
namespace threading_plugins {

// thread_pool_impl implementation
thread_pool_impl::thread_pool_impl(size_t thread_count, const std::string& pool_name)
    : pool_name_(pool_name.empty() ? "logger_pool" : pool_name),
      target_thread_count_(thread_count == 0 ? std::thread::hardware_concurrency() : thread_count) {

    if (target_thread_count_ == 0) {
        target_thread_count_ = 1; // Fallback to at least 1 thread
    }
}

thread_pool_impl::~thread_pool_impl() {
    stop();
}

void thread_pool_impl::start() {
    if (running_.load()) {
        return;
    }

    stop_requested_ = false;
    running_ = true;

    threads_.reserve(target_thread_count_);
    for (size_t i = 0; i < target_thread_count_; ++i) {
        threads_.emplace_back(&thread_pool_impl::worker_thread, this);
    }
}

void thread_pool_impl::stop() {
    if (!running_.load()) {
        return;
    }

    stop_requested_ = true;
    condition_.notify_all();

    for (auto& thread : threads_) {
        if (thread.joinable()) {
            thread.join();
        }
    }

    threads_.clear();
    running_ = false;

    // Clear remaining tasks
    std::lock_guard<std::mutex> lock(queue_mutex_);
    while (!tasks_.empty()) {
        tasks_.pop();
    }
}

bool thread_pool_impl::is_running() const {
    return running_.load();
}

void thread_pool_impl::resize(size_t thread_count) {
    if (thread_count == 0) {
        thread_count = std::thread::hardware_concurrency();
        if (thread_count == 0) thread_count = 1;
    }

    if (thread_count == threads_.size()) {
        return;
    }

    bool was_running = is_running();
    stop();

    target_thread_count_ = thread_count;

    if (was_running) {
        start();
    }
}

size_t thread_pool_impl::size() const {
    return threads_.size();
}

size_t thread_pool_impl::queue_size() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return tasks_.size();
}

void thread_pool_impl::submit_task(interfaces::task_type task) {
    if (!task) {
        return;
    }

    {
        std::lock_guard<std::mutex> lock(queue_mutex_);
        if (stop_requested_.load()) {
            return;
        }
        tasks_.emplace(std::move(task));
    }

    condition_.notify_one();
}

std::future<void> thread_pool_impl::submit_async_task(interfaces::task_type task) {
    if (!task) {
        std::promise<void> promise;
        promise.set_value();
        return promise.get_future();
    }

    auto promise = std::make_shared<std::promise<void>>();
    auto future = promise->get_future();

    auto wrapped_task = [task = std::move(task), promise]() mutable {
        try {
            task();
            promise->set_value();
        } catch (...) {
            promise->set_exception(std::current_exception());
        }
    };

    submit_task(std::move(wrapped_task));
    return future;
}

void thread_pool_impl::worker_thread() {
    while (!stop_requested_.load()) {
        std::function<void()> task;

        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            condition_.wait(lock, [this] {
                return stop_requested_.load() || !tasks_.empty();
            });

            if (stop_requested_.load() && tasks_.empty()) {
                break;
            }

            if (!tasks_.empty()) {
                task = std::move(tasks_.front());
                tasks_.pop();
            }
        }

        if (task) {
            try {
                task();
            } catch (...) {
                // Log error in a real implementation
            }
        }
    }
}

// thread_system_plugin implementation
thread_system_plugin::thread_system_plugin() {
#ifdef USE_THREAD_SYSTEM
    // Initialize with external thread_system if available
    try {
        // external_pool_ = std::make_unique<thread_system::thread_pool>();
        use_external_pool_ = true;
    } catch (...) {
        use_external_pool_ = false;
    }
#endif
}

thread_system_plugin::~thread_system_plugin() {
    shutdown_threading();
}

std::shared_ptr<interfaces::interface_thread_pool> thread_system_plugin::create_thread_pool(
    size_t thread_count, const std::string& pool_name) {

    auto pool = std::make_shared<thread_pool_impl>(thread_count, pool_name);
    pool->start();
    return pool;
}

void thread_system_plugin::submit_task(interfaces::task_type task) {
    ensure_default_pool();

    if (default_pool_) {
        default_pool_->submit_task(std::move(task));
    }
}

std::future<void> thread_system_plugin::submit_async_task(interfaces::task_type task) {
    ensure_default_pool();

    if (default_pool_) {
        return default_pool_->submit_async_task(std::move(task));
    }

    std::promise<void> promise;
    promise.set_value();
    return promise.get_future();
}

void thread_system_plugin::shutdown_threading() {
    std::lock_guard<std::mutex> lock(pool_mutex_);

    if (default_pool_) {
        default_pool_->stop();
        default_pool_.reset();
    }

#ifdef USE_THREAD_SYSTEM
    external_pool_.reset();
#endif

    initialized_ = false;
}

bool thread_system_plugin::is_available() const {
#ifdef USE_THREAD_SYSTEM
    return true;
#else
    return true; // Always available with built-in implementation
#endif
}

std::string thread_system_plugin::get_plugin_name() const {
    return "thread_system_plugin";
}

std::string thread_system_plugin::get_plugin_version() const {
    return "1.0.0";
}

void thread_system_plugin::ensure_default_pool() {
    if (initialized_.load()) {
        return;
    }

    std::lock_guard<std::mutex> lock(pool_mutex_);
    if (!initialized_.load()) {
        if (!default_pool_) {
            default_pool_ = std::make_shared<thread_pool_impl>(
                std::thread::hardware_concurrency(), "default_logger_pool");
            default_pool_->start();
        }
        initialized_ = true;
    }
}

// Plugin factory functions
extern "C" {

std::shared_ptr<void> create_plugin() {
    return std::static_pointer_cast<void>(std::make_shared<thread_system_plugin>());
}

void destroy_plugin(void* plugin) {
    if (plugin) {
        delete static_cast<thread_system_plugin*>(plugin);
    }
}

interfaces::plugin_info get_plugin_info() {
    return interfaces::plugin_info(
        "thread_system_plugin",
        "1.0.0",
        "Threading plugin with thread_system integration support",
        interfaces::plugin_type::threading,
        "",  // Path will be set by plugin manager
        true
    );
}

} // extern "C"

} // namespace threading_plugins
} // namespace logger_system