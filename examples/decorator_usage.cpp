/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

/**
 * @file decorator_usage.cpp
 * @brief Comprehensive decorator pattern usage examples
 * @author kcenon
 * @since 4.1.0
 *
 * This example demonstrates the complete decorator pattern functionality
 * in logger_system, including:
 * - All available decorators (async, buffered, encrypted, thread_safe, filtered)
 * - Decorator application order and rationale
 * - Performance implications of different compositions
 * - Real-world production scenarios
 *
 * Part of EPIC #391 - Decorator pattern refactoring
 * Related to Issue #421 - Documentation and examples
 */

#include <kcenon/logger/builders/writer_builder.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/interfaces/log_filter_interface.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>

#ifdef LOGGER_WITH_ENCRYPTION
#include <kcenon/logger/security/secure_key.h>
#include <kcenon/logger/security/secure_key_storage.h>
#endif

#include <iostream>
#include <memory>
#include <thread>
#include <chrono>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

/**
 * @brief Custom filter that only allows specific log levels
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
 * @brief Custom filter that filters by message content
 */
class content_filter : public log_filter_interface {
public:
    explicit content_filter(std::string keyword) : keyword_(std::move(keyword)) {}

    bool should_log(const log_entry& entry) const override {
        std::string msg(entry.message.data(), entry.message.size());
        return msg.find(keyword_) != std::string::npos;
    }

    std::string get_name() const override {
        return "content_filter";
    }

private:
    std::string keyword_;
};

void print_section(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << "\n";
    std::cout << title << "\n";
    std::cout << std::string(60, '=') << "\n";
}

/**
 * @brief Example 1: Core Writers (No Decorators)
 *
 * Demonstrates the base writers without any decorators.
 * These are the foundation that decorators wrap.
 */
void example_core_writers() {
    print_section("Example 1: Core Writers (No Decorators)");

    std::cout << "\n1.1 File Writer:\n";
    auto file_writer = writer_builder()
        .file("basic.log")
        .build();
    std::cout << "  Created: " << file_writer->get_name() << "\n";
    std::cout << "  Use case: Direct file output, simplest form\n";

    std::cout << "\n1.2 Console Writer:\n";
    auto console_writer = writer_builder()
        .console()
        .build();
    std::cout << "  Created: " << console_writer->get_name() << "\n";
    std::cout << "  Use case: Development, debugging, immediate visual feedback\n";

    std::cout << "\nKey Points:\n";
    std::cout << "  • Core writers provide the actual I/O functionality\n";
    std::cout << "  • No buffering, no async - synchronous and immediate\n";
    std::cout << "  • Best for: Simple use cases, testing, development\n";
}

/**
 * @brief Example 2: Single Decorator Usage
 *
 * Shows how each decorator works independently.
 */
