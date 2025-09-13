#include "plugin_registry.h"
#include <algorithm>

namespace logger_system {
namespace core {

plugin_registry& plugin_registry::instance() {
    static plugin_registry instance;
    return instance;
}

bool plugin_registry::plugin_exists(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(registry_mutex_);
    return factories_.find(plugin_name) != factories_.end();
}

std::vector<std::string> plugin_registry::list_registered_plugins() const {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    std::vector<std::string> plugin_names;
    plugin_names.reserve(factories_.size());

    for (const auto& [name, entry] : factories_) {
        plugin_names.push_back(name);
    }

    std::sort(plugin_names.begin(), plugin_names.end());
    return plugin_names;
}

std::vector<std::string> plugin_registry::list_plugins_by_type(const std::type_info& type) const {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    std::vector<std::string> plugin_names;
    std::type_index target_type(type);

    for (const auto& [name, entry] : factories_) {
        if (entry.type_index == target_type) {
            plugin_names.push_back(name);
        }
    }

    std::sort(plugin_names.begin(), plugin_names.end());
    return plugin_names;
}

bool plugin_registry::unregister_plugin(const std::string& plugin_name) {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    auto it = factories_.find(plugin_name);
    if (it == factories_.end()) {
        return false;
    }

    factories_.erase(it);
    return true;
}

void plugin_registry::clear_registry() {
    std::lock_guard<std::mutex> lock(registry_mutex_);
    factories_.clear();
}

size_t plugin_registry::get_plugin_count() const {
    std::lock_guard<std::mutex> lock(registry_mutex_);
    return factories_.size();
}

std::string plugin_registry::get_plugin_type_name(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    auto it = factories_.find(plugin_name);
    if (it == factories_.end()) {
        return "";
    }

    return it->second.type_name;
}

} // namespace core
} // namespace logger_system