/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/memory_t.h"

typedef struct array_t
{
    memory_t    memory;
    word_t      type_size;
    word_t      type_count;
    f32_t       grow_rate;
    u32_t       pad;
}
array_t;

void array_create(array_t* it, word_t type_count, f32_t grow_rate);
void array_destroy(array_t* it);
void array_reserve(array_t* it, word_t type_count);
