/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

enum
{
    UTF_POINT_FIRST                         = 0x00000000,
    UTF_POINT_LAST                          = 0x0010FFFF,

    UTF_SURROGATE_HIGH_FIRST                = 0x0000D800,
    UTF_SURROGATE_HIGH_LAST                 = 0x0000DBFF,
    UTF_SURROGATE_LOW_FIRST                 = 0x0000DC00,
    UTF_SURROGATE_LOW_LAST                  = 0x0000DFFF,

    UTF_BYTE_ORDER_MARKER                   = 0x0000FEFF,
    UTF_REPLACEMENT_CHARACTER               = 0x0000FFFD,

    UTF_16_REPLACEMENT_CHARACTER_SWAPPED    = 0xFDFF,

    UTF_32_REPLACEMENT_CHARACTER_SWAPPED    = 0xFDFF0000,
};

typedef uint8_t     utf_8_t;
typedef uint16_t    utf_16_t;
typedef uint32_t    utf_32_t;

bool_t  utf_16_is_surrogate(utf_16_t it);
bool_t  utf_16_is_high_surrogate(utf_16_t it);
bool_t  utf_16_is_low_surrogate(utf_16_t it);

bool_t  utf_32_is_point(utf_32_t it);
bool_t  utf_32_is_scalar(utf_32_t it);
bool_t  utf_32_is_surrogate(utf_32_t it);
bool_t  utf_32_is_high_surrogate(utf_32_t it);
bool_t  utf_32_is_low_surrogate(utf_32_t it);
