/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/string_t.h"
#include "regex/string_utf_16_be_i.h"
#include "regex/utf.h"

//string_i STRING_UTF_16_BE_i = DEFINE_STRING_i(string_utf_16_be);

string_i* string_utf_16_be_interface()
{
    return &STRING_UTF_16_BE_i;
}

word_t string_utf_16_be_unit_size()
{
    return sizeof(utf_16_t);
}
