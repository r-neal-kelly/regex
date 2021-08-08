/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_utf_32_be_t.h"
#include "regex/init.h"
#include "regex/os.h"
#include "regex/string_t.h"
#include "regex/utf.h"
#include "regex/utf_sequence_t.h"

word_t charcoder_utf_32_be_unit_size()
{
    return sizeof(utf_32_t);
}

void_t charcoder_utf_32_be_read_forward(const void_t* from, string_subsequence_t* result)
{
    assert(from);
    assert(result);

    if (OS_IS_BIG_ENDIAN) {
        result->units_read = utf_32_subsequence_forward(
            (utf_32_subsequence_t*)result,
            (utf_32_t*)from
        );
    } else if (OS_IS_LITTLE_ENDIAN) {
        result->units_read = utf_32_subsequence_swapped_forward(
            (utf_32_subsequence_t*)result,
            (utf_32_t*)from
        );
    } else {
        assert(0);
    }
}

void_t charcoder_utf_32_be_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result)
{
    assert(from);
    assert(first);
    assert(result);
    assert(from > first);

    if (OS_IS_BIG_ENDIAN) {
        result->units_read = utf_32_subsequence_reverse(
            (utf_32_subsequence_t*)result,
            (utf_32_t*)from,
            (utf_32_t*)first
        );
    } else if (OS_IS_LITTLE_ENDIAN) {
        result->units_read = utf_32_subsequence_swapped_reverse(
            (utf_32_subsequence_t*)result,
            (utf_32_t*)from,
            (utf_32_t*)first
        );
    } else {
        assert(0);
    }
}

void_t charcoder_utf_32_be_to_point(const string_subsequence_t* subsequence, u32_t* result)
{
    assert(subsequence);
    assert(result);

    if (OS_IS_BIG_ENDIAN) {
        *result = ((utf_32_subsequence_t*)subsequence)->a;
    } else if (OS_IS_LITTLE_ENDIAN) {
        *result = os_swap_bytes_u32(((utf_32_subsequence_t*)subsequence)->a);
    } else {
        assert(0);
    }
}

void_t charcoder_utf_32_be_to_subsequence(u32_t point, string_subsequence_t* result)
{
    assert(result);

    if (!utf_32_is_scalar(point)) {
        point = UTF_REPLACEMENT_CHARACTER;
    }

    if (OS_IS_BIG_ENDIAN) {
        ((utf_32_subsequence_t*)result)->a = point;
    } else if (OS_IS_LITTLE_ENDIAN) {
        ((utf_32_subsequence_t*)result)->a = os_swap_bytes_u32(point);
    } else {
        assert(0);
    }

    ((utf_32_subsequence_t*)result)->unit_count = 1;
    result->units_read = 1;
}

charcoder_utf_32_be_t CHARCODER_UTF_32_BE = DEFINE_CHARCODER_i(charcoder_utf_32_be);
