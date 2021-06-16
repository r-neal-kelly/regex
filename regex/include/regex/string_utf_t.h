/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/array_t.h"
#include "regex/error_e.h"
#include "regex/intrinsic.h"
#include "regex/string_utf_iterator_t.h"

typedef struct string_utf_i string_utf_i;

typedef struct string_utf_t
{
    string_utf_i*   interface;
    array_t         array;
    word_t          point_count;
}
string_utf_t;

error_e                 string_utf_create(string_utf_t* it, string_utf_i* interface, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
// maybe make these simpler by passing a void_t* string along with interface, "create_with"
error_e                 string_utf_create_8(string_utf_t* it, const utf_8_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_create_16(string_utf_t* it, const utf_16_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_create_16_le(string_utf_t* it, const utf_16_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_create_16_be(string_utf_t* it, const utf_16_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_create_32(string_utf_t* it, const utf_32_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_create_32_le(string_utf_t* it, const utf_32_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_create_32_be(string_utf_t* it, const utf_32_t* string, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e                 string_utf_copy(string_utf_t* it, const string_utf_t* other, allocator_i* allocator, float_t grow_rate);
void_t                  string_utf_destroy(string_utf_t* it);
bool_t                  string_utf_is_valid(const string_utf_t* it);

string_utf_i*           string_utf_interface(const string_utf_t* it);
word_t                  string_utf_unit_size(const string_utf_t* it);
word_t                  string_utf_unit_count(const string_utf_t* it);
word_t                  string_utf_unit_length(const string_utf_t* it);
word_t                  string_utf_point_count(const string_utf_t* it);
word_t                  string_utf_point_length(const string_utf_t* it);
word_t                  string_utf_max_unit_sequence_count(const string_utf_t* it);

void_t                  string_utf_clear(string_utf_t* it);

string_utf_iterator_t   string_utf_first(const string_utf_t* it);
string_utf_iterator_t   string_utf_last(const string_utf_t* it);
string_utf_iterator_t   string_utf_end(const string_utf_t* it);
bool_t                  string_utf_next(const string_utf_t* it, string_utf_iterator_t* iterator);
bool_t                  string_utf_previous(const string_utf_t* it, string_utf_iterator_t* iterator);
bool_t                  string_utf_is_first(const string_utf_t* it, const string_utf_iterator_t* iterator);
bool_t                  string_utf_is_last(const string_utf_t* it, const string_utf_iterator_t* iterator);
bool_t                  string_utf_is_end(const string_utf_t* it, const string_utf_iterator_t* iterator);
bool_t                  string_utf_is_valid_iterator(const string_utf_t* it, const string_utf_iterator_t* iterator);

/*
for (string_utf_iterator_t iter = string_utf_first(str); !string_utf_is_end(str, iter); string_utf_next(str, iter)) {
    utf_32_t point = iter.point;
}

for (string_utf_iterator_t iter = string_utf_end(str); !string_utf_is_first(str, iter); string_utf_previous(str, iter)) {
    utf_32_t point = iter.point;
}

string_utf_iterator_t iter = string_utf_first(str);
do {
    utf_32_t point = iter.point;
} while(string_utf_next(str, iter));

if (string_utf_point_count(str) > 0) {
    string_utf_iterator_t iter = string_utf_last(str); // skips null, but may assert.
    do {
        utf_32_t point = iter.point;
    } while(string_utf_previous(str, iter));
}
*/



error_e                 string_utf_join(string_utf_t* it, const string_utf_t* other);

// we're going to move the below funcs to their own interfaces, which string_utf_t will point to.
// I think for these functions, we should have a interfaced iterator that can move
// through each kind of string in conjunction with a function that can push a sequence.
// because the result param is reset, we can't determine type that way. however, we could have a func that does that?

/*void_t  string_utf_16_le_to_16_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_16_be_to_16_le(const string_utf_t* it, string_utf_t* result);

void_t  string_utf_32_to_8(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_le_to_8(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_be_to_8(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_to_16(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_le_to_16_le(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_be_to_16_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_le_to_32_be(const string_utf_t* it, string_utf_t* result);
void_t  string_utf_32_be_to_32_le(const string_utf_t* it, string_utf_t* result);*/
