/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <kcenon/logger/core/logger_config_builder.h>
#include <iostream>

using namespace kcenon::logger;

int main() {
    // Example 1: Use production defaults with custom settings
    auto config1_result = logger_config_builder()
        .use_production_defaults()
        .set_buffer_size(32768)
        .enable_metrics(false)
        .set_log_directory("./production_logs")
        .build();

    if (config1_result.is_ok()) {
        std::cout << "Production config created successfully" << std::endl;
        std::cout << "  Buffer size: " << config1_result.value().buffer_size << std::endl;
        std::cout << "  Log directory: " << config1_result.value().log_directory << std::endl;
    }

    // Example 2: Debug configuration
    auto config2_result = logger_config_builder()
        .use_debug_defaults()
        .enable_color_output(true)
        .enable_source_location(true)
        .build();

    if (config2_result.is_ok()) {
        std::cout << "\nDebug config created successfully" << std::endl;
        std::cout << "  Async: " << config2_result.value().async << std::endl;
        std::cout << "  Source location: " << config2_result.value().enable_source_location << std::endl;
    }

    // Example 3: Custom configuration from scratch
    auto config3_result = logger_config_builder()
        .set_async(true)
        .set_buffer_size(16384)
        .set_batch_size(100)
        .set_flush_interval(std::chrono::milliseconds(500))
        .enable_metrics(true)
        .enable_timestamp(true)
        .set_max_queue_size(50000)
        .set_overflow_policy(logger_config::overflow_policy::drop_oldest)
        .set_log_directory("./custom_logs")
        .set_log_file_prefix("myapp")
        .build();

    if (config3_result.is_ok()) {
        std::cout << "\nCustom config created successfully" << std::endl;
        std::cout << "  Buffer size: " << config3_result.value().buffer_size << std::endl;
        std::cout << "  Batch size: " << config3_result.value().batch_size << std::endl;
        std::cout << "  Max queue size: " << config3_result.value().max_queue_size << std::endl;
    }

    // Example 4: High-performance with network logging
    auto config4_result = logger_config_builder()
        .use_high_performance_defaults()
        .set_remote_logging("log-server.example.com", 5140)
        .set_network_timeout(std::chrono::milliseconds(3000))
        .set_network_retry_count(5)
        .build();

    if (config4_result.is_ok()) {
        std::cout << "\nHigh-performance network config created successfully" << std::endl;
        std::cout << "  Remote host: " << config4_result.value().remote_host << std::endl;
        std::cout << "  Remote port: " << config4_result.value().remote_port << std::endl;
    }

    // Example 5: Invalid configuration (will fail validation)
    auto config5_result = logger_config_builder()
        .set_buffer_size(0)  // Invalid: buffer size must be > 0
        .build();

    if (!config5_result.is_ok()) {
        std::cout << "\nValidation failed as expected:" << std::endl;
        std::cout << "  Error: " << config5_result.error().message << std::endl;
    }

    return 0;
}
