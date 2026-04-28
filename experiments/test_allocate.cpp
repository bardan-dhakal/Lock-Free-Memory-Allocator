#include "../include/allocator.hpp"
#include "../include/allocate.hpp"
#include <iostream>
#include <cstring>

struct MixedData
{
    char my_string[40];  
    size_t size;
};

int main()
{
    
    LockFreeAllocator::initialize_allocator();
    std::cout << "Allocator initialized\n\n";
    
    // Test 1: Single allocation
    std::cout << "=== Test 1: Single Allocation ===\n";
    void* ptr1 = LockFreeAllocator::allocate(64);
    
    if (ptr1 == nullptr)
    {
        std::cout << "Allocation failed!\n";
        return 1;
    }
    
    std::cout << "Allocated at: " << ptr1 << "\n";
    
    // Write and read data
    int* data = static_cast<int*>(ptr1);
    data[0] = 42;
    data[1] = 99;
    
    std::cout << "Wrote: 42, 99\n";
    std::cout << "Read:  " << data[0] << ", " << data[1] << "\n\n";
    
    // Test 2: Multiple allocations
    std::cout << "=== Test 2: Multiple Allocations ===\n";
    void* ptr2 = LockFreeAllocator::allocate(32);
    void* ptr3 = LockFreeAllocator::allocate(16);
    void* ptr4 = LockFreeAllocator::allocate(64);
    
    std::cout << "Ptr 1: " << ptr1 << "\n";
    std::cout << "Ptr 2: " << ptr2 << "\n";
    std::cout << "Ptr 3: " << ptr3 << "\n";
    std::cout << "Ptr 4: " << ptr4 << "\n\n";
    
    // Test 3: Size validation
    std::cout << "=== Test 3: Size Validation ===\n";
    void* big = LockFreeAllocator::allocate(100);  
    
    if (big == nullptr)
    {
        std::cout << "Correctly rejected 100-byte request\n\n";
    }
    else
    {
        std::cout << " Should have rejected 100-byte request\n\n";
    }
    
    // Test 4: Mixed data types with proper offsets
    std::cout << "=== Test 4: Mixed Data Types ===\n";
    void* test_block = LockFreeAllocator::allocate(64);
    
    if (test_block == nullptr)
    {
        std::cout << " Allocation failed!\n";
        return 1;
    }
    
    std::cout << "Allocated at: " << test_block << "\n";
    
    
    int* int_data = static_cast<int*>(test_block);
    int_data[0] = 1234;
    int_data[1] = 48;
    
    
    char* char_data = static_cast<char*>(test_block) + 8;
    char_data[0] = 'A';
    
    
    bool* bool_data = reinterpret_cast<bool*>(static_cast<char*>(test_block) + 9);
    *bool_data = true;
    
    std::cout << "Int data: " << int_data[0] << ", " << int_data[1] << "\n";
    std::cout << "Char data: " << char_data[0] << "\n";
    std::cout << "Bool data: " << *bool_data << "\n";
    
   
    MixedData* mixed = reinterpret_cast<MixedData*>(static_cast<char*>(test_block) + 16);
    strcpy(mixed->my_string, "Hello There");
    mixed->size = strlen(mixed->my_string);
    
    std::cout << "Mixed data string: " << mixed->my_string << "\n";
    std::cout << "Mixed data size: " << mixed->size << "\n\n";
    
    // Test 5: Exhaust the pool
    std::cout << "=== Test 5: Exhaust Pool ===\n";
    int count = 5;  // Already allocated 5 blocks
    
    while (true)
    {
        void* p = LockFreeAllocator::allocate(64);
        if (p == nullptr)
        {
            break;
        }
        count++;
    }
    
    std::cout << "Total allocations: " << count << "\n";
    std::cout << "Expected: 46\n";
    
    if (count == 32)
    {
        std::cout << "Correct!\n";
    }
    else
    {
        std::cout << " Wrong count! Got " << count << "\n";
    }
    
    return 0;
}