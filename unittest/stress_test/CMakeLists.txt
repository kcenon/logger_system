##################################################
# Stress Test CMakeLists.txt
# 
# Builds stress tests for the logger system to validate
# performance and stability under extreme load conditions.
##################################################

# Create stress test executable
add_executable(stress_test
    stress_test.cpp
)

# Link with logger library and dependencies
target_link_libraries(stress_test
    PRIVATE
        logger_system
        GTest::gtest
        GTest::gtest_main
        Threads::Threads
)

# Include directories
target_include_directories(stress_test
    PRIVATE
        ${CMAKE_SOURCE_DIR}/sources
        ${CMAKE_SOURCE_DIR}/unittest
)

# Add to CTest
add_test(
    NAME stress_test
    COMMAND stress_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)

# Set test properties
set_tests_properties(stress_test PROPERTIES
    TIMEOUT 300  # 5 minutes timeout for stress tests
    LABELS "stress;performance"
)

# Platform-specific settings
if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    target_compile_definitions(stress_test PRIVATE LINUX_PLATFORM)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    target_compile_definitions(stress_test PRIVATE MACOS_PLATFORM)
endif()

# Enable sanitizers in debug mode
if(CMAKE_BUILD_TYPE STREQUAL "Debug" AND LOGGER_ENABLE_SANITIZERS)
    target_compile_options(stress_test PRIVATE
        -fsanitize=address
        -fsanitize=thread
        -fsanitize=undefined
    )
    target_link_options(stress_test PRIVATE
        -fsanitize=address
        -fsanitize=thread
        -fsanitize=undefined
    )
endif()