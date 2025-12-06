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

// Simple test to verify backend integration
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
