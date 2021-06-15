/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>
#include <string.h>

#include "regex/array_t.h"

error_e array_create(array_t* it, allocator_i* allocator, word_t type_size, word_t reserve_type_count, float_t grow_rate)
{
    assert(it);
    assert(allocator);
    assert(type_size > 0);
    assert(reserve_type_count > 0);
    assert(grow_rate >= 1.0f);

    error_e error = memory_create(&it->memory, allocator, type_size * reserve_type_count);
    if (error) {
        return error;
    }

    it->type_size = type_size;
    it->type_count = 0;
    it->grow_rate = grow_rate;

    return error;
}

void_t array_destroy(array_t* it)
{
    assert(it);

    if (memory_is_valid(&it->memory)) {
        memory_destroy(&it->memory);
    }
    
    it->type_size = 0;
    it->type_count = 0;
    it->grow_rate = 0.0f;
}

bool_t array_is_valid(array_t* it)
{
    assert(it);

    return memory_is_valid(&it->memory) && it->type_size > 0 && it->grow_rate >= 1.0f;
}

error_e array_reserve(array_t* it, word_t reserve_type_count)
{
    assert(it);
    assert(reserve_type_count > 0);
    assert(array_is_valid(it));

    if (reserve_type_count <= MAX_WORD / it->type_size) {
        word_t reserve_byte_count = it->type_size * reserve_type_count;
        if (it->memory.pointer.byte_count < reserve_byte_count) {
            return memory_reserve(&it->memory, reserve_byte_count);
        } else {
            return ERROR_NONE_e;
        }
    } else {
        return ERROR_OVERFLOW_e;
    }
}

bool_t array_should_grow(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    word_t new_type_count = it->type_count + 1;
    if (new_type_count > it->type_count) {
        word_t new_byte_count = new_type_count * it->type_size;
        if (new_byte_count > it->type_count * it->type_size) {
            return new_byte_count > it->memory.pointer.byte_count;
        } else {
            return 1;
        }
    } else {
        return 1;
    }
}

error_e array_grow(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    word_t reserve_byte_count = (word_t)(it->memory.pointer.byte_count * it->grow_rate) + it->type_size;
    if (reserve_byte_count < it->memory.pointer.byte_count) {
        reserve_byte_count = it->memory.pointer.byte_count + it->type_size;
        if (reserve_byte_count < it->memory.pointer.byte_count) {
            return ERROR_OVERFLOW_e;
        }
    }

    return memory_reserve(&it->memory, reserve_byte_count);
}

word_t array_capacity(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->memory.pointer.byte_count / it->type_size;
}

word_t array_count(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->type_count;
}

word_t array_type_size(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->type_size;
}

void_t* array_access(array_t* it, word_t index)
{
    assert(it);
    assert(array_is_valid(it));
    assert(index < it->type_count);

    return it->memory.pointer.bytes + (index * it->type_size);
}

error_e array_push(array_t* it, const void_t* type_in)
{
    assert(it);
    assert(type_in);
    assert(array_is_valid(it));
    assert(it->type_count + 1 > it->type_count);

    if (array_should_grow(it)) {
        error_e error = array_grow(it);
        if (error) {
            return error;
        }
    }

    memcpy(it->memory.pointer.bytes + (it->type_count * it->type_size), type_in, it->type_size);
    it->type_count += 1;

    return ERROR_NONE_e;
}

bool_t array_pop(array_t* it, void_t* type_out)
{
    assert(it);
    assert(type_out);
    assert(array_is_valid(it));

    if (it->type_count > 0) {
        it->type_count -= 1;
        memcpy(type_out, it->memory.pointer.bytes + (it->type_count * it->type_size), it->type_size);
        return 1;
    } else {
        return 0;
    }
}

void_t array_clear(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    it->type_count = 0;
}
