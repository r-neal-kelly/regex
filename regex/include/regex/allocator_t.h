/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

typedef struct pointer_t pointer_t;

typedef struct allocator_t
{
    bool_t  (*allocate)(pointer_t* it, word_t byte_count);
    bool_t  (*reallocate)(pointer_t* it, word_t new_byte_count);
    void_t  (*deallocate)(pointer_t* it);
}
allocator_t;
