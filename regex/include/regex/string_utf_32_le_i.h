/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_i.h"

extern string_i STRING_UTF_32_LE_i;

string_i*   string_utf_32_le_interface();

word_t      string_utf_32_le_unit_size();

void_t      string_utf_32_le_read(const void_t* raw, word_t unit_index, string_subsequence_t* result);
void_t      string_utf_32_le_read_reverse(const void_t* raw, word_t unit_index, string_subsequence_t* result);
u32_t       string_utf_32_le_point(const string_subsequence_t* subsequence);

bool_t      string_utf_32_le_has_terminator(const string_t* it);
error_e     string_utf_32_le_terminate(string_t* it);
word_t      string_utf_32_le_terminator_unit_index(const string_t* it);

error_e     string_utf_32_le_push_point(string_t* it, u32_t point);
