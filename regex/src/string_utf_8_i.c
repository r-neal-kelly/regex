/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/string_utf_8_i.h"
#include "regex/string_utf_t.h"

string_utf_type_e string_utf_8_type()
{
    return STRING_UTF_8_e;
}

word_t string_utf_8_type_size()
{
    return sizeof(utf_8_t);
}

bool_t string_utf_8_has_terminator(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));
    assert(string_utf_type(it) == string_utf_8_type());

    word_t type_count = array_count(&it->array);
    if (type_count > 0) {
        utf_8_t* last = array_access(&it->array, type_count - 1);
        return *last == 0;
    } else {
        return 0;
    }
}

error_e string_utf_8_terminate(string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));
    assert(string_utf_type(it) == string_utf_8_type());

    if (!string_utf_8_has_terminator(it)) {
        utf_8_t terminator = 0;
        error_e error = array_push(&it->array, &terminator);
        if (error) {
            return error;
        }
    }

    return ERROR_NONE_e;
}

string_utf_i STRING_UTF_8_i = DEFINE_STRING_UTF_i(string_utf_8);
