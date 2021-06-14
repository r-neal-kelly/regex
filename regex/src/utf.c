/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

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

word_t utf_sequence_8_read(utf_sequence_8_t* it, const utf_8_t* from)
{
    #define IS_VALID_TRAIL_BYTE(INDEX_)     \
    (                                       \
        ((*(from + INDEX_)) >> 6) == 0x2    \
    )

    assert(it);
    assert(from);

    bool_t do_replace = 0;
    word_t read_count;

    if (((*from) >> 7) == 0x0) {
        it->a = *from;
        it->count = 1;
        read_count = 1;
    } else if (((*from) >> 5) == 0x6) {
        it->a = *from;
        if (IS_VALID_TRAIL_BYTE(1)) {
            it->b = *(from + 1);
            it->count = 2;
            read_count = 2;
        } else {
            do_replace = 1;
            read_count = 1;
        }
    } else if (((*from) >> 4) == 0xE) {
        it->a = *from;
        if (IS_VALID_TRAIL_BYTE(1)) {
            it->b = *(from + 1);
            if (IS_VALID_TRAIL_BYTE(2)) {
                it->c = *(from + 2);
                it->count = 3;
                read_count = 3;
            } else {
                do_replace = 1;
                read_count = 2;
            }
        } else {
            do_replace = 1;
            read_count = 1;
        }
    } else if (((*from) >> 3) == 0x1E) {
        it->a = *from;
        if (IS_VALID_TRAIL_BYTE(1)) {
            it->b = *(from + 1);
            if (IS_VALID_TRAIL_BYTE(2)) {
                it->c = *(from + 2);
                if (IS_VALID_TRAIL_BYTE(3)) {
                    it->d = *(from + 3);
                    it->count = 4;
                    read_count = 4;
                } else {
                    do_replace = 1;
                    read_count = 3;
                }
            } else {
                do_replace = 1;
                read_count = 2;
            }
        } else {
            do_replace = 1;
            read_count = 1;
        }
    } else {
        do_replace = 1;
        read_count = 1;
    }

    if (do_replace) {
        it->a = 0xEF;
        it->b = 0xBF;
        it->c = 0xBD;
        it->count = 3;
    }

    return read_count;

    #undef IS_VALID_TRAIL_BYTE
}

void_t utf_sequence_8_write(const utf_sequence_8_t* it, array_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_8_is_valid(it));
    assert(array_is_valid(to));
    assert(array_type_size(to) == sizeof(utf_8_t));

    if (it->count == 1) {
        array_push(to, &it->a);
    } else if (it->count == 2) {
        array_push(to, &it->a);
        array_push(to, &it->b);
    } else if (it->count == 3) {
        array_push(to, &it->a);
        array_push(to, &it->b);
        array_push(to, &it->c);
    } else {
        array_push(to, &it->a);
        array_push(to, &it->b);
        array_push(to, &it->c);
        array_push(to, &it->d);
    }
}

bool_t utf_sequence_8_is_valid(const utf_sequence_8_t* it)
{
    assert(it);

    if (it->count == 1) {
        return (it->a >> 7) == 0x0;
    } else if (it->count == 2) {
        return (it->a >> 5) == 0x6 && (it->b >> 6) == 0x2;
    } else if (it->count == 3) {
        return (it->a >> 4) == 0xE && (it->b >> 6) == 0x2 && (it->c >> 6) == 0x2;
    } else if (it->count == 4) {
        return (it->a >> 3) == 0x1E && (it->b >> 6) == 0x2 && (it->c >> 6) == 0x2 && (it->d >> 6) == 0x2;
    } else {
        return 0;
    }
}

void_t utf_sequence_8_to_16(const utf_sequence_8_t* it, utf_sequence_16_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_8_is_valid(it));

    utf_sequence_32_t utf_sequence_32;
    utf_sequence_8_to_32(it, &utf_sequence_32);
    utf_sequence_32_to_16(&utf_sequence_32, to);
}

