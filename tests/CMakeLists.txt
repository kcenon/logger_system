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
    add_executable(logger_security_test unit/security_test/security_test.cpp)

    if(TARGET GTest::gtest_main)
        target_link_libraries(logger_security_test PRIVATE LoggerSystem GTest::gtest_main GTest::gtest)
    else()
        target_link_libraries(logger_security_test PRIVATE LoggerSystem gtest_main gtest)
    endif()

    # Link OpenSSL if available
    find_package(OpenSSL)
    if(OpenSSL_FOUND)
        target_link_libraries(logger_security_test PRIVATE OpenSSL::SSL OpenSSL::Crypto)
        message(STATUS "Security tests: OpenSSL linked for secure key storage")
    else()
        message(WARNING "Security tests: OpenSSL not found, using fallback implementations")
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
