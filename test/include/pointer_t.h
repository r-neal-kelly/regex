/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include <stdio.h>

#include "regex/pointer_t.h"

#include "intrinsic.h"

inline void_t test_pointer()
{
    {
        pointer_t ptr;
        ptr.bytes = 0;
        ptr.byte_count = 0;
        test(pointer_is_valid(&ptr) == false);
    }
    {
        pointer_t ptr;
        ptr.bytes = (void_t*)1;
        ptr.byte_count = 0;
        test(pointer_is_valid(&ptr) == false);
    }
    {
        pointer_t ptr;
        ptr.bytes = (void_t*)1;
        ptr.byte_count = 1;
        test(pointer_is_valid(&ptr) == true);
    }
}
