##################################################
# Logger System Tests
##################################################

# Enable testing
enable_testing()

# Find GTest
find_package(GTest CONFIG REQUIRED)

# Test source files
set(TEST_SOURCES
    # Add test files here when they are created
    # logger_test.cpp
    # writer_test.cpp
)

# Create test executable only if test sources exist
if(TEST_SOURCES)
    add_executable(logger_tests ${TEST_SOURCES})
    
    target_link_libraries(logger_tests
        PRIVATE
            logger
            GTest::gtest
            GTest::gtest_main
            GTest::gmock
            GTest::gmock_main
    )
    
    target_include_directories(logger_tests
        PRIVATE
            ${CMAKE_SOURCE_DIR}/sources
    )
    
    # Add test
    add_test(NAME logger_tests COMMAND logger_tests)
else()
    # Create placeholder message for now
    message(STATUS "No test sources found - skipping test build")
endif()