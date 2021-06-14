/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/pointer_t.h"

bool_t pointer_is_valid(pointer_t* it)
{
    assert(it);

    return it->bytes && it->byte_count > 0;
}
