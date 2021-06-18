/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/utf.h"

bool_t utf_16_is_surrogate(utf_16_t it)
{
    return it >= UTF_SURROGATE_HIGH_FIRST && it <= UTF_SURROGATE_LOW_LAST;
}

bool_t utf_16_is_high_surrogate(utf_16_t it)
{
    return it >= UTF_SURROGATE_HIGH_FIRST && it <= UTF_SURROGATE_HIGH_LAST;
}

bool_t utf_16_is_low_surrogate(utf_16_t it)
{
    return it >= UTF_SURROGATE_LOW_FIRST && it <= UTF_SURROGATE_LOW_LAST;
}

bool_t utf_32_is_point(utf_32_t it)
{
    return it <= UTF_POINT_LAST;
}

bool_t utf_32_is_scalar(utf_32_t it)
{
    return it <= UTF_POINT_LAST && (it < UTF_SURROGATE_HIGH_FIRST || it > UTF_SURROGATE_LOW_LAST);
}

bool_t utf_32_is_surrogate(utf_32_t it)
{
    return it >= UTF_SURROGATE_HIGH_FIRST && it <= UTF_SURROGATE_LOW_LAST;
}

bool_t utf_32_is_high_surrogate(utf_32_t it)
{
    return it >= UTF_SURROGATE_HIGH_FIRST && it <= UTF_SURROGATE_HIGH_LAST;
}

bool_t utf_32_is_low_surrogate(utf_32_t it)
{
    return it >= UTF_SURROGATE_LOW_FIRST && it <= UTF_SURROGATE_LOW_LAST;
}
