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

#pragma once

#include <gtest/gtest.h>
#include <memory>
#include <chrono>
#include <thread>
#include <atomic>
#include <functional>
#include <fstream>
#include <filesystem>
#include <vector>
#include <sstream>

#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/console_writer.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/writers/async_writer.h>
#include <kcenon/logger/core/error_codes.h>
#include <kcenon/logger/interfaces/logger_types.h>

namespace integration_tests {

namespace fs = std::filesystem;

/**
 * @class LoggerSystemFixture
 * @brief Base fixture for integration tests providing common setup and teardown
 *
 * This fixture provides:
 * - Logger creation and management
 * - Temporary file management for log outputs
 * - Log verification helpers
 * - Common test utilities
 * - Cleanup and verification
 */
class LoggerSystemFixture : public ::testing::Test {
protected:
    void SetUp() override {
        // Create temporary directory for test logs
        temp_dir_ = fs::temp_directory_path() / ("logger_test_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        fs::create_directories(temp_dir_);

        // Reset counters
        log_count_.store(0);
        error_count_.store(0);
    }

    void TearDown() override {
        // Stop logger if still running
        if (logger_ && logger_->is_running()) {
            logger_->flush();
            logger_->stop();
        }

        logger_.reset();

        // Clean up temporary files
        if (fs::exists(temp_dir_)) {
            std::error_code ec;
            fs::remove_all(temp_dir_, ec);
        }

        // Clean up tracked temp files
        for (const auto& file : temp_files_) {
            if (fs::exists(file)) {
                std::error_code ec;
                fs::remove(file, ec);
            }
        }
    }

    /**
     * @brief Create a logger with specified configuration
     */
    void CreateLogger(bool async = true, std::size_t buffer_size = 8192) {
        logger_ = std::make_unique<kcenon::logger::logger>(async, buffer_size);
    }

    /**
     * @brief Create a logger with file writer
     */
    std::string CreateLoggerWithFileWriter(bool async = true) {
        CreateLogger(async);

        auto log_file = temp_dir_ / "test.log";
        auto writer = std::make_unique<kcenon::logger::file_writer>(log_file.string());

        logger_->add_writer(std::move(writer));
        logger_->start();

        return log_file.string();
    }

    /**
     * @brief Get a temporary file path
     */
    std::string GetTempFilePath(const std::string& name) {
        auto path = temp_dir_ / name;
        temp_files_.push_back(path);
        return path.string();
    }

    /**
     * @brief Wait for a condition to become true with timeout
     */
    template<typename Predicate>
    bool WaitForCondition(Predicate pred,
                         std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        auto start = std::chrono::steady_clock::now();
        while (!pred()) {
            if (std::chrono::steady_clock::now() - start > timeout) {
                return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return true;
    }

    /**
     * @brief Wait for logger to flush all messages
     *
     * @details This method properly waits for the flush to complete by calling
     * flush(), then stopping the logger to ensure all async operations complete,
     * and finally restarting it if needed.
     *
     * @note In Address Sanitizer environments, stop/start operations can be very slow
     * (2-5x overhead). This implementation adds a small delay after stop() to ensure
     * all async operations complete before proceeding.
     */
    void WaitForFlush(std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        if (!logger_) {
            return;
        }

        bool was_running = logger_->is_running();

        // Flush the logger to trigger write operations
        logger_->flush();

        // Stop the logger to ensure all async operations complete
        // This guarantees that all queued messages are processed and written
        if (was_running) {
            logger_->stop();

            // Give some time for async operations to complete
            // This is especially important in sanitizer environments where
            // operations are significantly slower
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // Restart the logger if it was running
        // This allows subsequent test operations to continue
        if (was_running) {
            logger_->start();
        }
    }

    /**
     * @brief Read log file contents
     */
    std::string ReadLogFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    /**
     * @brief Check if log file contains a specific string
     */
    bool LogContains(const std::string& filepath, const std::string& text) {
        auto contents = ReadLogFile(filepath);
        return contents.find(text) != std::string::npos;
    }

    /**
     * @brief Count number of lines in log file
     */
    size_t CountLogLines(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return 0;
        }

        size_t count = 0;
        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                count++;
            }
        }
        return count;
    }

    /**
     * @brief Count occurrences of a substring in log file
     */
    size_t CountOccurrences(const std::string& filepath, const std::string& text) {
        auto contents = ReadLogFile(filepath);
        size_t count = 0;
        size_t pos = 0;

        while ((pos = contents.find(text, pos)) != std::string::npos) {
            count++;
            pos += text.length();
        }

        return count;
    }

    /**
     * @brief Get all log lines from file
     */
    std::vector<std::string> GetLogLines(const std::string& filepath) {
        std::vector<std::string> lines;
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return lines;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (!line.empty()) {
                lines.push_back(line);
            }
        }

        return lines;
    }

    /**
     * @brief Wait for file to exist
     */
    bool WaitForFile(const std::string& filepath,
                    std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        return WaitForCondition([&filepath]() {
            return fs::exists(filepath);
        }, timeout);
    }

    /**
     * @brief Wait for log file to contain expected number of lines
     */
    bool WaitForLogLines(const std::string& filepath,
                        size_t expected_lines,
                        std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        return WaitForCondition([this, &filepath, expected_lines]() {
            return CountLogLines(filepath) >= expected_lines;
        }, timeout);
    }

    // Protected member variables
    std::unique_ptr<kcenon::logger::logger> logger_;
    fs::path temp_dir_;
    std::vector<fs::path> temp_files_;

    std::atomic<size_t> log_count_{0};
    std::atomic<size_t> error_count_{0};
};

/**
 * @class MultiLoggerFixture
 * @brief Fixture for tests requiring multiple logger instances
 */
class MultiLoggerFixture : public ::testing::Test {
protected:
    void SetUp() override {
        temp_dir_ = fs::temp_directory_path() / ("logger_multi_test_" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        fs::create_directories(temp_dir_);
    }

    void TearDown() override {
        // Stop all loggers
        for (auto& logger : loggers_) {
            if (logger && logger->is_running()) {
                logger->flush();
                logger->stop();
            }
        }
        loggers_.clear();

        // Clean up temporary directory
        if (fs::exists(temp_dir_)) {
            std::error_code ec;
            fs::remove_all(temp_dir_, ec);
        }
    }

    /**
     * @brief Create multiple loggers
     */
    void CreateMultipleLoggers(size_t count, bool async = true) {
        for (size_t i = 0; i < count; ++i) {
            auto logger = std::make_unique<kcenon::logger::logger>(async);

            auto log_file = temp_dir_ / ("logger_" + std::to_string(i) + ".log");
            auto writer = std::make_unique<kcenon::logger::file_writer>(log_file.string());

            logger->add_writer(std::move(writer));
            logger->start();

            loggers_.push_back(std::move(logger));
            log_files_.push_back(log_file.string());
        }
    }

    /**
     * @brief Wait for condition with timeout
     */
    template<typename Predicate>
    bool WaitForCondition(Predicate pred,
                         std::chrono::milliseconds timeout = std::chrono::seconds(5)) {
        auto start = std::chrono::steady_clock::now();
        while (!pred()) {
            if (std::chrono::steady_clock::now() - start > timeout) {
                return false;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        return true;
    }

    /**
     * @brief Read log file contents
     */
    std::string ReadLogFile(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }

    std::vector<std::unique_ptr<kcenon::logger::logger>> loggers_;
    std::vector<std::string> log_files_;
    fs::path temp_dir_;
};

} // namespace integration_tests
