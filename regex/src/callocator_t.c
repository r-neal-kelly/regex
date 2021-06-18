/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/callocator_t.h"
#include "regex/os.h"
#include "regex/pointer_t.h"

bool_t callocator_allocate(pointer_t* it, word_t byte_count)
{
    assert(it);
    assert(byte_count > 0);

    it->bytes = os_calloc(byte_count);
    if (it->bytes) {
        it->byte_count = byte_count;
        return true;
    } else {
        it->byte_count = 0;
        return false;
    }
}

bool_t callocator_reallocate(pointer_t* it, word_t new_byte_count)
{
    assert(it);
    assert(it->bytes);
    assert(new_byte_count > 0);

    byte_t* new_bytes = os_recalloc(it->bytes, new_byte_count);
    if (new_bytes) {
        it->bytes = new_bytes;
        it->byte_count = new_byte_count;
        return true;
    } else {
        return false;
    }
}

void_t callocator_deallocate(pointer_t* it)
{
    assert(it);

    if (it->bytes) {
        os_decalloc(it->bytes);
    }

    it->bytes = 0;
    it->byte_count = 0;
}

callocator_t CALLOCATOR =
{
    &callocator_allocate,
    &callocator_reallocate,
    &callocator_deallocate
};
