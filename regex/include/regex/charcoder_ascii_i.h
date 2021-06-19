/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/charcoder_i.h"
#include "regex/intrinsic.h"

extern charcoder_i CHARCODER_ASCII_i;

word_t  charcoder_ascii_unit_size();

void_t  charcoder_ascii_read_forward(const void_t* from, string_subsequence_t* result);
void_t  charcoder_ascii_read_reverse(const void_t* from, const void_t* first, string_subsequence_t* result);

void_t  charcoder_ascii_to_point(const string_subsequence_t* subsequence, u32_t* result);
void_t  charcoder_ascii_to_subsequence(u32_t point, string_subsequence_t* result);
