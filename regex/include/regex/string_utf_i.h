/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

typedef struct string_utf_t string_utf_t;

typedef struct string_utf_i
{
    word_t(* const type)();
    void_t(* const push_null)(string_utf_t* it);
}
string_utf_i;

// split these up into their own headers, which will make the pattern obvious how to extend the system.
// we may as well make the funcs publically available also
extern string_utf_i STRING_UTF_8_i;
extern string_utf_i STRING_UTF_16_i;
extern string_utf_i STRING_UTF_16_le_i;
extern string_utf_i STRING_UTF_16_be_i;
extern string_utf_i STRING_UTF_32_i;
extern string_utf_i STRING_UTF_32_le_i;
extern string_utf_i STRING_UTF_32_be_i;
