# options.cmake -- Build options, feature flags, and version variables for Logger System
#
# Inputs:
#   - PROJECT_VERSION: set by project() in root CMakeLists.txt
#   - LOGGER_CAN_USE_MODULES: derived from CMake version (set in root)
#   - USE_LOCKFREE: optional upstream variable (defaults to OFF if undefined)
# Outputs:
#   - BUILD_TESTS / BUILD_BENCHMARKS / BUILD_SAMPLES: standard build toggles
#   - BUILD_SHARED_LIBS: shared vs static
#   - LOGGER_BUILD_INTEGRATION_TESTS: integration test gate
#   - LOGGER_ENABLE_COVERAGE: coverage gate (also referenced by testing.cmake)
#   - LOGGER_ENABLE_OTLP / LOGGER_USE_ENCRYPTION: optional feature gates
#   - LOGGER_WITH_SERVER / LOGGER_WITH_ANALYSIS: SRP module gates (Issue #357)
#   - LOGGER_USE_MODULES: C++20 modules gate (requires CMake 3.28+)
#   - LOGGER_USE_DI / LOGGER_USE_MONITORING / LOGGER_USE_EXTERNAL_DI / etc.:
#     fine-grained feature flags (formerly in logger_features.cmake)
#   - LOGGER_DEFAULT_BUFFER_SIZE / LOGGER_DEFAULT_BATCH_SIZE / LOGGER_DEFAULT_QUEUE_SIZE /
#     LOGGER_MAX_WRITERS: performance tuning cache strings
#   - logger_print_features(): function, prints summary
#   - logger_configure_target(target): function, applies feature compile defs
#   - NO_VCPKG: optional vcpkg bypass

##################################################
# Top-level build options
##################################################
option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_SAMPLES "Build sample programs" ON)
option(BUILD_SHARED_LIBS "Build shared libraries" OFF)
option(LOGGER_BUILD_INTEGRATION_TESTS "Build integration tests" ON)
option(LOGGER_ENABLE_COVERAGE "Enable code coverage reporting" OFF)
option(LOGGER_ENABLE_OTLP "Enable OpenTelemetry (OTLP) integration for observability" OFF)

# Optional module options (Issue #357: SRP - separate server/analysis from core)
option(LOGGER_WITH_SERVER "Include log server functionality" ON)
option(LOGGER_WITH_ANALYSIS "Include log analysis functionality" ON)

# C++20 Modules option (requires CMake 3.28+; set in root before include())
if(LOGGER_CAN_USE_MODULES)
    option(LOGGER_USE_MODULES "Build with C++20 modules support" OFF)
else()
    set(LOGGER_USE_MODULES OFF CACHE BOOL "Build with C++20 modules (requires CMake 3.28+)" FORCE)
endif()

if(LOGGER_USE_MODULES)
    message(STATUS "Logger System: C++20 modules support enabled")
    set(CMAKE_CXX_SCAN_FOR_MODULES ON)
endif()

# Map LOGGER_BUILD_* to BUILD_* for compatibility with CI/CD
if(DEFINED LOGGER_BUILD_SAMPLES)
    set(BUILD_SAMPLES ${LOGGER_BUILD_SAMPLES} CACHE BOOL "Build sample programs" FORCE)
endif()
if(DEFINED LOGGER_BUILD_TESTS)
    set(BUILD_TESTS ${LOGGER_BUILD_TESTS} CACHE BOOL "Build unit tests" FORCE)
endif()

# Respect global BUILD_INTEGRATION_TESTS flag if set
if(DEFINED BUILD_INTEGRATION_TESTS)
    if(BUILD_INTEGRATION_TESTS)
        set(_LOGGER_BUILD_IT_VALUE ON)
    else()
        set(_LOGGER_BUILD_IT_VALUE OFF)
    endif()
    set(LOGGER_BUILD_INTEGRATION_TESTS ${_LOGGER_BUILD_IT_VALUE} CACHE BOOL "Build integration tests" FORCE)
endif()

option(NO_VCPKG "Skip vcpkg and use system libraries only" OFF)

