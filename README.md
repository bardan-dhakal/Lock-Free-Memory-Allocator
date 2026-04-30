# Lock-Free Memory Allocator

Learning project implementing lock-free synchronization for memory allocation.

## What I Built
- Lock-free free list using `std::atomic<T>` and compare-and-swap
- CAS retry loops for thread-safe pop/push operations  
- Multi-threaded testing with 4+ concurrent threads
- Benchmark framework comparing against malloc

## Build Instructions

**Requirements:** CMake 3.10+, C++20 compiler, internet connection (Google Benchmark fetched automatically)

```bash
git clone https://github.com/your-username/Lock-Free-Memory-Allocator.git
cd Lock-Free-Memory-Allocator
cmake -B build && cmake --build build

Run benchmarks:
./build/gbenchmark

Run tests:
./build/test_threads
./build/test_free
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

Benchmarks run with Google Benchmark v1.8.4, compiled with `-O2`, on a 12-core x86-64 system.

The meaningful comparison for a lock-free allocator is against a **mutex-protected allocator** —
the classic baseline that lock-free design is intended to replace. A mutex-based allocator
serializes all threads through a single lock: only one thread can allocate at a time, and the
rest block and wait. Under contention, the cost of mutex acquisition and thread context-switching
dominates throughput.

**Latency (ns/op) — lower is better**

| Benchmark            | 1 Thread | 2 Threads | 4 Threads | 8 Threads |
|----------------------|---------:|----------:|----------:|----------:|
| LockFree Allocator   |  16.4 ns |   57.0 ns |   73.7 ns |   76.0 ns |
| Mutex Allocator      |  27.3 ns |  112.0 ns |  114.0 ns |  191.0 ns |

**Throughput (ops/sec) — higher is better**

| Benchmark            | 1 Thread | 2 Threads | 4 Threads | 8 Threads |
|----------------------|---------:|----------:|----------:|----------:|
| LockFree Allocator   |    61.1M |     17.6M |     13.6M |     13.2M |
| Mutex Allocator      |    36.6M |      9.0M |      8.8M |      5.2M |

The lock-free allocator outperforms the mutex-based allocator across all thread counts. Notice
the latency profile: the mutex allocator's per-operation cost grows from 27 ns at 1 thread to
191 ns at 8 threads — a 7x degradation — because each thread waits for all others to release
the lock before it can proceed. The lock-free allocator grows from 16 ns to 76 ns over the same
range, remaining far more stable under contention.

### Why Lock-Free Wins Under Contention

  A mutex forces **serialization**: one thread holds the lock, all others park in the OS scheduler.
  The cost is paid on every allocation and every deallocation. CAS-based allocation instead
  **retries atomically**: a losing thread immediately retries the operation rather than yielding
  to the OS. For short critical sections like pointer swaps, this retry cost is far cheaper than
  a context switch, which is why latency stays controlled as thread count increases.


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

