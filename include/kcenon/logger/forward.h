#pragma once

/**
 * @file forward.h
 * @brief Forward declarations for logger_system types
 *
 * This header provides forward declarations for commonly used types
 * in the logger_system module to reduce compilation dependencies.
 */

namespace kcenon::logger {

// Core classes
namespace core {
    class logger;
    class log_manager;
    class log_record;
    class log_formatter;
    enum class log_level;
}

// Sinks
namespace sinks {
    class sink;
    class console_sink;
    class file_sink;
    class rotating_file_sink;
    class async_sink;
}

// Formatters
namespace formatters {
    class basic_formatter;
    class pattern_formatter;
    class json_formatter;
    class xml_formatter;
}

// Filters
namespace filters {
    class filter;
    class level_filter;
    class regex_filter;
    class category_filter;
}

// Utilities
namespace utils {
    class log_buffer;
    class timestamp;
    class thread_context;
}

// Monitoring integration
namespace monitoring {
    class monitoring_interface;
    class metrics_collector;
}

} // namespace kcenon::logger