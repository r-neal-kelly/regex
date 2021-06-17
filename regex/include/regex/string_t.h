/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/array_t.h"
#include "regex/error_e.h"
#include "regex/intrinsic.h"

typedef struct string_i string_i;

typedef struct string_t
{
    string_i*   interface;
    array_t     array;
    word_t      point_count;
}
string_t;

typedef struct string_subsequence_t
{
    u32_t   reserved_for_interface_00;
    u16_t   reserved_for_interface_04;
    u8_t    reserved_for_interface_06;
    u8_t    units_read;
}
string_subsequence_t;

typedef struct string_iterator_t
{
    const string_t*         owner;
    string_subsequence_t    subsequence;
    word_t                  unit_index;
    word_t                  point_index;
}
string_iterator_t;

error_e             string_create(string_t* it, string_i* interface, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e             string_create_with_raw(string_t* it, string_i* interface, const void_t* raw, string_i* raw_interface, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate);
error_e             string_copy(string_t* it, const string_t* other, allocator_i* allocator, float_t grow_rate);
void_t              string_destroy(string_t* it);
bool_t              string_is_valid(const string_t* it);

string_i*           string_interface(const string_t* it);
void_t*             string_raw(const string_t* it);

word_t              string_unit_size(const string_t* it);
word_t              string_unit_count(const string_t* it);
word_t              string_unit_length(const string_t* it);
word_t              string_point_count(const string_t* it);
word_t              string_point_length(const string_t* it);

error_e             string_push_point(string_t* it, u32_t point);
error_e             string_push_iterator(string_t* it, const string_iterator_t* iterator);
error_e             string_push_other(string_t* it, const string_t* other);
error_e             string_push_raw(string_t* it, const void_t* raw, string_i* raw_interface);
error_e             string_push_raw_guess(string_t* it, const void_t* raw);

error_e             string_join(const string_t* it, const string_t* other, string_t* result);

void_t              string_clear(string_t* it);

string_iterator_t   string_first(const string_t* it);
string_iterator_t   string_last(const string_t* it);
string_iterator_t   string_end(const string_t* it);
bool_t              string_next(string_iterator_t* iterator);
bool_t              string_previous(string_iterator_t* iterator);
u32_t               string_point(const string_iterator_t* iterator);
bool_t              string_is_valid_iterator(const string_iterator_t* iterator);
bool_t              string_is_first(const string_iterator_t* iterator);
bool_t              string_is_last(const string_iterator_t* iterator);
bool_t              string_is_end(const string_iterator_t* iterator);
