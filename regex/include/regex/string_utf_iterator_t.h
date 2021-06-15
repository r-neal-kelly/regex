/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

typedef struct string_utf_iterator_t
{
    word_t      type_index;
    word_t      point_index;
    utf_32_t    point;
    u8_t        sequence_type_count;
    bool_t      is_in_reverse;
}
string_utf_iterator_t;
