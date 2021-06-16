/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"
#include "regex/pointer_t.h"

typedef struct allocator_i allocator_i;

typedef struct memory_t
{
    pointer_t       pointer;
    allocator_i*    allocator;
}
memory_t;

error_e memory_create(memory_t* it, allocator_i* allocator, word_t reserve_byte_count);
void_t  memory_destroy(memory_t* it);
bool_t  memory_is_valid(const memory_t* it);
error_e memory_reserve(memory_t* it, word_t reserve_byte_count);
