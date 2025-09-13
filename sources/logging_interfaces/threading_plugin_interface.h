#ifndef THREADING_PLUGIN_INTERFACE_H
#define THREADING_PLUGIN_INTERFACE_H

#include <functional>
#include <future>
#include <memory>
#include <string>

namespace logger_system {
namespace interfaces {

using task_type = std::function<void()>;

class interface_thread_pool {
public:
    virtual ~interface_thread_pool() = default;

    virtual void start() = 0;

    virtual void stop() = 0;

    virtual bool is_running() const = 0;

    virtual void resize(size_t thread_count) = 0;

    virtual size_t size() const = 0;

    virtual size_t queue_size() const = 0;
};

class interface_threading_plugin {
public:
    virtual ~interface_threading_plugin() = default;

    virtual std::shared_ptr<interface_thread_pool> create_thread_pool(
        size_t thread_count = 0,
        const std::string& pool_name = "") = 0;

    virtual void submit_task(task_type task) = 0;

    virtual std::future<void> submit_async_task(task_type task) = 0;

    virtual void shutdown_threading() = 0;

    virtual bool is_available() const = 0;

    virtual std::string get_plugin_name() const = 0;

    virtual std::string get_plugin_version() const = 0;
};

} // namespace interfaces
} // namespace logger_system

#endif // THREADING_PLUGIN_INTERFACE_H