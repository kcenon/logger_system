# Formatter unit tests (json, logfmt, template, timestamp formatters)
add_executable(logger_formatters_test formatters_test.cpp)

if(TARGET GTest::gtest_main)
    target_link_libraries(logger_formatters_test PRIVATE LoggerSystem GTest::gtest_main)
else()
    target_link_libraries(logger_formatters_test PRIVATE LoggerSystem gtest_main)
endif()

add_test(NAME logger_formatters_test COMMAND logger_formatters_test)
set_target_properties(logger_formatters_test PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
