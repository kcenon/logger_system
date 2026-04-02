// BSD 3-Clause License
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

#include <iostream>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/backends/standalone_backend.h>
#include <kcenon/logger/writers/console_writer.h>

// Note: thread_system_backend was removed in Issue #225
// thread_system is now optional and standalone_backend is the default

using namespace kcenon::logger;

int main() {
    std::cout << "Testing backend integration..." << std::endl;

    // Test 1: Standalone backend
    {
        std::cout << "\n=== Test 1: Standalone Backend ===" << std::endl;
        auto standalone_backend = std::make_unique<backends::standalone_backend>();
        std::cout << "Backend name: " << standalone_backend->get_backend_name() << std::endl;

        auto logger_result = logger_builder()
            .with_standalone_backend()
            .with_async(false)
            .add_writer("console", std::make_unique<console_writer>())
            .build();

        if (logger_result) {
            auto logger_inst = std::move(logger_result.value());
            logger_inst->log(log_level::info, "Test message from standalone backend");
            std::cout << "✅ Standalone backend test passed" << std::endl;
        } else {
            std::cerr << "❌ Failed to build logger: " << logger_result.error_message() << std::endl;
            return 1;
        }
    }

    // Note: Test 2 (Thread system backend) was removed in Issue #225
    // thread_system is now optional and standalone_backend is used by default

    // Test 2: Auto-detection (default, always uses standalone_backend since Issue #225)
    {
        std::cout << "\n=== Test 2: Auto-Detection ===" << std::endl;
        auto logger_result = logger_builder()
            .with_async(false)
            .add_writer("console", std::make_unique<console_writer>())
            .build();

        if (logger_result) {
            auto logger_inst = std::move(logger_result.value());
            logger_inst->log(log_level::info, "Test message with auto-detected backend");
            std::cout << "✅ Auto-detection test passed (standalone backend)" << std::endl;
        } else {
            std::cerr << "❌ Failed to build logger: " << logger_result.error_message() << std::endl;
            return 1;
        }
    }

    std::cout << "\n=== All backend tests passed! ===" << std::endl;
    return 0;
}
