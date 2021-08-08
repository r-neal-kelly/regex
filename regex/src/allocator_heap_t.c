/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/allocator_heap_t.h"
#include "regex/os.h"
#include "regex/pointer_t.h"

bool_t allocator_heap_allocate(pointer_t* it, word_t byte_count)
{
    assert(it);
    assert(byte_count > 0);

    it->bytes = os_alloc(byte_count);
    if (it->bytes) {
        it->byte_count = byte_count;
        return true;
    } else {
        it->byte_count = 0;
        return false;
    }
}

bool_t allocator_heap_reallocate(pointer_t* it, word_t new_byte_count)
{
    assert(it);
    assert(it->bytes);
    assert(new_byte_count > 0);

    byte_t* new_bytes = os_realloc(it->bytes, new_byte_count);
    if (new_bytes) {
        it->bytes = new_bytes;
        it->byte_count = new_byte_count;
        return true;
    } else {
        return false;
    }
}

void_t allocator_heap_deallocate(pointer_t* it)
{
    assert(it);

    if (it->bytes) {
        os_dealloc(it->bytes);
    }

    it->bytes = 0;
    it->byte_count = 0;
}

allocator_heap_t ALLOCATOR_HEAP = DEFINE_ALLOCATOR_i(allocator_heap);
