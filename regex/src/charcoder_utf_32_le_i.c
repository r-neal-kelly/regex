/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_utf_32_le_i.h"
#include "regex/string_t.h"
#include "regex/utf.h"

//charcoder_i CHARCODER_UTF_32_LE_i = DEFINE_CHARCODER_i(charcoder_utf_32_le);

word_t charcoder_utf_32_le_unit_size()
{
    return sizeof(utf_32_t);
}
