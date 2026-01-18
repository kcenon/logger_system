/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
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
 * @file structured_logging_example.cpp
 * @brief Demonstrates structured logging features introduced in Phase 3.0
 *
 * This example covers:
 * - Basic structured logging with key-value fields
 * - Context propagation (trace ID, correlation ID)
 * - Thread-local context scopes
 * - JSON and Logfmt formatters
 * - Template-based custom formatters
 */

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/structured_log_builder.h>
#include <kcenon/logger/core/log_context_scope.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/formatters/json_formatter.h>
#include <kcenon/logger/formatters/logfmt_formatter.h>
#include <kcenon/logger/formatters/template_formatter.h>

#include <iostream>
#include <thread>
#include <chrono>

using namespace kcenon::logger;
using log_level = logger_system::log_level;

/**
 * @brief Demonstrates basic structured logging with key-value fields
 */
void basic_structured_logging() {
    std::cout << "\n=== Basic Structured Logging ===\n" << std::endl;

    auto logger_instance = std::make_shared<logger>(false);  // Synchronous mode
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();

    // Basic structured log with various field types using canonical API
    logger_instance->log_structured(log_level::info)
        .message("User login successful")
        .field("user_id", 12345)
        .field("username", "john_doe")
        .field("ip_address", "192.168.1.100")
        .field("login_time_ms", 42.5)
        .field("mfa_enabled", true)
        .emit();

    // Error with structured details
    logger_instance->log_structured(log_level::error)
        .message("Database connection failed")
        .field("host", "db.example.com")
        .field("port", 5432)
        .field("retry_count", 3)
        .field("timeout_ms", 30000)
        .emit();

    // Debug with technical details
    logger_instance->log_structured(log_level::debug)
        .message("Cache lookup performed")
        .field("cache_key", "user:12345:profile")
        .field("hit", false)
        .field("latency_us", 150)
        .emit();

    logger_instance->stop();
}

/**
 * @brief Demonstrates context propagation for distributed tracing
 */
void context_propagation_example() {
    std::cout << "\n=== Context Propagation Example ===\n" << std::endl;

    auto logger_instance = std::make_shared<logger>(false);
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();

    // Set distributed tracing context using generic API
    logger_instance->set_context_id("trace_id", "0af7651916cd43dd8448eb211c80319c");
    logger_instance->set_context_id("span_id", "b7ad6b7169203331");
    logger_instance->set_context_id("correlation_id", "req-abc-123");

    // All subsequent logs will include trace context
    logger_instance->log_structured(log_level::info)
        .message("Processing API request")
        .field("endpoint", "/api/v1/users")
        .field("method", "GET")
        .emit();

    logger_instance->log_structured(log_level::info)
        .message("Database query executed")
        .field("query_type", "SELECT")
        .field("rows_returned", 42)
        .emit();

    // Clear all context IDs when request is complete
    logger_instance->clear_all_context_ids();

    logger_instance->stop();
}

/**
 * @brief Demonstrates RAII-based context scopes
 */
void context_scope_example() {
    std::cout << "\n=== Context Scope Example ===\n" << std::endl;

    auto logger_instance = std::make_shared<logger>(false);
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();

    // Outer scope: request-level context
    {
        log_context_scope request_scope({
            {"request_id", std::string("req-12345")},
            {"user_id", static_cast<int64_t>(1001)},
            {"tenant", std::string("acme-corp")}
        });

        logger_instance->log_structured(log_level::info)
            .message("Request processing started")
            .emit();

        // Inner scope: operation-specific context
        {
            log_context_scope db_scope({
                {"operation", std::string("user_lookup")},
                {"database", std::string("users_db")}
            });

            logger_instance->log_structured(log_level::debug)
                .message("Executing database query")
                .field("query_time_ms", 15.3)
                .emit();
        }
        // db_scope ended - "operation" and "database" removed

        logger_instance->log_structured(log_level::info)
            .message("Request processing completed")
            .field("total_time_ms", 45.7)
            .emit();
    }
    // request_scope ended - all context cleared

    logger_instance->stop();
}

/**
 * @brief Demonstrates single-field scoped context
 */
void scoped_context_single_field() {
    std::cout << "\n=== Scoped Context (Single Field) ===\n" << std::endl;

    auto logger_instance = std::make_shared<logger>(false);
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();

    // Using scoped_context for single fields
    {
        scoped_context order_ctx("order_id", static_cast<int64_t>(98765));

        logger_instance->log_structured(log_level::info)
            .message("Processing order")
            .field("status", "pending")
            .emit();

        {
            scoped_context item_ctx("item_id", static_cast<int64_t>(42));

            logger_instance->log_structured(log_level::debug)
                .message("Validating item")
                .field("quantity", 3)
                .emit();
        }

        logger_instance->log_structured(log_level::info)
            .message("Order completed")
            .field("status", "completed")
            .emit();
    }

    logger_instance->stop();
}