void_t utf_sequence_8_to_32(const utf_sequence_8_t* it, utf_sequence_32_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_8_is_valid(it));

    if (it->count == 1) {
        to->a = it->a;
    } else if (it->count == 2) {
        to->a =
            (((u32_t)it->a & 0x1F) << 6) |
            ((u32_t)it->b & 0x3F);
    } else if (it->count == 3) {
        to->a =
            (((u32_t)it->a & 0x0F) << 12) |
            (((u32_t)it->b & 0x3F) << 6) |
            ((u32_t)it->c & 0x3F);
    } else {
        to->a =
            (((u32_t)it->a & 0x07) << 18) |
            (((u32_t)it->b & 0x3F) << 12) |
            (((u32_t)it->c & 0x3F) << 6) |
            ((u32_t)it->d & 0x3F);
    }

    to->count = 1;

    if (!utf_sequence_32_is_valid(to)) {
        to->a = REPLACEMENT_CHARACTER;
    }
}

word_t utf_sequence_16_read(utf_sequence_16_t* it, const utf_16_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u16(*from) : *from;
    if (it->a >= SURROGATE_HIGH_FIRST && it->a <= SURROGATE_HIGH_LAST) {
        it->b = do_swap ? os_swap_bytes_u16(*(from + 1)) : *(from + 1);
        if (it->b >= SURROGATE_LOW_FIRST && it->b <= SURROGATE_LOW_LAST) {
            it->count = 2;
        } else {
            it->a = REPLACEMENT_CHARACTER;
            it->count = 1;
        }
    } else if (it->a >= SURROGATE_LOW_FIRST && it->a <= SURROGATE_LOW_LAST) {
        it->a = REPLACEMENT_CHARACTER;
        it->count = 1;
    } else {
        it->count = 1;
    }

    return it->count;
}

void_t utf_sequence_16_write(const utf_sequence_16_t* it, array_t* to, bool_t do_swap)
{
    assert(it);
    assert(to);
    assert(utf_sequence_16_is_valid(it));
    assert(array_is_valid(to));
    assert(array_type_size(to) == sizeof(utf_16_t));

    if (do_swap) {
        if (it->count == 1) {
            utf_16_t a = os_swap_bytes_u16(it->a);
            array_push(to, &a);
        } else {
            utf_16_t a = os_swap_bytes_u16(it->a);
            utf_16_t b = os_swap_bytes_u16(it->b);
            array_push(to, &a);
            array_push(to, &b);
        }
    } else {
        if (it->count == 1) {
            array_push(to, &it->a);
        } else {
            array_push(to, &it->a);
            array_push(to, &it->b);
        }
    }
}

bool_t utf_sequence_16_is_valid(const utf_sequence_16_t* it)
{
    assert(it);

    if (it->count == 1) {
        return it->a < SURROGATE_HIGH_FIRST || it->a > SURROGATE_LOW_LAST;
    } else if (it->count == 2) {
        if (it->a >= SURROGATE_HIGH_FIRST &&
            it->a <= SURROGATE_HIGH_LAST &&
            it->b >= SURROGATE_LOW_FIRST &&
            it->b <= SURROGATE_LOW_LAST) {
            return 1;
        } else {
            return 0;
        }
    } else {
        return 0;
    }
}

void_t utf_sequence_16_to_8(const utf_sequence_16_t* it, utf_sequence_8_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_16_is_valid(it));

    utf_sequence_32_t utf_sequence_32;
    utf_sequence_16_to_32(it, &utf_sequence_32);
    utf_sequence_32_to_8(&utf_sequence_32, to);
}

void_t utf_sequence_16_to_32(const utf_sequence_16_t* it, utf_sequence_32_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_16_is_valid(it));

    if (it->count == 1) {
        to->a = it->a;
        to->count = 1;
    } else {
        to->a = ((it->a - SURROGATE_HIGH_FIRST) << 10) + (it->b - SURROGATE_LOW_FIRST) + 0x10000;
        to->count = 1;
    }
}

word_t utf_sequence_32_read(utf_sequence_32_t* it, const utf_32_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u32(*from) : *from;
    if (it->a > CODE_POINT_LAST || (it->a >= SURROGATE_HIGH_FIRST && it->a <= SURROGATE_LOW_LAST)) {
        it->a = REPLACEMENT_CHARACTER;
    }
    it->count = 1;

    return 1;
}

void_t utf_sequence_32_write(const utf_sequence_32_t* it, array_t* to, bool_t do_swap)
{
    assert(it);
    assert(to);
    assert(utf_sequence_32_is_valid(it));
    assert(array_is_valid(to));
    assert(array_type_size(to) == sizeof(utf_32_t));

    if (do_swap) {
        utf_32_t a = os_swap_bytes_u32(it->a);
        array_push(to, &a);
    } else {
        array_push(to, &it->a);
    }
}

