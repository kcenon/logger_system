# Basic usage sample - available when samples are enabled
if(BUILD_SAMPLES)
    add_executable(logger_basic_usage basic_usage.cpp)
    target_link_libraries(logger_basic_usage PRIVATE LoggerSystem)
    set_target_properties(logger_basic_usage PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Critical logging example - demonstrates critical log loss prevention mechanisms
if(BUILD_SAMPLES)
    add_executable(critical_logging_example critical_logging_example.cpp)
    target_link_libraries(critical_logging_example PRIVATE LoggerSystem)
    set_target_properties(critical_logging_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

# Phase 4 DI Pattern examples - available when common_system integration is enabled
if(BUILD_SAMPLES AND BUILD_WITH_COMMON_SYSTEM)
    # DI Pattern example
    add_executable(di_pattern_example di_pattern_example.cpp)
    target_link_libraries(di_pattern_example PRIVATE LoggerSystem)
    set_target_properties(di_pattern_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # Monitoring Integration example
    add_executable(monitoring_integration_example monitoring_integration_example.cpp)
    target_link_libraries(monitoring_integration_example PRIVATE LoggerSystem)
    set_target_properties(monitoring_integration_example PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    message(STATUS "Phase 4 DI pattern examples enabled")
endif()

# Advanced examples - available when samples are enabled and common_system is not integrated
if(BUILD_SAMPLES AND NOT BUILD_WITH_COMMON_SYSTEM)
    # Metrics demo sample
    add_executable(metrics_demo metrics_demo.cpp)
    target_link_libraries(metrics_demo PRIVATE LoggerSystem)
    set_target_properties(metrics_demo PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # Advanced features demo sample
    add_executable(advanced_features_demo advanced_features_demo.cpp)
    target_link_libraries(advanced_features_demo PRIVATE LoggerSystem)
    set_target_properties(advanced_features_demo PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    # TODO: distributed_logging_demo requires unimplemented features (log_server, log_aggregator)
    # Re-enable when Issue #XXX is resolved
    # add_executable(distributed_logging_demo distributed_logging_demo.cpp)
    # target_link_libraries(distributed_logging_demo PRIVATE LoggerSystem)
    # set_target_properties(distributed_logging_demo PROPERTIES
    #     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    # )

    # TODO: security_demo requires unimplemented features (log_sanitizer, structured_logger, get_router)
    # Re-enable when Issue #XXX is resolved
    # add_executable(security_demo security_demo.cpp)
    # target_link_libraries(security_demo PRIVATE LoggerSystem)
    # set_target_properties(security_demo PROPERTIES
    #     RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    # )

else()
    if(BUILD_WITH_COMMON_SYSTEM)
        message(STATUS "Advanced examples skipped when common_system integration is enabled")
    endif()
endif()
