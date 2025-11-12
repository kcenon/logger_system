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
 * @file log_server.h
 * @brief Log server for distributed logging
 */

#pragma once

#include <kcenon/logger/interfaces/logger_types.h>
#include <string>
#include <memory>
#include <vector>
#include <atomic>
#include <thread>

namespace kcenon::logger::server {

/**
 * @brief Configuration for log server
 */
struct server_config {
    std::string host = "localhost";
    uint16_t port = 9999;
    size_t max_connections = 100;
    size_t buffer_size = 8192;
    bool enable_compression = false;
    bool enable_encryption = false;
};

/**
 * @brief Log server for receiving distributed log messages
 */
class log_server {
private:
    server_config config_;
    std::atomic<bool> running_{false};
    std::vector<std::thread> worker_threads_;

public:
    explicit log_server(const server_config& config = {}) : config_(config) {}

    ~log_server() {
        stop();
    }

    /**
     * @brief Start the log server
     */
    bool start() {
        if (running_.load()) {
            return false;
        }

        running_.store(true);

        // Start worker threads
        for (size_t i = 0; i < std::thread::hardware_concurrency(); ++i) {
            worker_threads_.emplace_back([this]() {
                worker_loop();
            });
        }

        return true;
    }

    /**
     * @brief Stop the log server
     */
    void stop() {
        if (!running_.load()) {
            return;
        }

        running_.store(false);

        for (auto& thread : worker_threads_) {
            if (thread.joinable()) {
                thread.join();
            }
        }
        worker_threads_.clear();
    }

    /**
     * @brief Check if server is running
     */
    bool is_running() const {
        return running_.load();
    }

    /**
     * @brief Get server configuration
     */
    const server_config& get_config() const {
        return config_;
    }

private:
    void worker_loop() {
        while (running_.load()) {
            // Worker implementation would go here
            // For now, just sleep to simulate work
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
};

/**
 * @brief Factory for creating log servers
 */
class log_server_factory {
public:
    /**
     * @brief Create a basic log server
     */
    static std::unique_ptr<log_server> create_basic(const server_config& config = {}) {
        return std::make_unique<log_server>(config);
    }

    /**
     * @brief Create a log server with default configuration
     */
    static std::unique_ptr<log_server> create_default() {
        return create_basic();
    }
};

} // namespace kcenon::logger::server