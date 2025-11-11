##################################################
# Integration Tests CMakeLists.txt
#
# Configuration for logger_system integration tests
##################################################

cmake_minimum_required(VERSION 3.16)

# Ensure GoogleTest targets are available. The top-level CMake script populates
# GTest via logger_find_test_dependencies(); fall back to find_package only when
# the targets are still missing (e.g. system installations).
set(_LOGGER_GTEST_TARGET "")
set(_LOGGER_GTEST_MAIN_TARGET "")

foreach(_candidate IN ITEMS GTest::gtest GTest::GTest gtest)
    if(NOT _LOGGER_GTEST_TARGET AND TARGET ${_candidate})
        set(_LOGGER_GTEST_TARGET ${_candidate})
    endif()
endforeach()

foreach(_candidate IN ITEMS GTest::gtest_main GTest::Main gtest_main)
    if(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET ${_candidate})
        set(_LOGGER_GTEST_MAIN_TARGET ${_candidate})
    endif()
endforeach()

if(NOT _LOGGER_GTEST_TARGET OR NOT _LOGGER_GTEST_MAIN_TARGET)
    find_package(GTest QUIET)

    foreach(_candidate IN ITEMS GTest::gtest GTest::GTest gtest)
        if(NOT _LOGGER_GTEST_TARGET AND TARGET ${_candidate})
            set(_LOGGER_GTEST_TARGET ${_candidate})
        endif()
    endforeach()

    foreach(_candidate IN ITEMS GTest::gtest_main GTest::Main gtest_main)
        if(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET ${_candidate})
            set(_LOGGER_GTEST_MAIN_TARGET ${_candidate})
        endif()
    endforeach()
endif()

if(NOT _LOGGER_GTEST_TARGET AND GTEST_LIBRARIES AND GTEST_INCLUDE_DIRS)
    if(NOT TARGET logger_system_gtest_imported)
        add_library(logger_system_gtest_imported INTERFACE)
    endif()
    target_include_directories(logger_system_gtest_imported INTERFACE ${GTEST_INCLUDE_DIRS})
    target_link_libraries(logger_system_gtest_imported INTERFACE ${GTEST_LIBRARIES})
    set(_LOGGER_GTEST_TARGET logger_system_gtest_imported)
endif()

if(NOT _LOGGER_GTEST_MAIN_TARGET)
    if(GTEST_MAIN_LIBRARIES AND GTEST_INCLUDE_DIRS)
        if(NOT TARGET logger_system_gtest_main_imported)
            add_library(logger_system_gtest_main_imported INTERFACE)
        endif()
        target_include_directories(logger_system_gtest_main_imported INTERFACE ${GTEST_INCLUDE_DIRS})
        target_link_libraries(logger_system_gtest_main_imported INTERFACE ${GTEST_MAIN_LIBRARIES})
        set(_LOGGER_GTEST_MAIN_TARGET logger_system_gtest_main_imported)
    elseif(GTEST_BOTH_LIBRARIES AND GTEST_INCLUDE_DIRS)
        if(NOT TARGET logger_system_gtest_main_imported)
            add_library(logger_system_gtest_main_imported INTERFACE)
        endif()
        target_include_directories(logger_system_gtest_main_imported INTERFACE ${GTEST_INCLUDE_DIRS})
        target_link_libraries(logger_system_gtest_main_imported INTERFACE ${GTEST_BOTH_LIBRARIES})
        set(_LOGGER_GTEST_MAIN_TARGET logger_system_gtest_main_imported)
    endif()
endif()

# Final fallback: use the same pattern as tests/CMakeLists.txt
# This ensures compatibility when GTest targets have non-standard names
if(NOT _LOGGER_GTEST_TARGET AND TARGET gtest)
    set(_LOGGER_GTEST_TARGET gtest)
    message(STATUS "Integration tests: Using fallback GTest target 'gtest'")
endif()

if(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET gtest_main)
    set(_LOGGER_GTEST_MAIN_TARGET gtest_main)
    message(STATUS "Integration tests: Using fallback GTest main target 'gtest_main'")
endif()

