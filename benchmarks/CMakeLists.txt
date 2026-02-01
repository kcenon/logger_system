# Logger System Benchmarks
# Phase 0, Task 0.2: Baseline Performance Benchmarking

cmake_minimum_required(VERSION 3.20)

# Find Google Benchmark
find_package(benchmark QUIET)

if(NOT benchmark_FOUND)
    message(STATUS "Google Benchmark not found. Trying to find via pkg-config...")
    find_package(PkgConfig)
    if(PkgConfig_FOUND)
        pkg_check_modules(BENCHMARK benchmark)
    endif()
endif()

if(benchmark_FOUND OR BENCHMARK_FOUND)
    message(STATUS "Google Benchmark found. Building logger_system benchmarks...")

    # Benchmark executable
    # Note: Some older benchmarks are temporarily disabled due to API changes
    add_executable(logger_benchmarks
        # logger_write_bench.cpp     # TODO: Update to new API
        # logger_throughput_bench.cpp # TODO: Update to new API
        # logger_rotation_bench.cpp   # TODO: Update to new API
        # logger_async_bench.cpp      # TODO: Update to new API
        object_pool_bench.cpp
        main_bench.cpp
    )

    # Decorator pattern performance benchmark
    add_executable(decorator_benchmark
        decorator_performance.cpp
    )

    target_link_libraries(logger_benchmarks
        PRIVATE
            LoggerSystem
            benchmark::benchmark
            benchmark::benchmark_main
    )

    target_include_directories(logger_benchmarks
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

    target_compile_features(logger_benchmarks PRIVATE cxx_std_20)

    # Decorator benchmark configuration
    target_link_libraries(decorator_benchmark
        PRIVATE
            LoggerSystem
            benchmark::benchmark
            benchmark::benchmark_main
    )

    target_include_directories(decorator_benchmark
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/../include
    )

    target_compile_features(decorator_benchmark PRIVATE cxx_std_20)

    # Set compiler warnings
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        target_compile_options(logger_benchmarks PRIVATE
            -Wall -Wextra -Wpedantic
            -O3  # Maximum optimization for accurate benchmarks
            -DNDEBUG  # Disable asserts
        )

        target_compile_options(decorator_benchmark PRIVATE
            -Wall -Wextra -Wpedantic
            -O3  # Maximum optimization for accurate benchmarks
            -DNDEBUG  # Disable asserts
        )
    elseif(MSVC)
        target_compile_options(logger_benchmarks PRIVATE
            /W4 /O2 /DNDEBUG
        )

        target_compile_options(decorator_benchmark PRIVATE
            /W4 /O2 /DNDEBUG
        )
    endif()

    # Add custom target to run benchmarks
    add_custom_target(run_logger_benchmarks
        COMMAND logger_benchmarks --benchmark_format=json --benchmark_out=benchmark_results.json
        COMMAND logger_benchmarks --benchmark_format=console
        DEPENDS logger_benchmarks
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running logger_system performance benchmarks"
    )

    # Add custom target to run decorator benchmarks
    add_custom_target(run_decorator_benchmarks
        COMMAND decorator_benchmark --benchmark_format=json --benchmark_out=decorator_results.json
        COMMAND decorator_benchmark --benchmark_format=console
        DEPENDS decorator_benchmark
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running decorator pattern performance benchmarks"
    )

    # Installation
    install(TARGETS logger_benchmarks decorator_benchmark
        RUNTIME DESTINATION bin/benchmarks
    )

else()
    message(WARNING "Google Benchmark not found. Skipping logger_system benchmarks.")
    message(STATUS "Install with: brew install google-benchmark (macOS) or apt-get install libbenchmark-dev (Ubuntu)")
endif()
