/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/string_utf_i.h"

extern string_utf_i STRING_UTF_16_BE_i;

string_utf_type_e   string_utf_16_be_type();
word_t              string_utf_16_be_type_size();

bool_t              string_utf_16_be_has_terminator(const string_utf_t* it);
error_e             string_utf_16_be_terminate(string_utf_t* it);