/**
 * @brief Demonstrates JSON formatter for structured output
 */
void json_formatter_example() {
    std::cout << "\n=== JSON Formatter Example ===\n" << std::endl;

    // Create a log entry with structured fields
    log_entry entry(log_level::info, "User profile updated");
    entry.fields = log_fields{
        {"user_id", static_cast<int64_t>(12345)},
        {"fields_changed", static_cast<int64_t>(3)},
        {"notification_sent", true}
    };

    // Format with JSON formatter
    json_formatter formatter;
    std::string output = formatter.format(entry);

    std::cout << "JSON formatted output:\n" << output << std::endl;
}

/**
 * @brief Demonstrates Logfmt formatter for key=value output
 */
void logfmt_formatter_example() {
    std::cout << "\n=== Logfmt Formatter Example ===\n" << std::endl;

    // Create a log entry with structured fields
    log_entry entry(log_level::info, "HTTP request completed");
    entry.fields = log_fields{
        {"method", std::string("POST")},
        {"path", std::string("/api/orders")},
        {"status_code", static_cast<int64_t>(201)},
        {"duration_ms", 125.5}
    };

    // Format with logfmt formatter
    logfmt_formatter formatter;
    std::string output = formatter.format(entry);

    std::cout << "Logfmt formatted output:\n" << output << std::endl;
}

/**
 * @brief Demonstrates template-based custom formatter
 */
void template_formatter_example() {
    std::cout << "\n=== Template Formatter Example ===\n" << std::endl;

    // Create a log entry with structured fields
    log_entry entry(log_level::info, "Session created");
    entry.fields = log_fields{
        {"user_id", static_cast<int64_t>(12345)},
        {"session_id", std::string("sess-abc-123")}
    };

    // Format with custom template
    template_formatter formatter("[{timestamp_local}] [{level:8}] {message} | user={user_id}");
    std::string output = formatter.format(entry);

    std::cout << "Template formatted output:\n" << output << std::endl;

    // Try different templates
    formatter.set_template("{level}: {message} (session={session_id})");
    std::string output2 = formatter.format(entry);
    std::cout << "Alternative template:\n" << output2 << std::endl;
}

/**
 * @brief Demonstrates thread-safe context in multi-threaded environment
 */
void multithreaded_context_example() {
    std::cout << "\n=== Multithreaded Context Example ===\n" << std::endl;

    auto logger_instance = std::make_shared<logger>(true);  // Async mode for MT
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();

    auto worker = [&](int thread_id) {
        // Each thread has its own context (thread-local storage)
        log_context_scope scope({
            {"thread_id", static_cast<int64_t>(thread_id)},
            {"worker", std::string("processor-" + std::to_string(thread_id))}
        });

        for (int i = 0; i < 3; ++i) {
            logger_instance->log_structured(log_level::info)
                .message("Processing item")
                .field("item_number", i)
                .emit();

            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    };

    std::thread t1(worker, 1);
    std::thread t2(worker, 2);

    t1.join();
    t2.join();

    logger_instance->stop();
}

/**
 * @brief Demonstrates logger-level context fields
 */
void logger_context_example() {
    std::cout << "\n=== Logger Context Example ===\n" << std::endl;

    auto logger_instance = std::make_shared<logger>(false);
    logger_instance->add_writer(std::make_unique<console_writer>());
    logger_instance->start();

    // Set logger-level context (applies to all logs)
    logger_instance->set_context("service", std::string("order-service"));
    logger_instance->set_context("version", std::string("1.2.3"));
    logger_instance->set_context("environment", std::string("production"));

    // All logs will include service, version, and environment
    logger_instance->log_structured(log_level::info)
        .message("Service started")
        .emit();

    logger_instance->log_structured(log_level::info)
        .message("Processing request")
        .field("request_id", "req-001")
        .emit();

    // Check if context exists
    if (logger_instance->has_context()) {
        auto ctx = logger_instance->get_context();
        std::cout << "Current context has " << ctx.size() << " fields" << std::endl;
    }

    // Remove specific context
    logger_instance->remove_context("environment");

    // Clear all context
    logger_instance->clear_context();

    logger_instance->stop();
}

int main() {
    try {
        basic_structured_logging();
        context_propagation_example();
        context_scope_example();
        scoped_context_single_field();
        json_formatter_example();
        logfmt_formatter_example();
        template_formatter_example();
        multithreaded_context_example();
        logger_context_example();

        std::cout << "\n=== All structured logging examples completed! ===" << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
