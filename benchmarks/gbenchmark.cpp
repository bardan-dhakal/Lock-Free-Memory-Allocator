#include <benchmark/benchmark.h>
#include "allocator.hpp"
#include "allocate.hpp"
#include <memory>
#include <cstdlib>

static void BM_LockFreeAllocator(benchmark::State& state) {
    for (auto _ : state)
    {
        void* ptr = LockFreeAllocator::allocate(64);
        benchmark::DoNotOptimize(ptr);
        if (ptr) LockFreeAllocator::free(ptr);
    }

    state.SetItemsProcessed(state.iterations());
}

static void BM_Malloc(benchmark::State& state)
{
    for (auto _ : state)
    {
        void* ptr = malloc(64);
        benchmark::DoNotOptimize(ptr);
        free(ptr);
    }

    state.SetItemsProcessed(state.iterations());
}

static void BM_StdAllocator(benchmark::State& state)
{
    std::allocator<char> alloc;
    for (auto _ : state)
    {
        char* ptr = alloc.allocate(64);
        benchmark::DoNotOptimize(ptr);
        alloc.deallocate(ptr, 64);
    }

    state.SetItemsProcessed(state.iterations());
}

BENCHMARK(BM_LockFreeAllocator)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK(BM_Malloc)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();
BENCHMARK(BM_StdAllocator)->Threads(1)->Threads(2)->Threads(4)->Threads(8)->UseRealTime();

int main(int argc, char** argv)
{
    LockFreeAllocator::initialize_allocator();
    benchmark::Initialize(&argc, argv);
    if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return 0;
}