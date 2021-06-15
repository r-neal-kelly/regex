/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"
#include "regex/memory_t.h"

typedef struct array_t
{
    memory_t    memory;
    word_t      type_size;
    word_t      type_count;
    float_t     grow_rate;
}
array_t;

error_e array_create(array_t* it, allocator_i* allocator, word_t type_size, word_t reserve_type_count, float_t grow_rate);
void_t  array_destroy(array_t* it);
bool_t  array_is_valid(array_t* it);

error_e array_reserve(array_t* it, word_t reserve_type_count);
bool_t  array_should_grow(array_t* it);
error_e array_grow(array_t* it);

word_t  array_capacity(array_t* it);
word_t  array_count(array_t* it);
word_t  array_type_size(array_t* it);

void_t* array_access(array_t* it, word_t index);
error_e array_push(array_t* it, const void_t* type_in);
bool_t  array_pop(array_t* it, void_t* type_out);

void_t  array_clear(array_t* it);
