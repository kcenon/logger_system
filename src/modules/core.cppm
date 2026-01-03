/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*****************************************************************************/

/**
 * @file core.cppm
 * @brief Core partition for kcenon.logger module
 * @author kcenon
 * @since 3.0.0
 *
 * @details This partition exports the core logging functionality:
 * - logger: Main logger class with async support
 * - log_context: Source location context for log entries
 * - log_collector: Asynchronous log collection and batching
 * - Writers: Console, file, network, and rotating file writers
 * - Formatters: Base, JSON, and timestamp formatters
 * - Filters: Log filtering interfaces and implementations
 * - Interfaces: Log entry, writer, formatter, and sink interfaces
 */

export module kcenon.logger:core;

// Standard library imports
import <atomic>;
import <chrono>;
import <memory>;
import <mutex>;
import <string>;
import <string_view>;
import <thread>;
import <vector>;

// Import common_system dependencies
import kcenon.common;

// Export core classes
export {
    // Include the actual headers - these will be converted to module implementation
    // For now, we re-export the header content as module interface

    // Core types and enumerations
    namespace logger_system {
        enum class log_level {
            trace = 0,
            debug = 1,
            info = 2,
            warn = 3,
            warning = 3,  // Alias for warn
            error = 4,
            fatal = 5,
            critical = 5, // Alias for fatal
            off = 6
        };

        enum class overflow_policy {
            block,
            drop_oldest,
            drop_newest,
            grow
        };

        enum class health_status {
            healthy,
            degraded,
            unhealthy,
            critical
        };

        enum class logger_error_code {
            success = 0,
            invalid_configuration,
            writer_not_found,
            writer_already_exists,
            write_failed,
            flush_failed,
            buffer_full,
            invalid_level,
            invalid_pattern,
            file_open_failed,
            network_error,
            encryption_error,
            compression_error,
            monitoring_failed,
            component_not_found,
            di_resolution_failed,
            health_check_failed,
            writer_closed,
            unknown_error
        };

        inline const char* log_level_to_string(log_level level) {
            switch (level) {
                case log_level::trace: return "TRACE";
                case log_level::debug: return "DEBUG";
                case log_level::info:  return "INFO";
                case log_level::warn:  return "WARN";
                case log_level::error: return "ERROR";
                case log_level::fatal: return "FATAL";
                case log_level::off:   return "OFF";
                default: return "UNKNOWN";
            }
        }
    }

    // Core namespace exports
    namespace kcenon::logger {
        // Type aliases from logger_system
        using log_level = logger_system::log_level;
        using health_status = logger_system::health_status;
        using overflow_policy = logger_system::overflow_policy;

        namespace core {
            /**
             * @brief Log context containing source location information
             */
            struct log_context {
                std::string_view file{"unknown"};
                int line{0};
                std::string_view function{"unknown"};
                std::thread::id thread_id{std::this_thread::get_id()};
                std::chrono::time_point<std::chrono::system_clock> timestamp{
                    std::chrono::system_clock::now()};
            };

            /**
             * @brief Create a log context with source location
             */
            inline log_context make_log_context(std::string_view file,
                                                int line,
                                                std::string_view function) {
                log_context ctx;
                ctx.file = file;
                ctx.line = line;
                ctx.function = function;
                ctx.thread_id = std::this_thread::get_id();
                ctx.timestamp = std::chrono::system_clock::now();
                return ctx;
            }
        }

        // Forward declarations for core classes
        class logger;
        class log_collector;
        class base_writer;
        class console_writer;
        class file_writer;
        class rotating_file_writer;
        class network_writer;
        class async_writer;
        class batch_writer;
        class critical_writer;
        class composite_writer;

        // Forward declarations for formatters
        namespace formatters {
            class base_formatter;
            class json_formatter;
            class timestamp_formatter;
        }

        // Forward declarations for filters
        namespace filters {
            class log_filter;
        }

        // Forward declarations for interfaces
        class log_entry;
        class log_filter_interface;
        class log_formatter_interface;
        class log_writer_interface;
        class log_sink_interface;
        class output_sink_interface;

        // Forward declarations for metrics
        namespace metrics {
            struct logger_performance_stats;
        }

        // Forward declarations for security
        namespace security {
            class signal_manager;
            class critical_logger_interface;
        }

        // Forward declarations for strategies
        namespace strategies {
            class config_strategy_interface;
            class deployment_strategy;
            class environment_strategy;
            class performance_strategy;
            class composite_strategy;
        }

        // Forward declarations for builders
        class logger_builder;
        class logger_config_builder;
        struct logger_config;
    }
}
