#ifndef PLUGIN_LOADER_INTERFACE_H
#define PLUGIN_LOADER_INTERFACE_H

#include <string>
#include <memory>
#include <vector>
#include <map>

namespace logger_system {
namespace interfaces {

enum class plugin_type {
    threading,
    writer,
    formatter,
    filter,
    unknown
};

struct plugin_info {
    std::string name;
    std::string version;
    std::string description;
    plugin_type type;
    std::string path;
    bool loaded;

    plugin_info(const std::string& n, const std::string& v,
               const std::string& desc, plugin_type t,
               const std::string& p = "", bool l = false)
        : name(n), version(v), description(desc), type(t), path(p), loaded(l) {}
};

class interface_plugin_loader {
public:
    virtual ~interface_plugin_loader() = default;

    virtual bool load_plugin(const std::string& plugin_path) = 0;

    virtual bool unload_plugin(const std::string& plugin_name) = 0;

    virtual bool is_plugin_loaded(const std::string& plugin_name) const = 0;

    virtual std::vector<plugin_info> list_available_plugins() const = 0;

    virtual std::vector<plugin_info> list_loaded_plugins() const = 0;

    virtual plugin_info get_plugin_info(const std::string& plugin_name) const = 0;

    virtual void set_plugin_directory(const std::string& directory) = 0;

    virtual std::string get_plugin_directory() const = 0;

    virtual void discover_plugins() = 0;

    virtual bool validate_plugin(const std::string& plugin_path) const = 0;
};

} // namespace interfaces
} // namespace logger_system

#endif // PLUGIN_LOADER_INTERFACE_H