# Basic integration test
add_executable(basic_integration_test integration_test.cpp)

# Link with GTest
if(TARGET GTest::gtest_main)
    target_link_libraries(basic_integration_test PRIVATE LoggerSystem GTest::gtest_main)
else()
    target_link_libraries(basic_integration_test PRIVATE LoggerSystem gtest_main)
endif()

add_test(NAME basic_integration_test COMMAND basic_integration_test)
set_target_properties(basic_integration_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Thread system integration test
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/integration/thread_system_integration_test.cpp")
    add_executable(thread_system_integration_test integration/thread_system_integration_test.cpp)

    if(TARGET GTest::gtest_main)
        target_link_libraries(thread_system_integration_test PRIVATE LoggerSystem GTest::gtest_main)
    else()
        target_link_libraries(thread_system_integration_test PRIVATE LoggerSystem gtest_main)
    endif()

    target_compile_definitions(thread_system_integration_test PRIVATE USE_THREAD_SYSTEM)

    add_test(NAME thread_system_integration_test COMMAND thread_system_integration_test)
    set_target_properties(thread_system_integration_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Monitoring system integration test
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/integration/monitoring_integration_test.cpp")
    add_executable(monitoring_integration_test integration/monitoring_integration_test.cpp)

    if(TARGET GTest::gtest_main)
        target_link_libraries(monitoring_integration_test PRIVATE LoggerSystem GTest::gtest_main)
    else()
        target_link_libraries(monitoring_integration_test PRIVATE LoggerSystem gtest_main)
    endif()

    add_test(NAME monitoring_integration_test COMMAND monitoring_integration_test)
    set_target_properties(monitoring_integration_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Version compatibility test
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/compatibility/version_compatibility_test.cpp")
    add_executable(version_compatibility_test compatibility/version_compatibility_test.cpp)

    if(TARGET GTest::gtest_main)
        target_link_libraries(version_compatibility_test PRIVATE LoggerSystem GTest::gtest_main)
    else()
        target_link_libraries(version_compatibility_test PRIVATE LoggerSystem gtest_main)
    endif()

    add_test(NAME version_compatibility_test COMMAND version_compatibility_test)
    set_target_properties(version_compatibility_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Regression test ensuring minimum level filtering behaviour
add_executable(logger_min_level_threshold_test min_level_filter_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_min_level_threshold_test PRIVATE LoggerSystem GTest::gtest_main)
else()
    target_link_libraries(logger_min_level_threshold_test PRIVATE LoggerSystem gtest_main)
endif()

add_test(NAME logger_min_level_threshold_test COMMAND logger_min_level_threshold_test)
set_target_properties(logger_min_level_threshold_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Thread safety tests (Phase 1 - Task 1.1)
add_executable(logger_thread_safety_test thread_safety_tests.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_thread_safety_test PRIVATE LoggerSystem GTest::gtest_main)
else()
    target_link_libraries(logger_thread_safety_test PRIVATE LoggerSystem gtest_main)
endif()

add_test(NAME logger_thread_safety_test COMMAND logger_thread_safety_test)
set_target_properties(logger_thread_safety_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Log sampling tests (Issue #282)
add_executable(logger_sampling_test log_sampling_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_sampling_test PRIVATE LoggerSystem GTest::gtest_main)
else()
    target_link_libraries(logger_sampling_test PRIVATE LoggerSystem gtest_main)
endif()

add_test(NAME logger_sampling_test COMMAND logger_sampling_test)
set_target_properties(logger_sampling_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Routing integration tests (Issue #278)
add_executable(logger_routing_integration_test routing_integration_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_routing_integration_test PRIVATE LoggerSystem GTest::gtest_main)
else()
    target_link_libraries(logger_routing_integration_test PRIVATE LoggerSystem gtest_main)
endif()

add_test(NAME logger_routing_integration_test COMMAND logger_routing_integration_test)
set_target_properties(logger_routing_integration_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# ILogger interface compatibility tests (Phase 2 - Issue #223)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/ilogger_interface_test.cpp")
    add_executable(logger_ilogger_interface_test unit/ilogger_interface_test.cpp)

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_ilogger_interface_test PRIVATE LoggerSystem GTest::gtest_main)
    else()
        target_link_libraries(logger_ilogger_interface_test PRIVATE LoggerSystem gtest_main)
    endif()

    add_test(NAME logger_ilogger_interface_test COMMAND logger_ilogger_interface_test)
    set_target_properties(logger_ilogger_interface_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Security tests (Phase 4 - Task 4.1-4.4)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/security_test/security_test.cpp")
    add_executable(logger_security_test
        unit/security_test/security_test.cpp
        unit/security_test/log_sanitizer_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_security_test PRIVATE LoggerSystem GTest::gtest_main GTest::gtest)
    else()
        target_link_libraries(logger_security_test PRIVATE LoggerSystem gtest_main gtest)
    endif()

    # Link OpenSSL if available (prefer 3.x for security)
    find_package(OpenSSL 3.0 QUIET)
    if(OpenSSL_FOUND)
        target_link_libraries(logger_security_test PRIVATE OpenSSL::SSL OpenSSL::Crypto)
        message(STATUS "Security tests: OpenSSL 3.x linked (${OPENSSL_VERSION})")
    else()
        find_package(OpenSSL QUIET)
        if(OpenSSL_FOUND)
            target_link_libraries(logger_security_test PRIVATE OpenSSL::SSL OpenSSL::Crypto)
            if(OPENSSL_VERSION VERSION_LESS "3.0.0")
                message(WARNING "Security tests: OpenSSL ${OPENSSL_VERSION} is EOL. Consider upgrading to 3.x")
            else()
                message(STATUS "Security tests: OpenSSL linked (${OPENSSL_VERSION})")
            endif()
        else()
            message(WARNING "Security tests: OpenSSL not found, using fallback implementations")
        endif()
    endif()

    # Link standard C++ library explicitly on macOS
    if(APPLE)
        target_link_libraries(logger_security_test PRIVATE c++)
    endif()

    add_test(NAME logger_security_test COMMAND logger_security_test)
    set_target_properties(logger_security_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# thread_system_integration module tests (Issue #224)
# Tests for optional thread_system integration module
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/thread_system_integration_module_test.cpp")
    add_executable(logger_thread_system_integration_module_test
        unit/thread_system_integration_module_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_thread_system_integration_module_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_thread_system_integration_module_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    # Link thread_system if available for full integration tests
    if(THREAD_SYSTEM_FOUND)
        if(TARGET ThreadSystem)
            target_link_libraries(logger_thread_system_integration_module_test
                PRIVATE ThreadSystem
            )
        elseif(TARGET thread_system)
            target_link_libraries(logger_thread_system_integration_module_test
                PRIVATE thread_system
            )
        endif()
    endif()

    add_test(NAME logger_thread_system_integration_module_test
        COMMAND logger_thread_system_integration_module_test
    )
    set_target_properties(logger_thread_system_integration_module_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "thread_system_integration module tests: Added (THREAD_SYSTEM_FOUND=${THREAD_SYSTEM_FOUND})")
endif()

# OpenTelemetry integration tests (Issue #283)
add_executable(logger_otlp_test otlp_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_otlp_test PRIVATE LoggerSystem GTest::gtest_main GTest::gtest)
else()
    target_link_libraries(logger_otlp_test PRIVATE LoggerSystem gtest_main gtest)
endif()

# Link OpenTelemetry if available
if(LOGGER_ENABLE_OTLP AND opentelemetry-cpp_FOUND)
    target_link_libraries(logger_otlp_test PRIVATE
        opentelemetry-cpp::logs
        opentelemetry-cpp::otlp_http_log_record_exporter
    )
    target_compile_definitions(logger_otlp_test PRIVATE LOGGER_HAS_OTLP=1)
    message(STATUS "OTLP tests: OpenTelemetry SDK linked")
else()
    message(STATUS "OTLP tests: Using fallback implementation (no OTLP SDK)")
endif()

add_test(NAME logger_otlp_test COMMAND logger_otlp_test)
set_target_properties(logger_otlp_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# Encrypted writer tests (Issue #280)
add_executable(logger_encrypted_writer_test encrypted_writer_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_encrypted_writer_test PRIVATE LoggerSystem GTest::gtest_main GTest::gtest)
else()
    target_link_libraries(logger_encrypted_writer_test PRIVATE LoggerSystem gtest_main gtest)
endif()

# Link OpenSSL if available
find_package(OpenSSL 3.0 QUIET)
if(OpenSSL_FOUND)
    target_link_libraries(logger_encrypted_writer_test PRIVATE OpenSSL::SSL OpenSSL::Crypto)
    message(STATUS "Encrypted writer tests: OpenSSL 3.x linked (${OPENSSL_VERSION})")
else()
    find_package(OpenSSL QUIET)
    if(OpenSSL_FOUND)
        target_link_libraries(logger_encrypted_writer_test PRIVATE OpenSSL::SSL OpenSSL::Crypto)
        message(STATUS "Encrypted writer tests: OpenSSL linked (${OPENSSL_VERSION})")
    else()
        message(WARNING "Encrypted writer tests: OpenSSL not found, encryption tests will be skipped")
    endif()
endif()

# Link standard C++ library explicitly on macOS
if(APPLE)
    target_link_libraries(logger_encrypted_writer_test PRIVATE c++)
endif()

add_test(NAME logger_encrypted_writer_test COMMAND logger_encrypted_writer_test)
set_target_properties(logger_encrypted_writer_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

# executor_integration module tests (Issue #253)
# Tests for IExecutor-based async integration
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/executor_integration_test.cpp")
    add_executable(logger_executor_integration_test
        unit/executor_integration_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_executor_integration_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_executor_integration_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_executor_integration_test
        COMMAND logger_executor_integration_test
    )
    set_target_properties(logger_executor_integration_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "executor_integration module tests: Added")
endif()

# Structured logging tests (Issue #279)
add_executable(logger_structured_logging_test structured_logging_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_structured_logging_test PRIVATE LoggerSystem GTest::gtest_main GTest::gtest)
else()
    target_link_libraries(logger_structured_logging_test PRIVATE LoggerSystem gtest_main gtest)
endif()

add_test(NAME logger_structured_logging_test COMMAND logger_structured_logging_test)
set_target_properties(logger_structured_logging_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Structured logging tests: Added")

# Real-time analyzer tests (Issue #281)
add_executable(logger_realtime_analyzer_test realtime_analyzer_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_realtime_analyzer_test PRIVATE LoggerSystem GTest::gtest_main GTest::gtest)
else()
    target_link_libraries(logger_realtime_analyzer_test PRIVATE LoggerSystem gtest_main gtest)
endif()

add_test(NAME logger_realtime_analyzer_test COMMAND logger_realtime_analyzer_test)
set_target_properties(logger_realtime_analyzer_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

message(STATUS "Real-time analyzer tests: Added")

# Filtered writer decorator tests (Issue #358 - Decorator pattern refactoring)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/filtered_writer_test.cpp")
    add_executable(logger_filtered_writer_test
        unit/writers_test/filtered_writer_test.cpp
    )

    if(TARGET GTest::gtest_main AND TARGET GTest::gmock)
        target_link_libraries(logger_filtered_writer_test
            PRIVATE LoggerSystem GTest::gtest_main GTest::gmock
        )
    elseif(TARGET GTest::gtest_main)
        target_link_libraries(logger_filtered_writer_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_filtered_writer_test
            PRIVATE LoggerSystem gtest_main gmock
        )
    endif()

    add_test(NAME logger_filtered_writer_test
        COMMAND logger_filtered_writer_test
    )
    set_target_properties(logger_filtered_writer_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Filtered writer decorator tests: Added")
endif()

# Decorator writer base tests (Issue #361 - Decorator pattern refactoring Part 1)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/decorator_writer_base_test.cpp")
    add_executable(logger_decorator_writer_base_test
        unit/writers_test/decorator_writer_base_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_decorator_writer_base_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_decorator_writer_base_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_decorator_writer_base_test
        COMMAND logger_decorator_writer_base_test
    )
    set_target_properties(logger_decorator_writer_base_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Decorator writer base tests: Added")
endif()

# Buffered writer decorator tests (Issue #363 - Decorator pattern refactoring Part 2)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/buffered_writer_test.cpp")
    add_executable(logger_buffered_writer_test
        unit/writers_test/buffered_writer_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_buffered_writer_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_buffered_writer_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_buffered_writer_test
        COMMAND logger_buffered_writer_test
    )
    set_target_properties(logger_buffered_writer_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Buffered writer decorator tests: Added")
endif()

# Formatted writer decorator tests (Issue #365 - Decorator pattern refactoring Part 3)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/formatted_writer_test.cpp")
    add_executable(logger_formatted_writer_test
        unit/writers_test/formatted_writer_test.cpp
    )

    if(TARGET GTest::gtest_main AND TARGET GTest::gmock)
        target_link_libraries(logger_formatted_writer_test
            PRIVATE LoggerSystem GTest::gtest_main GTest::gmock
        )
    elseif(TARGET GTest::gtest_main)
        target_link_libraries(logger_formatted_writer_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_formatted_writer_test
            PRIVATE LoggerSystem gtest_main gmock
        )
    endif()

    add_test(NAME logger_formatted_writer_test
        COMMAND logger_formatted_writer_test
    )
    set_target_properties(logger_formatted_writer_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Formatted writer decorator tests: Added")
endif()

##################################################
# Automatic Coverage Registration
#
# Register all test targets for coverage instrumentation.
# This eliminates the need to manually maintain a static list in LoggerCoverage.cmake.
# New test targets added above will automatically be included in coverage reports.
##################################################

# List of all test targets defined in this file
set(_LOGGER_TEST_TARGETS
    basic_integration_test
    logger_min_level_threshold_test
    logger_thread_safety_test
    logger_routing_integration_test
    logger_otlp_test
    logger_encrypted_writer_test
    logger_structured_logging_test
    logger_realtime_analyzer_test
)

# Conditionally added targets
if(TARGET thread_system_integration_test)
    list(APPEND _LOGGER_TEST_TARGETS thread_system_integration_test)
endif()

if(TARGET monitoring_integration_test)
    list(APPEND _LOGGER_TEST_TARGETS monitoring_integration_test)
endif()

if(TARGET version_compatibility_test)
    list(APPEND _LOGGER_TEST_TARGETS version_compatibility_test)
endif()

if(TARGET logger_ilogger_interface_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_ilogger_interface_test)
endif()

if(TARGET logger_security_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_security_test)
endif()

if(TARGET logger_thread_system_integration_module_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_thread_system_integration_module_test)
endif()

if(TARGET logger_executor_integration_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_executor_integration_test)
endif()

if(TARGET logger_filtered_writer_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_filtered_writer_test)
endif()

if(TARGET logger_decorator_writer_base_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_decorator_writer_base_test)
endif()

if(TARGET logger_buffered_writer_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_buffered_writer_test)
endif()

if(TARGET logger_formatted_writer_test)
    list(APPEND _LOGGER_TEST_TARGETS logger_formatted_writer_test)
endif()

# Register all test targets for coverage instrumentation
# The logger_register_coverage_target function is defined in cmake/LoggerCoverage.cmake
foreach(_test_target IN LISTS _LOGGER_TEST_TARGETS)
    if(TARGET ${_test_target} AND COMMAND logger_register_coverage_target)
        logger_register_coverage_target(${_test_target})
    endif()
endforeach()

# Unified log context tests (Issue #375)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/core_test/unified_log_context_test.cpp")
    add_executable(logger_unified_log_context_test
        unit/core_test/unified_log_context_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_unified_log_context_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_unified_log_context_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_unified_log_context_test
        COMMAND logger_unified_log_context_test
    )
    set_target_properties(logger_unified_log_context_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Unified log context tests: Added")
endif()

# Scoped context guard tests (Issue #387)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/core_test/scoped_context_guard_test.cpp")
    add_executable(logger_scoped_context_guard_test
        unit/core_test/scoped_context_guard_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_scoped_context_guard_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_scoped_context_guard_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_scoped_context_guard_test
        COMMAND logger_scoped_context_guard_test
    )
    set_target_properties(logger_scoped_context_guard_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Scoped context guard tests: Added")
endif()

# Log entry structure tests (Issue #392)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/interfaces_test/log_entry_test.cpp")
    add_executable(logger_log_entry_test
        unit/interfaces_test/log_entry_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_log_entry_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_log_entry_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_log_entry_test
        COMMAND logger_log_entry_test
    )
    set_target_properties(logger_log_entry_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Log entry structure tests: Added")
endif()

# Writer builder tests (Issue #413 - Fluent builder for decorator composition)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/writer_builder_test.cpp")
    add_executable(logger_writer_builder_test
        unit/writers_test/writer_builder_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_writer_builder_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_writer_builder_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_writer_builder_test
        COMMAND logger_writer_builder_test
    )
    set_target_properties(logger_writer_builder_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Writer builder tests: Added")
endif()

# Batch writer tests (Issue #440 - batch_writer unit tests)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/batch_writer_test.cpp")
    add_executable(logger_batch_writer_test
        unit/writers_test/batch_writer_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_batch_writer_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_batch_writer_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_batch_writer_test
        COMMAND logger_batch_writer_test
    )
    set_target_properties(logger_batch_writer_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Batch writer tests: Added")
endif()

# Thread-safe writer tests (Issue #440 - thread_safe_writer unit tests)
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/unit/writers_test/thread_safe_writer_test.cpp")
    add_executable(logger_thread_safe_writer_test
        unit/writers_test/thread_safe_writer_test.cpp
    )

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_thread_safe_writer_test
            PRIVATE LoggerSystem GTest::gtest_main
        )
    else()
        target_link_libraries(logger_thread_safe_writer_test
            PRIVATE LoggerSystem gtest_main
        )
    endif()

    add_test(NAME logger_thread_safe_writer_test
        COMMAND logger_thread_safe_writer_test
    )
    set_target_properties(logger_thread_safe_writer_test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Thread-safe writer tests: Added")
endif()

# Cleanup
unset(_LOGGER_TEST_TARGETS)
unset(_test_target)
