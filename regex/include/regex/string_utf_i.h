/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"
#include "regex/string_utf_type_e.h"

typedef struct string_utf_t             string_utf_t;
typedef struct string_utf_iterator_t    string_utf_iterator_t;

typedef struct string_utf_i
{
    string_utf_type_e   (* const type)();
    word_t              (* const type_size)();

    bool_t              (* const has_terminator)(const string_utf_t* it);
    error_e             (* const terminate)(string_utf_t* it);

    void_t              (* const first)(const string_utf_t* it, string_utf_iterator_t* iterator);
    void_t              (* const last)(const string_utf_t* it, string_utf_iterator_t* iterator);
    void_t              (* const end)(const string_utf_t* it, string_utf_iterator_t* iterator);
    bool_t              (* const next)(const string_utf_t* it, string_utf_iterator_t* iterator);
    bool_t              (* const previous)(const string_utf_t* it, string_utf_iterator_t* iterator);

    bool_t              (* const is_first)(const string_utf_t* it, const string_utf_iterator_t* iterator);
    bool_t              (* const is_last)(const string_utf_t* it, const string_utf_iterator_t* iterator);
    bool_t              (* const is_end)(const string_utf_t* it, const string_utf_iterator_t* iterator);
}
string_utf_i;

#define DEFINE_STRING_UTF_i(LABEL_LEAD_)    \
{                                           \
    &LABEL_LEAD_ ## _type,                  \
    &LABEL_LEAD_ ## _type_size,             \
                                            \
    &LABEL_LEAD_ ## _has_terminator,        \
    &LABEL_LEAD_ ## _terminate,             \
                                            \
    &LABEL_LEAD_ ## _first,                 \
    &LABEL_LEAD_ ## _last,                  \
    &LABEL_LEAD_ ## _end,                   \
    &LABEL_LEAD_ ## _next,                  \
    &LABEL_LEAD_ ## _previous,              \
                                            \
    &LABEL_LEAD_ ## _is_first,              \
    &LABEL_LEAD_ ## _is_last,               \
    &LABEL_LEAD_ ## _is_end,                \
}
