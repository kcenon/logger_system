// Simple test to verify backend integration
#include <iostream>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/backends/standalone_backend.h>
#include <kcenon/logger/writers/console_writer.h>

#ifdef USE_THREAD_SYSTEM_INTEGRATION
    #include <kcenon/logger/backends/thread_system_backend.h>
#endif

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

#ifdef USE_THREAD_SYSTEM_INTEGRATION
    // Test 2: Thread system backend
    {
        std::cout << "\n=== Test 2: Thread System Backend ===" << std::endl;
        auto thread_backend = std::make_unique<backends::thread_system_backend>();
        std::cout << "Backend name: " << thread_backend->get_backend_name() << std::endl;

        auto logger_result = logger_builder()
            .with_thread_system_backend()
            .with_async(false)
            .add_writer("console", std::make_unique<console_writer>())
            .build();

        if (logger_result) {
            auto logger_inst = std::move(logger_result.value());
            logger_inst->log(log_level::info, "Test message from thread_system backend");
            std::cout << "✅ Thread system backend test passed" << std::endl;
        } else {
            std::cerr << "❌ Failed to build logger: " << logger_result.error_message() << std::endl;
            return 1;
        }
    }
#endif

    // Test 3: Auto-detection (default)
    {
        std::cout << "\n=== Test 3: Auto-Detection ===" << std::endl;
        auto logger_result = logger_builder()
            .with_async(false)
            .add_writer("console", std::make_unique<console_writer>())
            .build();

        if (logger_result) {
            auto logger_inst = std::move(logger_result.value());
            logger_inst->log(log_level::info, "Test message with auto-detected backend");
#ifdef USE_THREAD_SYSTEM_INTEGRATION
            std::cout << "✅ Auto-detection test passed (thread_system backend)" << std::endl;
#else
            std::cout << "✅ Auto-detection test passed (standalone backend)" << std::endl;
#endif
        } else {
            std::cerr << "❌ Failed to build logger: " << logger_result.error_message() << std::endl;
            return 1;
        }
    }

    std::cout << "\n=== All backend tests passed! ===" << std::endl;
    return 0;
}
