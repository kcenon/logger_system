#ifndef LOGGING_INTERFACES_H
#define LOGGING_INTERFACES_H

#include "logger_interface.h"
#include "writer_interface.h"
#include "threading_plugin_interface.h"
#include "plugin_loader_interface.h"

namespace logger_system {
namespace interfaces {

constexpr const char* LOGGING_INTERFACES_VERSION = "1.0.0";

} // namespace interfaces
} // namespace logger_system

#endif // LOGGING_INTERFACES_H