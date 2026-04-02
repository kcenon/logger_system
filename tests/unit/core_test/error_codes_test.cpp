// BSD 3-Clause License
// Copyright (c) 2025, 🍀☀🌕🌥 🌊
// See the LICENSE file in the project root for full license information.

/**
 * @file error_codes_test.cpp
 * @brief Unit tests for logger error codes and helper functions
 * @since 4.0.0
 */

#include <gtest/gtest.h>

#include <kcenon/logger/core/error_codes.h>

#include <string>

using namespace kcenon::logger;
namespace common = kcenon::common;

// =============================================================================
// logger_error_to_string
// =============================================================================

TEST(ErrorCodesTest, SuccessToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::success), "Success");
}

TEST(ErrorCodesTest, GeneralErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::unknown_error), "Unknown error");
    EXPECT_EQ(logger_error_to_string(logger_error_code::not_implemented), "Not implemented");
    EXPECT_EQ(logger_error_to_string(logger_error_code::invalid_argument), "Invalid argument");
}

TEST(ErrorCodesTest, WriterErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_not_found), "Writer not found");
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_initialization_failed), "Writer initialization failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_already_exists), "Writer already exists");
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_not_healthy), "Writer not healthy");
}

TEST(ErrorCodesTest, FileErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::file_open_failed), "Failed to open file");
    EXPECT_EQ(logger_error_to_string(logger_error_code::file_write_failed), "Failed to write to file");
    EXPECT_EQ(logger_error_to_string(logger_error_code::file_rotation_failed), "File rotation failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::file_permission_denied), "File permission denied");
}

TEST(ErrorCodesTest, NetworkErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::network_connection_failed), "Network connection failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::network_send_failed), "Network send failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::network_timeout), "Network timeout");
}

TEST(ErrorCodesTest, BufferErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::buffer_overflow), "Buffer overflow");
    EXPECT_EQ(logger_error_to_string(logger_error_code::queue_full), "Queue is full");
    EXPECT_EQ(logger_error_to_string(logger_error_code::queue_stopped), "Queue is stopped");
    EXPECT_EQ(logger_error_to_string(logger_error_code::queue_overflow_dropped), "Queue overflow: messages dropped");
    EXPECT_EQ(logger_error_to_string(logger_error_code::queue_overflow_blocked), "Queue overflow: operation blocked");
}

TEST(ErrorCodesTest, ConfigurationErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::invalid_configuration), "Invalid configuration");
    EXPECT_EQ(logger_error_to_string(logger_error_code::configuration_missing), "Configuration missing");
    EXPECT_EQ(logger_error_to_string(logger_error_code::configuration_conflict), "Configuration conflict");
}

TEST(ErrorCodesTest, MetricsErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::metrics_collection_failed), "Metrics collection failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::metrics_not_available), "Metrics not available");
}

TEST(ErrorCodesTest, ProcessingErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::flush_timeout), "Flush timeout");
    EXPECT_EQ(logger_error_to_string(logger_error_code::processing_failed), "Processing failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::filter_error), "Filter error");
    EXPECT_EQ(logger_error_to_string(logger_error_code::formatter_error), "Formatter error");
    EXPECT_EQ(logger_error_to_string(logger_error_code::batch_processing_timeout), "Batch processing timeout");
    EXPECT_EQ(logger_error_to_string(logger_error_code::batch_processing_failed), "Batch processing failed");
}

TEST(ErrorCodesTest, SecurityErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::encryption_failed), "Encryption failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::decryption_failed), "Decryption failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::authentication_failed), "Authentication failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::sanitization_failed), "Sanitization failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::path_traversal_detected), "Path traversal attack detected");
    EXPECT_EQ(logger_error_to_string(logger_error_code::invalid_key_size), "Invalid encryption key size");
    EXPECT_EQ(logger_error_to_string(logger_error_code::invalid_filename), "Invalid filename");
}

TEST(ErrorCodesTest, DIContainerErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::di_not_available), "DI container not available");
    EXPECT_EQ(logger_error_to_string(logger_error_code::component_not_found), "Component not found in DI container");
    EXPECT_EQ(logger_error_to_string(logger_error_code::registration_failed), "Failed to register component in DI container");
    EXPECT_EQ(logger_error_to_string(logger_error_code::creation_failed), "Failed to create component from factory");
    EXPECT_EQ(logger_error_to_string(logger_error_code::operation_failed), "DI container operation failed");
}

