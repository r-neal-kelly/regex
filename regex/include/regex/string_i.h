/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"

/*
    This interface requires that your decoded points be able to fit in 32 bits. (e.g. Unicode needs 21 bits.)
    Your max subsequence length must fit in 8 bits. (e.g. the max for Unicode is found in  UTF-8, being 4.)
    Your string_subsequence_t must fit within 56 bits. (e.g. each Unicode type fits in 40 bits.)
    The last 8 bits in the string_subsequence_t are reserved for how many units are read in your procedures.
    The lingua franca for decoded points is Unicode. Meaning if you wish to communicate with unknown interfaces,
    you need to be able to receive and send a Unicode point, converting it to your point before encoding.
    It is generally not safe to call functions in string_t, as there may be infinite recursion.
*/

typedef struct string_i             string_i;
typedef struct string_t             string_t;
typedef struct string_subsequence_t string_subsequence_t;
typedef struct string_itr           string_itr;

typedef struct string_i
{
    word_t              (* const unit_size)();

    void_t              (* const read)(const void_t* at, string_subsequence_t* result);
    void_t              (* const read_reverse)(const void_t* from, const void_t* first, string_subsequence_t* result);
    u32_t               (* const point)(const string_subsequence_t* subsequence);

    bool_t              (* const has_null)(const string_t* it);
    error_e             (* const push_null)(string_t* it);
    void_t*             (* const at_null)(const string_t* it);

    error_e             (* const push_point)(string_t* it, u32_t point);
    error_e             (* const push_raw)(string_t* it, const void_t* raw, string_i* raw_interface);
}
string_i;

#define DEFINE_STRING_i(LABEL_LEAD_)    \
{                                       \
    &LABEL_LEAD_ ## _unit_size,         \
                                        \
    &LABEL_LEAD_ ## _read,              \
    &LABEL_LEAD_ ## _read_reverse,      \
    &LABEL_LEAD_ ## _point,             \
                                        \
    &LABEL_LEAD_ ## _has_null,          \
    &LABEL_LEAD_ ## _push_null,         \
    &LABEL_LEAD_ ## _at_null,           \
                                        \
    &LABEL_LEAD_ ## _push_point,        \
    &LABEL_LEAD_ ## _push_raw,          \
}
