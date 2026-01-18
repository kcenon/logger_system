// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file critical_logging_example.cpp
 * @brief Example demonstrating critical log loss prevention mechanisms
 * @author kcenon
 * @date 2025-01-17
 *
 * This example shows how to configure the logger system to prevent
 * loss of critical log messages during crashes or abnormal termination.
 */

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/writers/critical_writer.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <iostream>
#include <thread>
#include <chrono>

using namespace kcenon::logger;
namespace ci = kcenon::common::interfaces;
using namespace std::chrono_literals;

/**
 * Example 1: Basic critical writer usage
 * Wraps a file writer to ensure critical logs are immediately flushed
 */
void example_basic_critical_writer() {
    std::cout << "\n=== Example 1: Basic Critical Writer ===\n";

    // Create logger with critical writer
    logger log(false); // Synchronous mode for this example

    // Wrap file writer with critical_writer
    auto file = std::make_unique<file_writer>("logs/critical_basic.log");
    auto critical = std::make_unique<critical_writer>(
        std::move(file),
        critical_writer_config{
            .force_flush_on_critical = true,
            .force_flush_on_error = false,
            .enable_signal_handlers = true,
            .write_ahead_log = false,
            .sync_on_critical = true
        }
    );

    log.add_writer(std::move(critical));

    // Normal logs (buffered)
    log.log(ci::log_level::info, std::string("Application started"));
    log.log(ci::log_level::debug, std::string("Debug information"));

    // Critical log (immediately flushed to disk)
    log.log(ci::log_level::critical, std::string("Critical error occurred - guaranteed on disk"));

    // Even if the program crashes here, the critical log above is safe
    std::cout << "Critical log written and flushed immediately\n";
}

/**
 * Example 2: Write-Ahead Logging (WAL) for maximum durability
 * Logs are written to a separate WAL file first, then to the main log
 */
void example_write_ahead_logging() {
    std::cout << "\n=== Example 2: Write-Ahead Logging ===\n";

    logger log(false);

    auto file = std::make_unique<rotating_file_writer>(
        "logs/wal_main.log",
        1024 * 1024, // 1 MB per file
        5            // Keep 5 rotations
    );

    auto critical = std::make_unique<critical_writer>(
        std::move(file),
        critical_writer_config{
            .force_flush_on_critical = true,
            .enable_signal_handlers = true,
            .write_ahead_log = true,  // Enable WAL
            .wal_path = "logs/.critical.wal",
            .sync_on_critical = true
        }
    );

    log.add_writer(std::move(critical));

    log.log(ci::log_level::info, std::string("Normal log"));
    log.log(ci::log_level::critical, std::string("Critical log - written to WAL first"));

    std::cout << "Check logs/.critical.wal for write-ahead log entries\n";
}

/**
 * Example 3: Hybrid writer - async for normal logs, sync for critical
 * Best of both worlds: performance + safety
 */
void example_hybrid_writer() {
    std::cout << "\n=== Example 3: Hybrid Writer (Async + Critical) ===\n";

    logger log(false);

    // Hybrid writer combines:
    // - Async queue for normal/debug/info/warn (high performance)
    // - Immediate flush for error/critical/fatal (no loss)
    auto hybrid = std::make_unique<hybrid_writer>(
        std::make_unique<file_writer>("logs/hybrid.log"),
        critical_writer_config{
            .force_flush_on_critical = true,
            .force_flush_on_error = true, // Also flush errors immediately
            .enable_signal_handlers = true
        },
        10000 // Async queue size
    );

    log.add_writer(std::move(hybrid));

    // These go through async queue (fast)
    for (int i = 0; i < 100; ++i) {
        log.log(ci::log_level::info, "High-frequency log " + std::to_string(i));
    }

    // This bypasses the queue and flushes immediately (safe)
    log.log(ci::log_level::critical, std::string("Critical error - no loss guaranteed"));

    std::cout << "Hybrid writer provides both performance and safety\n";
}

/**
 * Example 4: Signal handler demonstration
 * Shows how critical_writer handles abnormal termination
 */
void example_signal_handler() {
    std::cout << "\n=== Example 4: Signal Handler ===\n";

    logger log(false);

    auto critical = std::make_unique<critical_writer>(
        std::make_unique<file_writer>("logs/signal_test.log"),
        critical_writer_config{
            .force_flush_on_critical = true,
            .enable_signal_handlers = true,
            .write_ahead_log = true,
            .wal_path = "logs/.signal.wal"
        }
    );

    // Get stats reference before moving
    auto& stats = critical->get_stats();
    log.add_writer(std::move(critical));

    log.log(ci::log_level::info, std::string("Before critical log"));
    log.log(ci::log_level::critical, std::string("Critical log before potential crash"));

    std::cout << "Try sending SIGTERM (Ctrl+C) to this process\n";
    std::cout << "The signal handler will ensure logs are flushed\n";

    // Simulate some work
    std::this_thread::sleep_for(5s);

    std::cout << "Statistics:\n";
    std::cout << "  Critical writes: " << stats.total_critical_writes.load() << "\n";
    std::cout << "  Flushes: " << stats.total_flushes.load() << "\n";
    std::cout << "  WAL writes: " << stats.wal_writes.load() << "\n";
    std::cout << "  Signal invocations: " << stats.signal_handler_invocations.load() << "\n";
}

