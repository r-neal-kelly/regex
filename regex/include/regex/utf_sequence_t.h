/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/utf.h"

typedef struct array_t array_t;

typedef struct utf_subsequence_8_t
{
    utf_8_t a;
    utf_8_t b;
    utf_8_t c;
    utf_8_t d;
    u32_t   count;
}
utf_subsequence_8_t;

u8_t    utf_subsequence_8_next(utf_subsequence_8_t* it, const utf_8_t* from);
u8_t    utf_subsequence_8_previous(utf_subsequence_8_t* it, const utf_8_t* from, const utf_8_t* first);
bool_t  utf_subsequence_8_is_well_formed(const utf_subsequence_8_t* it);

void_t  utf_subsequence_8_write(const utf_subsequence_8_t* it, array_t* to);
void_t  utf_subsequence_8_to_16(const utf_subsequence_8_t* it, utf_sequence_16_t* to);
void_t  utf_subsequence_8_to_32(const utf_subsequence_8_t* it, utf_sequence_32_t* to);

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

u8_t    utf_sequence_16_read(utf_sequence_16_t* it, const utf_16_t* from, bool_t do_swap);
void_t  utf_sequence_16_write(const utf_sequence_16_t* it, array_t* to, bool_t do_swap);
bool_t  utf_sequence_16_is_valid(const utf_sequence_16_t* it);
void_t  utf_sequence_16_to_8(const utf_sequence_16_t* it, utf_sequence_8_t* to);
void_t  utf_sequence_16_to_32(const utf_sequence_16_t* it, utf_sequence_32_t* to);

u8_t    utf_sequence_32_read(utf_sequence_32_t* it, const utf_32_t* from, bool_t do_swap);
void_t  utf_sequence_32_write(const utf_sequence_32_t* it, array_t* to, bool_t do_swap);
bool_t  utf_sequence_32_is_valid(const utf_sequence_32_t* it);
void_t  utf_sequence_32_to_8(const utf_sequence_32_t* it, utf_sequence_8_t* to);
void_t  utf_sequence_32_to_16(const utf_sequence_32_t* it, utf_sequence_16_t* to);
