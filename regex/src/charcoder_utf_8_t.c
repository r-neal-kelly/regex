/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_utf_8_t.h"
#include "regex/string_t.h"
#include "regex/utf.h"
#include "regex/utf_sequence_t.h"

word_t charcoder_utf_8_unit_size()
{
    return sizeof(utf_8_t);
}

void_t charcoder_utf_8_read_forward(const void_t* from, string_subsequence_t* result)
{
    assert(from);
    assert(result);

    result->units_read = utf_8_subsequence_forward(
        (utf_8_subsequence_t*)result,
        (utf_8_t*)from
    );
}

void_t charcoder_utf_8_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result)
{
    assert(from);
    assert(first);
    assert(result);
    assert(from > first);

    result->units_read = utf_8_subsequence_reverse(
        (utf_8_subsequence_t*)result,
        (utf_8_t*)from,
        (utf_8_t*)first
    );
}

void_t charcoder_utf_8_to_point(const string_subsequence_t* subsequence, u32_t* result)
{
    assert(subsequence);
    assert(result);

    utf_32_subsequence_t to;
    utf_8_subsequence_to_32((utf_8_subsequence_t*)subsequence, &to);
    *result = to.a;
}

void_t charcoder_utf_8_to_subsequence(u32_t point, string_subsequence_t* result)
{
    assert(result);

    if (!utf_32_is_scalar(point)) {
        point = UTF_REPLACEMENT_CHARACTER;
    }

    utf_32_subsequence_t from;
    from.a = point;
    from.unit_count = 1;
    utf_32_subsequence_to_8(&from, (utf_8_subsequence_t*)result);
    result->units_read = ((utf_8_subsequence_t*)result)->unit_count;
}

charcoder_utf_8_t CHARCODER_UTF_8 = DEFINE_CHARCODER_i(charcoder_utf_8);
