/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"

typedef struct string_utf_t             string_utf_t;
typedef struct string_utf_iterator_t    string_utf_iterator_t;

typedef struct string_utf_i
{
    word_t              (* const unit_size)();

    bool_t              (* const has_terminator)(const string_utf_t* it);
    error_e             (* const terminate)(string_utf_t* it);
    word_t              (* const terminator_unit_index)(const string_utf_t* it);

    void_t              (* const read)(const string_utf_t* it, word_t unit_index, u8_t* out_units_read, utf_32_t* out_point);
    void_t              (* const read_reverse)(const string_utf_t* it, word_t unit_index, u8_t* out_units_read, utf_32_t* out_point);
}
string_utf_i;

#define DEFINE_STRING_UTF_i(LABEL_LEAD_)        \
{                                               \
    &LABEL_LEAD_ ## _unit_size,                 \
                                                \
    &LABEL_LEAD_ ## _has_terminator,            \
    &LABEL_LEAD_ ## _terminate,                 \
    &LABEL_LEAD_ ## _terminator_unit_index,     \
                                                \
    &LABEL_LEAD_ ## _read,                      \
    &LABEL_LEAD_ ## _read_reverse,              \
}
