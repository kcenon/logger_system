#include "plugin_manager.h"
#include <filesystem>
#include <algorithm>
#include <stdexcept>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <dlfcn.h>
#endif

namespace logger_system {
namespace core {

plugin_manager& plugin_manager::instance() {
    static plugin_manager instance;
    return instance;
}

bool plugin_manager::load_plugin(const std::string& plugin_path) {
    auto start_time = std::chrono::high_resolution_clock::now();

    std::lock_guard<std::mutex> lock(plugins_mutex_);

    if (!std::filesystem::exists(plugin_path)) {
        return false;
    }

    std::string plugin_name = extract_plugin_name(plugin_path);

    if (plugins_.find(plugin_name) != plugins_.end() && plugins_[plugin_name].loaded) {
        return true; // Already loaded
    }

    library_handle handle = load_library(plugin_path);
    if (!handle) {
        return false;
    }

    plugin_entry entry;
    entry.name = plugin_name;
    entry.path = plugin_path;
    entry.handle = handle;
    entry.load_time = std::chrono::system_clock::now();
    entry.loaded = true;
    entry.healthy = true;

    // Try to get plugin info function
    auto get_info_func = get_symbol<interfaces::plugin_info(*)()>(handle, "get_plugin_info");
    if (get_info_func) {
        entry.info = get_info_func();
    } else {
        entry.info = interfaces::plugin_info(plugin_name, "1.0.0", "Unknown plugin",
                                           interfaces::plugin_type::unknown, plugin_path, true);
    }

    // Try to get factory function
    auto factory_func = get_symbol<std::shared_ptr<void>(*)()>(handle, "create_plugin");
    if (factory_func) {
        entry.factory = [factory_func]() { return factory_func(); };
    }

    // Try to get destroyer function
    auto destroyer_func = get_symbol<void(*)(void*)>(handle, "destroy_plugin");
    if (destroyer_func) {
        entry.destroyer = [destroyer_func](void* ptr) { destroyer_func(ptr); };
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    entry.loading_duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);

    plugins_[plugin_name] = std::move(entry);

    return true;
}

bool plugin_manager::unload_plugin(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end() || !it->second.loaded) {
        return false;
    }

    auto& entry = it->second;

    // Destroy instance if exists
    if (entry.instance && entry.destroyer) {
        entry.destroyer(entry.instance.get());
    }
    entry.instance.reset();

    // Unload library
    if (entry.handle) {
        unload_library(entry.handle);
        entry.handle = nullptr;
    }

    entry.loaded = false;
    entry.healthy = false;

    return true;
}

std::vector<interfaces::plugin_info> plugin_manager::list_available_plugins() const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    std::vector<interfaces::plugin_info> plugins;
    for (const auto& [name, entry] : plugins_) {
        plugins.push_back(entry.info);
    }

    return plugins;
}

std::vector<interfaces::plugin_info> plugin_manager::list_loaded_plugins() const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    std::vector<interfaces::plugin_info> plugins;
    for (const auto& [name, entry] : plugins_) {
        if (entry.loaded) {
            plugins.push_back(entry.info);
        }
    }

    return plugins;
}

bool plugin_manager::is_plugin_loaded(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    return it != plugins_.end() && it->second.loaded;
}

interfaces::plugin_info plugin_manager::get_plugin_info(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it != plugins_.end()) {
        return it->second.info;
    }

    return interfaces::plugin_info("", "", "", interfaces::plugin_type::unknown);
}

void plugin_manager::set_plugin_directory(const std::string& directory) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    plugin_directory_ = directory;
}

std::string plugin_manager::get_plugin_directory() const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    return plugin_directory_;
}

void plugin_manager::discover_plugins() {
    if (plugin_directory_.empty()) {
        return;
    }

    if (!std::filesystem::exists(plugin_directory_)) {
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(plugin_directory_)) {
        if (entry.is_regular_file() && is_valid_plugin_file(entry.path().string())) {
            load_plugin(entry.path().string());
        }
    }
}

