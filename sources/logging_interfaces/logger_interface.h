#ifndef LOGGER_INTERFACE_H
#define LOGGER_INTERFACE_H

#include <string>
#include <memory>

namespace logger_system {
namespace interfaces {

enum class log_level {
    trace = 0,
    debug = 1,
    info = 2,
    warn = 3,
    error = 4,
    critical = 5,
    off = 6
};

class interface_log_writer;

class interface_logger {
public:
    virtual ~interface_logger() = default;

    virtual void log_message(log_level level, const std::string& message) = 0;

    virtual void set_log_level(log_level level) = 0;

    virtual void flush_logs() = 0;

    virtual void add_log_writer(std::shared_ptr<interface_log_writer> writer) = 0;

    virtual log_level get_log_level() const = 0;

    virtual bool is_enabled_for(log_level level) const = 0;
};

} // namespace interfaces
} // namespace logger_system

#endif // LOGGER_INTERFACE_H