/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>
#include <string.h>

#include "regex/array_t.h"

error_e array_create(array_t* it, allocator_i* allocator, word_t unit_size, word_t reserve_unit_count, float_t grow_rate)
{
    assert(it);
    assert(allocator);
    assert(unit_size > 0);
    assert(reserve_unit_count > 0);
    assert(grow_rate >= 1.0f);

    error_e error = memory_create(&it->memory, allocator, unit_size * reserve_unit_count);
    if (error) {
        return error;
    }

    it->unit_size = unit_size;
    it->unit_count = 0;
    it->grow_rate = grow_rate;

    return error;
}

void_t array_destroy(array_t* it)
{
    assert(it);

    if (memory_is_valid(&it->memory)) {
        memory_destroy(&it->memory);
    }
    
    it->unit_size = 0;
    it->unit_count = 0;
    it->grow_rate = 0.0f;
}

bool_t array_is_valid(const array_t* it)
{
    assert(it);

    return memory_is_valid(&it->memory) && it->unit_size > 0 && it->grow_rate >= 1.0f;
}

error_e array_reserve(array_t* it, word_t reserve_unit_count)
{
    assert(it);
    assert(reserve_unit_count > 0);
    assert(array_is_valid(it));

    if (reserve_unit_count <= MAX_WORD / it->unit_size) {
        word_t reserve_byte_count = it->unit_size * reserve_unit_count;
        if (it->memory.pointer.byte_count < reserve_byte_count) {
            return memory_reserve(&it->memory, reserve_byte_count);
        } else {
            return ERROR_NONE_e;
        }
    } else {
        return ERROR_OVERFLOW_e;
    }
}

word_t array_capacity(const array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->memory.pointer.byte_count / it->unit_size;
}

bool_t array_should_grow(const array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    word_t new_unit_count = it->unit_count + 1;
    if (new_unit_count > it->unit_count) {
        word_t new_byte_count = new_unit_count * it->unit_size;
        if (new_byte_count > it->unit_count * it->unit_size) {
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

    word_t reserve_byte_count = (word_t)(it->memory.pointer.byte_count * it->grow_rate) + it->unit_size;
    if (reserve_byte_count < it->memory.pointer.byte_count) {
        reserve_byte_count = it->memory.pointer.byte_count + it->unit_size;
        if (reserve_byte_count < it->memory.pointer.byte_count) {
            return ERROR_OVERFLOW_e;
        }
    }

    return memory_reserve(&it->memory, reserve_byte_count);
}

void_t* array_bytes(const array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->memory.pointer.bytes;
}

word_t array_unit_size(const array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->unit_size;
}

word_t array_unit_count(const array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    return it->unit_count;
}

void_t* array_access(const array_t* it, word_t index)
{
    assert(it);
    assert(array_is_valid(it));
    assert(index < it->unit_count);

    return it->memory.pointer.bytes + (index * it->unit_size);
}

error_e array_push(array_t* it, const void_t* unit_in)
{
    assert(it);
    assert(unit_in);
    assert(array_is_valid(it));
    assert(it->unit_count + 1 > it->unit_count);

    if (array_should_grow(it)) {
        error_e error = array_grow(it);
        if (error) {
            return error;
        }
    }

    memcpy(it->memory.pointer.bytes + (it->unit_count * it->unit_size), unit_in, it->unit_size);
    it->unit_count += 1;

    return ERROR_NONE_e;
}

bool_t array_pop(array_t* it, void_t* unit_out)
{
    assert(it);
    assert(unit_out);
    assert(array_is_valid(it));

    if (it->unit_count > 0) {
        it->unit_count -= 1;
        memcpy(unit_out, it->memory.pointer.bytes + (it->unit_count * it->unit_size), it->unit_size);
        return 1;
    } else {
        return 0;
    }
}

void_t array_clear(array_t* it)
{
    assert(it);
    assert(array_is_valid(it));

    it->unit_count = 0;
}
