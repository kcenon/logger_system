##################################################
# Integration Tests CMakeLists.txt
#
# Configuration for logger_system integration tests
##################################################

cmake_minimum_required(VERSION 3.16)

# Find dependencies
find_package(GTest REQUIRED)

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
    GTest::gtest
    GTest::gtest_main
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
