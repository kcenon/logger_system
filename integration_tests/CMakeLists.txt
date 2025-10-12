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

if(TARGET GTest::gtest)
    set(_LOGGER_GTEST_TARGET GTest::gtest)
elseif(TARGET gtest)
    set(_LOGGER_GTEST_TARGET gtest)
endif()

if(TARGET GTest::gtest_main)
    set(_LOGGER_GTEST_MAIN_TARGET GTest::gtest_main)
elseif(TARGET gtest_main)
    set(_LOGGER_GTEST_MAIN_TARGET gtest_main)
endif()

if(NOT _LOGGER_GTEST_TARGET OR NOT _LOGGER_GTEST_MAIN_TARGET)
    find_package(GTest QUIET)
    if(GTest_FOUND)
        if(NOT _LOGGER_GTEST_TARGET AND TARGET GTest::gtest)
            set(_LOGGER_GTEST_TARGET GTest::gtest)
        elseif(NOT _LOGGER_GTEST_TARGET AND TARGET gtest)
            set(_LOGGER_GTEST_TARGET gtest)
        endif()
        if(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET GTest::gtest_main)
            set(_LOGGER_GTEST_MAIN_TARGET GTest::gtest_main)
        elseif(NOT _LOGGER_GTEST_MAIN_TARGET AND TARGET gtest_main)
            set(_LOGGER_GTEST_MAIN_TARGET gtest_main)
        endif()
    endif()
endif()

if(NOT _LOGGER_GTEST_TARGET OR NOT _LOGGER_GTEST_MAIN_TARGET)
    message(FATAL_ERROR "GoogleTest targets not available. Verify logger_find_test_dependencies() ran before configuring integration tests.")
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

# Link libraries
target_link_libraries(integration_tests PRIVATE
    ${_LOGGER_GTEST_TARGET}
    ${_LOGGER_GTEST_MAIN_TARGET}
    LoggerSystem  # Correct target name
)

# Compiler settings
target_compile_features(integration_tests PRIVATE cxx_std_20)

# Add test to CTest
add_test(NAME IntegrationTests COMMAND integration_tests)

# Set test properties
set_tests_properties(IntegrationTests PROPERTIES
    TIMEOUT 600
    LABELS "integration"
)

# Install integration tests (optional)
install(TARGETS integration_tests
    RUNTIME DESTINATION bin/tests
)

message(STATUS "Integration tests configured successfully")
