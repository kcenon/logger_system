// BSD 3-Clause License
//
// Copyright (c) 2021-2025, 🍀☀🌕🌥 🌊
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// 1. Redistributions of source code must retain the above copyright notice, this
//    list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright notice,
//    this list of conditions and the following disclaimer in the documentation
//    and/or other materials provided with the distribution.
//
// 3. Neither the name of the copyright holder nor the names of its
//    contributors may be used to endorse or promote products derived from
//    this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

/**
 * @file object_pool_bench.cpp
 * @brief Performance benchmarks for object_pool and thread_local_object_pool
 *
 * Phase 2, Task 2.1: Thread-local object_pool optimization
 *
 * This benchmark compares:
 * 1. Original object_pool (mutex on every acquire/release)
 * 2. thread_local_object_pool (thread-local cache + batch transfers)
 *
 * Expected results:
 * - Single thread: Similar performance
 * - Multi-threaded: 2-5x improvement with thread_local_object_pool
 */

#include <benchmark/benchmark.h>
#include "../src/impl/memory/object_pool.h"
#include <thread>
#include <vector>
#include <cstring>

using namespace kcenon::logger::memory;

// Mock log entry for testing
struct test_object {
    char data[256];
    int id;

    test_object() : id(0) {
        std::memset(data, 0, sizeof(data));
    }
};

//==============================================================================
// Benchmark 1: Original object_pool - Single Thread
//==============================================================================

static void BM_ObjectPool_SingleThread(benchmark::State& state) {
    object_pool<test_object> pool;

    for (auto _ : state) {
        auto obj = pool.acquire();
        benchmark::DoNotOptimize(obj);
        pool.release(std::move(obj));
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(test_object));
}
BENCHMARK(BM_ObjectPool_SingleThread);

//==============================================================================
// Benchmark 2: thread_local_object_pool - Single Thread
//==============================================================================

static void BM_ThreadLocalObjectPool_SingleThread(benchmark::State& state) {
    thread_local_object_pool<test_object> pool;

    for (auto _ : state) {
        auto obj = pool.acquire();
        benchmark::DoNotOptimize(obj);
        pool.release(std::move(obj));
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(test_object));
}
BENCHMARK(BM_ThreadLocalObjectPool_SingleThread);

//==============================================================================
// Benchmark 3: Original object_pool - Multi-threaded
//==============================================================================

static void BM_ObjectPool_MultiThread(benchmark::State& state) {
    static object_pool<test_object> pool;

    for (auto _ : state) {
        auto obj = pool.acquire();
        benchmark::DoNotOptimize(obj);

        // Simulate some work
        if (obj) {
            obj->id = static_cast<int>(state.iterations());
        }

        pool.release(std::move(obj));
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(test_object));
}
BENCHMARK(BM_ObjectPool_MultiThread)
    ->Threads(1)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->UseRealTime();

//==============================================================================
// Benchmark 4: thread_local_object_pool - Multi-threaded
//==============================================================================

static void BM_ThreadLocalObjectPool_MultiThread(benchmark::State& state) {
    static thread_local_object_pool<test_object> pool;

    for (auto _ : state) {
        auto obj = pool.acquire();
        benchmark::DoNotOptimize(obj);

        // Simulate some work
        if (obj) {
            obj->id = static_cast<int>(state.iterations());
        }

        pool.release(std::move(obj));
    }

    state.SetItemsProcessed(state.iterations());
    state.SetBytesProcessed(state.iterations() * sizeof(test_object));
}
BENCHMARK(BM_ThreadLocalObjectPool_MultiThread)
    ->Threads(1)
    ->Threads(2)
    ->Threads(4)
    ->Threads(8)
    ->UseRealTime();

//==============================================================================
// Benchmark 5: Stress test - High contention scenario
//==============================================================================

static void BM_ObjectPool_HighContention(benchmark::State& state) {
    static object_pool<test_object> pool;

    for (auto _ : state) {
        // Acquire multiple objects
        std::vector<std::unique_ptr<test_object>> objects;
        objects.reserve(10);

        for (int i = 0; i < 10; ++i) {
            objects.push_back(pool.acquire());
        }

        benchmark::DoNotOptimize(objects);

        // Release all
        for (auto& obj : objects) {
            pool.release(std::move(obj));
        }
    }

    state.SetItemsProcessed(state.iterations() * 10);
}
BENCHMARK(BM_ObjectPool_HighContention)
    ->Threads(4)
    ->Threads(8)
    ->UseRealTime();

static void BM_ThreadLocalObjectPool_HighContention(benchmark::State& state) {
    static thread_local_object_pool<test_object> pool;

    for (auto _ : state) {
        // Acquire multiple objects
        std::vector<std::unique_ptr<test_object>> objects;
        objects.reserve(10);

        for (int i = 0; i < 10; ++i) {
            objects.push_back(pool.acquire());
        }

        benchmark::DoNotOptimize(objects);

        // Release all
        for (auto& obj : objects) {
            pool.release(std::move(obj));
        }
    }

    state.SetItemsProcessed(state.iterations() * 10);
}
BENCHMARK(BM_ThreadLocalObjectPool_HighContention)
    ->Threads(4)
    ->Threads(8)
    ->UseRealTime();

//==============================================================================
// Benchmark 6: Cache efficiency test
//==============================================================================

static void BM_ThreadLocalObjectPool_CacheEfficiency(benchmark::State& state) {
    thread_local_object_pool<test_object>::config cfg;
    cfg.local_cache_size = state.range(0);

    thread_local_object_pool<test_object> pool(cfg);

    for (auto _ : state) {
        auto obj = pool.acquire();
        benchmark::DoNotOptimize(obj);
        pool.release(std::move(obj));
    }

    // Report statistics
    auto stats = pool.get_statistics();
    state.counters["local_cache_hits"] = static_cast<double>(stats.local_cache_hits);
    state.counters["global_pool_hits"] = static_cast<double>(stats.global_pool_hits);
    state.counters["new_allocations"] = static_cast<double>(stats.new_allocations);

    double cache_hit_rate = 0.0;
    if (stats.local_cache_hits + stats.global_pool_hits + stats.new_allocations > 0) {
        cache_hit_rate = static_cast<double>(stats.local_cache_hits) /
            (stats.local_cache_hits + stats.global_pool_hits + stats.new_allocations);
    }
    state.counters["cache_hit_rate"] = cache_hit_rate;

    state.SetItemsProcessed(state.iterations());
}
BENCHMARK(BM_ThreadLocalObjectPool_CacheEfficiency)
    ->Arg(4)    // Small cache
    ->Arg(8)
    ->Arg(16)   // Optimal (default)
    ->Arg(32)   // Larger cache
    ->Arg(64)
    ->Threads(4)
    ->UseRealTime();