# Disable tests and benchmarks for MinGW builds due to compatibility issues
if(MINGW)
    if(BUILD_TESTS)
        message(STATUS "Disabling tests for MinGW build due to compatibility issues")
        set(BUILD_TESTS OFF CACHE BOOL "Tests disabled for MinGW" FORCE)
    endif()
    if(BUILD_BENCHMARKS)
        message(STATUS "Disabling benchmarks for MinGW build due to compatibility issues")
        set(BUILD_BENCHMARKS OFF CACHE BOOL "Benchmarks disabled for MinGW" FORCE)
    endif()
endif()

##################################################
# Fine-grained feature flags (formerly logger_features.cmake)
##################################################
option(LOGGER_USE_DI "Enable dependency injection support" ON)
option(LOGGER_USE_MONITORING "Enable monitoring and metrics collection" ON)
option(LOGGER_USE_EXTERNAL_DI "Use external DI container if available" OFF)
option(LOGGER_FORCE_LIGHTWEIGHT "Force lightweight implementations only" ON)
option(LOGGER_ENABLE_SANITIZERS "Enable address and thread sanitizers" OFF)
option(LOGGER_USE_COMPRESSION "Enable log compression support" OFF)
option(LOGGER_USE_ENCRYPTION "Enable log encryption support" ON)
option(LOGGER_ENABLE_CRASH_HANDLER "Enable crash handler integration" ON)
option(LOGGER_ENABLE_STRUCTURED_LOGGING "Enable structured logging (JSON)" OFF)
option(LOGGER_USE_LOCK_FREE_QUEUE "Use lock-free queue implementation" ${USE_LOCKFREE})
option(LOGGER_ENABLE_NETWORK_WRITER "Enable network log writer" OFF)
option(LOGGER_ENABLE_FILE_ROTATION "Enable file rotation support" ON)
option(LOGGER_ENABLE_ASYNC "Enable asynchronous logging" ON)

# thread_system is now OPTIONAL (Issue #222, #224)
option(LOGGER_USE_THREAD_SYSTEM "Enable optional thread_system integration" OFF)

# Performance tuning options
set(LOGGER_DEFAULT_BUFFER_SIZE "8192" CACHE STRING "Default buffer size in bytes")
set(LOGGER_DEFAULT_BATCH_SIZE "100" CACHE STRING "Default batch size for processing")
set(LOGGER_DEFAULT_QUEUE_SIZE "10000" CACHE STRING "Default maximum queue size")
set(LOGGER_MAX_WRITERS "10" CACHE STRING "Maximum number of concurrent writers")

# Validation of options
if(LOGGER_USE_EXTERNAL_DI AND LOGGER_FORCE_LIGHTWEIGHT)
    message(WARNING "LOGGER_USE_EXTERNAL_DI and LOGGER_FORCE_LIGHTWEIGHT are mutually exclusive. Disabling LOGGER_USE_EXTERNAL_DI.")
    set(LOGGER_USE_EXTERNAL_DI OFF CACHE BOOL "Use external DI container if available" FORCE)
endif()

if(NOT LOGGER_ENABLE_ASYNC AND LOGGER_USE_LOCK_FREE_QUEUE)
    message(WARNING "Lock-free queue requires async mode. Disabling LOGGER_USE_LOCK_FREE_QUEUE.")
    set(LOGGER_USE_LOCK_FREE_QUEUE OFF CACHE BOOL "Use lock-free queue implementation" FORCE)
endif()

