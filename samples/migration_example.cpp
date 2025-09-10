/**
 * @file migration_example.cpp
 * @brief Example demonstrating migration from Logger System v1 to v2
 * 
 * This example shows how to migrate from the old v1 API to the new v2 API,
 * including the use of compatibility wrappers for gradual migration.
 */

#include <iostream>
#include <memory>
#include <chrono>
#include <thread>

// New v2 API headers
#include "../sources/logger/logger.h"
#include "../sources/logger/config/logger_builder.h"
#include "../sources/logger/writers/console_writer.h"
#include "../sources/logger/writers/file_writer.h"

// Compatibility header for v1 API
#include "../sources/logger/compatibility/logger_v1_compat.h"

using namespace logger_module;
using namespace std::chrono_literals;

/**
 * @brief Example of legacy v1 code using compatibility wrapper
 */
void legacy_v1_code_example() {
    std::cout << "\n=== Legacy v1 Code (using compatibility wrapper) ===\n";
    
    // Using v1 compatibility namespace
    using namespace logger_module::v1_compat;
    
    // Old-style logger creation (deprecated)
    auto* logger = create_logger("legacy_app");
    
    // Old-style logging with format strings (deprecated)
    LOG_INFO(logger, "Application started at %s", "2025-09-10");
    LOG_DEBUG(logger, "Debug value: %d", 42);
    LOG_ERROR(logger, "Error code: %d, message: %s", 404, "Not found");
    
    // Old-style writer addition (deprecated)
    add_file_writer(logger, "legacy.log");
    
    // Old-style configuration (deprecated)
    set_level(logger, log_level::debug);  // This will show deprecation warning
    
    std::cout << "Note: Deprecation warnings indicate v1 API usage\n";
}

/**
 * @brief Example of modern v2 code
 */
void modern_v2_code_example() {
    std::cout << "\n=== Modern v2 Code ===\n";
    
    // Modern logger creation with builder pattern
    auto logger = logger_builder()
        .with_name("modern_app")
        .with_min_level(log_level::debug)
        .with_console_writer()
        .with_file_writer("modern.log")
        .with_rotation(10 * 1024 * 1024)  // 10MB rotation
        .with_colored_output(true)
        .build();
    
    // Modern structured logging
    logger->info("Application started", {
        {"timestamp", "2025-09-10"},
        {"version", "2.0.0"}
    });
    
    logger->debug("Debug information", {
        {"value", 42},
        {"type", "answer"}
    });
    
    logger->error("Request failed", {
        {"error_code", 404},
        {"message", "Not found"},
        {"path", "/api/resource"}
    });
    
    // Using modern features
    logger->info("Performance metrics", {
        {"latency_ms", 45.2},
        {"throughput", 1000},
        {"cpu_usage", 0.65}
    });
}

/**
 * @brief Side-by-side comparison of v1 and v2 APIs
 */
void api_comparison_example() {
    std::cout << "\n=== API Comparison: v1 vs v2 ===\n";
    
    // ========== Logger Creation ==========
    std::cout << "\nLogger Creation:\n";
    
    // v1 style (deprecated)
    {
        using namespace logger_module::v1_compat;
        auto* v1_logger = create_file_logger("app_v1.log");
        LOG_INFO(v1_logger, "Created with v1 API");
    }
    
    // v2 style
    {
        auto v2_logger = logger_builder()
            .with_file_writer("app_v2.log")
            .build();
        v2_logger->info("Created with v2 API");
    }
    
    // ========== Configuration ==========
    std::cout << "\nConfiguration:\n";
    
    // v1 style (deprecated)
    {
        using namespace logger_module::v1_compat;
        logger_config_v1 config;
        config.level = log_level::warning;
        config.async = true;
        config.buffer_size = 10000;
        auto v1_logger = config.create();
        LOG_WARNING(v1_logger.get(), "Configured with v1 style");
    }
    
    // v2 style
    {
        auto v2_logger = logger_builder()
            .with_min_level(log_level::warning)
            .with_async(true)
            .with_buffer_size(10000)
            .build();
        v2_logger->warning("Configured with v2 style", {});
    }
    
    // ========== Logging ==========
    std::cout << "\nLogging:\n";
    
    auto logger = logger_builder().with_console_writer().build();
    
    // v1 style with macros (deprecated)
    {
        using namespace logger_module::v1_compat;
        LOG_INFO(logger.get(), "User %s logged in from %s", "john", "192.168.1.1");
    }
    
    // v2 style with structured fields
    {
        logger->info("User logged in", {
            {"username", "john"},
            {"ip_address", "192.168.1.1"},
            {"session_id", "abc123"}
        });
    }
}

/**
 * @brief Example of gradual migration strategy
 */
class GradualMigrationExample {
private:
    std::shared_ptr<logger> logger_;
    
public:
    GradualMigrationExample() {
        // Start with v2 logger creation
        logger_ = logger_builder()
            .with_name("migration_app")
            .with_console_writer()
            .with_file_writer("migration.log")
            .build();
    }
    
    // Phase 1: Keep old interface, use new implementation
    void old_interface_method() {
        // Can still use v1 macros during transition
        using namespace logger_module::v1_compat;
        LOG_INFO(logger_.get(), "Using old interface with new logger");
    }
    
