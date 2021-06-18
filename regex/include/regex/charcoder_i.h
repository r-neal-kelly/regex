/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/error_e.h"
#include "regex/intrinsic.h"

/*
    This interface requires that your decoded points be able to fit in 32 bits. (e.g. Unicode needs 21 bits.)
    Your max subsequence length must fit in 8 bits. (e.g. the max for Unicode is found in UTF-8, being 4.)
    Your string_subsequence_t must fit within 56 bits. (e.g. each Unicode type fits in 40 bits.)
    The last 8 bits in the string_subsequence_t are reserved for how many units are read in your procedures
    or how many units are contained in the subsequence in your conversion procedures.
    The string_subsequence_t itself must have the unit subsequence in order at the beginning of the struct.
    The lingua franca for decoded points is Unicode. Meaning if you wish to communicate with unknown interfaces,
    you need to be able to receive and send a Unicode point, converting it to your point before encoding.
    Although not necessarily important for encoding, the string_t always uses a null-terminated unit.
*/

typedef struct string_subsequence_t string_subsequence_t;

typedef struct charcoder_i
{
    word_t  (* const unit_size)();

    void_t  (* const read_forward)(const void_t* from, string_subsequence_t* result);
    void_t  (* const read_reverse)(const void_t* from, const void_t* first, string_subsequence_t* result);

    void_t  (* const to_point)(const string_subsequence_t* subsequence, u32_t* result);
    void_t  (* const to_subsequence)(u32_t point, string_subsequence_t* result);
}
charcoder_i;

#define DEFINE_CHARCODER_i(LABEL_LEAD_) \
{                                       \
    &LABEL_LEAD_ ## _unit_size,         \
                                        \
    &LABEL_LEAD_ ## _read_forward,      \
    &LABEL_LEAD_ ## _read_reverse,      \
                                        \
    &LABEL_LEAD_ ## _to_point,          \
    &LABEL_LEAD_ ## _to_subsequence,    \
}
