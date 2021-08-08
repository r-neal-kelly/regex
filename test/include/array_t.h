/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/allocator_heap_t.h"
#include "regex/array_t.h"

#include "intrinsic.h"

inline void_t test_array_t()
{
    array_t array;

    test_begin(L"array_t: can create a reasonable array");
    {
        array_create(&array, &ALLOCATOR_HEAP, sizeof(word_t), 8, 0, 1.7f);
        test(array_is_valid(&array));
        test(array_unit_count(&array) == 0);
    }
    test_end();
}
