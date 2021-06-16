/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/utf.h"

bool_t utf_32_is_point(utf_32_t it)
{
    return it <= UTF_POINT_LAST;
}

bool_t utf_32_is_scalar_point(utf_32_t it)
{
    return it <= UTF_POINT_LAST && (it < UTF_SURROGATE_HIGH_FIRST || it > UTF_SURROGATE_LOW_LAST);
}
