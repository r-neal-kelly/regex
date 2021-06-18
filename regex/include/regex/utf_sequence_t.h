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

u8_t    utf_8_subsequence_forward(utf_8_subsequence_t* it, const utf_8_t* from);
u8_t    utf_8_subsequence_reverse(utf_8_subsequence_t* it, const utf_8_t* from, const utf_8_t* first);
bool_t  utf_8_subsequence_is_well_formed(const utf_8_subsequence_t* it);
void_t  utf_8_subsequence_to_16(const utf_8_subsequence_t* it, utf_16_subsequence_t* to);
void_t  utf_8_subsequence_to_32(const utf_8_subsequence_t* it, utf_32_subsequence_t* to);

u8_t    utf_16_subsequence_forward(utf_16_subsequence_t* it, const utf_16_t* from);
u8_t    utf_16_subsequence_reverse(utf_16_subsequence_t* it, const utf_16_t* from, const utf_16_t* first);
bool_t  utf_16_subsequence_is_well_formed(const utf_16_subsequence_t* it);
void_t  utf_16_subsequence_to_8(const utf_16_subsequence_t* it, utf_8_subsequence_t* to);
void_t  utf_16_subsequence_to_32(const utf_16_subsequence_t* it, utf_32_subsequence_t* to);

u8_t    utf_16_subsequence_swapped_forward(utf_16_subsequence_t* it, const utf_16_t* from);
u8_t    utf_16_subsequence_swapped_reverse(utf_16_subsequence_t* it, const utf_16_t* from, const utf_16_t* first);
bool_t  utf_16_subsequence_swapped_is_well_formed(const utf_16_subsequence_t* it);
void_t  utf_16_subsequence_swapped_to_8(const utf_16_subsequence_t* it, utf_8_subsequence_t* to);
void_t  utf_16_subsequence_swapped_to_16(const utf_16_subsequence_t* it, utf_16_subsequence_t* to);
void_t  utf_16_subsequence_swapped_to_32(const utf_16_subsequence_t* it, utf_32_subsequence_t* to);

u8_t    utf_32_subsequence_forward(utf_32_subsequence_t* it, const utf_32_t* from);
u8_t    utf_32_subsequence_reverse(utf_32_subsequence_t* it, const utf_32_t* from, const utf_32_t* first);
bool_t  utf_32_subsequence_is_well_formed(const utf_32_subsequence_t* it);
void_t  utf_32_subsequence_to_8(const utf_32_subsequence_t* it, utf_8_subsequence_t* to);
void_t  utf_32_subsequence_to_16(const utf_32_subsequence_t* it, utf_16_subsequence_t* to);
void_t  utf_32_subsequence_to_16_swapped(const utf_32_subsequence_t* it, utf_16_subsequence_t* to);

u8_t    utf_32_subsequence_swapped_forward(utf_32_subsequence_t* it, const utf_32_t* from);
u8_t    utf_32_subsequence_swapped_reverse(utf_32_subsequence_t* it, const utf_32_t* from, const utf_32_t* first);
bool_t  utf_32_subsequence_swapped_is_well_formed(const utf_32_subsequence_t* it);
void_t  utf_32_subsequence_swapped_to_8(const utf_32_subsequence_t* it, utf_8_subsequence_t* to);
void_t  utf_32_subsequence_swapped_to_16(const utf_32_subsequence_t* it, utf_16_subsequence_t* to);
void_t  utf_32_subsequence_swapped_to_32(const utf_32_subsequence_t* it, utf_32_subsequence_t* to);
