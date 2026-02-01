/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file writer_builder_example.cpp
 * @brief Demonstrates writer_builder usage patterns
 * @author kcenon
 * @since 4.1.0
 *
 * This example shows how to use the writer_builder class to compose
 * log writers using the Decorator pattern. The builder provides a fluent
 * API that makes it easy to chain multiple decorators together.
 *
 * Part of EPIC #391 - Decorator pattern refactoring.
 */

#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/writers/async_writer.h>

#ifdef LOGGER_WITH_ENCRYPTION
#include <kcenon/logger/security/secure_key.h>
#include <kcenon/logger/security/secure_key_storage.h>
#endif

#include <iostream>
#include <memory>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

/**
 * @brief Simple level filter for demonstration
 */
class level_filter : public log_filter_interface {
public:
    explicit level_filter(log_level min_level) : min_level_(min_level) {}

    bool should_log(const log_entry& entry) const override {
        return entry.level >= min_level_;
    }

    std::string get_name() const override {
        return "level_filter";
    }

private:
    log_level min_level_;
};

/**
 * @brief Example 1: Basic file writer
 *
 * The simplest usage - just a file writer with no decorators.
 */
void example_basic_file_writer() {
    std::cout << "\n=== Example 1: Basic File Writer ===\n";

    auto writer = writer_builder()
        .file("basic.log")
        .build();

    std::cout << "Created basic file writer: " << writer->get_name() << "\n";
}

/**
 * @brief Example 2: Async + Buffered File Writer
 *
 * High-performance setup combining buffering and async writing.
 * - Buffered: Batches up to 500 entries before writing
 * - Async: Processes writes in background thread with 20000-entry queue
 */
void example_async_buffered_file() {
    std::cout << "\n=== Example 2: Async + Buffered File Writer ===\n";

    auto writer = writer_builder()
        .file("app.log")
        .buffered(500)       // Buffer up to 500 entries
        .async(20000)        // Large queue for high throughput
        .build();

    std::cout << "Created async+buffered writer: " << writer->get_name() << "\n";
    std::cout << "  - Buffering reduces I/O operations\n";
    std::cout << "  - Async processing enables high throughput\n";

    // Note: async_writer needs to be started
    if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
        async_w->start();
        std::cout << "  - Async writer started\n";
        async_w->stop();
    }
}

/**
 * @brief Example 3: Console Writer with Filtering
 *
 * Console writer that only logs warnings and errors.
 */
void example_filtered_console() {
    std::cout << "\n=== Example 3: Filtered Console Writer ===\n";

    auto filter = std::make_unique<level_filter>(log_level::warning);

    auto writer = writer_builder()
        .console()
        .filtered(std::move(filter))
        .build();

    std::cout << "Created filtered console writer: " << writer->get_name() << "\n";
    std::cout << "  - Only logs WARNING and above\n";
}

#ifdef LOGGER_WITH_ENCRYPTION
/**
 * @brief Example 4: Encrypted Logging
 *
 * File writer with encryption for sensitive data.
 * Requires OpenSSL support.
 */
void example_encrypted_logging() {
    std::cout << "\n=== Example 4: Encrypted Logging ===\n";

    using namespace kcenon::logger::security;

    auto key_result = secure_key_storage::generate_key(32);
    if (key_result.is_err()) {
        std::cout << "Failed to generate encryption key: "
                  << key_result.error().message << "\n";
        return;
    }

    auto writer = writer_builder()
        .file("secure.log.enc")
        .encrypted(std::move(key_result.value()))
        .buffered(100)
        .async()
        .build();

    std::cout << "Created encrypted writer: " << writer->get_name() << "\n";
    std::cout << "  - 32-byte AES encryption\n";
    std::cout << "  - Buffered for efficiency\n";
    std::cout << "  - Async for performance\n";

    if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
        async_w->start();
        async_w->stop();
    }
}
#endif

