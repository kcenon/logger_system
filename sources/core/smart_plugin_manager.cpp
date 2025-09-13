/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include "smart_plugin_manager.h"
#include <fstream>
#include <algorithm>
#include <chrono>

namespace logger_module {

smart_plugin_manager& smart_plugin_manager::instance() {
    static smart_plugin_manager instance;
    return instance;
}

smart_plugin_manager::~smart_plugin_manager() {
    shutdown();
}

bool smart_plugin_manager::initialize(const std::string& config_path) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    if (initialized_) {
        return true; // Already initialized
    }

    config_path_ = config_path;
    dependency_resolver_ = std::make_unique<plugin_dependency_resolver>();

    // Start health monitoring thread if enabled
    if (health_monitoring_enabled_) {
        health_monitor_thread_ = std::thread(&smart_plugin_manager::health_monitor_thread, this);
    }

    initialized_ = true;
    return true;
}

void smart_plugin_manager::shutdown() {
    if (!initialized_) {
        return;
    }

    // Stop health monitoring
    shutdown_requested_ = true;
    health_check_cv_.notify_all();

    if (health_monitor_thread_.joinable()) {
        health_monitor_thread_.join();
    }

    // Unload all plugins in reverse dependency order
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto resolution = dependency_resolver_->resolve_loading_order();
    if (resolution.success) {
        // Unload in reverse order
        for (auto it = resolution.loading_order.rbegin(); it != resolution.loading_order.rend(); ++it) {
            unload_plugin(*it, true);
        }
    }

    plugins_.clear();
    dependency_resolver_.reset();
    initialized_ = false;
}

bool smart_plugin_manager::register_plugin(const plugin_config& config) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    if (plugins_.find(config.name) != plugins_.end()) {
        return false; // Already registered
    }

    // Add to dependency resolver
    std::vector<std::string> dep_names;
    for (const auto& dep : config.dependencies) {
        dep_names.push_back(dep.name);
        dependency_resolver_->add_dependency(
            plugin_dependency_resolver::dependency_edge(
                config.name, dep.name, dep.version, dep.optional));
    }

    dependency_resolver_->add_plugin(config.name, dep_names);

    // Create managed plugin entry
    plugins_.emplace(config.name, managed_plugin(config));

    return true;
}

bool smart_plugin_manager::load_plugin(const std::string& plugin_name, bool force_load) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return false; // Plugin not registered
    }

    auto& plugin = it->second;

    // Check if already loaded
    if (plugin.state == plugin_state::loaded) {
        return true;
    }

    // Check if lazy loading is disabled or force load requested
    if (!plugin.config.lazy_load || force_load) {
        std::unordered_set<std::string> visited;
        return load_plugin_internal(plugin_name, visited);
    }

    // Mark for lazy loading
    plugin.state = plugin_state::unloaded;
    return true;
}

bool smart_plugin_manager::unload_plugin(const std::string& plugin_name, bool force_unload) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return false; // Plugin not found
    }

    auto& plugin = it->second;

    // Check for dependents
    if (!force_unload && !plugin.dependents.empty()) {
        return false; // Has active dependents
    }

    // Unload from base manager
    if (plugin.state == plugin_state::loaded) {
        base_manager_.unload_plugin(plugin_name);
        plugin.instance.reset();
    }

    plugin.state = plugin_state::unloaded;
    plugin.stats.unload_count++;

    return true;
}

smart_plugin_manager::plugin_state smart_plugin_manager::get_plugin_state(
    const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return plugin_state::unloaded;
    }

    return it->second.state;
}

smart_plugin_manager::plugin_stats smart_plugin_manager::get_plugin_stats(
    const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return plugin_stats{};
    }

    return it->second.stats;
}

bool smart_plugin_manager::are_dependencies_satisfied(const std::string& plugin_name) const {
    auto deps = dependency_resolver_->get_dependencies(plugin_name, false);

    for (const auto& dep : deps) {
        auto it = plugins_.find(dep);
        if (it == plugins_.end() || it->second.state != plugin_state::loaded) {
            // Check if dependency is optional
            auto& plugin = plugins_.at(plugin_name);
            auto dep_it = std::find_if(plugin.config.dependencies.begin(),
                                       plugin.config.dependencies.end(),
                                       [&dep](const auto& d) { return d.name == dep; });
            if (dep_it != plugin.config.dependencies.end() && dep_it->optional) {
                continue; // Optional dependency, skip
            }
            return false;
        }
    }

    return true;
}

std::vector<std::string> smart_plugin_manager::get_loading_order(
    const std::vector<std::string>& plugin_names) const {
    auto resolution = dependency_resolver_->resolve_loading_order(plugin_names);
    return resolution.loading_order;
}

bool smart_plugin_manager::restart_plugin(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return false;
    }

    auto& plugin = it->second;

    // Check restart attempts
    if (plugin.restart_attempts >= plugin.config.max_restart_attempts) {
        plugin.state = plugin_state::failed;
        return false;
    }

    plugin.state = plugin_state::restarting;
    plugin.stats.restart_count++;
    plugin.restart_attempts++;

    // Unload the plugin
    if (plugin.instance) {
        base_manager_.unload_plugin(plugin_name);
        plugin.instance.reset();
    }

    // Attempt to reload
    std::unordered_set<std::string> visited;
    if (load_plugin_internal(plugin_name, visited)) {
        plugin.restart_attempts = 0; // Reset on success
        plugin.state = plugin_state::loaded;
        schedule_health_check(plugin_name);
        return true;
    }

    plugin.state = plugin_state::failed;
    return false;
}

