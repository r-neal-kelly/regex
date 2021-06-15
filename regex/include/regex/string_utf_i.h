/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"
#include "regex/string_utf_type_e.h"

typedef struct string_utf_t string_utf_t;

typedef struct string_utf_i
{
    string_utf_type_e   (* const type)();
    word_t              (* const type_size)();

    bool_t              (* const has_terminator)(const string_utf_t* it);
    error_e             (* const terminate)(string_utf_t* it);
}
string_utf_i;

#define DEFINE_STRING_UTF_i(LABEL_LEAD_)    \
{                                           \
    &LABEL_LEAD_ ## _type,                  \
    &LABEL_LEAD_ ## _type_size,             \
                                            \
    &LABEL_LEAD_ ## _has_terminator,        \
    &LABEL_LEAD_ ## _terminate,             \
}