bool_t utf_sequence_32_is_valid(const utf_sequence_32_t* it)
{
    assert(it);

    if (it->count == 1 &&
        it->a <= CODE_POINT_LAST &&
        (it->a < SURROGATE_HIGH_FIRST || it->a > SURROGATE_LOW_LAST)) {
        return 1;
    } else {
        return 0;
    }
}

void_t utf_sequence_32_to_8(const utf_sequence_32_t* it, utf_sequence_8_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_32_is_valid(it));

    if (it->a <= 0x7F) {
        to->a = it->a;
        to->count = 1;
    } else if (it->a <= 0x7FF) {
        to->a = ((it->a >> 6) & 0x1F) | 0xC0;
        to->b = (it->a & 0x3F) | 0x80;
        to->count = 2;
    } else if (it->a <= 0xFFFF) {
        to->a = ((it->a >> 12) & 0x0F) | 0xE0;
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

void_t utf_sequence_32_to_16(const utf_sequence_32_t* it, utf_sequence_16_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_32_is_valid(it));

    if (it->a <= 0xFFFF) {
        to->a = it->a;
        to->count = 1;
    } else {
        to->a = ((it->a - 0x10000) >> 10) + SURROGATE_HIGH_FIRST;
        to->b = ((it->a - 0x10000) & 0x3FF) + SURROGATE_LOW_FIRST;
        to->count = 2;
    }
}

void_t utf_string_8_to_16_le(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_big_endian();
        utf_sequence_8_t from;
        utf_sequence_16_t to;
        while (*it) {
            it += utf_sequence_8_read(&from, it);
            utf_sequence_8_to_16(&from, &to);
            utf_sequence_16_write(&to, result, do_swap);
        }
        array_push(result, it);
    } else {
        array_push(result, it);
    }
}

void_t utf_string_16_be_to_16_le(const utf_16_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_little_endian();
        utf_sequence_16_t sequence;
        while (*it) {
            it += utf_sequence_16_read(&sequence, it, do_swap);
            utf_sequence_16_write(&sequence, result, !do_swap);
        }
        array_push(result, it);
    } else {
        array_push(result, it);
    }
}

void_t utf_string_32_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    array_clear(result);

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            utf_string_32_le_to_8(it, result);
        } else {
            if (os_is_little_endian()) {
                utf_string_32_le_to_8(it, result);
            } else {
                utf_string_32_be_to_8(it, result);
            }
        }
    } else {
        array_push(result, it);
    }
}

void_t utf_string_32_le_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_big_endian();
        utf_sequence_32_t from;
        utf_sequence_8_t to;
        while (*it) {
            it += utf_sequence_32_read(&from, it, do_swap);
            utf_sequence_32_to_8(&from, &to);
            utf_sequence_8_write(&to, result);
        }
        array_push(result, it);
    } else {
        array_push(result, it);
    }
}

void_t utf_string_32_be_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_little_endian();
        utf_sequence_32_t from;
        utf_sequence_8_t to;
        while (*it) {
            it += utf_sequence_32_read(&from, it, do_swap);
            utf_sequence_32_to_8(&from, &to);
            utf_sequence_8_write(&to, result);
        }
        array_push(result, it);
    } else {
        array_push(result, it);
    }
}

void_t utf_string_32_to_16(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            utf_string_32_le_to_16_le(it, result);
        } else {
            if (os_is_little_endian()) {
                utf_string_32_le_to_16_le(it, result);
            } else {
                utf_string_32_be_to_16_be(it, result);
            }
        }
    } else {
        array_push(result, it);
    }
}

void_t utf_string_32_le_to_16_le(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_big_endian();
        utf_sequence_32_t from;
        utf_sequence_16_t to;
        while (*it) {
            it += utf_sequence_32_read(&from, it, do_swap);
            utf_sequence_32_to_16(&from, &to);
            utf_sequence_16_write(&to, result, do_swap);
        }
        array_push(result, it);
    } else {
        array_push(result, it);
    }
}

void_t utf_string_32_be_to_16_be(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_little_endian();
        utf_sequence_32_t from;
        utf_sequence_16_t to;
        while (*it) {
            it += utf_sequence_32_read(&from, it, do_swap);
            utf_sequence_32_to_16(&from, &to);
            utf_sequence_16_write(&to, result, do_swap);
        }
        array_push(result, it);
    } else {
        array_push(result, it);
    }
}
