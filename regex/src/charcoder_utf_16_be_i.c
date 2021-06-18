/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_utf_16_be_i.h"
#include "regex/string_t.h"
#include "regex/utf.h"

//charcoder_i CHARCODER_UTF_16_BE_i = DEFINE_CHARCODER_i(charcoder_utf_16_be);

word_t charcoder_utf_16_be_unit_size()
{
    return sizeof(utf_16_t);
}
