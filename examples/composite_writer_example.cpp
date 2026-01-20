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

/**
 * @file composite_writer_example.cpp
 * @brief Example demonstrating the new composite writer architecture
 * @since 1.3.0 (Sprint 5-6)
 *
 * This example shows how to use the new SRP-compliant architecture:
 * - Formatters handle formatting only
 * - Sinks handle I/O only
 * - Composite writers coordinate the pipeline
 */

#include <iostream>
#include <memory>
#include <kcenon/logger/writers/composite_writer.h>
#include <kcenon/logger/sinks/console_sink.h>
#include <kcenon/logger/sinks/file_sink.h>
#include <kcenon/logger/formatters/timestamp_formatter.h>
#include <kcenon/logger/interfaces/log_entry.h>

using namespace kcenon::logger;

int main() {
    std::cout << "=== Composite Writer Example (SRP Architecture) ===" << std::endl;
    std::cout << std::endl;

    // Example 1: Console writer with timestamp formatter
    std::cout << "Example 1: Timestamp formatter + Console sink" << std::endl;
    {
        auto formatter = std::make_unique<timestamp_formatter>();
        auto sink = std::make_unique<console_sink>();
        auto writer = make_composite_writer(std::move(formatter), std::move(sink));

        log_entry entry;
        entry.level = log_level::info;
        entry.message = "Application started successfully";
        entry.timestamp = std::chrono::system_clock::now();

        writer->write(entry);
        std::cout << "Writer name: " << writer->get_name() << std::endl;
        std::cout << "Is healthy: " << (writer->is_healthy() ? "yes" : "no") << std::endl;
    }
    std::cout << std::endl;

    // Example 2: File writer with timestamp formatter
    std::cout << "Example 2: Timestamp formatter + File sink" << std::endl;
    {
        auto formatter = std::make_unique<timestamp_formatter>();
        auto sink = std::make_unique<file_sink>("/tmp/app.log", true);
        auto writer = make_composite_writer(std::move(formatter), std::move(sink));

        log_entry entry;
        entry.level = log_level::warning;
        entry.message = "Configuration file not found, using defaults";
        entry.timestamp = std::chrono::system_clock::now();

        writer->write(entry);
        writer->flush();
        std::cout << "Logged to file: " << writer->get_sink()->get_info() << std::endl;
    }
    std::cout << std::endl;

    // Example 3: Multiple writers with different sink configurations
    std::cout << "Example 3: Multiple writers (different configurations)" << std::endl;
    {
        // Console to stdout
        auto console_formatter = std::make_unique<timestamp_formatter>();
        auto console_sink = std::make_unique<console_sink>(false, true); // stdout, auto-flush
        auto console_writer = make_composite_writer(
            std::move(console_formatter),
            std::move(console_sink)
        );

        // Console to stderr  
        auto error_formatter = std::make_unique<timestamp_formatter>();
        auto error_sink = std::make_unique<console_sink>(true, true); // stderr, auto-flush
        auto error_writer = make_composite_writer(
            std::move(error_formatter),
            std::move(error_sink)
        );

        log_entry info_entry;
        info_entry.level = log_level::info;
        info_entry.message = "This goes to stdout";
        info_entry.timestamp = std::chrono::system_clock::now();
        console_writer->write(info_entry);

        log_entry error_entry;
        error_entry.level = log_level::error;
        error_entry.message = "This goes to stderr";
        error_entry.timestamp = std::chrono::system_clock::now();
        error_writer->write(error_entry);
    }
    std::cout << std::endl;

    std::cout << "=== Benefits of SRP Architecture ===" << std::endl;
    std::cout << "1. Single Responsibility: Each component has one job" << std::endl;
    std::cout << "2. Open/Closed: Add new formatters/sinks without modifying existing code" << std::endl;
    std::cout << "3. Testability: Each component can be tested independently" << std::endl;
    std::cout << "4. Flexibility: Mix and match any formatter with any sink" << std::endl;
    std::cout << "5. Reusability: Formatters and sinks can be reused in different combinations" << std::endl;

    return 0;
}