void example_single_decorators() {
    print_section("Example 2: Single Decorator Usage");

    std::cout << "\n2.1 Async Writer (Performance):\n";
    auto async_log_writer = writer_builder()
        .file("async.log")
        .async(10000)  // Queue size: 10000 entries
        .build();
    std::cout << "  Created: " << async_log_writer->get_name() << "\n";
    std::cout << "  Benefit: Non-blocking writes, background thread processing\n";
    std::cout << "  Trade-off: Slight delay in writing, memory for queue\n";
    std::cout << "  Best for: High-throughput applications\n";

    // Start and stop async writer
    if (auto* async_w = dynamic_cast<async_writer*>(async_log_writer.get())) {
        async_w->start();
        std::cout << "  Status: Started background thread\n";
        async_w->stop();
    }

    std::cout << "\n2.2 Buffered Writer (I/O Efficiency):\n";
    auto buffered_writer = writer_builder()
        .file("buffered.log")
        .buffered(100)  // Buffer up to 100 entries
        .build();
    std::cout << "  Created: " << buffered_writer->get_name() << "\n";
    std::cout << "  Benefit: Reduces I/O operations by batching\n";
    std::cout << "  Trade-off: Logs may be delayed until buffer fills\n";
    std::cout << "  Best for: Moderate throughput, reducing disk I/O\n";

    std::cout << "\n2.3 Filtered Writer (Selective Logging):\n";
    auto filter = std::make_unique<level_filter>(log_level::warning);
    auto filtered_writer = writer_builder()
        .file("warnings.log")
        .filtered(std::move(filter))
        .build();
    std::cout << "  Created: " << filtered_writer->get_name() << "\n";
    std::cout << "  Benefit: Only writes logs meeting criteria (e.g., WARNING+)\n";
    std::cout << "  Trade-off: Filtering overhead (usually negligible)\n";
    std::cout << "  Best for: Separate error logs, compliance logging\n";

#ifdef LOGGER_WITH_ENCRYPTION
    std::cout << "\n2.4 Encrypted Writer (Security):\n";
    using namespace kcenon::logger::security;
    auto key_result = secure_key_storage::generate_key(32);
    if (key_result.is_ok()) {
        auto encrypted_writer = writer_builder()
            .file("secure.log.enc")
            .encrypted(std::move(key_result.value()))
            .build();
        std::cout << "  Created: " << encrypted_writer->get_name() << "\n";
        std::cout << "  Benefit: AES-256-GCM encryption for sensitive data\n";
        std::cout << "  Trade-off: CPU overhead for encryption\n";
        std::cout << "  Best for: Compliance (GDPR, PCI DSS), sensitive logs\n";
    }
#else
    std::cout << "\n2.4 Encrypted Writer: Skipped (encryption not available)\n";
#endif

    std::cout << "\nKey Points:\n";
    std::cout << "  • Each decorator adds specific functionality\n";
    std::cout << "  • Single decorators are easier to reason about\n";
    std::cout << "  • Combine multiple decorators for complex scenarios\n";
}

/**
 * @brief Example 3: Multiple Decorator Composition
 *
 * Demonstrates combining multiple decorators.
 * ORDER MATTERS: Core → Filter → Buffer → Encrypt → Thread-Safe → Async
 */
void example_multiple_decorators() {
    print_section("Example 3: Multiple Decorator Composition");

    std::cout << "\n3.1 Buffered + Async (Common Pattern):\n";
    auto buffered_async = writer_builder()
        .file("app.log")
        .buffered(500)     // Buffer first
        .async(20000)      // Then async
        .build();
    std::cout << "  Created: " << buffered_async->get_name() << "\n";
    std::cout << "  Order: file → buffered(500) → async(20000)\n";
    std::cout << "  Rationale:\n";
    std::cout << "    1. Buffering reduces I/O operations\n";
    std::cout << "    2. Async provides non-blocking writes\n";
    std::cout << "  Best for: High-performance production applications\n";

    if (auto* async_w = dynamic_cast<async_writer*>(buffered_async.get())) {
        async_w->start();
        async_w->stop();
    }

    std::cout << "\n3.2 Filtered + Buffered + Async (Error Log):\n";
    auto error_filter = std::make_unique<level_filter>(log_level::error);
    auto filtered_buffered_async = writer_builder()
        .file("errors.log")
        .filtered(std::move(error_filter))  // Filter first
        .buffered(100)                      // Then buffer
        .async(5000)                        // Then async
        .build();
    std::cout << "  Created: " << filtered_buffered_async->get_name() << "\n";
    std::cout << "  Order: file → filtered → buffered(100) → async(5000)\n";
    std::cout << "  Rationale:\n";
    std::cout << "    1. Filter early to reduce processing\n";
    std::cout << "    2. Buffer to batch error logs\n";
    std::cout << "    3. Async for non-blocking error logging\n";
    std::cout << "  Best for: Separate error logs with filtering\n";

    if (auto* async_w = dynamic_cast<async_writer*>(filtered_buffered_async.get())) {
        async_w->start();
        async_w->stop();
    }

#ifdef LOGGER_WITH_ENCRYPTION
    std::cout << "\n3.3 Buffered + Encrypted + Async (Secure High-Performance):\n";
    using namespace kcenon::logger::security;
    auto key_result = secure_key_storage::generate_key(32);
    if (key_result.is_ok()) {
        auto secure_writer = writer_builder()
            .file("secure.log.enc")
            .buffered(200)                           // Buffer first
            .encrypted(std::move(key_result.value())) // Then encrypt
            .async(10000)                            // Then async
            .build();
        std::cout << "  Created: " << secure_writer->get_name() << "\n";
        std::cout << "  Order: file → buffered(200) → encrypted → async(10000)\n";
        std::cout << "  Rationale:\n";
        std::cout << "    1. Buffer to reduce encryption overhead\n";
        std::cout << "    2. Encrypt for security (batches are encrypted together)\n";
        std::cout << "    3. Async to prevent encryption from blocking\n";
        std::cout << "  Best for: Secure, high-performance logging\n";

        if (auto* async_w = dynamic_cast<async_writer*>(secure_writer.get())) {
            async_w->start();
            async_w->stop();
        }
    }
#else
    std::cout << "\n3.3 Encrypted combination: Skipped (encryption not available)\n";
#endif

    std::cout << "\nDecorator Order Principle:\n";
    std::cout << "  Core Writer → Filtering → Buffering → Encryption → Thread-Safety → Async\n";
    std::cout << "\nWhy this order?\n";
    std::cout << "  1. Filter early: Reduce work for downstream decorators\n";
    std::cout << "  2. Buffer before encrypt: Amortize encryption cost\n";
    std::cout << "  3. Thread-safe before async: Ensure consistency\n";
    std::cout << "  4. Async outermost: Maximize non-blocking benefits\n";
}

