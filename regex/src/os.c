/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>
#include <errno.h>
#include <stdlib.h>

#include "regex/os.h"

#if defined(IS_WINDOWS)
byte_t* os_alloc(word_t byte_count)
{
    assert(byte_count > 0);

    errno = 0;
    byte_t* bytes = malloc(byte_count);
    if (errno == 0) {
        return bytes;
    } else {
        return 0;
    }
}

byte_t* os_realloc(byte_t* bytes, word_t new_byte_count)
{
    assert(bytes);
    assert(new_byte_count > 0);

    errno = 0;
    byte_t* new_bytes = realloc(bytes, new_byte_count);
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
    assert(byte_count > 0);

    errno = 0;
    byte_t* bytes = calloc(byte_count, sizeof(byte_t));
    if (errno == 0) {
        return bytes;
    } else {
        return 0;
    }
}

byte_t* os_recalloc(byte_t* bytes, word_t new_byte_count)
{
    assert(bytes);
    assert(new_byte_count > 0);

    errno = 0;
    byte_t* new_bytes = _recalloc(bytes, new_byte_count, sizeof(byte_t));
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

u16_t os_swap_bytes_u16(u16_t bytes)
{
    return _byteswap_ushort(bytes);
}

u32_t os_swap_bytes_u32(u32_t bytes)
{
    return _byteswap_ulong(bytes);
}

u64_t os_swap_bytes_u64(u64_t bytes)
{
    return _byteswap_uint64(bytes);
}
#endif

bool_t os_is_little_endian()
{
    volatile word_t e = 1;
    return ((const byte_t*)&e)[0] == 1;
}

bool_t os_is_big_endian()
{
    volatile word_t e = 1;
    return ((const byte_t*)&e)[0] == 0;
}
