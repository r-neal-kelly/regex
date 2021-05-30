/*
    Copyright 2021 r-neal-kelly
*/

#include "assert.h"

#include "regex/allocator_i.h"
#include "regex/memory_t.h"

bool_t memory_create(memory_t* it, allocator_i* allocator, word_t byte_count)
{
    assert(it);
    assert(allocator);
    assert(byte_count > 0);

    it->allocator = allocator;
    return it->allocator->allocate(&it->pointer, byte_count);
}

void_t memory_destroy(memory_t* it)
{
    assert(it);

    if (it->pointer.bytes) {
        it->allocator->deallocate(&it->pointer);
    }
}

bool_t memory_is_valid(memory_t* it)
{
    assert(it);

    return it->allocator && pointer_is_valid(&it->pointer);
}
