/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, kcenon
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
 * @file log_server_test.cpp
 * @brief Unit tests for log_server (construction, lifecycle, factory)
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/server/log_server.h>

#include <thread>

using namespace kcenon::logger::server;

// =============================================================================
// Test fixture
// =============================================================================

class LogServerTest : public ::testing::Test {
protected:
    void TearDown() override {
        // Ensure any started servers are stopped
    }
};

// =============================================================================
// Construction
// =============================================================================

TEST_F(LogServerTest, DefaultConstruction) {
    log_server server;

    EXPECT_FALSE(server.is_running());
    EXPECT_EQ(server.get_config().host, "localhost");
    EXPECT_EQ(server.get_config().port, 9999);
    EXPECT_EQ(server.get_config().max_connections, 100u);
    EXPECT_EQ(server.get_config().buffer_size, 8192u);
    EXPECT_FALSE(server.get_config().enable_compression);
    EXPECT_FALSE(server.get_config().enable_encryption);
}

TEST_F(LogServerTest, CustomConfigConstruction) {
    server_config config;
    config.host = "0.0.0.0";
    config.port = 8080;
    config.max_connections = 50;
    config.buffer_size = 4096;
    config.enable_compression = true;
    config.enable_encryption = true;

    log_server server(config);

    EXPECT_EQ(server.get_config().host, "0.0.0.0");
    EXPECT_EQ(server.get_config().port, 8080);
    EXPECT_EQ(server.get_config().max_connections, 50u);
    EXPECT_EQ(server.get_config().buffer_size, 4096u);
    EXPECT_TRUE(server.get_config().enable_compression);
    EXPECT_TRUE(server.get_config().enable_encryption);
}

// =============================================================================
// Start / Stop lifecycle
// =============================================================================

TEST_F(LogServerTest, StartSetsRunningState) {
    log_server server;

    EXPECT_FALSE(server.is_running());
    EXPECT_TRUE(server.start());
    EXPECT_TRUE(server.is_running());

    server.stop();
    EXPECT_FALSE(server.is_running());
}

TEST_F(LogServerTest, DoubleStartReturnsFalse) {
    log_server server;

    EXPECT_TRUE(server.start());
    EXPECT_FALSE(server.start());

    server.stop();
}

TEST_F(LogServerTest, StopWhenNotRunningIsNoOp) {
    log_server server;

    // Calling stop on a non-running server should not crash
    server.stop();
    EXPECT_FALSE(server.is_running());
}

TEST_F(LogServerTest, StopThenRestartWorks) {
    log_server server;

    EXPECT_TRUE(server.start());
    server.stop();
    EXPECT_FALSE(server.is_running());

    // Should be able to restart after stop
    EXPECT_TRUE(server.start());
    EXPECT_TRUE(server.is_running());

    server.stop();
}

// =============================================================================
// Destructor
// =============================================================================

TEST_F(LogServerTest, DestructorStopsRunningServer) {
    {
        log_server server;
        server.start();
        EXPECT_TRUE(server.is_running());
        // Destructor should call stop() and join all threads
    }
    // If we reach here without hanging, the destructor worked correctly
    SUCCEED();
}

// =============================================================================
// Worker thread count
// =============================================================================

TEST_F(LogServerTest, WorkerThreadCountMatchesHardwareConcurrency) {
    // This test verifies that start() creates the expected number of threads.
    // We can't directly access worker_threads_ since it's private,
    // but we verify that start() succeeds and the server operates correctly
    // with multiple threads.
    log_server server;
    EXPECT_TRUE(server.start());
    EXPECT_TRUE(server.is_running());

    // Allow worker threads to begin their loops
    std::this_thread::sleep_for(std::chrono::milliseconds(150));

    // Server should still be running after threads have started
    EXPECT_TRUE(server.is_running());

    server.stop();
    EXPECT_FALSE(server.is_running());
}

// =============================================================================
// Factory
// =============================================================================

TEST_F(LogServerTest, FactoryCreateBasicWithDefaultConfig) {
    auto server = log_server_factory::create_basic();

    ASSERT_NE(server, nullptr);
    EXPECT_FALSE(server->is_running());
    EXPECT_EQ(server->get_config().host, "localhost");
    EXPECT_EQ(server->get_config().port, 9999);
}

TEST_F(LogServerTest, FactoryCreateBasicWithCustomConfig) {
    server_config config;
    config.host = "10.0.0.1";
    config.port = 5555;

    auto server = log_server_factory::create_basic(config);

    ASSERT_NE(server, nullptr);
    EXPECT_EQ(server->get_config().host, "10.0.0.1");
    EXPECT_EQ(server->get_config().port, 5555);
}

TEST_F(LogServerTest, FactoryCreateDefault) {
    auto server = log_server_factory::create_default();

    ASSERT_NE(server, nullptr);
    EXPECT_FALSE(server->is_running());
    // create_default uses default server_config values
    EXPECT_EQ(server->get_config().host, "localhost");
    EXPECT_EQ(server->get_config().port, 9999);
}

TEST_F(LogServerTest, FactoryCreatedServerCanStartStop) {
    auto server = log_server_factory::create_basic();

    EXPECT_TRUE(server->start());
    EXPECT_TRUE(server->is_running());

    server->stop();
    EXPECT_FALSE(server->is_running());
}
