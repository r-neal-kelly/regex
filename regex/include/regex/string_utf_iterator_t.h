/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/utf.h"

typedef struct string_utf_t string_utf_t;

typedef struct string_utf_iterator_t
{
    u8_t    units_read;
    u32_t   point;
    word_t  unit_index;
    word_t  point_index;
}
string_utf_iterator_t;
