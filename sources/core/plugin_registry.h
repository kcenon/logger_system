#ifndef PLUGIN_REGISTRY_H
#define PLUGIN_REGISTRY_H

#include <string>
#include <memory>
#include <unordered_map>
#include <functional>
#include <mutex>
#include <any>
#include <typeindex>
#include <vector>

namespace logger_system {
namespace core {

class plugin_registry {
public:
    using factory_function = std::function<std::shared_ptr<void>(const std::string&)>;
    using config_map = std::unordered_map<std::string, std::any>;

    static plugin_registry& instance();

    template<typename T>
    bool register_plugin_factory(const std::string& plugin_name,
                                std::function<std::shared_ptr<T>(const std::string&)> factory);

    template<typename T>
    std::shared_ptr<T> create_plugin(const std::string& plugin_name, const std::string& config = "");

    template<typename T>
    std::shared_ptr<T> create_plugin_with_config(const std::string& plugin_name, const config_map& config);

    bool plugin_exists(const std::string& plugin_name) const;

    std::vector<std::string> list_registered_plugins() const;

    std::vector<std::string> list_plugins_by_type(const std::type_info& type) const;

    bool unregister_plugin(const std::string& plugin_name);

    template<typename T>
    bool is_plugin_type(const std::string& plugin_name) const;

    void clear_registry();

    size_t get_plugin_count() const;

    std::string get_plugin_type_name(const std::string& plugin_name) const;

private:
    plugin_registry() = default;
    plugin_registry(const plugin_registry&) = delete;
    plugin_registry& operator=(const plugin_registry&) = delete;

    struct plugin_factory_entry {
        factory_function factory;
        std::type_index type_index;
        std::string type_name;

        plugin_factory_entry(factory_function f, std::type_index ti, const std::string& tn)
            : factory(f), type_index(ti), type_name(tn) {}
    };

    mutable std::mutex registry_mutex_;
    std::unordered_map<std::string, plugin_factory_entry> factories_;
};

template<typename T>
bool plugin_registry::register_plugin_factory(const std::string& plugin_name,
                                             std::function<std::shared_ptr<T>(const std::string&)> factory) {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    // Wrap the typed factory in a generic factory function
    factory_function generic_factory = [factory](const std::string& config) -> std::shared_ptr<void> {
        return std::static_pointer_cast<void>(factory(config));
    };

    factories_.emplace(plugin_name,
                      plugin_factory_entry(generic_factory, std::type_index(typeid(T)), typeid(T).name()));

    return true;
}

template<typename T>
std::shared_ptr<T> plugin_registry::create_plugin(const std::string& plugin_name, const std::string& config) {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    auto it = factories_.find(plugin_name);
    if (it == factories_.end()) {
        return nullptr;
    }

    // Check if the requested type matches the registered type
    if (it->second.type_index != std::type_index(typeid(T))) {
        return nullptr;
    }

    auto instance = it->second.factory(config);
    return std::static_pointer_cast<T>(instance);
}

template<typename T>
std::shared_ptr<T> plugin_registry::create_plugin_with_config(const std::string& plugin_name, const config_map& config) {
    // Convert config_map to string representation
    // This is a simplified approach - in a real implementation, you might use JSON or another serialization format
    std::string config_string = "{";
    bool first = true;
    for (const auto& [key, value] : config) {
        if (!first) config_string += ",";
        config_string += key + ":";

        // Simple type handling for demonstration
        if (value.type() == typeid(std::string)) {
            config_string += std::any_cast<std::string>(value);
        } else if (value.type() == typeid(int)) {
            config_string += std::to_string(std::any_cast<int>(value));
        } else if (value.type() == typeid(bool)) {
            config_string += std::any_cast<bool>(value) ? "true" : "false";
        }
        first = false;
    }
    config_string += "}";

    return create_plugin<T>(plugin_name, config_string);
}

template<typename T>
bool plugin_registry::is_plugin_type(const std::string& plugin_name) const {
    std::lock_guard<std::mutex> lock(registry_mutex_);

    auto it = factories_.find(plugin_name);
    if (it == factories_.end()) {
        return false;
    }

    return it->second.type_index == std::type_index(typeid(T));
}

} // namespace core
} // namespace logger_system

#endif // PLUGIN_REGISTRY_H