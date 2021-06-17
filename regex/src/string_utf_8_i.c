/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/string_t.h"
#include "regex/string_utf_8_i.h"
#include "regex/utf.h"
#include "regex/utf_sequence_t.h"

string_i STRING_UTF_8_i = DEFINE_STRING_i(string_utf_8);

string_i* string_utf_8_interface()
{
    return &STRING_UTF_8_i;
}

word_t string_utf_8_unit_size()
{
    return sizeof(utf_8_t);
}

void_t string_utf_8_read(const void_t* raw, word_t unit_index, string_subsequence_t* result)
{
    assert(raw);
    assert(result);

    result->units_read = utf_8_subsequence_create(
        (utf_8_subsequence_t*)result,
        (utf_8_t*)raw + unit_index
    );
}

void_t string_utf_8_read_reverse(const void_t* raw, word_t unit_index, string_subsequence_t* result)
{
    assert(raw);
    assert(result);
    assert(unit_index > 0);

    result->units_read = utf_8_subsequence_create_reverse(
        (utf_8_subsequence_t*)result,
        (utf_8_t*)raw + unit_index,
        (utf_8_t*)raw
    );
}

u32_t string_utf_8_point(const string_subsequence_t* subsequence)
{
    assert(subsequence);

    utf_32_subsequence_t point;
    utf_8_subsequence_to_32((utf_8_subsequence_t*)subsequence, &point);
    return point.a;
}

bool_t string_utf_8_has_terminator(const string_t* it)
{
    assert(it);

    word_t unit_count = array_unit_count(&it->array);
    if (unit_count > 0) {
        utf_8_t* last = array_access(&it->array, unit_count - 1);
        return *last == 0;
    } else {
        return false;
    }
}

error_e string_utf_8_terminate(string_t* it)
{
    assert(it);

    if (!string_utf_8_has_terminator(it)) {
        utf_8_t terminator = 0;
        error_e error = array_push(&it->array, &terminator);
        if (error) {
            return error;
        }
        it->point_count += 1;
    }

    return ERROR_NONE_e;
}

word_t string_utf_8_terminator_unit_index(const string_t* it)
{
    assert(it);
    assert(string_utf_8_has_terminator(it));

    return array_unit_count(&it->array) - 1;
}

static error_e _string_utf_8_push_sequence(string_t* it, utf_8_subsequence_t* sequence)
{
    assert(it);
    assert(sequence);

    error_e error;
    for (word_t idx = 0, end = sequence->unit_count; idx < end; idx += 1) {
        error = array_push(&it->array, ((utf_8_t*)sequence) + idx);
        if (error) {
            return error;
        }
    }
    it->point_count += 1;

    return ERROR_NONE_e;
}

error_e string_utf_8_push_point(string_t* it, u32_t point)
{
    assert(it);

    if (point != 0) {
        if (!utf_32_is_scalar_point(point)) {
            point = UTF_REPLACEMENT_CHARACTER;
        }

        utf_32_subsequence_t from;
        from.a = point;
        from.unit_count = 1;

        utf_8_subsequence_t to;
        utf_32_subsequence_to_8(&from, &to);

        utf_8_t terminator;
        array_pop(&it->array, &terminator);

        error_e error = _string_utf_8_push_sequence(it, &to);
        if (error) {
            return error;
        }

        error = array_push(&it->array, &terminator);
        if (error) {
            return error;
        }
    }

    return ERROR_NONE_e;
}

error_e string_utf_8_push_raw(string_t* it, const void_t* raw, string_i* raw_interface)
{
    assert(it);
    assert(raw);
    assert(raw_interface);

    error_e error;

    utf_8_t terminator;
    array_pop(&it->array, &terminator);

    string_subsequence_t subsequence;
    if (raw_interface == string_utf_8_interface()) {
        for (word_t idx = 0; ;) {
            raw_interface->read(raw, idx, &subsequence);
            if (((utf_8_subsequence_t*)&subsequence)->a) {
                error = _string_utf_8_push_sequence(it, (utf_8_subsequence_t*)&subsequence);
                if (error) {
                    return error;
                }
                idx += subsequence.units_read;
            } else {
                break;
            }
        }
    } else {
        utf_32_subsequence_t from;
        utf_8_subsequence_t to;
        from.unit_count = 1;
        for (word_t idx = 0; ;) {
            raw_interface->read(raw, idx, &subsequence);
            from.a = raw_interface->point(&subsequence);
            if (from.a) {
                utf_32_subsequence_to_8(&from, &to);
                error = _string_utf_8_push_sequence(it, &to);
                if (error) {
                    return error;
                }
                idx += subsequence.units_read;
            } else {
                break;
            }
        }
    }

    error = array_push(&it->array, &terminator);
    if (error) {
        return error;
    }

    return ERROR_NONE_e;
}
