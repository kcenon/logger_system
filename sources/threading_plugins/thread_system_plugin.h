#ifndef THREAD_SYSTEM_PLUGIN_H
#define THREAD_SYSTEM_PLUGIN_H

#include "../logging_interfaces/threading_plugin_interface.h"
#include "../logging_interfaces/plugin_loader_interface.h"
#include <memory>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <future>

// Conditional compilation based on thread_system availability
#ifdef USE_THREAD_SYSTEM
    // Forward declarations for thread_system components
    namespace thread_system {
        class thread_pool;
    }
#endif

namespace logger_system {
namespace threading_plugins {

class thread_pool_impl : public interfaces::interface_thread_pool {
public:
    explicit thread_pool_impl(size_t thread_count = std::thread::hardware_concurrency(),
                             const std::string& pool_name = "logger_pool");
    ~thread_pool_impl();

    void start() override;
    void stop() override;
    bool is_running() const override;
    void resize(size_t thread_count) override;
    size_t size() const override;
    size_t queue_size() const override;

    void submit_task(interfaces::task_type task);
    std::future<void> submit_async_task(interfaces::task_type task);

private:
    void worker_thread();

    std::vector<std::thread> threads_;
    std::queue<std::function<void()>> tasks_;
    mutable std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic<bool> running_{false};
    std::atomic<bool> stop_requested_{false};
    std::string pool_name_;
    size_t target_thread_count_;
};

class thread_system_plugin : public interfaces::interface_threading_plugin {
public:
    thread_system_plugin();
    ~thread_system_plugin();

    std::shared_ptr<interfaces::interface_thread_pool> create_thread_pool(
        size_t thread_count = 0,
        const std::string& pool_name = "") override;

    void submit_task(interfaces::task_type task) override;

    std::future<void> submit_async_task(interfaces::task_type task) override;

    void shutdown_threading() override;

    bool is_available() const override;

    std::string get_plugin_name() const override;

    std::string get_plugin_version() const override;

private:
    void ensure_default_pool();

    std::shared_ptr<thread_pool_impl> default_pool_;
    std::mutex pool_mutex_;
    std::atomic<bool> initialized_{false};

#ifdef USE_THREAD_SYSTEM
    std::unique_ptr<thread_system::thread_pool> external_pool_;
    bool use_external_pool_{true};
#else
    static constexpr bool use_external_pool_{false};
#endif
};

// Plugin factory functions for dynamic loading
extern "C" {
    std::shared_ptr<void> create_plugin();
    void destroy_plugin(void* plugin);
    interfaces::plugin_info get_plugin_info();
}

} // namespace threading_plugins
} // namespace logger_system

#endif // THREAD_SYSTEM_PLUGIN_H