#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file smart_plugin_manager.h
 * @brief Smart plugin management system with dependency resolution and lazy loading
 *
 * This file provides an enhanced plugin management system that supports:
 * - Dependency graph management
 * - Lazy loading mechanism
 * - Plugin health monitoring
 * - Automatic failure recovery
 * - Plugin state caching
 */

#include "plugin_manager.h"
#include "plugin_dependency_resolver.h"
#include "../logging_interfaces/plugin_loader_interface.h"
#include <unordered_set>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>

namespace logger_module {

/**
 * @brief Smart plugin manager with advanced features
 */
class smart_plugin_manager {
public:
    /**
     * @brief Plugin state enumeration
     */
    enum class plugin_state {
        unloaded,      ///< Plugin not loaded
        loading,       ///< Plugin currently loading
        loaded,        ///< Plugin loaded successfully
        failed,        ///< Plugin failed to load
        suspended,     ///< Plugin suspended due to health issues
        restarting     ///< Plugin being restarted
    };

    /**
     * @brief Plugin dependency information
     */
    struct plugin_dependency {
        std::string name;           ///< Plugin name
        std::string version;        ///< Required version
        bool optional;              ///< Whether dependency is optional
        std::chrono::milliseconds timeout{5000}; ///< Loading timeout

        plugin_dependency(const std::string& n, const std::string& v = "", bool opt = false)
            : name(n), version(v), optional(opt) {}
    };

    /**
     * @brief Plugin configuration
     */
    struct plugin_config {
        std::string name;
        std::string path;
        std::vector<plugin_dependency> dependencies;
        bool lazy_load{true};       ///< Enable lazy loading
        bool auto_restart{true};    ///< Enable automatic restart on failure
        std::chrono::seconds health_check_interval{30}; ///< Health check interval
        int max_restart_attempts{3}; ///< Maximum restart attempts
        std::chrono::milliseconds startup_timeout{10000}; ///< Startup timeout

        plugin_config(const std::string& n, const std::string& p)
            : name(n), path(p) {}
    };

    /**
     * @brief Plugin statistics
     */
    struct plugin_stats {
        std::atomic<uint64_t> load_count{0};
        std::atomic<uint64_t> unload_count{0};
        std::atomic<uint64_t> health_check_count{0};
        std::atomic<uint64_t> restart_count{0};
        std::atomic<uint64_t> failure_count{0};

        std::chrono::steady_clock::time_point last_load_time;
        std::chrono::steady_clock::time_point last_health_check;
        std::chrono::milliseconds total_load_time{0};
        std::chrono::milliseconds average_load_time{0};

        void reset() {
            load_count = 0;
            unload_count = 0;
            health_check_count = 0;
            restart_count = 0;
            failure_count = 0;
            total_load_time = std::chrono::milliseconds{0};
            average_load_time = std::chrono::milliseconds{0};
        }
    };

    /**
     * @brief Get singleton instance
     * @return Reference to singleton instance
     */
    static smart_plugin_manager& instance();

    /**
     * @brief Initialize the smart plugin manager
     * @param config_path Path to plugin configuration file
     * @return true if initialization successful
     */
    bool initialize(const std::string& config_path = "");

    /**
     * @brief Shutdown the smart plugin manager
     */
    void shutdown();

    /**
     * @brief Register a plugin configuration
     * @param config Plugin configuration
     * @return true if registration successful
     */
    bool register_plugin(const plugin_config& config);

    /**
     * @brief Load a plugin with dependency resolution
     * @param plugin_name Name of plugin to load
     * @param force_load Force immediate loading (disable lazy loading)
     * @return true if loading successful or scheduled
     */
    bool load_plugin(const std::string& plugin_name, bool force_load = false);

    /**
     * @brief Unload a plugin and its dependents
     * @param plugin_name Name of plugin to unload
     * @param force_unload Force unload even with active dependencies
     * @return true if unloading successful
     */
    bool unload_plugin(const std::string& plugin_name, bool force_unload = false);

    /**
     * @brief Get plugin instance with lazy loading
     * @tparam T Plugin interface type
     * @param plugin_name Name of plugin
     * @return Shared pointer to plugin instance
     */
    template<typename T>
    std::shared_ptr<T> get_plugin(const std::string& plugin_name);

    /**
     * @brief Get plugin state
     * @param plugin_name Name of plugin
     * @return Current plugin state
     */
    plugin_state get_plugin_state(const std::string& plugin_name) const;

    /**
     * @brief Get plugin statistics
     * @param plugin_name Name of plugin
     * @return Plugin statistics
     */
    plugin_stats get_plugin_stats(const std::string& plugin_name) const;

    /**
     * @brief Check if all dependencies are satisfied
     * @param plugin_name Name of plugin
     * @return true if all dependencies are satisfied
     */
    bool are_dependencies_satisfied(const std::string& plugin_name) const;

