/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file advanced_features_demo.cpp
 * @brief Demonstrates advanced logging features including filtering and writers
 *
 * This example showcases:
 * - Level-based filtering
 * - Regex-based filtering
 * - Composite filters with AND/OR logic
 * - Custom function filters
 * - Rotating file writers
 * - Multiple named writers
 *
 * @note Routing functionality is not yet implemented (see TODO in logger.h)
 */

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/rotating_file_writer.h>
#include <kcenon/logger/filters/log_filter.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/common/interfaces/logger_interface.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <filesystem>

using namespace kcenon::logger;
using namespace kcenon::logger::filters;
namespace ci = kcenon::common::interfaces;

int main() {
    std::cout << "=== Logger Advanced Features Demo ===" << std::endl;

    // Ensure logs directory exists
    std::filesystem::create_directories("logs");

    // Create logger instance
    auto logger = std::make_unique<kcenon::logger::logger>(true, 1024);

    // Add named writers
    logger->add_writer("console", std::make_unique<console_writer>());
    logger->add_writer("error_file", std::make_unique<file_writer>("logs/errors.log"));
    logger->add_writer("debug_file", std::make_unique<file_writer>("logs/debug.log"));
    logger->add_writer("rotating", std::make_unique<rotating_file_writer>(
        "logs/app.log",
        1024 * 1024,  // 1MB max size
        5             // Keep 5 backup files
    ));

    // Start the logger
    logger->start();

    std::cout << "\n1. Testing Basic Filtering (level >= warning):" << std::endl;

    // Set global filter to only log warnings and above
    // Note: level_filter still uses logger_system::log_level internally
    logger->set_filter(std::make_unique<level_filter>(log_level::warning));

    logger->log(ci::log_level::trace, std::string("This trace message should be filtered out"));
    logger->log(ci::log_level::debug, std::string("This debug message should be filtered out"));
    logger->log(ci::log_level::info, std::string("This info message should be filtered out"));
    logger->log(ci::log_level::warning, std::string("This warning should be logged"));
    logger->log(ci::log_level::error, std::string("This error should be logged"));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n2. Testing Regex Filtering (exclude 'sensitive'):" << std::endl;

    // Filter out messages containing "sensitive"
    logger->set_filter(std::make_unique<regex_filter>("sensitive", false));

    logger->log(ci::log_level::error, std::string("This contains sensitive data - should be filtered"));
    logger->log(ci::log_level::error, std::string("This is a normal error message - should be logged"));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n3. Testing Composite Filtering:" << std::endl;
    std::cout << "   (level >= warning) AND (not contains 'ignore')" << std::endl;

    // Create composite filter: (level >= warning) AND (not contains "ignore")
    auto composite = std::make_unique<composite_filter>(composite_filter::logic_type::AND);
    composite->add_filter(std::make_unique<level_filter>(log_level::warning));
    composite->add_filter(std::make_unique<regex_filter>("ignore", false));

    logger->set_filter(std::move(composite));

    logger->log(ci::log_level::info, std::string("Info: Should be filtered by level"));
    logger->log(ci::log_level::warning, std::string("Warning: Should be logged"));
    logger->log(ci::log_level::error, std::string("Error: Please ignore this - filtered by regex"));
    logger->log(ci::log_level::error, std::string("Error: Real error message - should be logged"));

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    std::cout << "\n4. Testing Rotating File Writer:" << std::endl;

    // Clear filter for rotating test
    logger->set_filter(nullptr);

    // Generate enough logs to trigger rotation
    std::cout << "   Writing 1000 log entries to trigger file rotation..." << std::endl;
    for (int i = 0; i < 1000; ++i) {
        std::string msg = "Log entry " + std::to_string(i) +
                         " - This is a longer message to fill up the file size quickly. "
                         "Adding more text to reach the rotation threshold faster.";
        logger->log(ci::log_level::info, msg);
    }

    std::cout << "\n5. Testing Custom Function Filter:" << std::endl;
    std::cout << "   (only log messages from main thread)" << std::endl;

    // Filter that only logs messages from specific threads
    auto main_thread_id = std::this_thread::get_id();
    auto thread_filter = std::make_unique<function_filter>(
        [main_thread_id](const log_entry& entry) {
            (void)entry;  // Filter based on calling thread, not entry content
            return std::this_thread::get_id() == main_thread_id;
        }
    );

    logger->set_filter(std::move(thread_filter));

    // Log from main thread
    logger->log(ci::log_level::info, std::string("Message from main thread - should be logged"));

    // Log from another thread
    std::thread other_thread([&logger]() {
        logger->log(ci::log_level::info, std::string("Message from other thread - should be filtered"));
    });
    other_thread.join();

    // Cleanup
    logger->flush();
    logger->stop();

    std::cout << "\n=== Demo Complete ===" << std::endl;
    std::cout << "Check the logs/ directory for output files:" << std::endl;
    std::cout << "- errors.log: Contains error messages" << std::endl;
    std::cout << "- debug.log: Contains debug level messages" << std::endl;
    std::cout << "- app.log*: Rotating log files" << std::endl;

    return 0;
}
