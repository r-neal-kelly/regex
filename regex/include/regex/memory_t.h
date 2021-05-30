/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/pointer_t.h"

typedef struct allocator_t allocator_t;

typedef struct memory_t
{
    pointer_t       pointer;
    allocator_t*    allocator;
}
memory_t;
