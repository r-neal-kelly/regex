/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/utf.h"

typedef struct string_utf_t string_utf_t;

typedef struct string_utf_iterator_t
{
    word_t      unit_index;
    word_t      point_index;
    u8_t        read_unit_count;
    bool_t      is_in_reverse;
    utf_32_t    point;
}
string_utf_iterator_t;
