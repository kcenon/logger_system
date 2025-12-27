/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/interfaces/logger_types.h>
#include <thread>
#include <iostream>
#include <random>

using namespace kcenon::logger;
namespace logger_module = kcenon::logger;

void generate_logs(logger* log, int thread_id, int count) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> level_dist(0, 5);
    std::uniform_int_distribution<> message_size_dist(10, 200);
    
    for (int i = 0; i < count; ++i) {
        auto level = static_cast<kcenon::logger::log_level>(level_dist(gen));

        // Generate message of random size
        std::string message = "Thread " + std::to_string(thread_id) + " - Message " + std::to_string(i);
        message.append(message_size_dist(gen), 'x');

        log->log(level, message);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void print_metrics(const logger_metrics& metrics) {
    std::cout << "\n=== Logger Performance Metrics ===" << std::endl;
    std::cout << "Messages logged: " << metrics.messages_logged.load() << std::endl;
    std::cout << "Messages dropped: " << metrics.messages_dropped.load() << std::endl;
    std::cout << "Total log time: " << metrics.total_log_time_ns.load() / 1000000.0 << " ms" << std::endl;
    std::cout << "Throughput: " << metrics.get_messages_per_second() << " msg/s" << std::endl;
    std::cout << "Queue utilization: " << metrics.get_queue_utilization_percent() << "%" << std::endl;
    std::cout << "Avg enqueue time: " << metrics.get_avg_enqueue_time_ns() << " ns" << std::endl;
    std::cout << "Writer errors: " << metrics.writer_errors.load() << std::endl;
    std::cout << "Flush operations: " << metrics.flush_operations.load() << std::endl;
}

// Note: Structured logging functionality has been simplified in this version
void test_structured_logging() {
    std::cout << "\n=== Testing Basic Logging ===" << std::endl;

    auto base_logger = std::make_shared<logger>(false); // Sync mode for testing
    base_logger->add_writer(std::make_unique<console_writer>());
    base_logger->start();

    // Test basic logging at different levels
    base_logger->log(kcenon::logger::log_level::info, "User logged in - user_id: 12345, ip: 192.168.1.100");
    base_logger->log(kcenon::logger::log_level::error, "Database connection failed - host: db.example.com, port: 5432");
    base_logger->log(kcenon::logger::log_level::debug, "Retry attempt 3 of 5");

    base_logger->stop();
}

int main() {
    // Create logger with metrics enabled
    auto logger = std::make_unique<logger_module::logger>(true, 1024); // Small buffer to test dropping
    
    // Add console writer
    logger->add_writer(std::make_unique<console_writer>());
    
    // Enable metrics collection
    auto enable_result = logger->enable_metrics_collection(true);
    if (enable_result.is_err()) {
        std::cerr << "Failed to enable metrics collection" << std::endl;
    }
    
    // Start logger
    logger->start();
    
    std::cout << "Starting logger metrics demo..." << std::endl;
    std::cout << "Generating logs from multiple threads..." << std::endl;
    
    // Launch multiple threads to generate logs
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(generate_logs, logger.get(), i, 100);
    }
    
    // Wait for threads to complete
    for (auto& t : threads) {
        t.join();
    }
    
    // Wait a bit for processing
    std::this_thread::sleep_for(std::chrono::seconds(1));
    
    // Get and display metrics
    auto metrics_result = logger->get_current_metrics();
    if (metrics_result.has_value()) {
        print_metrics(metrics_result.value());
    } else {
        std::cerr << "Failed to get metrics" << std::endl;
    }
    
    // Test basic logging functionality
    test_structured_logging();
    
    // Stop logger
    logger->stop();
    
    std::cout << "\nDemo completed!" << std::endl;
    
    return 0;
}