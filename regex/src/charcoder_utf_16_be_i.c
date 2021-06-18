/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_utf_16_be_i.h"
#include "regex/init.h"
#include "regex/string_t.h"
#include "regex/utf.h"
#include "regex/utf_sequence_t.h"

charcoder_i CHARCODER_UTF_16_BE_i = DEFINE_CHARCODER_i(charcoder_utf_16_be);

word_t charcoder_utf_16_be_unit_size()
{
    return sizeof(utf_16_t);
}

void_t charcoder_utf_16_be_read_forward(const void_t* from, string_subsequence_t* result)
{
    assert(from);
    assert(result);

    result->units_read = utf_16_subsequence_create(
        (utf_16_subsequence_t*)result,
        (utf_16_t*)from,
        OS_IS_LITTLE_ENDIAN
    );
}

void_t charcoder_utf_16_be_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result)
{
    assert(from);
    assert(first);
    assert(result);
    assert(from > first);

    result->units_read = utf_16_subsequence_create_reverse(
        (utf_16_subsequence_t*)result,
        (utf_16_t*)from,
        (utf_16_t*)first,
        OS_IS_LITTLE_ENDIAN
    );
}

void_t charcoder_utf_16_be_to_point(const string_subsequence_t* subsequence, u32_t* result)
{
    assert(subsequence);
    assert(result);

    utf_32_subsequence_t to;
    utf_16_subsequence_to_32((utf_16_subsequence_t*)subsequence, &to);
    *result = to.a;
}

void_t charcoder_utf_16_be_to_subsequence(u32_t point, string_subsequence_t* result)
{
    assert(result);

    if (!utf_32_is_scalar_point(point)) {
        point = UTF_REPLACEMENT_CHARACTER;
    }

    utf_32_subsequence_t from;
    from.a = point;
    from.unit_count = 1;
    utf_32_subsequence_to_16(&from, (utf_16_subsequence_t*)result);
    result->units_read = ((utf_16_subsequence_t*)result)->unit_count;
}
