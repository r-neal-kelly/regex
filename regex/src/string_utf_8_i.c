/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/string_utf_t.h"
#include "regex/string_utf_8_i.h"
#include "regex/utf_sequence_t.h"

string_utf_i STRING_UTF_8_i = DEFINE_STRING_UTF_i(string_utf_8);

string_utf_i* string_utf_8_interface()
{
    return &STRING_UTF_8_i;
}

word_t string_utf_8_unit_size()
{
    return sizeof(utf_8_t);
}

bool_t string_utf_8_has_terminator(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_interface(it) == string_utf_8_interface());

    word_t unit_count = array_unit_count(&it->array);
    if (unit_count > 0) {
        utf_8_t* last = array_access(&it->array, unit_count - 1);
        return *last == 0;
    } else {
        return false;
    }
}

error_e string_utf_8_terminate(string_utf_t* it)
{
    assert(it);
    assert(string_utf_interface(it) == string_utf_8_interface());

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

word_t string_utf_8_terminator_unit_index(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_interface(it) == string_utf_8_interface());
    assert(string_utf_8_has_terminator(it));

    return array_unit_count(&it->array) - 1;
}

void_t string_utf_8_read(const string_utf_t* it, word_t unit_index, u8_t* out_units_read, utf_32_t* out_point)
{
    assert(it);
    assert(out_units_read);
    assert(out_point);
    assert(string_utf_interface(it) == string_utf_8_interface());
    assert(unit_index < it->array.unit_count);

    utf_8_subsequence_t from;
    utf_32_subsequence_t to;

    *out_units_read = utf_8_subsequence_create(
        &from,
        array_access(&it->array, unit_index)
    );
    
    utf_8_subsequence_to_32(&from, &to);
    *out_point = to.a;
}

void_t string_utf_8_read_reverse(const string_utf_t* it, word_t unit_index, u8_t* out_units_read, utf_32_t* out_point)
{
    assert(it);
    assert(out_units_read);
    assert(out_point);
    assert(string_utf_interface(it) == string_utf_8_interface());
    assert(unit_index > 0 && unit_index <= it->array.unit_count);

    utf_8_subsequence_t from;
    utf_32_subsequence_t to;

    *out_units_read = utf_8_subsequence_create_reverse(
        &from,
        array_access(&it->array, unit_index),
        array_access(&it->array, 0)
    );

    utf_8_subsequence_to_32(&from, &to);
    *out_point = to.a;
}