    // Phase 2: New interface with compatibility
    void transitional_method() {
        // Mix old and new styles during transition
        logger_->info("Transitional phase");
        
        // Can still handle old-style parameters
        auto message = v1_compat::format_string("Value: %d", 42);
        logger_->info(message);
    }
    
    // Phase 3: Fully migrated to v2
    void new_interface_method() {
        // Pure v2 API usage
        logger_->info("Fully migrated to v2", {
            {"phase", "complete"},
            {"api_version", "2.0"}
        });
    }
    
    void demonstrate_migration() {
        std::cout << "\n=== Gradual Migration Strategy ===\n";
        
        std::cout << "Phase 1 - Using compatibility wrapper:\n";
        old_interface_method();
        
        std::cout << "\nPhase 2 - Transitional (mixed API):\n";
        transitional_method();
        
        std::cout << "\nPhase 3 - Fully migrated:\n";
        new_interface_method();
    }
};

/**
 * @brief Performance comparison between v1 and v2
 */
void performance_comparison() {
    std::cout << "\n=== Performance Comparison ===\n";
    
    const int iterations = 10000;
    
    // v1 style performance (using compatibility)
    {
        using namespace logger_module::v1_compat;
        auto* v1_logger = create_file_logger("perf_v1.log");
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            LOG_INFO(v1_logger, "Message %d with value %f", i, 3.14159);
        }
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "v1 API (compatibility): " << duration.count() << "ms\n";
    }
    
    // v2 style performance
    {
        auto v2_logger = logger_builder()
            .with_file_writer("perf_v2.log")
            .with_async(true)
            .with_batch_writing(true)
            .build();
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            v2_logger->info("Message", {
                {"index", i},
                {"value", 3.14159}
            });
        }
        v2_logger->flush();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "v2 API (native): " << duration.count() << "ms\n";
    }
    
    // v2 with optimization
    {
        auto v2_optimized = logger_builder()
            .with_file_writer("perf_v2_opt.log")
            .with_async(true)
            .with_batch_writing(true)
            .with_batch_size(1000)
            .with_queue_size(100000)
            .build();
        
        auto start = std::chrono::high_resolution_clock::now();
        for (int i = 0; i < iterations; ++i) {
            v2_optimized->info("Message", {
                {"index", i},
                {"value", 3.14159}
            });
        }
        v2_optimized->flush();
        auto end = std::chrono::high_resolution_clock::now();
        
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "v2 API (optimized): " << duration.count() << "ms\n";
    }
}

/**
 * @brief Common migration pitfalls and solutions
 */
void migration_pitfalls_example() {
    std::cout << "\n=== Common Migration Pitfalls ===\n";
    
    // Pitfall 1: Memory management
    std::cout << "\n1. Memory Management:\n";
    {
        // Wrong: Raw pointer (v1 style)
        // auto* logger = new logger();  // Memory leak risk
        
        // Right: Smart pointer (v2 style)
        auto logger = logger_builder().build();  // RAII
        logger->info("Proper memory management with RAII");
    }
    
    // Pitfall 2: Error handling
    std::cout << "\n2. Error Handling:\n";
    {
        auto logger = logger_builder().build();
        
        // Wrong: Ignoring errors
        // logger->add_writer(std::make_unique<file_writer>("/invalid/path"));
        
        // Right: Checking results
        auto result = logger->add_writer(
            std::make_unique<file_writer>("valid.log")
        );
        if (!result) {
            std::cerr << "Failed to add writer: " 
                     << result.error().message() << "\n";
        }
    }
    
    // Pitfall 3: Configuration immutability
    std::cout << "\n3. Configuration Immutability:\n";
    {
        auto logger = logger_builder()
            .with_min_level(log_level::info)
            .build();
        
        // Wrong: Trying to modify after creation
        // logger->set_min_level(log_level::debug);  // Not supported in v2
        
        // Right: Recreate with new configuration
        logger = logger_builder()
            .with_min_level(log_level::debug)
            .build();
        logger->debug("Logger recreated with new configuration");
    }
    
    // Pitfall 4: Thread safety
    std::cout << "\n4. Thread Safety:\n";
    {
        auto logger = logger_builder()
            .with_console_writer()
            .build();
        
        // v2 loggers are thread-safe by default
        std::vector<std::thread> threads;
        for (int i = 0; i < 5; ++i) {
            threads.emplace_back([&logger, i]() {
                logger->info("Thread message", {{"thread_id", i}});
            });
        }
        
        for (auto& t : threads) {
            t.join();
        }
    }
}

int main() {
    std::cout << "===================================\n";
    std::cout << "Logger System Migration Examples\n";
    std::cout << "===================================\n";
    
    // Show different migration approaches
    legacy_v1_code_example();
    modern_v2_code_example();
    api_comparison_example();
    
    // Demonstrate gradual migration
    GradualMigrationExample migration;
    migration.demonstrate_migration();
    
    // Performance comparison
    performance_comparison();
    
    // Common pitfalls
    migration_pitfalls_example();
    
    std::cout << "\n=== Migration Complete ===\n";
    std::cout << "See MIGRATION_GUIDE.md for detailed migration instructions.\n";
    
    return 0;
}