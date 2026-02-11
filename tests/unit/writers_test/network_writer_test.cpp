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
 * @file network_writer_test.cpp
 * @brief Unit tests for network_writer (TCP/UDP log shipping)
 * @since 1.0.0
 *
 * @note Network writer tests are limited to construction and configuration
 * since actual network connectivity is not available in unit tests.
 * Integration tests with real sockets should be in a separate test suite.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/writers/network_writer.h>
#include <kcenon/logger/interfaces/log_entry.h>
#include <kcenon/logger/core/error_codes.h>

#include <memory>
#include <string>
#include <chrono>

using namespace kcenon::logger;
namespace common = kcenon::common;
using log_level = common::interfaces::log_level;

// =============================================================================
// Construction tests
// =============================================================================

TEST(NetworkWriterTest, ConstructionWithTcp) {
    auto writer = std::make_unique<network_writer>(
        "localhost", 9514, network_writer::protocol_type::tcp);
    EXPECT_NE(writer, nullptr);
}

TEST(NetworkWriterTest, ConstructionWithUdp) {
    auto writer = std::make_unique<network_writer>(
        "localhost", 9514, network_writer::protocol_type::udp);
    EXPECT_NE(writer, nullptr);
}

TEST(NetworkWriterTest, ConstructionWithCustomParams) {
    auto writer = std::make_unique<network_writer>(
        "192.168.1.100", 5140,
        network_writer::protocol_type::tcp,
        16384,  // buffer_size
        std::chrono::seconds(10));  // reconnect_interval
    EXPECT_NE(writer, nullptr);
}

// =============================================================================
// Name test
// =============================================================================

TEST(NetworkWriterTest, GetName) {
    network_writer writer("localhost", 9514);
    EXPECT_EQ(writer.get_name(), "network");
}

// =============================================================================
// Connection state tests (no actual network)
// =============================================================================

TEST(NetworkWriterTest, NotConnectedInitially) {
    network_writer writer("localhost", 9514);
    EXPECT_FALSE(writer.is_connected());
}

// =============================================================================
// Statistics tests
// =============================================================================

TEST(NetworkWriterTest, InitialStatsAreZero) {
    network_writer writer("localhost", 9514);
    auto stats = writer.get_stats();

    EXPECT_EQ(stats.messages_sent, 0u);
    EXPECT_EQ(stats.bytes_sent, 0u);
    EXPECT_EQ(stats.connection_failures, 0u);
    EXPECT_EQ(stats.send_failures, 0u);
}

// =============================================================================
// Write without connection (should handle gracefully)
// =============================================================================

TEST(NetworkWriterTest, WriteWithoutConnection) {
    network_writer writer("127.0.0.1", 19999,
        network_writer::protocol_type::tcp);

    log_entry entry(log_level::info, "test message");
    // Should not crash or hang; result may be error (no connection)
    EXPECT_NO_THROW(writer.write(entry));
}

TEST(NetworkWriterTest, FlushWithoutConnection) {
    network_writer writer("127.0.0.1", 19999);
    EXPECT_NO_THROW(writer.flush());
}

// =============================================================================
// Protocol type test
// =============================================================================

TEST(NetworkWriterTest, ProtocolTypeEnum) {
    // Verify both protocol types exist
    auto tcp = network_writer::protocol_type::tcp;
    auto udp = network_writer::protocol_type::udp;
    EXPECT_NE(static_cast<int>(tcp), static_cast<int>(udp));
}

// =============================================================================
// Category tag test
// =============================================================================

TEST(NetworkWriterTest, HasAsyncWriterTag) {
    network_writer writer("localhost", 9514);
    auto* tag = dynamic_cast<async_writer_tag*>(&writer);
    EXPECT_NE(tag, nullptr);
}
