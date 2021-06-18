/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_i.h"

extern string_i STRING_UTF_8_i;

string_i*   string_utf_8_interface();

word_t      string_utf_8_unit_size();

void_t      string_utf_8_read(const void_t* at, string_subsequence_t* result);
void_t      string_utf_8_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result);
u32_t       string_utf_8_point(const string_subsequence_t* subsequence);

bool_t      string_utf_8_has_null(const string_t* it);
error_e     string_utf_8_push_null(string_t* it);
void_t*     string_utf_8_at_null(const string_t* it);

error_e     string_utf_8_push_point(string_t* it, u32_t point);
error_e     string_utf_8_push_raw(string_t* it, const void_t* raw, string_i* raw_interface);

bool_t      string_utf_8_join(string_t* it, const string_t* other); // this will check if they are the same type for efficiency





// these are going to be changed into more abstracted funcs that don't need to know so much about other utf types
error_e             string_utf_8_convert(const string_t* it, string_t* into);

error_e             string_utf_8_to_16_le(const string_t* it, string_t* into);
error_e             string_utf_8_to_16_be(const string_t* it, string_t* into);
error_e             string_utf_8_to_32_le(const string_t* it, string_t* into);
error_e             string_utf_8_to_32_be(const string_t* it, string_t* into);
