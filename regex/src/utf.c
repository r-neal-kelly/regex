/*
    Copyright 2021 r-neal-kelly
*/

#include "assert.h"

#include "regex/array_t.h"
#include "regex/os.h"
#include "regex/utf.h"

#define CODE_POINT_FIRST        0x00000000
#define CODE_POINT_LAST         0x0010FFFF

#define BYTE_ORDER_MARKER       0x0000FEFF
#define REPLACEMENT_CHARACTER   0x0000FFFD

#define SURROGATE_HIGH_FIRST    0x0000D800
#define SURROGATE_HIGH_LAST     0x0000DBFF
#define SURROGATE_LOW_FIRST     0x0000DC00
#define SURROGATE_LOW_LAST      0x0000DFFF

void_t utf_sequence_32_create(utf_sequence_32_t* it, const utf_32_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u32(*from) : *from;
    if (it->a > CODE_POINT_LAST || (it->a >= SURROGATE_HIGH_FIRST && it->a <= SURROGATE_LOW_LAST)) {
        it->a = REPLACEMENT_CHARACTER;
    }
}

void_t utf_sequence_32_to_8(utf_sequence_32_t* it, utf_sequence_8_t* to)
{
    assert(it);
    assert(to);

    if (it->a <= 0x7F) {
        to->a = it->a;
        to->count = 1;
    } else if (it->a <= 0x7FF) {
        to->a = ((it->a >> 6) & 0x1F) | 0xC0;
        to->b = (it->a & 0x3F) | 0x80;
        to->count = 2;
    } else if (it->a <= 0xFFFF) {
        to->a = ((it->a >> 16) & 0x0F) | 0xE0;
        to->b = ((it->a >> 6) & 0x3F) | 0x80;
        to->c = (it->a & 0x3F) | 0x80;
        to->count = 3;
    } else {
        to->a = ((it->a >> 18) & 0x07) | 0xF0;
        to->b = ((it->a >> 12) & 0x3F) | 0x80;
        to->c = ((it->a >> 6) & 0x3F) | 0x80;
        to->d = (it->a & 0x3F) | 0x80;
        to->count = 4;
    }
}

void_t utf_sequence_32_to_8_array(utf_sequence_32_t* it, array_t* to)
{
    assert(it);
    assert(to);

    utf_sequence_8_t sequence;
    if (it->a <= 0x7F) {
        sequence.a = it->a;
        array_push(to, &sequence.a);
    } else if (it->a <= 0x7FF) {
        sequence.a = ((it->a >> 6) & 0x1F) | 0xC0;
        sequence.b = (it->a & 0x3F) | 0x80;
        array_push(to, &sequence.a);
        array_push(to, &sequence.b);
    } else if (it->a <= 0xFFFF) {
        sequence.a = ((it->a >> 16) & 0x0F) | 0xE0;
        sequence.b = ((it->a >> 6) & 0x3F) | 0x80;
        sequence.c = (it->a & 0x3F) | 0x80;
        array_push(to, &sequence.a);
        array_push(to, &sequence.b);
        array_push(to, &sequence.c);
    } else {
        sequence.a = ((it->a >> 18) & 0x07) | 0xF0;
        sequence.b = ((it->a >> 12) & 0x3F) | 0x80;
        sequence.c = ((it->a >> 6) & 0x3F) | 0x80;
        sequence.d = (it->a & 0x3F) | 0x80;
        array_push(to, &sequence.a);
        array_push(to, &sequence.b);
        array_push(to, &sequence.c);
        array_push(to, &sequence.d);
    }
}

bool_t string_utf_32_to_utf_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            return string_utf_32_le_to_utf_8(it, result);
        } else {
            return string_utf_32_be_to_utf_8(it, result);
        }
    } else {
        array_clear(result);
        array_push(result, it);
        return 1;
    }
}

bool_t string_utf_32_le_to_utf_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    if (*it) {
        utf_sequence_32_t from;
        bool_t do_swap = os_is_big_endian();

        while (*it) {
            utf_sequence_32_create(&from, it, do_swap);
            utf_sequence_32_to_8_array(&from, result);
            it += 1;
        }
        utf_sequence_32_create(&from, it, do_swap);
        utf_sequence_32_to_8_array(&from, result);

        return 1;
    } else {
        array_clear(result);
        array_push(result, it);

        return 1;
    }
}

bool_t string_utf_32_be_to_utf_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    if (*it) {
        utf_sequence_32_t from;
        bool_t do_swap = os_is_little_endian();

        while (*it) {
            utf_sequence_32_create(&from, it, do_swap);
            utf_sequence_32_to_8_array(&from, result);
            it += 1;
        }
        utf_sequence_32_create(&from, it, do_swap);
        utf_sequence_32_to_8_array(&from, result);

        return 1;
    } else {
        array_clear(result);
        array_push(result, it);

        return 1;
    }
}
