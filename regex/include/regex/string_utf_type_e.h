/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

enum
{
    STRING_UTF_8_e,
    STRING_UTF_16_e,    // delegates to LE or BE
    STRING_UTF_16_LE_e,
    STRING_UTF_16_BE_e,
    STRING_UTF_32_e,    // delegates to LE or BE
    STRING_UTF_32_LE_e,
    STRING_UTF_32_BE_e,
};

typedef word_t  string_utf_type_e;
