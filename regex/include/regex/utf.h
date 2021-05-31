/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

typedef array_t array_t;

typedef struct utf_sequence_8_t
{
    utf_8_t a;
    utf_8_t b;
    utf_8_t c;
    utf_8_t d;
    word_t  count;
}
utf_sequence_8_t;

typedef struct utf_sequence_16_t
{
    utf_16_t    a;
    utf_16_t    b;
    word_t      count;
}
utf_sequence_16_t;

typedef struct utf_sequence_32_t
{
    utf_32_t    a;
}
utf_sequence_32_t;

void_t  utf_sequence_32_create(utf_sequence_32_t* it, const utf_32_t* from, bool_t do_swap);
void_t  utf_sequence_32_to_8(utf_sequence_32_t* it, utf_sequence_8_t* to);
void_t  utf_sequence_32_to_8_array(utf_sequence_32_t* it, array_t* to); // currently, I just don't want to check the count twice

bool_t  string_utf_32_to_utf_8(const utf_32_t* it, array_t* result); // both args will eventually be string types
bool_t  string_utf_32_le_to_utf_8(const utf_32_t* it, array_t* result);
bool_t  string_utf_32_be_to_utf_8(const utf_32_t* it, array_t* result);