    /**
     * @brief Get loading order for plugins
     * @param plugin_names List of plugins to order
     * @return Ordered list of plugin names
     */
    std::vector<std::string> get_loading_order(const std::vector<std::string>& plugin_names) const;

    /**
     * @brief Restart a failed plugin
     * @param plugin_name Name of plugin to restart
     * @return true if restart initiated
     */
    bool restart_plugin(const std::string& plugin_name);

    /**
     * @brief Enable/disable automatic health monitoring
     * @param enabled Whether to enable monitoring
     */
    void set_health_monitoring(bool enabled);

    /**
     * @brief Get list of plugins that depend on the given plugin
     * @param plugin_name Name of plugin
     * @return List of dependent plugin names
     */
    std::vector<std::string> get_dependents(const std::string& plugin_name) const;

    /**
     * @brief Get dependency graph as dot format for visualization
     * @return DOT graph representation
     */
    std::string export_dependency_graph() const;

private:
    /**
     * @brief Internal plugin state tracking
     */
    struct managed_plugin {
        plugin_config config;
        plugin_state state;
        plugin_stats stats;
        std::shared_ptr<void> instance;
        std::vector<std::string> dependents; ///< Plugins that depend on this one
        int restart_attempts{0};
        std::chrono::steady_clock::time_point last_failure_time;

        managed_plugin(const plugin_config& cfg)
            : config(cfg), state(plugin_state::unloaded) {}
    };

    /**
     * @brief Health check task information
     */
    struct health_check_task {
        std::string plugin_name;
        std::chrono::steady_clock::time_point next_check;

        bool operator<(const health_check_task& other) const {
            return next_check > other.next_check; // For min-heap
        }
    };

    smart_plugin_manager() = default;
    ~smart_plugin_manager();

    // Non-copyable
    smart_plugin_manager(const smart_plugin_manager&) = delete;
    smart_plugin_manager& operator=(const smart_plugin_manager&) = delete;

    /**
     * @brief Internal load plugin implementation
     * @param plugin_name Name of plugin
     * @param visited Set of visited plugins (for cycle detection)
     * @return true if loading successful
     */
    bool load_plugin_internal(const std::string& plugin_name, std::unordered_set<std::string>& visited);

    /**
     * @brief Load plugin dependencies
     * @param plugin_name Name of plugin
     * @param visited Set of visited plugins
     * @return true if all dependencies loaded
     */
    bool load_dependencies(const std::string& plugin_name, std::unordered_set<std::string>& visited);

    /**
     * @brief Health monitoring thread function
     */
    void health_monitor_thread();

    /**
     * @brief Perform health check on a plugin
     * @param plugin_name Name of plugin
     * @return true if plugin is healthy
     */
    bool perform_health_check(const std::string& plugin_name);

    /**
     * @brief Schedule health check for a plugin
     * @param plugin_name Name of plugin
     */
    void schedule_health_check(const std::string& plugin_name);

    /**
     * @brief Handle plugin failure
     * @param plugin_name Name of failed plugin
     */
    void handle_plugin_failure(const std::string& plugin_name);

    /**
     * @brief Update plugin statistics
     * @param plugin_name Name of plugin
     * @param load_time Time taken to load
     */
    void update_stats(const std::string& plugin_name, std::chrono::milliseconds load_time);

    // Member variables
    std::unordered_map<std::string, managed_plugin> plugins_;
    std::unique_ptr<plugin_dependency_resolver> dependency_resolver_;
    logger_system::core::plugin_manager& base_manager_{logger_system::core::plugin_manager::instance()};

    // Threading and monitoring
    std::thread health_monitor_thread_;
    std::atomic<bool> health_monitoring_enabled_{true};
    std::atomic<bool> shutdown_requested_{false};

    std::priority_queue<health_check_task> health_check_queue_;
    mutable std::mutex plugins_mutex_;
    mutable std::mutex health_queue_mutex_;
    std::condition_variable health_check_cv_;

    // Configuration
    std::string config_path_;
    bool initialized_{false};
};

/**
 * @brief Template implementation for get_plugin
 */
template<typename T>
std::shared_ptr<T> smart_plugin_manager::get_plugin(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        return nullptr;
    }

    auto& plugin = it->second;

    // Lazy loading if not already loaded
    if (plugin.state == plugin_state::unloaded && plugin.config.lazy_load) {
        std::unordered_set<std::string> visited;
        if (!load_plugin_internal(plugin_name, visited)) {
            return nullptr;
        }
    }

    if (plugin.state != plugin_state::loaded || !plugin.instance) {
        return nullptr;
    }

    return std::static_pointer_cast<T>(plugin.instance);
}

} // namespace logger_module