if(NOT _LOGGER_GTEST_TARGET OR NOT _LOGGER_GTEST_MAIN_TARGET)
    message(WARNING "GoogleTest targets not available. Integration tests will be skipped.")
    message(WARNING "  - GTest target found: ${_LOGGER_GTEST_TARGET}")
    message(WARNING "  - GTest main target found: ${_LOGGER_GTEST_MAIN_TARGET}")
    message(WARNING "Please ensure logger_find_test_dependencies() ran before configuring integration tests.")
    return()
endif()

# Thread sanitizer: Skip integration tests entirely
# Integration tests involve heavy file I/O and hundreds of log messages per test,
# which becomes impractically slow (10+ minutes) under thread sanitizer's 20-100x overhead.
# Thread sanitizer's purpose is correctness verification (race detection), not E2E scenarios.
# Unit tests already provide comprehensive thread safety coverage.
if(CMAKE_CXX_FLAGS MATCHES "fsanitize=thread" OR CMAKE_C_FLAGS MATCHES "fsanitize=thread")
    message(STATUS "Thread sanitizer detected: skipping integration tests (use unit tests for race detection)")
    message(STATUS "  Rationale: Integration tests use file I/O + bulk logging (incompatible with TSan)")
    message(STATUS "  Coverage: Unit tests provide thread safety verification")
    return()
endif()

# Collect all integration test source files
file(GLOB SCENARIO_TESTS scenarios/*.cpp)
file(GLOB FAILURE_TESTS failures/*.cpp)
file(GLOB PERFORMANCE_TESTS performance/*.cpp)

# Create integration test executable
add_executable(integration_tests
    ${SCENARIO_TESTS}
    ${FAILURE_TESTS}
    ${PERFORMANCE_TESTS}
)

# Include directories
target_include_directories(integration_tests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# Add common_system include path (required for Phase 2.2+)
if(COMMON_SYSTEM_INCLUDE_DIR)
    target_include_directories(integration_tests PRIVATE
        ${COMMON_SYSTEM_INCLUDE_DIR}
    )
endif()

# Link common_system (required)
if(common_system_FOUND)
    if(TARGET kcenon::common_system)
        target_link_libraries(integration_tests PRIVATE kcenon::common_system)
    endif()
endif()

# Ensure GTest targets are built before integration_tests
# This prevents "file truncated" errors during parallel builds
add_dependencies(integration_tests ${_LOGGER_GTEST_TARGET} ${_LOGGER_GTEST_MAIN_TARGET})

# Link libraries
# Note: GTest targets may already include gtest, so we link gtest_main which depends on gtest
target_link_libraries(integration_tests PRIVATE
    ${_LOGGER_GTEST_MAIN_TARGET}
    LoggerSystem  # Correct target name
)

# Only link gtest separately if gtest_main doesn't already provide it
# This avoids duplicate library warnings
if(NOT ${_LOGGER_GTEST_MAIN_TARGET} MATCHES "gtest_main")
    target_link_libraries(integration_tests PRIVATE ${_LOGGER_GTEST_TARGET})
endif()

# Compiler settings
target_compile_features(integration_tests PRIVATE cxx_std_20)

# Apply coverage flags if enabled
# Note: This must be done here because integration_tests is added after
# logger_enable_coverage_for_all() is called in the parent CMakeLists.txt
if(LOGGER_ENABLE_COVERAGE AND CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang|AppleClang")
        target_compile_options(integration_tests PRIVATE
            --coverage
            -fprofile-arcs
            -ftest-coverage
            -O0  # No optimization for accurate coverage
            -g   # Debug symbols
        )

        target_link_options(integration_tests PRIVATE
            --coverage
        )

        message(STATUS "Enabled code coverage for integration_tests")
    endif()
endif()

# Add test to CTest
add_test(NAME IntegrationTests COMMAND integration_tests)

# Set test properties
set_tests_properties(IntegrationTests PROPERTIES
    TIMEOUT 1200  # 20 minutes for integration tests (extended for Debug builds and sanitizers)
    LABELS "integration"
)

# Install integration tests (optional)
install(TARGETS integration_tests
    RUNTIME DESTINATION bin/tests
)

message(STATUS "Integration tests configured successfully")
