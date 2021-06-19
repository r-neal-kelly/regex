/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_ascii_i.h"
#include "regex/string_t.h"

charcoder_i CHARCODER_ASCII_i = DEFINE_CHARCODER_i(charcoder_ascii);

word_t charcoder_ascii_unit_size()
{
    return sizeof(u8_t);
}

void_t charcoder_ascii_read_forward(const void_t* from, string_subsequence_t* result)
{
    assert(from);
    assert(result);

    *(u8_t*)result = *(u8_t*)from;
    result->units_read = 1;
}

void_t charcoder_ascii_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result)
{
    assert(from);
    assert(first);
    assert(result);
    assert(from > first);

    *(u8_t*)result = *((u8_t*)from - 1);
    result->units_read = 1;
}

void_t charcoder_ascii_to_point(const string_subsequence_t* subsequence, u32_t* result)
{
    assert(subsequence);
    assert(result);

    *result = *(u8_t*)subsequence;
}

void_t charcoder_ascii_to_subsequence(u32_t point, string_subsequence_t* result)
{
    assert(result);

    if (point > 127) {
        point = '?';
    }

    *(u8_t*)result = (u8_t)point;
    result->units_read = 1;
}
