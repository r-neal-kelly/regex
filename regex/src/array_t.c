/*
    Copyright 2021 r-neal-kelly
*/

#include "assert.h"

#include "regex/array_t.h"

bool_t array_create(array_t* it, allocator_i* allocator, word_t type_size, word_t reserve_type_count, f32_t grow_rate)
{
    assert(it);
    assert(allocator);
    assert(type_size > 0);
    assert(reserve_type_count > 0);
    assert(grow_rate >= 1.0f);

    it->type_size = type_size;
    it->type_count = 0;
    it->grow_rate = grow_rate;

    return memory_create(&it->memory, allocator, type_size * reserve_type_count);
}

void_t array_destroy(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    memory_destroy(&it->memory);
    it->type_size = 0;
    it->type_count = 0;
    it->grow_rate = 0.0f;
}

bool_t array_is_valid(array_t* it)
{
    assert(it);

    return memory_is_valid(&it->memory) && it->type_size > 0 && it->grow_rate >= 1.0f;
}
