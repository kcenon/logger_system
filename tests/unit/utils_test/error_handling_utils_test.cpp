// BSD 3-Clause License
// Copyright (c) 2025, kcenon
// See the LICENSE file in the project root for full license information.

/**
 * @file error_handling_utils_test.cpp
 * @brief Unit tests for structured error context and helper wrappers
 *
 * Covers error_context construction/formatting, try_write_operation exception
 * mapping, stream state checks, directory helpers, and destructor-safe
 * operation wrappers.
 */

#include <gtest/gtest.h>

#include <kcenon/logger/core/error_codes.h>
#include <kcenon/logger/utils/error_handling_utils.h>

#include <filesystem>
#include <fstream>
#include <ios>
#include <sstream>
#include <stdexcept>
#include <string>
#include <system_error>

namespace common = kcenon::common;
using namespace kcenon::logger;
using namespace kcenon::logger::utils;

// =============================================================================
// error_context
// =============================================================================

TEST(ErrorContextTest, BasicToStringContainsCodeAndMessage) {
    error_context ctx(logger_error_code::file_write_failed, "disk full");

    std::string text = ctx.to_string();
    EXPECT_NE(text.find("Failed to write to file"), std::string::npos);
    EXPECT_NE(text.find("disk full"), std::string::npos);
}

TEST(ErrorContextTest, OperationAppearsInOutput) {
    error_context ctx(logger_error_code::file_open_failed, "boom", "open_log_file");

    std::string text = ctx.to_string();
    EXPECT_NE(text.find("during: open_log_file"), std::string::npos);
}

TEST(ErrorContextTest, EmptyMessageStillIncludesCode) {
    error_context ctx(logger_error_code::writer_not_found, "");

    std::string text = ctx.to_string();
    EXPECT_NE(text.find("Writer not found"), std::string::npos);
}

TEST(ErrorContextTest, TimestampIsSet) {
    auto before = std::chrono::system_clock::now();
    error_context ctx(logger_error_code::unknown_error, "msg");
    auto after = std::chrono::system_clock::now();

    EXPECT_GE(ctx.timestamp, before);
    EXPECT_LE(ctx.timestamp, after);
}

// =============================================================================
// log_error_context
// =============================================================================

TEST(LogErrorContextTest, WritesFormattedMessageToStderr) {
    // Redirect stderr into a stringstream for the duration of the call.
    std::stringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    error_context ctx(logger_error_code::encryption_failed, "crypto", "encrypt");
    log_error_context(ctx);

    std::cerr.rdbuf(original);
    std::string output = captured.str();

    EXPECT_NE(output.find("logger_system"), std::string::npos);
    EXPECT_NE(output.find("crypto"), std::string::npos);
    EXPECT_NE(output.find("encrypt"), std::string::npos);
}

// =============================================================================
// try_write_operation exception mapping
// =============================================================================

TEST(TryWriteOperationTest, PassesThroughSuccess) {
    auto result = try_write_operation([]() -> common::VoidResult {
        return common::ok();
    });
    EXPECT_FALSE(result.is_err());
}

TEST(TryWriteOperationTest, PassesThroughExplicitError) {
    auto result = try_write_operation([]() -> common::VoidResult {
        return make_logger_void_result(
            logger_error_code::queue_full, "explicit");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result), logger_error_code::queue_full);
}

