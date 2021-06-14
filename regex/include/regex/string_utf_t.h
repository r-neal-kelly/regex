/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/array_t.h"
#include "regex/intrinsic.h"
#include "regex/string_utf_type_e.h"

typedef struct string_utf_i string_utf_i;

typedef struct string_utf_t
{
    array_t         array;
    string_utf_i*   interface;
    word_t          point_count;
}
string_utf_t;

// type is used to determine interface.
bool_t              string_utf_create(string_utf_t* it, string_utf_type_e type, allocator_i* allocator, word_t reserve_type_count, f32_t grow_rate);
void_t              string_utf_destroy(string_utf_t* it);
bool_t              string_utf_is_valid(const string_utf_t* it);
string_utf_type_e   string_utf_type(const string_utf_t* it);
word_t              string_utf_type_size(const string_utf_t* it);
word_t              string_utf_type_count(const string_utf_t* it);
word_t              string_utf_point_count(const string_utf_t* it);
bool_t              string_utf_push_8(string_utf_t* it, const utf_8_t* utf_8); // this will convert if needed. we'll have all the others too. related to the iterator type
bool_t              string_utf_push_null(string_utf_t* it);
void_t              string_utf_clear(string_utf_t* it);
void_t              string_utf_clear_and_retype(string_utf_t* it, string_utf_type_e type);

// we're going to move the below funcs to their own interfaces, which string_utf_t will point to.
// I think for these functions, we should have a interfaced iterator that can move
// through each kind of string in conjunction with a function that can push a sequence.
// because the result param is reset, we can't determine type that way. however, we could have a func that does that?

void_t  string_utf_8_to_16_le(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_8_to_16_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_8_to_32_le(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_8_to_32_be(const string_utf_t* it, string_utf_t* result);

void_t  string_utf_16_le_to_16_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_16_be_to_16_le(const string_utf_t* it, string_utf_t* result);

void_t  string_utf_32_to_8(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_le_to_8(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_be_to_8(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_to_16(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_le_to_16_le(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_be_to_16_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_le_to_32_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_be_to_32_le(const string_utf_t* it, string_utf_t* result);
