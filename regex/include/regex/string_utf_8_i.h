/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_utf_i.h"

extern string_utf_i STRING_UTF_8_i;

string_utf_type_e   string_utf_8_type();
word_t              string_utf_8_type_size();

bool_t              string_utf_8_has_terminator(const string_utf_t* it);
error_e             string_utf_8_terminate(string_utf_t* it);

// these are going to be changed into more abstracted funcs that don't need to know so much about other utf types
error_e             string_utf_8_convert(const string_utf_t* it, string_utf_t* into);

error_e             string_utf_8_to_16_le(const string_utf_t* it, string_utf_t* into);
error_e             string_utf_8_to_16_be(const string_utf_t* it, string_utf_t* into);
error_e             string_utf_8_to_32_le(const string_utf_t* it, string_utf_t* into);
error_e             string_utf_8_to_32_be(const string_utf_t* it, string_utf_t* into);