/**
 * @brief Example 4: Performance Comparison
 *
 * Demonstrates performance characteristics of different decorator combinations.
 */
void example_performance_patterns() {
    print_section("Example 4: Performance Patterns");

    std::cout << "\n4.1 High-Throughput Pattern:\n";
    std::cout << "  Configuration: file → buffered(1000) → async(50000)\n";
    std::cout << "  Throughput: ~4M messages/second (single thread)\n";
    std::cout << "  Latency: ~148ns average\n";
    std::cout << "  Use case: Logging-heavy applications, analytics\n";

    std::cout << "\n4.2 Low-Latency Pattern:\n";
    std::cout << "  Configuration: file → async(small_queue)\n";
    std::cout << "  Throughput: Lower than buffered\n";
    std::cout << "  Latency: Minimal buffering delay\n";
    std::cout << "  Use case: Real-time systems, trading platforms\n";

    std::cout << "\n4.3 Balanced Pattern:\n";
    std::cout << "  Configuration: file → buffered(500) → async(20000)\n";
    std::cout << "  Throughput: Good (millions of messages/second)\n";
    std::cout << "  Latency: Acceptable for most applications\n";
    std::cout << "  Use case: General production applications\n";

    std::cout << "\n4.4 Security-First Pattern:\n";
    std::cout << "  Configuration: file → filtered → encrypted → buffered → async\n";
    std::cout << "  Throughput: Moderate (encryption overhead)\n";
    std::cout << "  Latency: Higher due to encryption\n";
    std::cout << "  Use case: Compliance-critical applications\n";

    std::cout << "\nPerformance Tips:\n";
    std::cout << "  • Larger buffers = fewer I/O ops but more memory\n";
    std::cout << "  • Larger async queues = better burst handling\n";
    std::cout << "  • Encryption adds ~10-20% CPU overhead\n";
    std::cout << "  • Filtering early reduces downstream processing\n";
}

/**
 * @brief Example 5: Real-World Production Scenarios
 *
 * Practical examples from production use cases.
 */
