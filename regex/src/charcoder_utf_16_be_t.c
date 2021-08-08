/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_utf_16_be_t.h"
#include "regex/init.h"
#include "regex/string_t.h"
#include "regex/utf.h"
#include "regex/utf_sequence_t.h"

word_t charcoder_utf_16_be_unit_size()
{
    return sizeof(utf_16_t);
}

void_t charcoder_utf_16_be_read_forward(const void_t* from, string_subsequence_t* result)
{
    assert(from);
    assert(result);

    if (OS_IS_BIG_ENDIAN) {
        result->units_read = utf_16_subsequence_forward(
            (utf_16_subsequence_t*)result,
            (utf_16_t*)from
        );
    } else if (OS_IS_LITTLE_ENDIAN) {
        result->units_read = utf_16_subsequence_swapped_forward(
            (utf_16_subsequence_t*)result,
            (utf_16_t*)from
        );
    } else {
        assert(0);
    }
}

void_t charcoder_utf_16_be_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result)
{
    assert(from);
    assert(first);
    assert(result);
    assert(from > first);

    if (OS_IS_BIG_ENDIAN) {
        result->units_read = utf_16_subsequence_reverse(
            (utf_16_subsequence_t*)result,
            (utf_16_t*)from,
            (utf_16_t*)first
        );
    } else if (OS_IS_LITTLE_ENDIAN) {
        result->units_read = utf_16_subsequence_swapped_reverse(
            (utf_16_subsequence_t*)result,
            (utf_16_t*)from,
            (utf_16_t*)first
        );
    } else {
        assert(0);
    }
}

void_t charcoder_utf_16_be_to_point(const string_subsequence_t* subsequence, u32_t* result)
{
    assert(subsequence);
    assert(result);

    utf_32_subsequence_t to;

    if (OS_IS_BIG_ENDIAN) {
        utf_16_subsequence_to_32((utf_16_subsequence_t*)subsequence, &to);
    } else if (OS_IS_LITTLE_ENDIAN) {
        utf_16_subsequence_swapped_to_32((utf_16_subsequence_t*)subsequence, &to);
    } else {
        assert(0);
    }

    *result = to.a;
}

void_t charcoder_utf_16_be_to_subsequence(u32_t point, string_subsequence_t* result)
{
    assert(result);

    if (!utf_32_is_scalar(point)) {
        point = UTF_REPLACEMENT_CHARACTER;
    }

    utf_32_subsequence_t from;
    from.a = point;
    from.unit_count = 1;

    if (OS_IS_BIG_ENDIAN) {
        utf_32_subsequence_to_16(&from, (utf_16_subsequence_t*)result);
    } else if (OS_IS_LITTLE_ENDIAN) {
        utf_32_subsequence_to_16_swapped(&from, (utf_16_subsequence_t*)result);
    } else {
        assert(0);
    }

    result->units_read = ((utf_16_subsequence_t*)result)->unit_count;
}

charcoder_utf_16_be_t CHARCODER_UTF_16_BE = DEFINE_CHARCODER_i(charcoder_utf_16_be);
