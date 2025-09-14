#ifndef PLUGIN_MANAGER_H
#define PLUGIN_MANAGER_H

#include <string>
#include <memory>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <chrono>
#include <functional>
#include <stdexcept>  // Added for exception handling
#include "../logging_interfaces/plugin_loader_interface.h"

#ifdef _WIN32
    #include <windows.h>
    using library_handle = HMODULE;
#else
    #include <dlfcn.h>
    using library_handle = void*;
#endif

namespace logger_system {
namespace core {

class plugin_manager {
public:
    using plugin_factory_func = std::function<std::shared_ptr<void>()>;
    using plugin_destroyer_func = std::function<void(void*)>;

    static plugin_manager& instance();

    bool load_plugin(const std::string& plugin_path);

    bool unload_plugin(const std::string& plugin_name);

    template<typename T>
    std::shared_ptr<T> get_plugin(const std::string& plugin_name);

    std::vector<interfaces::plugin_info> list_available_plugins() const;

    std::vector<interfaces::plugin_info> list_loaded_plugins() const;

    bool is_plugin_loaded(const std::string& plugin_name) const;

    interfaces::plugin_info get_plugin_info(const std::string& plugin_name) const;

    void set_plugin_directory(const std::string& directory);

    std::string get_plugin_directory() const;

    void discover_plugins();

    bool validate_plugin(const std::string& plugin_path) const;

    void monitor_plugin_health();

    bool register_plugin_factory(const std::string& plugin_name,
                                const std::string& type_name,
                                plugin_factory_func factory,
                                plugin_destroyer_func destroyer = nullptr);

    void set_plugin_timeout(std::chrono::milliseconds timeout);

    std::chrono::milliseconds get_plugin_loading_time(const std::string& plugin_name) const;

    ~plugin_manager();

private:
    plugin_manager() = default;
    plugin_manager(const plugin_manager&) = delete;
    plugin_manager& operator=(const plugin_manager&) = delete;

    struct plugin_entry {
        std::string name;
        std::string path;
        std::string type_name;
        library_handle handle;
        plugin_factory_func factory;
        plugin_destroyer_func destroyer;
        std::shared_ptr<void> instance;
        interfaces::plugin_info info;
        std::chrono::system_clock::time_point load_time;
        std::chrono::milliseconds loading_duration{0};
        bool loaded;
        bool healthy;

        plugin_entry() : handle(nullptr), loaded(false), healthy(false),
                        info("", "", "", interfaces::plugin_type::unknown, "", false) {}
    };

    library_handle load_library(const std::string& path);
    void unload_library(library_handle handle);
    std::string get_last_error() const;

    template<typename T>
    T get_symbol(library_handle handle, const std::string& symbol_name) const;

    bool is_valid_plugin_file(const std::string& path) const;
    std::string extract_plugin_name(const std::string& path) const;

    mutable std::mutex plugins_mutex_;
    std::unordered_map<std::string, plugin_entry> plugins_;
    std::string plugin_directory_;
    std::chrono::milliseconds plugin_timeout_{5000}; // 5 seconds default
};

template<typename T>
std::shared_ptr<T> plugin_manager::get_plugin(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end() || !it->second.loaded) {
        return nullptr;
    }

    auto& entry = it->second;
    if (!entry.instance && entry.factory) {
        entry.instance = entry.factory();
    }

    return std::static_pointer_cast<T>(entry.instance);
}

template<typename T>
T plugin_manager::get_symbol(library_handle handle, const std::string& symbol_name) const {
#ifdef _WIN32
    return reinterpret_cast<T>(GetProcAddress(handle, symbol_name.c_str()));
#else
    return reinterpret_cast<T>(dlsym(handle, symbol_name.c_str()));
#endif
}

} // namespace core
} // namespace logger_system

#endif // PLUGIN_MANAGER_H