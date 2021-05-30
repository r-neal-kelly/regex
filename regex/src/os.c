/*
    Copyright 2021 r-neal-kelly
*/

#include "assert.h"
#include "errno.h"
#include "stdlib.h"

#include "regex/os.h"

byte_t* os_alloc(word_t byte_count)
{
    byte_t* bytes;

    assert(byte_count > 0);

    errno = 0;
    bytes = malloc(byte_count);
    if (errno == 0) {
        return bytes;
    } else {
        return 0;
    }
}

byte_t* os_realloc(byte_t* bytes, word_t new_byte_count)
{
    byte_t* new_bytes;

    assert(bytes);
    assert(new_byte_count > 0);

    errno = 0;
    new_bytes = realloc(bytes, new_byte_count);
    if (errno == 0) {
        return new_bytes;
    } else {
        return 0;
    }
}

void_t os_dealloc(byte_t* bytes)
{
    assert(bytes);

    free(bytes);
}

byte_t* os_calloc(word_t byte_count)
{
    byte_t* bytes;

    assert(byte_count > 0);

    errno = 0;
    bytes = calloc(byte_count, sizeof(byte_t));
    if (errno == 0) {
        return bytes;
    } else {
        return 0;
    }
}

byte_t* os_recalloc(byte_t* bytes, word_t new_byte_count)
{
    byte_t* new_bytes;

    assert(bytes);
    assert(new_byte_count > 0);

    errno = 0;
    new_bytes = _recalloc(bytes, new_byte_count, sizeof(byte_t));
    if (errno == 0) {
        return new_bytes;
    } else {
        return 0;
    }
}

void_t os_decalloc(byte_t* bytes)
{
    assert(bytes);

    free(bytes);
}