void example_production_scenarios() {
    print_section("Example 5: Production Scenarios");

    logger log;

    std::cout << "\n5.1 Web Application Logging:\n";
    std::cout << "  Requirements: High throughput, separate error logs, async\n";

    // Main application log
    auto main_log = writer_builder()
        .file("app.log")
        .buffered(500)
        .async(20000)
        .build();
    std::cout << "  Main log: " << main_log->get_name() << "\n";

    if (auto* async_w = dynamic_cast<async_writer*>(main_log.get())) {
        async_w->start();
    }
    log.add_writer("main", std::move(main_log));

    // Error log (separate file, errors only)
    auto error_filter = std::make_unique<level_filter>(log_level::error);
    auto error_log = writer_builder()
        .file("errors.log")
        .filtered(std::move(error_filter))
        .async()
        .build();
    std::cout << "  Error log: " << error_log->get_name() << "\n";

    if (auto* async_w = dynamic_cast<async_writer*>(error_log.get())) {
        async_w->start();
    }
    log.add_writer("errors", std::move(error_log));

    // Console for development
    auto console = writer_builder()
        .console()
        .build();
    std::cout << "  Console: " << console->get_name() << "\n";
    log.add_writer("console", std::move(console));

    std::cout << "\n5.2 Microservice with Observability:\n";
    std::cout << "  Requirements: Structured logs, filtering, async\n";
    // In real scenario, would use structured logging and OTLP writer
    std::cout << "  Pattern: file → filtered → buffered → async\n";
    std::cout << "  Additional: OTLP writer for OpenTelemetry export\n";

#ifdef LOGGER_WITH_ENCRYPTION
    std::cout << "\n5.3 Healthcare/Financial Application:\n";
    std::cout << "  Requirements: HIPAA/PCI compliance, encryption, audit trail\n";
    using namespace kcenon::logger::security;
    auto key_result = secure_key_storage::generate_key(32);
    if (key_result.is_ok()) {
        auto secure_log = writer_builder()
            .file("audit.log.enc")
            .buffered(100)
            .encrypted(std::move(key_result.value()))
            .async()
            .build();
        std::cout << "  Audit log: " << secure_log->get_name() << "\n";
        std::cout << "  Pattern: file → buffered → encrypted → async\n";

        if (auto* async_w = dynamic_cast<async_writer*>(secure_log.get())) {
            async_w->start();
        }
        log.add_writer("audit", std::move(secure_log));
    }
#else
    std::cout << "\n5.3 Secure logging: Skipped (encryption not available)\n";
#endif

    // Log some messages
    std::cout << "\nTesting the setup:\n";
    log.log(log_level::info, std::string("User logged in"));
    log.log(log_level::warning, std::string("Session expiring soon"));
    log.log(log_level::error, std::string("Payment processing failed"));

    std::cout << "  INFO: → main log, console\n";
    std::cout << "  WARNING: → main log, console\n";
    std::cout << "  ERROR: → main log, errors log, console\n";

    log.flush();
    std::cout << "\nProduction Best Practices:\n";
    std::cout << "  1. Always use async for production\n";
    std::cout << "  2. Separate error logs for quick triage\n";
    std::cout << "  3. Buffer sizes: 100-1000 entries\n";
    std::cout << "  4. Async queue: 10000-50000 entries\n";
    std::cout << "  5. Monitor queue utilization\n";
    std::cout << "  6. Always call flush() on shutdown\n";
}

/**
 * @brief Example 6: Custom Filtering Patterns
 *
 * Shows advanced filtering scenarios.
 */
void example_custom_filtering() {
    print_section("Example 6: Custom Filtering Patterns");

    std::cout << "\n6.1 Content-Based Filtering:\n";
    auto content_filter_ptr = std::make_unique<content_filter>("database");
    auto db_log = writer_builder()
        .file("database_events.log")
        .filtered(std::move(content_filter_ptr))
        .buffered(50)
        .async()
        .build();
    std::cout << "  Created: " << db_log->get_name() << "\n";
    std::cout << "  Filter: Only logs containing 'database'\n";
    std::cout << "  Use case: Component-specific logging\n";

    if (auto* async_w = dynamic_cast<async_writer*>(db_log.get())) {
        async_w->start();
        async_w->stop();
    }

    std::cout << "\n6.2 Level-Based Filtering:\n";
    std::cout << "  Critical errors only:\n";
    auto critical_filter = std::make_unique<level_filter>(log_level::critical);
    auto critical_log = writer_builder()
        .file("critical.log")
        .filtered(std::move(critical_filter))
        .build();
    std::cout << "    Created: " << critical_log->get_name() << "\n";

    std::cout << "\nFiltering Strategies:\n";
    std::cout << "  • By level: Common, efficient\n";
    std::cout << "  • By content: Flexible, component-specific\n";
    std::cout << "  • By custom logic: Implement log_filter_interface\n";
    std::cout << "  • Composite filters: Combine multiple filters (AND/OR)\n";
}

