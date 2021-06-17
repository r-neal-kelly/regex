/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"
#include "regex/utf.h"

typedef struct array_t array_t;

typedef struct utf_8_subsequence_t
{
    utf_8_t a;
    utf_8_t b;
    utf_8_t c;
    utf_8_t d;
    u8_t    unit_count;
}
utf_8_subsequence_t;

typedef struct utf_16_subsequence_t
{
    utf_16_t    a;
    utf_16_t    b;
    u8_t        unit_count;
}
utf_16_subsequence_t;

typedef struct utf_32_subsequence_t
{
    utf_32_t    a;
    u8_t        unit_count;
}
utf_32_subsequence_t;

u8_t    utf_8_subsequence_create(utf_8_subsequence_t* it, const utf_8_t* string);
u8_t    utf_8_subsequence_create_reverse(utf_8_subsequence_t* it, const utf_8_t* string, const utf_8_t* first);
bool_t  utf_8_subsequence_is_well_formed(const utf_8_subsequence_t* it);
void_t  utf_8_subsequence_to_16(const utf_8_subsequence_t* it, utf_16_subsequence_t* to);
void_t  utf_8_subsequence_to_32(const utf_8_subsequence_t* it, utf_32_subsequence_t* to);

u8_t    utf_16_subsequence_create(utf_16_subsequence_t* it, const utf_16_t* string, bool_t do_swap);
u8_t    utf_16_subsequence_create_reverse(utf_16_subsequence_t* it, const utf_16_t* string, const utf_16_t* first, bool_t do_swap);
bool_t  utf_16_subsequence_is_well_formed(const utf_16_subsequence_t* it);
void_t  utf_16_subsequence_to_8(const utf_16_subsequence_t* it, utf_8_subsequence_t* to);
void_t  utf_16_subsequence_to_32(const utf_16_subsequence_t* it, utf_32_subsequence_t* to);

u8_t    utf_32_subsequence_create(utf_32_subsequence_t* it, const utf_32_t* string, bool_t do_swap);
u8_t    utf_32_subsequence_create_reverse(utf_32_subsequence_t* it, const utf_32_t* string, const utf_32_t* first, bool_t do_swap);
bool_t  utf_32_subsequence_is_well_formed(const utf_32_subsequence_t* it);
void_t  utf_32_subsequence_to_8(const utf_32_subsequence_t* it, utf_8_subsequence_t* to);
void_t  utf_32_subsequence_to_16(const utf_32_subsequence_t* it, utf_16_subsequence_t* to);
