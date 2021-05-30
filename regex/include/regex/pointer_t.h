/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/intrinsic.h"

typedef struct pointer_t
{
    byte_t* bytes;
    word_t  byte_count;
}
pointer_t;
