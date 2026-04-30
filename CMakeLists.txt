cmake_minimum_required(VERSION 3.10)
project(ZeroLockAllocator CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release CACHE STRING "" FORCE)

# Thread support
find_package(Threads REQUIRED)

#Downloading and building google benchmark
include(FetchContent)
FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.8.4
)
set(BENCHMARK_ENABLE_TESTING     OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googlebenchmark)


# Source files
set(ALLOCATOR_SOURCES
    src/allocator.cpp
    src/allocate.cpp
)

# Include directories
include_directories(include)

# Library
add_library(allocator ${ALLOCATOR_SOURCES})

# Optimization flag for benchmarking
target_compile_options(allocator PRIVATE -O2)

target_link_libraries(allocator Threads::Threads)

# Test executables
add_executable(test_threads experiments/test_threads.cpp)
target_link_libraries(test_threads allocator Threads::Threads)

add_executable(test_free experiments/test_free.cpp)
target_link_libraries(test_free allocator Threads::Threads)

add_executable(gbenchmark benchmarks/gbenchmark.cpp)
target_link_libraries(gbenchmark PRIVATE allocator benchmark::benchmark Threads::Threads)
target_compile_options(gbenchmark PRIVATE -O2)

add_executable(my_benchmark benchmarks/benchmark.cpp)
target_link_libraries(my_benchmark allocator Threads::Threads)

# Test target
enable_testing()
add_test(NAME ThreadSafety COMMAND test_threads)