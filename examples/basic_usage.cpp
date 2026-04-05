// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file basic_usage.cpp
 * @brief Basic logger usage examples covering core logging functionality
 *
 * @example basic_usage.cpp
 * This example demonstrates fundamental logger_system features:
 * - Creating a logger and attaching a console writer
 * - Logging at every severity level (trace through critical)
 * - Multi-threaded logging with a shared async logger
 * - Log-level filtering to control output verbosity
 * - Synchronous vs asynchronous performance comparison
 */

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/common/interfaces/logger_interface.h>
namespace ci = kcenon::common::interfaces;
using log_level_type = ci::log_level;
#include <thread>
#include <vector>
#include <iostream>
#include <chrono>

using namespace kcenon::logger;
namespace logger_module = kcenon::logger;

void basic_logging_example() {
    std::cout << "\n=== Basic Logging Example ===\n" << std::endl;
    
    // Create logger instance
    auto logger_instance = std::make_shared<logger>();
    
    // Add console writer
    logger_instance->add_writer(std::make_unique<console_writer>());
    
    // Start logger (for async mode)
    logger_instance->start();
    
    // Log messages at different levels
    logger_instance->log(log_level_type::trace, std::string("This is a trace message"));
    logger_instance->log(log_level_type::debug, std::string("Debug information here"));
    logger_instance->log(log_level_type::info, std::string("Application started successfully"));
    logger_instance->log(log_level_type::warning, std::string("This is a warning"));
    logger_instance->log(log_level_type::error, std::string("An error occurred!"));
    logger_instance->log(log_level_type::critical, std::string("Critical system failure!"));

    // Log with simple message (source_location auto-captured internally)
    logger_instance->log(log_level_type::info, std::string("Message with auto-captured location"));
    
    // Stop and flush
    logger_instance->stop();
}

void multithreaded_logging_example() {
    std::cout << "\n=== Multithreaded Logging Example ===\n" << std::endl;
    
    // Create async logger
    auto logger_instance = std::make_shared<logger>(true, 16384); // Larger buffer for MT
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();
    
    // Launch multiple threads
    std::vector<std::thread> threads;
    
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back([logger_instance, i]() {
            for (int j = 0; j < 10; ++j) {
                logger_instance->log(log_level_type::info,
                           "Thread " + std::to_string(i) + " - Message " + std::to_string(j));
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        });
    }
    
    // Wait for all threads
    for (auto& t : threads) {
        t.join();
    }
    
    logger_instance->stop();
}

void log_level_filtering_example() {
    std::cout << "\n=== Log Level Filtering Example ===\n" << std::endl;
    
    auto logger_instance = std::make_shared<logger>();
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();
    
    // Set minimum level to INFO
    logger_instance->set_level(log_level_type::info);
    std::cout << "Minimum level set to INFO\n" << std::endl;

    // These won't be logged
    logger_instance->log(log_level_type::trace, std::string("This trace won't show"));
    logger_instance->log(log_level_type::debug, std::string("This debug won't show"));

    // These will be logged
    logger_instance->log(log_level_type::info, std::string("This info will show"));
    logger_instance->log(log_level_type::warning, std::string("This warning will show"));
    
    logger_instance->stop();
}

void sync_vs_async_example() {
    std::cout << "\n=== Sync vs Async Logging Example ===\n" << std::endl;
    
    // Synchronous logger
    std::cout << "Synchronous logging:" << std::endl;
    auto sync_logger = std::make_shared<logger>(false); // sync mode
    sync_logger->add_writer(std::make_unique<console_writer>());
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        sync_logger->log(log_level_type::info, "Sync log " + std::to_string(i));
    }
    auto sync_time = std::chrono::high_resolution_clock::now() - start;
    
    // Asynchronous logger
    std::cout << "\nAsynchronous logging:" << std::endl;
    auto async_logger = std::make_shared<logger>(true); // async mode
    async_logger->add_writer(std::make_unique<console_writer>());
    async_logger->start();
    
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 100; ++i) {
        async_logger->log(log_level_type::info, "Async log " + std::to_string(i));
    }
    auto async_time = std::chrono::high_resolution_clock::now() - start;
    
    async_logger->stop();
    
    std::cout << "\nSync time: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(sync_time).count() 
              << " µs" << std::endl;
    std::cout << "Async time: " 
              << std::chrono::duration_cast<std::chrono::microseconds>(async_time).count() 
              << " µs" << std::endl;
}

int main() {
    try {
        basic_logging_example();
        multithreaded_logging_example();
        log_level_filtering_example();
        sync_vs_async_example();
        
        std::cout << "\n=== All examples completed successfully! ===" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
