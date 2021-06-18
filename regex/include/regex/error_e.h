/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

enum
{
    ERROR_NONE_e,

    ERROR_INVALID_ARGUMENT_e,
    ERROR_OUT_OF_MEMORY_e,
    ERROR_OVERFLOW_e,
};

typedef word_t  error_e;
