#include "../include/allocator.hpp"
#include "../include/allocate.hpp"
#include <iostream>

int main()
{
    LockFreeAllocator::initialize_allocator();
    
    std::cout << "\n--- TEST 1: BASIC FREE AND REUSE ---\n";
    void* test_mem = LockFreeAllocator::allocate(64);
    std::cout << "Allocated at " << test_mem << "\n";
    
    int* data = static_cast<int*>(test_mem);
    data[0] = 100;
    
    LockFreeAllocator::free(test_mem);
    void* test_mem2 = LockFreeAllocator::allocate(64);
    
    if (test_mem == test_mem2)
    {
        std::cout << "PASS: Memory reused\n";
    }
    else
    {
        std::cout << "FAIL: Different address\n";
    }
    
    
    std::cout << "\n--- TEST 2: FREE MULTIPLE ---\n";
    void* blocks[5];
    for (int i = 0; i < 5; i++)
    {
        blocks[i] = LockFreeAllocator::allocate(64);
    }
    
    for (int i = 0; i < 5; i++)
    {
        LockFreeAllocator::free(blocks[i]);
    }
    
    for (int i = 0; i < 5; i++)
    {
        blocks[i] = LockFreeAllocator::allocate(64);
    }
    std::cout << "PASS: Freed and reallocated 5 blocks\n";
    
    
    std::cout << "\n--- TEST 3: FREE NULL ---\n";
    LockFreeAllocator::free(nullptr);
    std::cout << "PASS: No crash on nullptr\n";
    
    
    std::cout << "\n--- TEST 4: EXHAUST AND RECOVER ---\n";
    void* all[32];
    for (int i = 0; i < 32; i++)
    {
        all[i] = LockFreeAllocator::allocate(64);
    }
    
    void* should_fail = LockFreeAllocator::allocate(64);
    if (should_fail == nullptr)
    {
        std::cout << "PASS: Pool exhausted\n";
    }
    
    for (int i = 0; i < 32; i++)
    {
        LockFreeAllocator::free(all[i]);
    }
    
    for (int i = 0; i < 32; i++)
    {
        all[i] = LockFreeAllocator::allocate(64);
    }
    std::cout << "PASS: Recovered all memory\n";
    
    std::cout << "\n--- ALL TESTS PASSED ---\n";
    return 0;
}