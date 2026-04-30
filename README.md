# Lock-Free Memory Allocator

Learning project implementing lock-free synchronization for memory allocation.

## What I Built
- Lock-free free list using `std::atomic<T>` and compare-and-swap
- CAS retry loops for thread-safe pop/push operations  
- Multi-threaded testing with 4+ concurrent threads
- Benchmark framework comparing against malloc

## Building

### Using CMake (Recommended):
```bash
mkdir build && cd build
cmake ..
make
./test_threads
```

### Using g++ directly:
```bash
g++ -std=c++20 -pthread experiments/test_threads.cpp src/allocator.cpp src/allocate.cpp -o test_threads
```

## Key Implementation
```cpp
// Lock-free pop using CAS retry loop
do {
    expected = head.load();
    if (!expected) return nullptr;
    new_head = expected->next;
} while (!head.compare_exchange_weak(expected, new_head));
```

## Technical Concepts Demonstrated
- Atomic operations and memory ordering
- Compare-and-swap for lock-free synchronization
- Race condition prevention without mutexes
- Concurrent data structure design

## Benchmark Results
Current implementation: ~8-15M ops/sec across thread counts, improving efficiency is in progress, expected boost after adding Thread Local Caching.

## Valgrind Analysis

  Run with: `valgrind --leak-check=full --track-origins=yes ./build/test_threads`

  ==HEAP SUMMARY:
      in use at exit: 0 bytes in 0 blocks
    total heap usage: 13 allocs, 13 frees, 76,152 bytes allocated

  All heap blocks were freed -- no leaks are possible

  ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)

  **Results:** Zero memory errors, zero leaks. All 13 heap allocations (thread stacks and
  internal C++ runtime overhead) were fully freed at exit. The allocator's own memory
  pool is acquired via `mmap` directly — bypassing the heap entirely — which is why it
  does not appear in the heap summary. Valgrind confirmed no invalid reads, no invalid
  writes, and no use-after-free across 4 concurrent threads running 1,000 iterations each.

## Learning Outcomes
- Deep understanding of atomics and CAS
- Experience with concurrent programming challenges
- Knowledge of memory allocator architecture
- Understanding of performance tradeoffs in lock-free design

## Next Steps If Continued
- Add thread-local caching (major performance improvement)
- Implement multiple size classes
- Add ABA problem mitigation
- Larger memory pools