TEST(TryWriteOperationTest, MapsFilesystemError) {
    auto result = try_write_operation([]() -> common::VoidResult {
        throw std::filesystem::filesystem_error(
            "denied",
            std::make_error_code(std::errc::permission_denied));
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_permission_denied);
}

TEST(TryWriteOperationTest, MapsIosBaseFailure) {
    auto result = try_write_operation([]() -> common::VoidResult {
        throw std::ios_base::failure("stream bad");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
}

TEST(TryWriteOperationTest, MapsSystemErrorToDefault) {
    auto result = try_write_operation([]() -> common::VoidResult {
        throw std::system_error(
            std::make_error_code(std::errc::io_error), "sys");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
}

TEST(TryWriteOperationTest, SystemErrorHonorsCustomDefault) {
    auto result = try_write_operation(
        []() -> common::VoidResult {
            throw std::system_error(
                std::make_error_code(std::errc::io_error), "sys");
        },
        logger_error_code::network_send_failed);

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::network_send_failed);
}

TEST(TryWriteOperationTest, MapsBadAllocToBufferOverflow) {
    auto result = try_write_operation([]() -> common::VoidResult {
        throw std::bad_alloc();
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::buffer_overflow);
}

TEST(TryWriteOperationTest, MapsGenericException) {
    auto result = try_write_operation([]() -> common::VoidResult {
        throw std::runtime_error("boom");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
    EXPECT_NE(get_logger_error_message(result).find("boom"),
              std::string::npos);
}

TEST(TryWriteOperationTest, MapsNonStandardException) {
    auto result = try_write_operation([]() -> common::VoidResult {
        throw 42;
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
}

// =============================================================================
// Specialized wrappers
// =============================================================================

TEST(TryOpenOperationTest, UsesFileOpenFailedAsDefault) {
    auto result = try_open_operation([]() -> common::VoidResult {
        throw std::runtime_error("cannot open");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_open_failed);
}

TEST(TryNetworkOperationTest, UsesNetworkSendFailedAsDefault) {
    auto result = try_network_operation([]() -> common::VoidResult {
        throw std::runtime_error("socket closed");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::network_send_failed);
}

TEST(TryEncryptionOperationTest, UsesEncryptionFailedAsDefault) {
    auto result = try_encryption_operation([]() -> common::VoidResult {
        throw std::runtime_error("cipher error");
    });

    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::encryption_failed);
}

// =============================================================================
// check_condition
// =============================================================================

TEST(CheckConditionTest, TrueReturnsOk) {
    auto result = check_condition(true,
                                  logger_error_code::file_write_failed,
                                  "should not fire");
    EXPECT_FALSE(result.is_err());
}

TEST(CheckConditionTest, FalseReturnsError) {
    auto result = check_condition(false,
                                  logger_error_code::file_write_failed,
                                  "custom message");
    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
    EXPECT_EQ(get_logger_error_message(result), "custom message");
}

// =============================================================================
// check_stream_state
// =============================================================================

TEST(CheckStreamStateTest, GoodStreamReturnsOk) {
    std::stringstream ss;
    ss << "ok";
    auto result = check_stream_state(ss, "write");
    EXPECT_FALSE(result.is_err());
}

TEST(CheckStreamStateTest, FailedStreamReturnsError) {
    std::stringstream ss;
    ss.setstate(std::ios::failbit);
    auto result = check_stream_state(ss, "read");
    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
    EXPECT_NE(get_logger_error_message(result).find("read"),
              std::string::npos);
}

TEST(CheckStreamStateTest, BadStreamReturnsError) {
    std::stringstream ss;
    ss.setstate(std::ios::badbit);
    auto result = check_stream_state(ss, "write");
    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_write_failed);
}

// =============================================================================
// check_file_exists
// =============================================================================

TEST(CheckFileExistsTest, ReturnsOkForExistingFile) {
    auto tmp = std::filesystem::temp_directory_path() /
               "logger_error_util_exists.tmp";
    {
        std::ofstream ofs(tmp);
        ofs << "hi";
    }

    auto result = check_file_exists(tmp);
    EXPECT_FALSE(result.is_err());

    std::error_code ec;
    std::filesystem::remove(tmp, ec);
}

TEST(CheckFileExistsTest, ReturnsErrorForMissingFile) {
    auto tmp = std::filesystem::temp_directory_path() /
               "logger_error_util_missing_file.tmp";
    // Ensure it does not exist
    std::error_code ec;
    std::filesystem::remove(tmp, ec);

    auto result = check_file_exists(tmp);
    ASSERT_TRUE(result.is_err());
    EXPECT_EQ(get_logger_error_code(result),
              logger_error_code::file_open_failed);
}

// =============================================================================
// ensure_directory_exists
// =============================================================================

TEST(EnsureDirectoryExistsTest, EmptyPathReturnsOk) {
    auto result = ensure_directory_exists(std::filesystem::path{});
    EXPECT_FALSE(result.is_err());
}

TEST(EnsureDirectoryExistsTest, ExistingDirectoryReturnsOk) {
    auto base = std::filesystem::temp_directory_path() /
                "logger_error_util_dir_existing";
    std::filesystem::create_directories(base);

    auto result = ensure_directory_exists(base);
    EXPECT_FALSE(result.is_err());

    std::error_code ec;
    std::filesystem::remove_all(base, ec);
}

TEST(EnsureDirectoryExistsTest, CreatesMissingDirectory) {
    auto base = std::filesystem::temp_directory_path() /
                "logger_error_util_dir_new" /
                "nested" /
                "child";
    std::error_code ec;
    std::filesystem::remove_all(
        std::filesystem::temp_directory_path() /
            "logger_error_util_dir_new",
        ec);

    auto result = ensure_directory_exists(base);
    EXPECT_FALSE(result.is_err());
    EXPECT_TRUE(std::filesystem::exists(base));

    std::filesystem::remove_all(
        std::filesystem::temp_directory_path() /
            "logger_error_util_dir_new",
        ec);
}

// =============================================================================
// safe_destructor_operation
// =============================================================================

TEST(SafeDestructorOperationTest, SwallowsStandardException) {
    // Redirect stderr so test output stays clean.
    std::stringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    int ran = 0;
    EXPECT_NO_THROW(
        safe_destructor_operation("cleanup", [&ran]() {
            ++ran;
            throw std::runtime_error("boom");
        })
    );

    std::cerr.rdbuf(original);
    EXPECT_EQ(ran, 1);
    EXPECT_NE(captured.str().find("cleanup"), std::string::npos);
}

TEST(SafeDestructorOperationTest, SwallowsNonStandardException) {
    std::stringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    EXPECT_NO_THROW(
        safe_destructor_operation("shutdown", []() {
            throw 7;
        })
    );

    std::cerr.rdbuf(original);
    EXPECT_NE(captured.str().find("shutdown"), std::string::npos);
}

TEST(SafeDestructorOperationTest, RunsOperationWhenNoException) {
    int ran = 0;
    EXPECT_NO_THROW(
        safe_destructor_operation("ok_op", [&ran]() { ++ran; })
    );
    EXPECT_EQ(ran, 1);
}

// =============================================================================
// safe_destructor_result_operation
// =============================================================================

TEST(SafeDestructorResultOperationTest, LogsErrorResult) {
    std::stringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    EXPECT_NO_THROW(
        safe_destructor_result_operation("flush", []() -> common::VoidResult {
            return make_logger_void_result(
                logger_error_code::flush_timeout, "timed out");
        })
    );

    std::cerr.rdbuf(original);
    std::string output = captured.str();
    EXPECT_NE(output.find("flush"), std::string::npos);
    EXPECT_NE(output.find("timed out"), std::string::npos);
}

TEST(SafeDestructorResultOperationTest, SwallowsException) {
    std::stringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    EXPECT_NO_THROW(
        safe_destructor_result_operation(
            "close", []() -> common::VoidResult {
                throw std::runtime_error("ouch");
            })
    );

    std::cerr.rdbuf(original);
    EXPECT_NE(captured.str().find("close"), std::string::npos);
}

TEST(SafeDestructorResultOperationTest, SilentOnSuccess) {
    std::stringstream captured;
    std::streambuf* original = std::cerr.rdbuf(captured.rdbuf());

    EXPECT_NO_THROW(
        safe_destructor_result_operation(
            "ok", []() -> common::VoidResult {
                return common::ok();
            })
    );

    std::cerr.rdbuf(original);
    EXPECT_TRUE(captured.str().empty());
}
