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
    u32_t   count;
}
utf_sequence_8_t;

typedef struct utf_sequence_16_t
{
    utf_16_t    a;
    utf_16_t    b;
    u32_t       count;
}
utf_sequence_16_t;

typedef struct utf_sequence_32_t
{
    utf_32_t    a;
    u32_t       count;
}
utf_sequence_32_t;

word_t  utf_sequence_8_read(utf_sequence_8_t* it, const utf_8_t* from);
void_t  utf_sequence_8_write(const utf_sequence_8_t* it, array_t* to);
bool_t  utf_sequence_8_is_valid(const utf_sequence_8_t* it);
void_t  utf_sequence_8_to_16(const utf_sequence_8_t* it, utf_sequence_16_t* to);
void_t  utf_sequence_8_to_32(const utf_sequence_8_t* it, utf_sequence_32_t* to);

word_t  utf_sequence_16_read(utf_sequence_16_t* it, const utf_16_t* from, bool_t do_swap);
void_t  utf_sequence_16_write(const utf_sequence_16_t* it, array_t* to, bool_t do_swap);
bool_t  utf_sequence_16_is_valid(const utf_sequence_16_t* it);
void_t  utf_sequence_16_to_8(const utf_sequence_16_t* it, utf_sequence_8_t* to);
void_t  utf_sequence_16_to_32(const utf_sequence_16_t* it, utf_sequence_32_t* to);

word_t  utf_sequence_32_read(utf_sequence_32_t* it, const utf_32_t* from, bool_t do_swap);
void_t  utf_sequence_32_write(const utf_sequence_32_t* it, array_t* to, bool_t do_swap);
bool_t  utf_sequence_32_is_valid(const utf_sequence_32_t* it);
void_t  utf_sequence_32_to_8(const utf_sequence_32_t* it, utf_sequence_8_t* to);
void_t  utf_sequence_32_to_16(const utf_sequence_32_t* it, utf_sequence_16_t* to);

// both args in the following functions will eventually be string types

void_t  utf_string_32_to_8(const utf_32_t* it, array_t* result);
void_t  utf_string_32_le_to_8(const utf_32_t* it, array_t* result);
void_t  utf_string_32_be_to_8(const utf_32_t* it, array_t* result);
void_t  utf_string_32_to_16(const utf_32_t* it, array_t* result);
void_t  utf_string_32_le_to_16_le(const utf_32_t* it, array_t* result);
void_t  utf_string_32_be_to_16_be(const utf_32_t* it, array_t* result);