bool plugin_manager::validate_plugin(const std::string& plugin_path) const {
    if (!std::filesystem::exists(plugin_path)) {
        return false;
    }

    if (!is_valid_plugin_file(plugin_path)) {
        return false;
    }

    // Try to load temporarily for validation
    library_handle handle = const_cast<plugin_manager*>(this)->load_library(plugin_path);
    if (!handle) {
        return false;
    }

    // Check for required symbols
    bool valid = true;
    auto get_info_func = get_symbol<interfaces::plugin_info(*)()>(handle, "get_plugin_info");
    if (!get_info_func) {
        valid = false;
    }

    const_cast<plugin_manager*>(this)->unload_library(handle);

    return valid;
}

void plugin_manager::monitor_plugin_health() {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    for (auto& [name, entry] : plugins_) {
        if (entry.loaded) {
            // Simple health check - verify handle is still valid
            if (!entry.handle) {
                entry.healthy = false;
            }
        }
    }
}

bool plugin_manager::register_plugin_factory(const std::string& plugin_name,
                                            const std::string& type_name,
                                            plugin_factory_func factory,
                                            plugin_destroyer_func destroyer) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it == plugins_.end()) {
        plugin_entry entry;
        entry.name = plugin_name;
        entry.type_name = type_name;
        entry.factory = factory;
        entry.destroyer = destroyer;
        entry.loaded = true;
        entry.healthy = true;
        entry.info = interfaces::plugin_info(plugin_name, "1.0.0", "In-memory plugin",
                                           interfaces::plugin_type::unknown, "", true);

        plugins_[plugin_name] = std::move(entry);
    } else {
        it->second.factory = factory;
        it->second.destroyer = destroyer;
        it->second.type_name = type_name;
    }

    return true;
}

void plugin_manager::set_plugin_timeout(std::chrono::milliseconds timeout) {
    std::lock_guard<std::mutex> lock(plugins_mutex_);
    plugin_timeout_ = timeout;
}

std::chrono::milliseconds plugin_manager::get_plugin_loading_time(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    auto it = plugins_.find(plugin_name);
    if (it != plugins_.end()) {
        return it->second.loading_duration;
    }

    return std::chrono::milliseconds{0};
}

plugin_manager::~plugin_manager() {
    std::lock_guard<std::mutex> lock(plugins_mutex_);

    for (auto& [name, entry] : plugins_) {
        if (entry.loaded) {
            if (entry.instance && entry.destroyer) {
                entry.destroyer(entry.instance.get());
            }
            if (entry.handle) {
                unload_library(entry.handle);
            }
        }
    }
}

library_handle plugin_manager::load_library(const std::string& path) {
#ifdef _WIN32
    return LoadLibraryA(path.c_str());
#else
    return dlopen(path.c_str(), RTLD_LAZY);
#endif
}

void plugin_manager::unload_library(library_handle handle) {
    if (!handle) return;

#ifdef _WIN32
    FreeLibrary(handle);
#else
    dlclose(handle);
#endif
}

std::string plugin_manager::get_last_error() const {
#ifdef _WIN32
    DWORD error = GetLastError();
    if (error == 0) return "";

    LPSTR messageBuffer = nullptr;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

    std::string message(messageBuffer, size);
    LocalFree(messageBuffer);
    return message;
#else
    const char* error = dlerror();
    return error ? error : "";
#endif
}

bool plugin_manager::is_valid_plugin_file(const std::string& path) const {
    std::filesystem::path file_path(path);
    std::string extension = file_path.extension().string();

#ifdef _WIN32
    return extension == ".dll";
#elif defined(__APPLE__)
    return extension == ".dylib";
#else
    return extension == ".so";
#endif
}

std::string plugin_manager::extract_plugin_name(const std::string& path) const {
    std::filesystem::path file_path(path);
    return file_path.stem().string();
}

} // namespace core
} // namespace logger_system