#include <iostream>
#include "../include/allocator.hpp"

int main()
{
    LockFreeAllocator::initialize_allocator();

    BlockHeader* current = LockFreeAllocator::free_list_head.load();  // Load the atomic
    
    if (current)
    {
        std::cout << "Free List Head Initialized\n" << std::endl;
    }

    size_t block_count = 0;
    while (current)  // Use the loaded copy
    {
        block_count++;
        
        std::cout << "Block " << block_count << " address - " << static_cast<void*>(current) << std::endl;
        std::cout << "Block Size - " << current->size << std::endl;
        std::cout << "Block Header Size - " << sizeof(*current) << std::endl;
        std::cout << "Next Block - " << static_cast<void*>(current->next) << std::endl;
        std::cout << "\n\n";

        current = current->next;  // Move to next using the copy
    }

    if (block_count != 32)
    {
        std::cout << "Block number is not 46." << std::endl;
    }

    return 0;
}