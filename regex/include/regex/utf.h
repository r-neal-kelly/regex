/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

enum
{
    UTF_POINT_FIRST             = 0x00000000,
    UTF_POINT_LAST              = 0x0010FFFF,

    UTF_SURROGATE_HIGH_FIRST    = 0x0000D800,
    UTF_SURROGATE_HIGH_LAST     = 0x0000DBFF,
    UTF_SURROGATE_LOW_FIRST     = 0x0000DC00,
    UTF_SURROGATE_LOW_LAST      = 0x0000DFFF,
};

typedef uint8_t     utf_8_t;
typedef uint16_t    utf_16_t;
typedef uint32_t    utf_32_t;

bool_t  utf_32_is_point(utf_32_t it);
bool_t  utf_32_is_scalar_point(utf_32_t it);
