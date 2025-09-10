# Basic integration test
add_executable(basic_integration_test integration_test.cpp)

# Link with GTest
if(TARGET GTest::gtest_main)
    target_link_libraries(basic_integration_test PRIVATE logger GTest::gtest_main)
else()
    target_link_libraries(basic_integration_test PRIVATE logger gtest_main)
endif()

add_test(NAME basic_integration_test COMMAND basic_integration_test)
set_target_properties(basic_integration_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)