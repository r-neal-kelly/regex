/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/string_utf_32_be_i.h"

string_utf_type_e string_utf_32_be_type()
{
    return STRING_UTF_32_BE_e;
}

word_t string_utf_32_be_type_size()
{
    return sizeof(utf_32_t);
}

string_utf_i STRING_UTF_32_BE_i = DEFINE_STRING_UTF_i(string_utf_32_be);
