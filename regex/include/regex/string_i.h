/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"

/*
    This interface requires that your decoded points be able to fit in a u32.
    Also, your string_subsequence_t must fit within a u56, with the last 8 bits
    reserved for the units that are consumed by your read procedure.
    It is generally not safe to call the derived functions, as there may be infinite recursion.
*/

typedef struct string_i             string_i;
typedef struct string_t             string_t;
typedef struct string_subsequence_t string_subsequence_t;
typedef struct string_iterator_t    string_iterator_t;

typedef struct string_i
{
    word_t              (* const unit_size)();

    void_t              (* const read)(const void_t* raw, word_t unit_index, string_subsequence_t* result);
    void_t              (* const read_reverse)(const void_t* raw, word_t unit_index, string_subsequence_t* result);
    u32_t               (* const point)(const string_subsequence_t* subsequence);

    bool_t              (* const has_terminator)(const string_t* it);
    error_e             (* const terminate)(string_t* it);
    word_t              (* const terminator_unit_index)(const string_t* it);

    error_e             (* const push_point)(string_t* it, u32_t point);
    error_e             (* const push_raw)(string_t* it, const void_t* raw, string_i* raw_interface);
}
string_i;

#define DEFINE_STRING_i(LABEL_LEAD_)        \
{                                           \
    &LABEL_LEAD_ ## _unit_size,             \
                                            \
    &LABEL_LEAD_ ## _read,                  \
    &LABEL_LEAD_ ## _read_reverse,          \
    &LABEL_LEAD_ ## _point,                 \
                                            \
    &LABEL_LEAD_ ## _has_terminator,        \
    &LABEL_LEAD_ ## _terminate,             \
    &LABEL_LEAD_ ## _terminator_unit_index, \
                                            \
    &LABEL_LEAD_ ## _push_point,            \
    &LABEL_LEAD_ ## _push_raw,              \
}
