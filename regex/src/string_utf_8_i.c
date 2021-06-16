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

word_t string_utf_8_max_unit_sequence_count()
{
    return 4;
}

bool_t string_utf_8_has_terminator(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));
    assert(string_utf_interface(it) == string_utf_8_interface());

    word_t unit_count = array_unit_count(&it->array);
    if (unit_count > 0) {
        utf_8_t* last = array_access(&it->array, unit_count - 1);
        return *last == 0;
    } else {
        return 0;
    }
}

error_e string_utf_8_terminate(string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));
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

bool_t string_utf_8_point_ahead(const string_utf_t* it, word_t index, u32_t* out_point, u8_t* out_sequence_count)
{
    assert(it);
    assert(out_point);
    assert(out_sequence_count);
    assert(string_utf_is_valid(it));
    assert(index < string_utf_unit_count(it));

    utf_sequence_8_t from;
    *out_sequence_count = utf_sequence_8_read_ahead(&from, array_access(&it->array, index));

    utf_sequence_32_t to;
    utf_sequence_8_to_32(&from, &to);
    *out_point = to.a;
}

void_t string_utf_8_first(const string_utf_t* it, string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);
    assert(string_utf_is_valid(it));

    iterator->unit_index = 0;
    iterator->point_index = 0;
    iterator->is_in_reverse = false;

    utf_sequence_8_t from;
    iterator->unit_sequence_count = utf_sequence_8_read(&from, array_access(&it->array, iterator->unit_index));

    utf_sequence_32_t to;
    utf_sequence_8_to_32(&from, &to);
    iterator->point = to.a;
}

void_t string_utf_8_last(const string_utf_t* it, string_utf_iterator_t* iterator)
{

}

void_t string_utf_8_end(const string_utf_t* it, string_utf_iterator_t* iterator)
{

}

bool_t string_utf_8_next(const string_utf_t* it, string_utf_iterator_t* iterator)
{

}

bool_t string_utf_8_previous(const string_utf_t* it, string_utf_iterator_t* iterator)
{

}