void smart_plugin_manager::set_health_monitoring(bool enabled) {
    health_monitoring_enabled_ = enabled;

    if (enabled && !health_monitor_thread_.joinable()) {
        health_monitor_thread_ = std::thread(&smart_plugin_manager::health_monitor_thread, this);
    }
}

std::vector<std::string> smart_plugin_manager::get_dependents(const std::string& plugin_name) const {
    return dependency_resolver_->get_dependents(plugin_name, true);
}

std::string smart_plugin_manager::export_dependency_graph() const {
    return dependency_resolver_->export_as_dot();
}

bool smart_plugin_manager::load_plugin_internal(const std::string& plugin_name,
                                               std::unordered_set<std::string>& visited) {
    // Check for circular dependencies
    if (visited.find(plugin_name) != visited.end()) {
        return false; // Circular dependency detected
    }

    visited.insert(plugin_name);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return false;
    }

    auto& plugin = it->second;

    // Check if already loaded
    if (plugin.state == plugin_state::loaded) {
        return true;
    }

    plugin.state = plugin_state::loading;

    // Load dependencies first
    if (!load_dependencies(plugin_name, visited)) {
        plugin.state = plugin_state::failed;
        plugin.stats.failure_count++;
        return false;
    }

    // Load the plugin using base manager
    auto start_time = std::chrono::steady_clock::now();

    if (!base_manager_.load_plugin(plugin.config.path)) {
        plugin.state = plugin_state::failed;
        plugin.stats.failure_count++;
        return false;
    }

    auto end_time = std::chrono::steady_clock::now();
    auto load_time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    // Get plugin instance from base manager
    plugin.instance = base_manager_.get_plugin<void>(plugin_name);

    if (!plugin.instance) {
        plugin.state = plugin_state::failed;
        plugin.stats.failure_count++;
        return false;
    }

    plugin.state = plugin_state::loaded;
    plugin.stats.load_count++;
    plugin.stats.last_load_time = std::chrono::steady_clock::now();
    update_stats(plugin_name, load_time);

    // Schedule health check
    schedule_health_check(plugin_name);

    return true;
}

bool smart_plugin_manager::load_dependencies(const std::string& plugin_name,
                                            std::unordered_set<std::string>& visited) {
    auto deps = dependency_resolver_->get_dependencies(plugin_name, false);

    for (const auto& dep : deps) {
        if (!load_plugin_internal(dep, visited)) {
            // Check if dependency is optional
            auto& plugin = plugins_.at(plugin_name);
            auto dep_it = std::find_if(plugin.config.dependencies.begin(),
                                       plugin.config.dependencies.end(),
                                       [&dep](const auto& d) { return d.name == dep; });
            if (dep_it != plugin.config.dependencies.end() && dep_it->optional) {
                continue; // Optional dependency, skip
            }
            return false;
        }

        // Add this plugin to dependent's list
        auto& dep_plugin = plugins_.at(dep);
        dep_plugin.dependents.push_back(plugin_name);
    }

    return true;
}

void smart_plugin_manager::health_monitor_thread() {
    while (!shutdown_requested_) {
        std::unique_lock<std::mutex> lock(health_queue_mutex_);

        // Wait for next health check or shutdown
        if (health_check_queue_.empty()) {
            health_check_cv_.wait_for(lock, std::chrono::seconds(1));
            continue;
        }

        auto now = std::chrono::steady_clock::now();
        auto next_check = health_check_queue_.top();

        if (next_check.next_check > now) {
            health_check_cv_.wait_until(lock, next_check.next_check);
            continue;
        }

        health_check_queue_.pop();
        lock.unlock();

        // Perform health check
        if (perform_health_check(next_check.plugin_name)) {
            // Reschedule next check
            schedule_health_check(next_check.plugin_name);
        } else {
            // Handle failure
            handle_plugin_failure(next_check.plugin_name);
        }
    }
}

bool smart_plugin_manager::perform_health_check(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end() || it->second.state != plugin_state::loaded) {
        return true; // Skip check if not loaded
    }

    auto& plugin = it->second;
    plugin.stats.health_check_count++;
    plugin.stats.last_health_check = std::chrono::steady_clock::now();

    // Use base manager's health check
    return base_manager_.validate_plugin(plugin.config.path);
}

void smart_plugin_manager::schedule_health_check(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(health_queue_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return;
    }

    health_check_task task;
    task.plugin_name = plugin_name;
    task.next_check = std::chrono::steady_clock::now() + it->second.config.health_check_interval;

    health_check_queue_.push(task);
    health_check_cv_.notify_one();
}

void smart_plugin_manager::handle_plugin_failure(const std::string& plugin_name) {
    auto& plugin = plugins_.at(plugin_name);
    plugin.state = plugin_state::suspended;
    plugin.last_failure_time = std::chrono::steady_clock::now();

    // Attempt automatic restart if enabled
    if (plugin.config.auto_restart) {
        restart_plugin(plugin_name);
    }
}

void smart_plugin_manager::update_stats(const std::string& plugin_name,
                                        std::chrono::milliseconds load_time) {
    auto& stats = plugins_.at(plugin_name).stats;

    stats.total_load_time += load_time;
    if (stats.load_count > 0) {
        stats.average_load_time = stats.total_load_time / stats.load_count;
    }
}

} // namespace logger_module