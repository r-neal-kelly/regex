/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/string_utf_16_le_i.h"

string_utf_type_e string_utf_16_le_type()
{
    return STRING_UTF_16_LE_e;
}

word_t string_utf_16_le_type_size()
{
    return sizeof(utf_16_t);
}

string_utf_i STRING_UTF_16_LE_i = DEFINE_STRING_UTF_i(string_utf_16_le);
