/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/allocator_i.h"
#include "regex/memory_t.h"

error_e memory_create(memory_t* it, allocator_i* allocator, word_t reserve_byte_count, word_t preface_byte_count)
{
    assert(it);
    assert(allocator);
    assert(reserve_byte_count > 0);

    if (reserve_byte_count + it->preface_byte_count < reserve_byte_count) {
        it->pointer.bytes = 0;
        it->pointer.byte_count = 0;
        it->preface_byte_count = 0;
        it->allocator = 0;
        return ERROR_OVERFLOW_e;
    }

    it->pointer.bytes = 0;
    it->pointer.byte_count = 0;
    it->preface_byte_count = preface_byte_count;
    it->allocator = allocator;

    if (!it->allocator->allocate(&it->pointer, reserve_byte_count + preface_byte_count)) {
        it->pointer.bytes = 0;
        it->pointer.byte_count = 0;
        it->preface_byte_count = 0;
        it->allocator = 0;
        return ERROR_OUT_OF_MEMORY_e;
    }

    it->pointer.bytes += it->preface_byte_count;
    it->pointer.byte_count -= it->preface_byte_count;

    return ERROR_NONE_e;
}

void_t memory_destroy(memory_t* it)
{
    assert(it);

    it->pointer.bytes -= it->preface_byte_count;
    it->pointer.byte_count += it->preface_byte_count;

    if (pointer_is_valid(&it->pointer)) {
        it->allocator->deallocate(&it->pointer);
    }

    it->pointer.bytes = 0;
    it->pointer.byte_count = 0;
    it->preface_byte_count = 0;
    it->allocator = 0;
}

bool_t memory_is_valid(const memory_t* it)
{
    assert(it);

    return it->allocator && pointer_is_valid(&it->pointer);
}

error_e memory_reserve(memory_t* it, word_t reserve_byte_count)
{
    assert(it);
    assert(reserve_byte_count > 0);
    assert(memory_is_valid(it));

    if (reserve_byte_count > it->pointer.byte_count) {
        if (reserve_byte_count + it->preface_byte_count < reserve_byte_count) {
            return ERROR_OVERFLOW_e;
        }

        it->pointer.bytes -= it->preface_byte_count;
        it->pointer.byte_count += it->preface_byte_count;

        if (!it->allocator->reallocate(&it->pointer, reserve_byte_count + it->preface_byte_count)) {
            it->pointer.bytes += it->preface_byte_count;
            it->pointer.byte_count -= it->preface_byte_count;
            return ERROR_OUT_OF_MEMORY_e;
        }

        it->pointer.bytes += it->preface_byte_count;
        it->pointer.byte_count -= it->preface_byte_count;
    }

    return ERROR_NONE_e;
}
