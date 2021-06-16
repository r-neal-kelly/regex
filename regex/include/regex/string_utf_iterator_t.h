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
    bool_t      is_in_reverse;
    u8_t        unit_sequence_count;
    utf_32_t    point;
}
string_utf_iterator_t;
