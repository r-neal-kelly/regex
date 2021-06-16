/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_utf_i.h"

extern string_utf_i STRING_UTF_32_BE_i;

string_utf_i*   string_utf_32_be_interface();
word_t          string_utf_32_be_unit_size();
word_t          string_utf_32_be_max_unit_sequence_count();

bool_t          string_utf_32_be_has_terminator(const string_utf_t* it);
error_e         string_utf_32_be_terminate(string_utf_t* it);
