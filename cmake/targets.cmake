# targets.cmake -- logger_system library target with explicit source list
#
# Inputs:
#   - LOGGER_INCLUDE_DIR / LOGGER_SOURCE_DIR: include/src directory paths
#     (set in this module from CMAKE_CURRENT_SOURCE_DIR)
#   - LOGGER_WITH_SERVER / LOGGER_WITH_ANALYSIS: SRP module gates (options.cmake)
#   - LOGGER_USE_ENCRYPTION / LOGGER_ENABLE_OTLP: feature flags (options.cmake)
#   - LOGGER_USE_MODULES / LOGGER_CAN_USE_MODULES: C++20 modules gates
#   - BUILD_SHARED_LIBS: shared vs static
#   - common_system_FOUND / kcenon::common_system: from dependencies.cmake
#   - thread_system_TARGET / THREAD_SYSTEM_FOUND: from dependencies.cmake
# Outputs:
#   - logger_system: library target with EXPORT_NAME logger
#   - _LOGGER_HAS_LOCAL_THREAD_SYSTEM: TRUE if local non-IMPORTED
#     thread_system was linked (consumed by install.cmake)
#   - LOGGER_SOURCES / LOGGER_HEADERS: explicit source lists (no file(GLOB))

##################################################
# Source / header directories
##################################################
set(LOGGER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(LOGGER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

if(NOT EXISTS ${LOGGER_INCLUDE_DIR}/kcenon/logger)
    message(FATAL_ERROR "Logger System: Required directory missing: include/kcenon/logger")
endif()
if(NOT EXISTS ${LOGGER_SOURCE_DIR})
    message(FATAL_ERROR "Logger System: Required directory missing: src/")
endif()

message(STATUS "Logger System: Using new directory structure")
include_directories(${LOGGER_INCLUDE_DIR})

##################################################
# Explicit source list (replaces former file(GLOB_RECURSE) calls).
# Keep alphabetically sorted by directory and filename.
##################################################
set(LOGGER_SOURCES
    ${LOGGER_SOURCE_DIR}/core/log_collector.cpp
    ${LOGGER_SOURCE_DIR}/core/log_context_scope.cpp
    ${LOGGER_SOURCE_DIR}/core/logger.cpp
    ${LOGGER_SOURCE_DIR}/core/logger_context.cpp
    ${LOGGER_SOURCE_DIR}/core/logger_metrics.cpp
    ${LOGGER_SOURCE_DIR}/core/logger_registry.cpp
    ${LOGGER_SOURCE_DIR}/core/scoped_context_guard.cpp
    ${LOGGER_SOURCE_DIR}/core/signal_manager_context.cpp
    ${LOGGER_SOURCE_DIR}/core/unified_log_context.cpp
    ${LOGGER_SOURCE_DIR}/impl/async/async_worker.cpp
    ${LOGGER_SOURCE_DIR}/impl/async/batch_processor.cpp
    ${LOGGER_SOURCE_DIR}/impl/async/high_performance_async_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/builders/writer_builder.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/base_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/batch_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/buffered_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/console_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/critical_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/decorator_writer_base.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/encrypted_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/file_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/filtered_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/formatted_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/network_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/otlp_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/rotating_file_writer.cpp
    ${LOGGER_SOURCE_DIR}/impl/writers/thread_safe_writer.cpp
    ${LOGGER_SOURCE_DIR}/integration/executor_integration.cpp
    ${LOGGER_SOURCE_DIR}/integration/standalone_executor.cpp
    ${LOGGER_SOURCE_DIR}/integration/thread_system_integration.cpp
    ${LOGGER_SOURCE_DIR}/sampling/log_sampler.cpp
    ${LOGGER_SOURCE_DIR}/security/signal_manager.cpp
)

set(LOGGER_HEADERS
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/adapters/common_logger_adapter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/adapters/common_system_adapter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/adapters/logger_adapter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/analysis/log_analyzer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/analysis/realtime_log_analyzer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/backends/integration_backend.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/backends/monitoring_backend.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/backends/standalone_backend.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/builders/writer_builder.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/compatibility.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/error_codes.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/filtered_logger.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/log_collector.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/log_context.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/log_context_scope.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/logger.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/logger_builder.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/logger_config.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/logger_config_builder.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/logger_context.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/logger_registry.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/metrics/logger_metrics.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/monitoring_integration_detector.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/scoped_context_guard.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/signal_manager_context.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/small_string.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/strategies/composite_strategy.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/strategies/config_strategy_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/strategies/deployment_strategy.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/strategies/environment_strategy.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/strategies/performance_strategy.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/structured_log_builder.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/thread_integration_detector.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/core/unified_log_context.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/di/service_registration.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/factories/filter_factory.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/factories/formatter_factory.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/factories/writer_factory.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/filters/log_filter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/formatters/base_formatter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/formatters/json_formatter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/formatters/logfmt_formatter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/formatters/raw_formatter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/formatters/template_formatter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/formatters/timestamp_formatter.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/forward.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/integration/executor_integration.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/integration/standalone_executor.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/integration/thread_system_integration.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/log_entry.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/log_filter_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/log_formatter_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/log_sink_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/log_writer_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/logger_types.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/output_sink_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/interfaces/writer_category.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/logger_export.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/otlp/otel_context.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/routing/log_router.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/safety/crash_safe_logger.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/sampling/log_sampler.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/sampling/sampling_config.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/audit_logger.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/integrity_policy.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/log_sanitizer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/path_validator.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/secure_key_storage.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/signal_manager.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/security/signal_manager_interface.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/server/log_server.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/sinks/console_sink.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/sinks/file_sink.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/structured/structured_logger.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/utils/error_handling_utils.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/utils/file_utils.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/utils/string_utils.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/utils/time_utils.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/async_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/base_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/batch_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/buffered_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/composite_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/console_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/critical_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/decorator_writer_base.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/encrypted_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/file_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/filtered_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/formatted_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/network_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/otlp_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/queued_writer_base.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/rotating_file_writer.h
    ${LOGGER_INCLUDE_DIR}/kcenon/logger/writers/thread_safe_writer.h
)

##################################################
# Optional module filtering (Issue #357: SRP)
##################################################
if(NOT LOGGER_WITH_SERVER)
    message(STATUS "Logger System: Server module disabled (LOGGER_WITH_SERVER=OFF)")
    list(FILTER LOGGER_HEADERS EXCLUDE REGEX ".*/server/.*")
    # Note: server is header-only, no cpp files to exclude
else()
    message(STATUS "Logger System: Server module enabled")
endif()

if(NOT LOGGER_WITH_ANALYSIS)
    message(STATUS "Logger System: Analysis module disabled (LOGGER_WITH_ANALYSIS=OFF)")
    list(FILTER LOGGER_HEADERS EXCLUDE REGEX ".*/analysis/.*")
    # Note: analysis is header-only in include/, but has cppm in modules/
else()
    message(STATUS "Logger System: Analysis module enabled")
endif()

##################################################
# Library target
##################################################
add_library(logger_system ${LOGGER_SOURCES} ${LOGGER_HEADERS})
set_target_properties(logger_system PROPERTIES EXPORT_NAME logger)
target_include_directories(logger_system
    PUBLIC
        $<BUILD_INTERFACE:${LOGGER_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# DLL export support
if(BUILD_SHARED_LIBS)
    target_compile_definitions(logger_system PRIVATE LOGGER_SYSTEM_BUILDING)
else()
    target_compile_definitions(logger_system PUBLIC LOGGER_SYSTEM_STATIC)
endif()

# Optional module compile definitions (Issue #357: SRP)
if(LOGGER_WITH_SERVER)
    target_compile_definitions(logger_system PUBLIC LOGGER_WITH_SERVER=1)
endif()

if(LOGGER_WITH_ANALYSIS)
    target_compile_definitions(logger_system PUBLIC LOGGER_WITH_ANALYSIS=1)
endif()

##################################################
# Suppress warnings inherited from parent project for the library target.
# These are scoped to logger_system only; warnings.cmake still applies the
# strict warning set elsewhere via logger_add_warnings()/...for_all().
##################################################
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(logger_system PRIVATE
        -Wno-sign-conversion
        -Wno-conversion
        -Wno-float-equal
        -Wno-double-promotion
        -Wno-shadow
        -Wno-unused-private-field
    )
elseif(MSVC)
    target_compile_options(logger_system PRIVATE
        /wd4244  # conversion from 'type1' to 'type2', possible loss of data
        /wd4267  # conversion from 'size_t' to 'type', possible loss of data
        /wd4305  # truncation from 'type1' to 'type2'
        /wd4456  # declaration of 'identifier' hides previous local declaration
        /wd4457  # declaration of 'identifier' hides function parameter
        /wd4458  # declaration of 'identifier' hides class member
    )
endif()

##################################################
# common_system integration (always linked if available)
##################################################
if(common_system_FOUND)
    message(STATUS "Logger System: common_system found, linking for Result<T> compatibility")

    if(TARGET kcenon::common_system)
        target_link_libraries(logger_system PUBLIC kcenon::common_system)
    else()
        target_include_directories(logger_system
            PUBLIC
                $<BUILD_INTERFACE:${COMMON_SYSTEM_INCLUDE_DIR}>
        )
    endif()
else()
    message(STATUS "Logger System: common_system not found, using standalone mode")
endif()

##################################################
# thread_system integration (optional, for advanced features)
##################################################
if(THREAD_SYSTEM_FOUND)
    message(STATUS "Logger System: thread_system found, optional backend available")

    # Use the resolved target from UnifiedDependencies
    if(thread_system_TARGET AND TARGET ${thread_system_TARGET})
        get_target_property(_ts_imported ${thread_system_TARGET} IMPORTED)
        if(_ts_imported)
            # IMPORTED target (vcpkg / find_package): safe to export
            target_link_libraries(logger_system PUBLIC ${thread_system_TARGET})
            target_compile_definitions(logger_system PUBLIC LOGGER_HAS_THREAD_SYSTEM)
        else()
            # Local (submodule) target: cannot be in our export set
            target_link_libraries(logger_system PRIVATE ${thread_system_TARGET})
            target_compile_definitions(logger_system PRIVATE LOGGER_HAS_THREAD_SYSTEM)
            set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
        endif()
    elseif(TARGET ThreadSystem)
        get_target_property(_ts_imported ThreadSystem IMPORTED)
        target_link_libraries(logger_system PUBLIC ThreadSystem)
        target_compile_definitions(logger_system PUBLIC LOGGER_HAS_THREAD_SYSTEM)
        if(NOT _ts_imported)
            set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
        endif()
    elseif(TARGET thread_system)
        get_target_property(_ts_imported thread_system IMPORTED)
        target_link_libraries(logger_system PUBLIC thread_system)
        target_compile_definitions(logger_system PUBLIC LOGGER_HAS_THREAD_SYSTEM)
        if(NOT _ts_imported)
            set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
        endif()
    else()
        # Fallback: header-only access when aggregate targets are unavailable.
        target_include_directories(logger_system
            PUBLIC
                $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../thread_system/include>
        )
        target_compile_definitions(logger_system PRIVATE LOGGER_HAS_THREAD_SYSTEM)
        set(_LOGGER_HAS_LOCAL_THREAD_SYSTEM TRUE)
    endif()
else()
    message(STATUS "Logger System: Using standalone async implementation (no thread_system)")
endif()

##################################################
# OpenSSL (encryption)
##################################################
if(LOGGER_USE_ENCRYPTION)
    find_package(OpenSSL QUIET)
    if(OpenSSL_FOUND)
        target_link_libraries(logger_system PUBLIC OpenSSL::SSL OpenSSL::Crypto)
        target_compile_definitions(logger_system PUBLIC LOGGER_HAS_OPENSSL=1)
        message(STATUS "Logger System: OpenSSL linked for encryption support (${OPENSSL_VERSION})")
    else()
        message(WARNING "Logger System: LOGGER_USE_ENCRYPTION=ON but OpenSSL not found, encryption features disabled")
        # Remove encrypted_writer from build since OpenSSL is not available
        get_target_property(CURRENT_SOURCES logger_system SOURCES)
        list(FILTER CURRENT_SOURCES EXCLUDE REGEX ".*encrypted_writer.*")
        set_target_properties(logger_system PROPERTIES SOURCES "${CURRENT_SOURCES}")
    endif()
else()
    # If encryption is disabled, remove encrypted_writer from build
    get_target_property(CURRENT_SOURCES logger_system SOURCES)
    list(FILTER CURRENT_SOURCES EXCLUDE REGEX ".*encrypted_writer.*")
    set_target_properties(logger_system PROPERTIES SOURCES "${CURRENT_SOURCES}")
    message(STATUS "Logger System: Encryption support disabled (LOGGER_USE_ENCRYPTION=OFF)")
endif()

##################################################
# OpenTelemetry / OTLP
##################################################
if(LOGGER_ENABLE_OTLP)
    find_package(opentelemetry-cpp CONFIG QUIET)
    find_package(protobuf CONFIG QUIET)
    find_package(gRPC CONFIG QUIET)
    if(opentelemetry-cpp_FOUND)
        target_link_libraries(logger_system PUBLIC
            opentelemetry-cpp::logs
            opentelemetry-cpp::otlp_http_log_record_exporter
            opentelemetry-cpp::otlp_grpc_log_record_exporter
        )
        target_compile_definitions(logger_system PUBLIC LOGGER_HAS_OTLP=1)
        message(STATUS "Logger System: OpenTelemetry linked for OTLP support")
    else()
        message(WARNING "Logger System: LOGGER_ENABLE_OTLP=ON but opentelemetry-cpp not found")
        message(WARNING "  Install with: vcpkg install kcenon-logger-system[otlp]")
    endif()
endif()

set(USE_LEGACY_SOURCES FALSE)

##################################################
# C++20 Modules support (optional, requires CMake 3.28+)
##################################################
if(LOGGER_USE_MODULES AND LOGGER_CAN_USE_MODULES)
    message(STATUS "Logger System: Configuring C++20 module files")

    # Module source files (core modules always included)
    set(LOGGER_MODULE_FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/logger.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/core.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/backends.cppm
    )

    # Conditionally include analysis module (Issue #357: SRP)
    if(LOGGER_WITH_ANALYSIS)
        list(APPEND LOGGER_MODULE_FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/src/modules/analysis.cppm
        )
    endif()

    # Check if module files exist
    set(MODULES_AVAILABLE TRUE)
    foreach(MODULE_FILE ${LOGGER_MODULE_FILES})
        if(NOT EXISTS ${MODULE_FILE})
            message(WARNING "Module file not found: ${MODULE_FILE}")
            set(MODULES_AVAILABLE FALSE)
        endif()
    endforeach()

    if(MODULES_AVAILABLE)
        # Add module sources to the library using FILE_SET
        target_sources(logger_system
            PUBLIC
                FILE_SET CXX_MODULES
                FILES ${LOGGER_MODULE_FILES}
        )

        # Define macro to indicate modules are available
        target_compile_definitions(logger_system PUBLIC LOGGER_HAS_MODULES=1)

        message(STATUS "Logger System: C++20 modules configured successfully")
        message(STATUS "  Module files:")
        foreach(MODULE_FILE ${LOGGER_MODULE_FILES})
            get_filename_component(MODULE_NAME ${MODULE_FILE} NAME)
            message(STATUS "    - ${MODULE_NAME}")
        endforeach()
    else()
        message(STATUS "Logger System: Some module files missing, modules disabled")
    endif()
endif()
