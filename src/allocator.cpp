#include <iostream>
#include <sys/mman.h>
#include "../include/allocator.hpp"
#include <atomic>


namespace LockFreeAllocator
{
    std::atomic<BlockHeader*> free_list_head {nullptr};


    void* allocate_raw_memory()
    {
        void* mem_addr;

        constexpr size_t size = PAGE_SIZE;
        constexpr int permissions = PROT_READ | PROT_WRITE;
        constexpr int flag = MAP_ANONYMOUS | MAP_PRIVATE;
        constexpr int file_descriptor = -1;
        constexpr off_t offset = 0;

        mem_addr = mmap(NULL, size, permissions, flag, file_descriptor, offset);

        if (mem_addr == MAP_FAILED)
        {
            throw std::runtime_error("Memory Allocation Failed");
        }


        return mem_addr;

    }

    void initialize_allocator()
    {

        void* raw_mem;

        try 
        {
            raw_mem = allocate_raw_memory();
        }

        catch (const std::runtime_error& e)
        {
            std::cerr << "Error" << e.what() << std::endl;
            return ;
        }

        size_t block_size = sizeof(BlockHeader) + PAYLOAD;
        int num_blocks = PAGE_SIZE / block_size;

        char* current_position = static_cast<char*>(raw_mem);

        for (size_t i{}; i < num_blocks; i++)
        {
        BlockHeader* block_header = reinterpret_cast<BlockHeader*>(current_position);
        
        block_header->size = PAYLOAD;
        block_header->is_free = true;

        if (i == num_blocks - 1)
        {
                block_header->next = nullptr;
        }

        else
        {
                block_header->next = reinterpret_cast<BlockHeader*>(reinterpret_cast<char*>(block_header) + block_size);
        }

        current_position += block_size;

        }

        free_list_head.store(reinterpret_cast<BlockHeader*>(raw_mem));
        

    }

}