/**
 * @brief Example 5: Production Setup with Multiple Writers
 *
 * Realistic production scenario with multiple log destinations:
 * - Main log: All messages with async+buffered
 * - Error log: Only errors with separate file
 */
void example_production_setup() {
    std::cout << "\n=== Example 5: Production Multi-Writer Setup ===\n";

    logger log;

    // Main log: async file with buffering
    auto main_writer = writer_builder()
        .file("app.log")
        .buffered(500)
        .async(20000)
        .build();

    std::cout << "Main writer: " << main_writer->get_name() << "\n";

    // Start async writer before adding to logger
    if (auto* async_main = dynamic_cast<async_writer*>(main_writer.get())) {
        async_main->start();
    }

    log.add_writer("main", std::move(main_writer));

    // Error log: separate file for errors only
    auto error_filter = std::make_unique<level_filter>(log_level::error);

    auto error_writer = writer_builder()
        .file("errors.log")
        .filtered(std::move(error_filter))
        .async()
        .build();

    std::cout << "Error writer: " << error_writer->get_name() << "\n";

    if (auto* async_error = dynamic_cast<async_writer*>(error_writer.get())) {
        async_error->start();
    }

    log.add_writer("errors", std::move(error_writer));

    // Log some messages
    std::cout << "\nLogging messages:\n";
    log.log(log_level::info, std::string("Application started"));
    log.log(log_level::warning, std::string("Low disk space"));
    log.log(log_level::error, std::string("Failed to connect to database"));

    std::cout << "  - INFO: Goes to app.log\n";
    std::cout << "  - WARNING: Goes to app.log\n";
    std::cout << "  - ERROR: Goes to both app.log and errors.log\n";

    log.flush();
}

/**
 * @brief Example 6: Comparing Builder vs Manual Composition
 *
 * Shows the difference between using the builder and manual nesting.
 */
void example_builder_vs_manual() {
    std::cout << "\n=== Example 6: Builder vs Manual Composition ===\n";

    std::cout << "\nManual composition (verbose):\n";
    std::cout << "  auto writer = std::make_unique<async_writer>(\n";
    std::cout << "      std::make_unique<buffered_writer>(\n";
    std::cout << "          std::make_unique<file_writer>(\"app.log\"),\n";
    std::cout << "          500\n";
    std::cout << "      ),\n";
    std::cout << "      20000\n";
    std::cout << "  );\n";

    std::cout << "\nBuilder pattern (readable):\n";
    std::cout << "  auto writer = writer_builder()\n";
    std::cout << "      .file(\"app.log\")\n";
    std::cout << "      .buffered(500)\n";
    std::cout << "      .async(20000)\n";
    std::cout << "      .build();\n";

    auto writer = writer_builder()
        .file("app.log")
        .buffered(500)
        .async(20000)
        .build();

    std::cout << "\nResult: " << writer->get_name() << "\n";
    std::cout << "Benefits:\n";
    std::cout << "  ✓ More readable and self-documenting\n";
    std::cout << "  ✓ Less error-prone (no manual nesting)\n";
    std::cout << "  ✓ Easy to add/remove decorators\n";
    std::cout << "  ✓ Type-safe at compile time\n";

    if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
        async_w->start();
        async_w->stop();
    }
}

int main() {
    std::cout << "Writer Builder Examples\n";
    std::cout << "=======================\n";
    std::cout << "\nThese examples demonstrate how to use writer_builder\n";
    std::cout << "to compose log writers using the Decorator pattern.\n";

    try {
        example_basic_file_writer();
        example_async_buffered_file();
        example_filtered_console();

#ifdef LOGGER_WITH_ENCRYPTION
        example_encrypted_logging();
#else
        std::cout << "\n=== Example 4: Encrypted Logging ===\n";
        std::cout << "Skipped (LOGGER_WITH_ENCRYPTION not defined)\n";
#endif

        example_production_setup();
        example_builder_vs_manual();

        std::cout << "\n=== All Examples Completed Successfully ===\n";
    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
