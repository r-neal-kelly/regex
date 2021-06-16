/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/string_utf_t.h"
#include "regex/string_utf_32_le_i.h"

string_utf_i STRING_UTF_32_LE_i = DEFINE_STRING_UTF_i(string_utf_32_le);

string_utf_i* string_utf_32_le_interface()
{
    return &STRING_UTF_32_LE_i;
}

word_t string_utf_32_le_unit_size()
{
    return sizeof(utf_32_t);
}
