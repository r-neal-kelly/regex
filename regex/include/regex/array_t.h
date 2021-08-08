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
    word_t      unit_size;
    word_t      unit_count;
    float_t     grow_rate;
}
array_t;

error_e array_create(array_t* it,
                     allocator_i* allocator,
                     word_t unit_size,
                     word_t reserve_unit_count,
                     word_t preface_unit_count,
                     float_t grow_rate);
void_t  array_destroy(array_t* it);
bool_t  array_is_valid(const array_t* it);

error_e array_reserve(array_t* it, word_t reserve_unit_count);
word_t  array_capacity(const array_t* it);

float_t array_grow_rate(const array_t* it);
bool_t  array_should_grow(const array_t* it);
error_e array_grow(array_t* it);

void_t* array_raw(const array_t* it);

word_t  array_unit_size(const array_t* it);
word_t  array_unit_count(const array_t* it);

void_t* array_access(const array_t* it, word_t index);
error_e array_push(array_t* it, const void_t* unit_in);
error_e array_push_none(array_t* it);
bool_t  array_pop(array_t* it, void_t* unit_out);
bool_t  array_pop_none(array_t* it);

void_t  array_clear(array_t* it);
