/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_utf_i.h"

extern string_utf_i STRING_UTF_8_i;

string_utf_i*   string_utf_8_interface();
word_t          string_utf_8_unit_size();
word_t          string_utf_8_max_unit_sequence_count();

bool_t          string_utf_8_has_terminator(const string_utf_t* it);
error_e         string_utf_8_terminate(string_utf_t* it);

bool_t          string_utf_8_point_ahead(const string_utf_t* it, word_t index, u32_t* out_point, u8_t* out_sequence_count);
bool_t          string_utf_8_point_behind(const string_utf_t* it, word_t index, u32_t* out_point, u8_t* out_sequence_count);

bool_t          string_utf_8_join(string_utf_t* it, const string_utf_t* other); // this will check if they are the same type for efficiency





// these are going to be changed into more abstracted funcs that don't need to know so much about other utf types
error_e             string_utf_8_convert(const string_utf_t* it, string_utf_t* into);

error_e             string_utf_8_to_16_le(const string_utf_t* it, string_utf_t* into);
error_e             string_utf_8_to_16_be(const string_utf_t* it, string_utf_t* into);
error_e             string_utf_8_to_32_le(const string_utf_t* it, string_utf_t* into);
error_e             string_utf_8_to_32_be(const string_utf_t* it, string_utf_t* into);
