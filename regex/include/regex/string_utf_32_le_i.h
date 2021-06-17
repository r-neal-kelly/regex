/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_utf_i.h"

extern string_utf_i STRING_UTF_32_LE_i;

string_utf_i*   string_utf_32_le_interface();

word_t          string_utf_32_le_unit_size();

bool_t          string_utf_32_le_has_terminator(const string_utf_t* it);
error_e         string_utf_32_le_terminate(string_utf_t* it);
word_t          string_utf_32_le_terminator_unit_index(const string_utf_t* it);

void_t          string_utf_32_le_read(const string_utf_t* it, word_t unit_index, u8_t* out_units_read, utf_32_t* out_point);
void_t          string_utf_32_le_read_reverse(const string_utf_t* it, word_t unit_index, u8_t* out_units_read, utf_32_t* out_point);
