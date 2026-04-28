#pragma once

#include <cstddef>

/*Block header is aligned to 64, so the payload is cache aligned, 
no two threads allocate to the same cache line*/
struct alignas(64) BlockHeader
{
    size_t size; //size of payload, 64 bytes only for now
    bool is_free; 
    BlockHeader* next;

};