/**
 * Example 5: Production configuration
 * Recommended setup for production systems
 */
void example_production_setup() {
    std::cout << "\n=== Example 5: Production Configuration ===\n";

    // Use logger_builder for comprehensive configuration
    auto result = logger_builder()
        .with_async(true)           // Async for performance
        .with_buffer_size(32768)    // Large buffer for high throughput
        .with_min_level(log_level::info) // Production: info and above
        .add_writer("main",
            std::make_unique<hybrid_writer>(
                std::make_unique<rotating_file_writer>(
                    "logs/production.log",
                    100 * 1024 * 1024, // 100 MB per file
                    10                  // Keep 10 rotations
                ),
                critical_writer_config{
                    .force_flush_on_critical = true,
                    .force_flush_on_error = true,
                    .enable_signal_handlers = true,
                    .write_ahead_log = true,
                    .wal_path = "logs/.production.wal",
                    .sync_on_critical = true,
                    .critical_write_timeout_ms = 5000
                },
                50000 // Large async queue
            )
        )
        .build();

    if (!result) {
        std::cerr << "Failed to build logger: " << result.error_message() << "\n";
        return;
    }

    auto log = std::move(result.value());
    log->start();

    // Production logging examples
    log->log(ci::log_level::info, std::string("Service started"));
    log->log(ci::log_level::warning, std::string("Cache miss rate high"));
    log->log(ci::log_level::error, std::string("Database connection timeout"));
    log->log(ci::log_level::critical, std::string("Out of memory - terminating"));

    log->flush();
    log->stop();

    std::cout << "Production setup complete\n";
    std::cout << "Configuration:\n";
    std::cout << "  - Async logging for normal messages (performance)\n";
    std::cout << "  - Immediate flush for errors and critical (safety)\n";
    std::cout << "  - Write-ahead logging for crash recovery\n";
    std::cout << "  - Signal handlers for graceful shutdown\n";
    std::cout << "  - File rotation to manage disk space\n";
}

/**
 * Example 6: Error handling and statistics
 */
void example_error_handling() {
    std::cout << "\n=== Example 6: Error Handling & Statistics ===\n";

    logger log(false);

    auto critical = std::make_unique<critical_writer>(
        std::make_unique<file_writer>("logs/stats.log"),
        critical_writer_config{
            .force_flush_on_critical = true,
            .enable_signal_handlers = false, // Disable for this example
            .write_ahead_log = true,
            .wal_path = "logs/.stats.wal"
        }
    );

    // Store reference before moving
    const auto& config = critical->get_config();
    const auto& stats = critical->get_stats();

    log.add_writer(std::move(critical));

    // Generate logs
    log.log(ci::log_level::info, std::string("Info message"));
    log.log(ci::log_level::warning, std::string("Warning message"));
    log.log(ci::log_level::error, std::string("Error message"));
    log.log(ci::log_level::critical, std::string("Critical message 1"));
    log.log(ci::log_level::critical, std::string("Critical message 2"));
    log.log(ci::log_level::critical, std::string("Fatal message"));

    // Check statistics
    std::cout << "\nConfiguration:\n";
    std::cout << "  Force flush on critical: " << config.force_flush_on_critical << "\n";
    std::cout << "  Force flush on error: " << config.force_flush_on_error << "\n";
    std::cout << "  WAL enabled: " << config.write_ahead_log << "\n";
    std::cout << "  Sync on critical: " << config.sync_on_critical << "\n";

    std::cout << "\nStatistics:\n";
    std::cout << "  Total critical writes: " << stats.total_critical_writes.load() << "\n";
    std::cout << "  Total flushes: " << stats.total_flushes.load() << "\n";
    std::cout << "  WAL writes: " << stats.wal_writes.load() << "\n";
    std::cout << "  Sync calls: " << stats.sync_calls.load() << "\n";

    // Runtime configuration change
    std::cout << "\nChanging configuration at runtime...\n";
    // Note: set_force_flush_on_critical() would be called on the writer directly
    // This example shows the concept
}

int main() {
    std::cout << "Critical Logging Examples\n";
    std::cout << "=========================\n";

    try {
        example_basic_critical_writer();
        example_write_ahead_logging();
        example_hybrid_writer();
        example_signal_handler();
        example_production_setup();
        example_error_handling();

        std::cout << "\n=== All Examples Completed Successfully ===\n";
        std::cout << "\nCheck the logs/ directory for output files:\n";
        std::cout << "  - *.log: Main log files\n";
        std::cout << "  - .*.wal: Write-ahead log files\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