##################################################
# Feature summary printer
##################################################
function(logger_print_features)
    message(STATUS "========================================")
    message(STATUS "Logger System Feature Configuration:")
    message(STATUS "  Core Features:")
    message(STATUS "    Dependency Injection: ${LOGGER_USE_DI}")
    message(STATUS "    Monitoring: ${LOGGER_USE_MONITORING}")
    message(STATUS "    Async Logging: ${LOGGER_ENABLE_ASYNC}")
    message(STATUS "    Crash Handler: ${LOGGER_ENABLE_CRASH_HANDLER}")
    message(STATUS "  Advanced Features:")
    message(STATUS "    External DI: ${LOGGER_USE_EXTERNAL_DI}")
    message(STATUS "    Lock-free Queue: ${LOGGER_USE_LOCK_FREE_QUEUE}")
    message(STATUS "    Structured Logging: ${LOGGER_ENABLE_STRUCTURED_LOGGING}")
    message(STATUS "    Network Writer: ${LOGGER_ENABLE_NETWORK_WRITER}")
    message(STATUS "    File Rotation: ${LOGGER_ENABLE_FILE_ROTATION}")
    message(STATUS "    Compression: ${LOGGER_USE_COMPRESSION}")
    message(STATUS "    Encryption: ${LOGGER_USE_ENCRYPTION}")
    message(STATUS "  Performance Settings:")
    message(STATUS "    Buffer Size: ${LOGGER_DEFAULT_BUFFER_SIZE}")
    message(STATUS "    Batch Size: ${LOGGER_DEFAULT_BATCH_SIZE}")
    message(STATUS "    Queue Size: ${LOGGER_DEFAULT_QUEUE_SIZE}")
    message(STATUS "    Max Writers: ${LOGGER_MAX_WRITERS}")
    message(STATUS "  Build Options:")
    message(STATUS "    Force Lightweight: ${LOGGER_FORCE_LIGHTWEIGHT}")
    message(STATUS "    Sanitizers: ${LOGGER_ENABLE_SANITIZERS}")
    message(STATUS "    Coverage: ${LOGGER_ENABLE_COVERAGE}")
    message(STATUS "========================================")
endfunction()

##################################################
# Apply feature compile definitions to a target
##################################################
function(logger_configure_target target)
    if(LOGGER_USE_DI)
        target_compile_definitions(${target} PUBLIC LOGGER_USE_DI)
    endif()

    if(LOGGER_USE_MONITORING)
        target_compile_definitions(${target} PUBLIC LOGGER_USE_MONITORING)
    endif()

    if(LOGGER_USE_EXTERNAL_DI)
        target_compile_definitions(${target} PUBLIC LOGGER_USE_EXTERNAL_DI)
    endif()

    if(LOGGER_FORCE_LIGHTWEIGHT)
        target_compile_definitions(${target} PUBLIC LOGGER_FORCE_LIGHTWEIGHT)
    endif()

    if(LOGGER_ENABLE_CRASH_HANDLER)
        target_compile_definitions(${target} PUBLIC LOGGER_ENABLE_CRASH_HANDLER)
    endif()

    if(LOGGER_ENABLE_STRUCTURED_LOGGING)
        target_compile_definitions(${target} PUBLIC LOGGER_ENABLE_STRUCTURED_LOGGING)
    endif()

    if(LOGGER_USE_LOCK_FREE_QUEUE)
        target_compile_definitions(${target} PUBLIC LOGGER_USE_LOCK_FREE_QUEUE)
    endif()

    if(LOGGER_ENABLE_NETWORK_WRITER)
        target_compile_definitions(${target} PUBLIC LOGGER_ENABLE_NETWORK_WRITER)
    endif()

    if(LOGGER_ENABLE_FILE_ROTATION)
        target_compile_definitions(${target} PUBLIC LOGGER_ENABLE_FILE_ROTATION)
    endif()

    if(LOGGER_USE_COMPRESSION)
        target_compile_definitions(${target} PUBLIC LOGGER_USE_COMPRESSION)
    endif()

    if(LOGGER_USE_ENCRYPTION)
        target_compile_definitions(${target} PUBLIC LOGGER_USE_ENCRYPTION)
    endif()

    if(LOGGER_ENABLE_ASYNC)
        target_compile_definitions(${target} PUBLIC LOGGER_ENABLE_ASYNC)
    endif()

    # Add performance settings as compile definitions
    target_compile_definitions(${target} PUBLIC
        LOGGER_DEFAULT_BUFFER_SIZE=${LOGGER_DEFAULT_BUFFER_SIZE}
        LOGGER_DEFAULT_BATCH_SIZE=${LOGGER_DEFAULT_BATCH_SIZE}
        LOGGER_DEFAULT_QUEUE_SIZE=${LOGGER_DEFAULT_QUEUE_SIZE}
        LOGGER_MAX_WRITERS=${LOGGER_MAX_WRITERS}
    )

    # Note: Sanitizers handled by sanitizer logic (compiler.cmake / dedicated module)
    # Coverage handled by testing.cmake
endfunction()
