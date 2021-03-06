/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/array_t.h"
#include "regex/error_e.h"
#include "regex/intrinsic.h"

typedef struct charcoder_i charcoder_i;

typedef struct string_t
{
    array_t         array;
    word_t          point_count;
    charcoder_i*    charcoder;
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

typedef struct string_itr
{
    const string_t*         owner;
    string_subsequence_t    subsequence;
    byte_t*                 byte_pointer;
    word_t                  point_index;
}
string_itr;

error_e         string_create(string_t* it,
                              charcoder_i* charcoder,
                              allocator_i* allocator,
                              word_t reserve_unit_count,
                              float_t grow_rate);
error_e         string_create_from(string_t* it,
                                   charcoder_i* charcoder,
                                   const string_t* other,
                                   allocator_i* allocator,
                                   float_t grow_rate);
error_e         string_create_from_raw(string_t* it,
                                       charcoder_i* charcoder,
                                       const void_t* raw,
                                       charcoder_i* raw_charcoder,
                                       allocator_i* allocator,
                                       word_t reserve_unit_count,
                                       float_t grow_rate);
error_e         string_create_copy(string_t* it,
                                   const string_t* other,
                                   allocator_i* allocator,
                                   float_t grow_rate);
void_t          string_destroy(string_t* it);
bool_t          string_is_valid(const string_t* it);

charcoder_i*    string_charcoder(const string_t* it);
allocator_i*    string_allocator(const string_t* it);

byte_t*         string_raw(const string_t* it);
byte_t*         string_raw_null(const string_t* it);
byte_t*         string_raw_prefix(const string_t* it);
byte_t*         string_raw_postfix(const string_t* it);

word_t          string_unit_size(const string_t* it);
word_t          string_unit_count(const string_t* it);
word_t          string_unit_length(const string_t* it);
word_t          string_point_count(const string_t* it);
word_t          string_point_length(const string_t* it);

error_e         string_push_point(string_t* it, u32_t point);
error_e         string_push_iterator(string_t* it, const string_itr* iterator);
error_e         string_push_raw(string_t* it, const void_t* raw, charcoder_i* raw_charcoder);
error_e         string_push_raw_guess(string_t* it, const void_t* raw); // We may have to implement this somewhere else
error_e         string_push_other(string_t* it, const string_t* other);

error_e         string_join(const string_t* it, const string_t* other, string_t* result);

void_t          string_clear(string_t* it);

string_itr      string_first(const string_t* it);
string_itr      string_last(const string_t* it);
string_itr      string_null(const string_t* it);

bool_t          string_itr_is_valid(const string_itr* it);
bool_t          string_itr_is_prefix(const string_itr* it);
bool_t          string_itr_is_null(const string_itr* it);
bool_t          string_itr_is_postfix(const string_itr* it);

bool_t          string_itr_next(string_itr* it);
bool_t          string_itr_previous(string_itr* it);

u32_t           string_itr_point(const string_itr* it);
word_t          string_itr_unit_index(const string_itr* it);
word_t          string_itr_point_index(const string_itr* it);
