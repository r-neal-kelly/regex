/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

byte_t* os_alloc(word_t byte_count);
byte_t* os_realloc(byte_t* bytes, word_t new_byte_count);
void_t  os_dealloc(byte_t* bytes);

byte_t* os_calloc(word_t byte_count);
byte_t* os_recalloc(byte_t* bytes, word_t new_byte_count);
void_t  os_decalloc(byte_t* bytes);
