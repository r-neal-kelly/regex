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
    word_t              (* const max_unit_sequence_count)();

    bool_t              (* const has_terminator)(const string_utf_t* it);
    error_e             (* const terminate)(string_utf_t* it);
}
string_utf_i;

#define DEFINE_STRING_UTF_i(LABEL_LEAD_)        \
{                                               \
    &LABEL_LEAD_ ## _unit_size,                 \
    &LABEL_LEAD_ ## _max_unit_sequence_count,   \
                                                \
    &LABEL_LEAD_ ## _has_terminator,            \
    &LABEL_LEAD_ ## _terminate,                 \
}
