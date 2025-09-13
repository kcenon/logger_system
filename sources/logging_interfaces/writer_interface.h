#ifndef WRITER_INTERFACE_H
#define WRITER_INTERFACE_H

#include <string>
#include <chrono>
#include "logger_interface.h"

namespace logger_system {
namespace interfaces {

struct log_entry {
    log_level level;
    std::string message;
    std::chrono::system_clock::time_point timestamp;
    std::string thread_id;
    std::string logger_name;

    log_entry(log_level lvl, const std::string& msg,
             const std::string& name = "",
             const std::string& tid = "")
        : level(lvl), message(msg), logger_name(name), thread_id(tid),
          timestamp(std::chrono::system_clock::now()) {}
};

class interface_log_writer {
public:
    virtual ~interface_log_writer() = default;

    virtual void write_log_entry(const log_entry& entry) = 0;

    virtual void flush_writer() = 0;

    virtual bool is_writer_thread_safe() const = 0;

    virtual void set_pattern(const std::string& pattern) = 0;

    virtual std::string get_writer_name() const = 0;
};

} // namespace interfaces
} // namespace logger_system

#endif // WRITER_INTERFACE_H