/*****************************************************************************
BSD 3-Clause License

Copyright (c) 2025, 🍀☀🌕🌥 🌊
All rights reserved.
*****************************************************************************/

#include <gtest/gtest.h>
#include <kcenon/logger/core/logger.h>
#include <kcenon/logger/core/logger_builder.h>
#include <kcenon/logger/writers/file_writer.h>
#include <kcenon/logger/filters/log_filter.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <thread>
#include <chrono>

using namespace kcenon::logger;
using namespace std::chrono_literals;

class RoutingIntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        cleanup_test_files();
    }

    void TearDown() override {
        cleanup_test_files();
    }

    void cleanup_test_files() {
        std::vector<std::string> test_files = {
            "test_all.log",
            "test_errors.log",
            "test_security.log",
            "test_debug.log"
        };

        for (const auto& file : test_files) {
            std::filesystem::remove(file);
        }
    }

    std::string read_file_content(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "";
        }
        return std::string((std::istreambuf_iterator<char>(file)),
                          std::istreambuf_iterator<char>());
    }

    bool file_contains(const std::string& filename, const std::string& text) {
        return read_file_content(filename).find(text) != std::string::npos;
    }
};

// Test 1: Basic router access
TEST_F(RoutingIntegrationTest, GetRouterAccess) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("console", std::make_unique<file_writer>("test_all.log"))
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Should be able to access router
    auto& router = test_logger->get_router();
    EXPECT_FALSE(router.is_exclusive_routing());
}

// Test 2: Exclusive routing - errors only to specific file
TEST_F(RoutingIntegrationTest, ExclusiveRoutingByLevel) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("all", std::make_unique<file_writer>("test_all.log"))
        .add_writer("errors", std::make_unique<file_writer>("test_errors.log"))
        .route_level(log_level::error, {"errors"})
        .with_exclusive_routing(true)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Log messages at different levels
    test_logger->log(log_level::info, "Info message");
    test_logger->log(log_level::error, "Error message");
    test_logger->log(log_level::warning, "Warning message");

    test_logger->flush();

    // In exclusive mode with error route:
    // - Error messages should go to "errors" writer
    // - Other messages should not match any route, so they go nowhere (exclusive mode)
    EXPECT_TRUE(file_contains("test_errors.log", "Error message"));
    EXPECT_FALSE(file_contains("test_errors.log", "Info message"));
    EXPECT_FALSE(file_contains("test_errors.log", "Warning message"));
}

// Test 3: Non-exclusive routing - all messages to all writers
TEST_F(RoutingIntegrationTest, NonExclusiveRouting) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("all", std::make_unique<file_writer>("test_all.log"))
        .add_writer("errors", std::make_unique<file_writer>("test_errors.log"))
        .route_level(log_level::error, {"errors"})
        .with_exclusive_routing(false)  // Non-exclusive (default)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Log messages
    test_logger->log(log_level::info, "Info message");
    test_logger->log(log_level::error, "Error message");

    test_logger->flush();

    // In non-exclusive mode, all messages go to all writers
    EXPECT_TRUE(file_contains("test_all.log", "Info message"));
    EXPECT_TRUE(file_contains("test_all.log", "Error message"));
}

// Test 4: Pattern-based routing
TEST_F(RoutingIntegrationTest, PatternBasedRouting) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("all", std::make_unique<file_writer>("test_all.log"))
        .add_writer("security", std::make_unique<file_writer>("test_security.log"))
        .route_pattern("security|auth", {"security"})
        .with_exclusive_routing(true)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Log messages
    test_logger->log(log_level::info, "Normal operation");
    test_logger->log(log_level::info, "Security check passed");
    test_logger->log(log_level::warning, "Authentication failed for user");

    test_logger->flush();

    // Security-related messages should go to security file
    EXPECT_TRUE(file_contains("test_security.log", "Security check passed"));
    EXPECT_TRUE(file_contains("test_security.log", "Authentication failed"));
    EXPECT_FALSE(file_contains("test_security.log", "Normal operation"));
}

// Test 5: Direct router configuration
TEST_F(RoutingIntegrationTest, DirectRouterConfiguration) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("all", std::make_unique<file_writer>("test_all.log"))
        .add_writer("debug", std::make_unique<file_writer>("test_debug.log"))
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Configure router directly
    auto& router = test_logger->get_router();
    router.set_exclusive_routes(true);

    routing::route_config config;
    config.writer_names = {"debug"};
    config.filter = std::make_unique<filters::level_filter>(log_level::debug);
    config.stop_propagation = false;
    router.add_route(std::move(config));

    // Log messages
    test_logger->log(log_level::debug, "Debug message");
    test_logger->log(log_level::info, "Info message");

    test_logger->flush();

    // Debug messages should go to debug file
    EXPECT_TRUE(file_contains("test_debug.log", "Debug message"));
    EXPECT_FALSE(file_contains("test_debug.log", "Info message"));
}

// Test 6: Multiple routes
TEST_F(RoutingIntegrationTest, MultipleRoutes) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("all", std::make_unique<file_writer>("test_all.log"))
        .add_writer("errors", std::make_unique<file_writer>("test_errors.log"))
        .add_writer("debug", std::make_unique<file_writer>("test_debug.log"))
        .route_level(log_level::error, {"errors"})
        .route_level(log_level::debug, {"debug"})
        .with_exclusive_routing(true)
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Log messages at different levels
    test_logger->log(log_level::debug, "Debug message");
    test_logger->log(log_level::info, "Info message");
    test_logger->log(log_level::error, "Error message");

    test_logger->flush();

    // Each level should go to appropriate file
    EXPECT_TRUE(file_contains("test_debug.log", "Debug message"));
    EXPECT_FALSE(file_contains("test_debug.log", "Error message"));

    EXPECT_TRUE(file_contains("test_errors.log", "Error message"));
    EXPECT_FALSE(file_contains("test_errors.log", "Debug message"));
}

// Test 7: has_routing() check
TEST_F(RoutingIntegrationTest, HasRoutingCheck) {
    auto result = logger_builder()
        .with_async(false)
        .add_writer("all", std::make_unique<file_writer>("test_all.log"))
        .build();

    ASSERT_TRUE(result.has_value());
    auto& test_logger = result.value();

    // Initially has router but it's empty
    EXPECT_TRUE(test_logger->has_routing());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