/**
 * @brief Example 7: Migration from Manual Nesting
 *
 * Shows how to migrate from deprecated manual decorator nesting.
 */
void example_migration_patterns() {
    print_section("Example 7: Migration from Manual Nesting");

    std::cout << "\nOLD (Deprecated - Manual Nesting):\n";
    std::cout << "  auto writer = std::make_unique<async_writer>(\n";
    std::cout << "      std::make_unique<buffered_writer>(\n";
    std::cout << "          std::make_unique<file_writer>(\"app.log\"),\n";
    std::cout << "          500),\n";
    std::cout << "      20000);\n";
    std::cout << "\nProblems:\n";
    std::cout << "  ✗ Verbose and error-prone\n";
    std::cout << "  ✗ Nesting order is unclear\n";
    std::cout << "  ✗ Hard to modify (add/remove decorators)\n";

    std::cout << "\nNEW (Recommended - Writer Builder):\n";
    std::cout << "  auto writer = writer_builder()\n";
    std::cout << "      .file(\"app.log\")\n";
    std::cout << "      .buffered(500)\n";
    std::cout << "      .async(20000)\n";
    std::cout << "      .build();\n";
    std::cout << "\nBenefits:\n";
    std::cout << "  ✓ Clear, self-documenting\n";
    std::cout << "  ✓ Type-safe at compile time\n";
    std::cout << "  ✓ Easy to modify\n";
    std::cout << "  ✓ Follows decorator order convention\n";

    auto writer = writer_builder()
        .file("migration_example.log")
        .buffered(500)
        .async(20000)
        .build();

    std::cout << "\nResult: " << writer->get_name() << "\n";

    if (auto* async_w = dynamic_cast<async_writer*>(writer.get())) {
        async_w->start();
        async_w->stop();
    }

    std::cout << "\nMigration Steps:\n";
    std::cout << "  1. Identify manual decorator nesting in your code\n";
    std::cout << "  2. Replace with writer_builder() calls\n";
    std::cout << "  3. Test thoroughly (behavior should be identical)\n";
    std::cout << "  4. Enjoy improved readability and maintainability\n";
}

int main() {
    std::cout << "Comprehensive Decorator Pattern Usage Examples\n";
    std::cout << std::string(60, '=') << "\n";
    std::cout << "\nThis example covers all decorator functionality:\n";
    std::cout << "  • Core writers (file, console)\n";
    std::cout << "  • All decorators (async, buffered, filtered, encrypted)\n";
    std::cout << "  • Decorator composition and order\n";
    std::cout << "  • Performance patterns and trade-offs\n";
    std::cout << "  • Real-world production scenarios\n";
    std::cout << "  • Migration from manual nesting\n";

    try {
        example_core_writers();
        example_single_decorators();
        example_multiple_decorators();
        example_performance_patterns();
        example_production_scenarios();
        example_custom_filtering();
        example_migration_patterns();

        print_section("Summary: Decorator Order Convention");
        std::cout << "\nRecommended Order (innermost to outermost):\n";
        std::cout << "  1. Core Writer (file/console) - The actual I/O\n";
        std::cout << "  2. Filtering - Reduce work early\n";
        std::cout << "  3. Buffering - Batch for efficiency\n";
        std::cout << "  4. Encryption - Encrypt batches\n";
        std::cout << "  5. Thread-Safety - Ensure consistency\n";
        std::cout << "  6. Async - Outermost for maximum non-blocking benefit\n";

        std::cout << "\nQuick Reference:\n";
        std::cout << "  • High throughput: .buffered(1000).async(50000)\n";
        std::cout << "  • Low latency: .async(small_queue)\n";
        std::cout << "  • Secure: .buffered().encrypted().async()\n";
        std::cout << "  • Filtered: .filtered(filter).buffered().async()\n";

        print_section("All Examples Completed Successfully");
        std::cout << "\nFor more details, see:\n";
        std::cout << "  • README.md - Quick start and examples\n";
        std::cout << "  • docs/guides/DECORATOR_MIGRATION.md - Migration guide\n";
        std::cout << "  • examples/writer_builder_example.cpp - Builder examples\n";

    } catch (const std::exception& e) {
        std::cerr << "\nError: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
