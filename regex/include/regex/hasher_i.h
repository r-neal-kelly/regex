/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

typedef struct pointer_t pointer_t;

typedef struct hasher_i
{
    word_t  (* const hash_word)(word_t word);
}
hasher_i;

#define DEFINE_HASHER_i(LABEL_LEAD_)    \
{                                       \
    &LABEL_LEAD_ ## _hash_word,         \
}
