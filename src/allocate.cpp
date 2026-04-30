#include "../include/allocate.hpp"
#include <iostream>


namespace LockFreeAllocator
{
    void* allocate(size_t size)
    {

        if (size > PAYLOAD)
        {
            return nullptr;
        }

        BlockHeader* expected_head;
        BlockHeader* new_head;

        do
        {
            expected_head = free_list_head.load();
                
            if (expected_head == nullptr)
            {
                return nullptr;
            }
 
            new_head = expected_head->next;

        } while (!free_list_head.compare_exchange_weak(expected_head, new_head));


        expected_head->is_free = false;

        return reinterpret_cast<void*>(expected_head + 1);  

        


    }

    void free(void* ptr)
    {

        if (ptr == nullptr)
        {
            return;
        }

        BlockHeader* free_blk_header = reinterpret_cast<BlockHeader*>(ptr) - 1;
        BlockHeader* old_head;
        
        free_blk_header->is_free = true;
        
        do
        {
            old_head = free_list_head.load();
            free_blk_header->next = old_head;


        } while(!free_list_head.compare_exchange_weak(old_head, free_blk_header));



        return;

    }
}

