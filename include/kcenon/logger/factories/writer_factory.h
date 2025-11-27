#pragma once

/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
All rights reserved.
*****************************************************************************/

/**
 * @file writer_factory.h
 * @brief Factory for creating log writer instances
 * @since 2.0.0
 *
 * @details Provides unified factory methods for creating all types of log writers.
 * Supports both static factory methods and a registry pattern for custom writers.
 */

#include <memory>
#include <string>
#include <unordered_map>
#include <functional>
#include <chrono>

#include "../writers/base_writer.h"
#include "../writers/console_writer.h"
#include "../writers/file_writer.h"
#include "../writers/rotating_file_writer.h"
#include "../writers/network_writer.h"
#include "../writers/batch_writer.h"

namespace kcenon::logger {

/**
 * @class writer_factory
 * @brief Factory for creating log writer instances
 *
 * @details Provides static factory methods for all built-in writer types
 * and a registry pattern for custom writer types.
 *
 * @since 2.0.0
 */
class writer_factory {
public:
    using creator_fn = std::function<std::unique_ptr<base_writer>()>;

    //========================================================================
    // Static Factory Methods
    //========================================================================

    /**
     * @brief Create a console writer
     * @param use_stderr Send output to stderr (default: stdout)
     * @param auto_detect_color Auto-detect terminal color support
     * @return Unique pointer to console writer
     */
    static std::unique_ptr<base_writer> create_console(
        bool use_stderr = false,
        bool auto_detect_color = true
    ) {
        return std::make_unique<console_writer>(use_stderr, auto_detect_color);
    }

    /**
     * @brief Create a file writer
     * @param filename Path to log file
     * @param append Append to existing file (default: true)
     * @param buffer_size Output buffer size
     * @return Unique pointer to file writer
     */
    static std::unique_ptr<base_writer> create_file(
        const std::string& filename,
        bool append = true,
        std::size_t buffer_size = 8192
    ) {
        return std::make_unique<file_writer>(filename, append, buffer_size);
    }

    /**
     * @brief Create a rotating file writer (size-based)
     * @param filename Path to log file
     * @param max_size Maximum file size before rotation
     * @param max_files Maximum number of backup files
     * @param check_interval Writes between rotation checks
     * @return Unique pointer to rotating file writer
     */
    static std::unique_ptr<base_writer> create_rotating_file(
        const std::string& filename,
        std::size_t max_size,
        std::size_t max_files,
        std::size_t check_interval = 100
    ) {
        return std::make_unique<rotating_file_writer>(
            filename, max_size, max_files, check_interval
        );
    }

    /**
     * @brief Create a rotating file writer (time-based)
     * @param filename Path to log file
     * @param type Rotation type (daily/hourly)
     * @param max_files Maximum number of backup files
     * @param check_interval Writes between rotation checks
     * @return Unique pointer to rotating file writer
     */
    static std::unique_ptr<base_writer> create_rotating_file(
        const std::string& filename,
        rotation_type type,
        std::size_t max_files,
        std::size_t check_interval = 100
    ) {
        return std::make_unique<rotating_file_writer>(
            filename, type, max_files, check_interval
        );
    }

    /**
     * @brief Create a network writer
     * @param host Remote host address
     * @param port Remote port
     * @param protocol Network protocol (TCP/UDP)
     * @param buffer_size Internal buffer size
     * @param reconnect_interval Reconnection interval
     * @return Unique pointer to network writer
     */
    static std::unique_ptr<base_writer> create_network(
        const std::string& host,
        uint16_t port,
        network_writer::protocol_type protocol = network_writer::protocol_type::tcp,
        std::size_t buffer_size = 8192,
        std::chrono::seconds reconnect_interval = std::chrono::seconds(5)
    ) {
        return std::make_unique<network_writer>(
            host, port, protocol, buffer_size, reconnect_interval
        );
    }

    /**
     * @brief Wrap a writer with batch processing
     * @param writer The writer to wrap
     * @param batch_size Maximum batch size
     * @param flush_interval Auto-flush interval
     * @return Unique pointer to batch writer
     */
    static std::unique_ptr<base_writer> create_batch(
        std::unique_ptr<base_writer> writer,
        std::size_t batch_size = 100,
        std::chrono::milliseconds flush_interval = std::chrono::milliseconds(1000)
    ) {
        batch_writer::config cfg;
        cfg.max_batch_size = batch_size;
        cfg.flush_interval = flush_interval;
        return std::make_unique<batch_writer>(std::move(writer), cfg);
    }

    //========================================================================
    // Preset Factory Methods
    //========================================================================

    /**
     * @brief Create a development preset writer
     * @return Console writer with colors enabled
     */
    static std::unique_ptr<base_writer> create_development() {
        return create_console(false, true);
    }

    /**
     * @brief Create a production preset writer
     * @param log_directory Directory for log files
     * @param filename Log file name
     * @return Batched rotating file writer
     */
    static std::unique_ptr<base_writer> create_production(
        const std::string& log_directory = "./logs",
        const std::string& filename = "app.log"
    ) {
        std::string full_path = log_directory + "/" + filename;
        auto rotating = create_rotating_file(
            full_path,
            100 * 1024 * 1024,  // 100MB
            10
        );
        return create_batch(std::move(rotating), 200, std::chrono::milliseconds(2000));
    }

    /**
     * @brief Create a high-performance preset writer
     * @param filename Log file path
     * @return Batched file writer with large buffers
     */
    static std::unique_ptr<base_writer> create_high_performance(
        const std::string& filename = "./logs/app.log"
    ) {
        auto file = std::make_unique<file_writer>(filename, true, 65536);
        return create_batch(std::move(file), 500, std::chrono::milliseconds(5000));
    }

    //========================================================================
    // Registry Pattern
    //========================================================================

    /**
     * @brief Register a custom writer type
     * @param name Type name for the writer
     * @param creator Factory function to create the writer
     */
    static void register_type(const std::string& name, creator_fn creator) {
        registry()[name] = std::move(creator);
    }

    /**
     * @brief Create a writer by registered name
     * @param name Registered type name
     * @return Unique pointer to writer, or nullptr if not found
     */
    static std::unique_ptr<base_writer> create(const std::string& name) {
        auto it = registry().find(name);
        if (it == registry().end()) {
            return nullptr;
        }
        return it->second();
    }

    /**
     * @brief Check if a writer type is registered
     * @param name Type name to check
     * @return true if registered
     */
    static bool has_type(const std::string& name) {
        return registry().find(name) != registry().end();
    }

private:
    static std::unordered_map<std::string, creator_fn>& registry() {
        static std::unordered_map<std::string, creator_fn> instance;
        return instance;
    }
};

} // namespace kcenon::logger
