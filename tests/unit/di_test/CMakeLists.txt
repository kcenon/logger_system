##################################################
# DI Container Tests CMakeLists.txt
#
# Tests validating logger_system integration with
# common_system's service_container for DI.
##################################################

# Define the test executable
add_executable(di_container_test
    di_container_test.cpp
)

# Link with required libraries
# LoggerSystem provides common_system transitively (PUBLIC link)
target_link_libraries(di_container_test PRIVATE
    LoggerSystem
    GTest::gtest
    GTest::gtest_main
    Threads::Threads
)

# Set compile features
target_compile_features(di_container_test PRIVATE cxx_std_20)

# Add to CTest
add_test(NAME di_container_test COMMAND di_container_test)

# Set test properties
set_tests_properties(di_container_test PROPERTIES
    TIMEOUT 30
    LABELS "unit;di"
)

# Enable verbose output for debugging if needed
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(di_container_test PRIVATE
        DEBUG_MODE=1
    )
endif()