TEST(ErrorCodesTest, WriterExtendedErrorCodesToString) {
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_not_available), "Writer not available");
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_configuration_error), "Writer configuration error");
    EXPECT_EQ(logger_error_to_string(logger_error_code::writer_operation_failed), "Writer operation failed");
    EXPECT_EQ(logger_error_to_string(logger_error_code::destructor_cleanup_failed), "Destructor cleanup failed");
}

// =============================================================================
// make_logger_void_result
// =============================================================================

TEST(ErrorCodesTest, MakeLoggerVoidResultError) {
    auto result = make_logger_void_result(logger_error_code::file_open_failed, "test.log");
    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(result.error().code, static_cast<int>(logger_error_code::file_open_failed));
    EXPECT_EQ(result.error().message, "test.log");
}

TEST(ErrorCodesTest, MakeLoggerVoidResultDefaultMessage) {
    auto result = make_logger_void_result(logger_error_code::queue_full);
    EXPECT_TRUE(result.is_err());
    EXPECT_EQ(result.error().message, "Queue is full");
}

// =============================================================================
// make_logger_void_success
// =============================================================================

TEST(ErrorCodesTest, MakeLoggerVoidSuccess) {
    auto result = make_logger_void_success();
    EXPECT_TRUE(result.is_ok());
    EXPECT_FALSE(result.is_err());
}

// =============================================================================
// get_logger_error_code
// =============================================================================

TEST(ErrorCodesTest, GetLoggerErrorCodeFromError) {
    auto result = make_logger_void_result(logger_error_code::network_timeout);
    auto code = get_logger_error_code(result);
    EXPECT_EQ(code, logger_error_code::network_timeout);
}

TEST(ErrorCodesTest, GetLoggerErrorCodeFromSuccess) {
    auto result = make_logger_void_success();
    auto code = get_logger_error_code(result);
    EXPECT_EQ(code, logger_error_code::success);
}

// =============================================================================
// has_logger_error
// =============================================================================

TEST(ErrorCodesTest, HasLoggerErrorTrue) {
    auto result = make_logger_void_result(logger_error_code::buffer_overflow);
    EXPECT_TRUE(has_logger_error(result));
}

TEST(ErrorCodesTest, HasLoggerErrorFalse) {
    auto result = make_logger_void_success();
    EXPECT_FALSE(has_logger_error(result));
}

// =============================================================================
// get_logger_error_message
// =============================================================================

TEST(ErrorCodesTest, GetLoggerErrorMessageFromError) {
    auto result = make_logger_void_result(logger_error_code::encryption_failed, "AES error");
    auto msg = get_logger_error_message(result);
    EXPECT_EQ(msg, "AES error");
}

TEST(ErrorCodesTest, GetLoggerErrorMessageFromSuccess) {
    auto result = make_logger_void_success();
    auto msg = get_logger_error_message(result);
    EXPECT_EQ(msg, "");
}

// =============================================================================
// result<T> wrapper
// =============================================================================

TEST(ErrorCodesTest, ResultWithValue) {
    result<int> res(42);
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), 42);
}

TEST(ErrorCodesTest, ResultWithError) {
    result<int> res(logger_error_code::invalid_argument, "bad input");
    EXPECT_FALSE(res.has_value());
    EXPECT_EQ(res.error_code(), logger_error_code::invalid_argument);
    EXPECT_EQ(res.error_message(), "bad input");
}

TEST(ErrorCodesTest, ResultBoolConversion) {
    result<int> ok_res(42);
    result<int> err_res(logger_error_code::unknown_error);

    EXPECT_TRUE(static_cast<bool>(ok_res));
    EXPECT_FALSE(static_cast<bool>(err_res));
}

TEST(ErrorCodesTest, ResultDefaultErrorMessage) {
    result<std::string> res(logger_error_code::file_open_failed);
    EXPECT_EQ(res.error_message(), "Failed to open file");
}

TEST(ErrorCodesTest, ResultStringValue) {
    result<std::string> res(std::string("hello"));
    EXPECT_TRUE(res.has_value());
    EXPECT_EQ(res.value(), "hello");
}
