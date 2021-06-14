/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/os.h"
#include "regex/utf_sequence_t.h"
#include "regex/string_utf_t.h"

void_t string_utf_8_to_16_le(const utf_8_t* it, array_t* result)
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
    }

    utf_16_t null = 0;
    array_push(result, &null);
}

void_t string_utf_8_to_16_be(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_little_endian();
        utf_sequence_8_t from;
        utf_sequence_16_t to;
        while (*it) {
            it += utf_sequence_8_read(&from, it);
            utf_sequence_8_to_16(&from, &to);
            utf_sequence_16_write(&to, result, do_swap);
        }
    }

    utf_16_t null = 0;
    array_push(result, &null);
}

void_t string_utf_8_to_32_le(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_32_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_big_endian();
        utf_sequence_8_t from;
        utf_sequence_32_t to;
        while (*it) {
            it += utf_sequence_8_read(&from, it);
            utf_sequence_8_to_32(&from, &to);
            utf_sequence_32_write(&to, result, do_swap);
        }
    }

    utf_32_t null = 0;
    array_push(result, &null);
}

void_t string_utf_8_to_32_be(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_32_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_little_endian();
        utf_sequence_8_t from;
        utf_sequence_32_t to;
        while (*it) {
            it += utf_sequence_8_read(&from, it);
            utf_sequence_8_to_32(&from, &to);
            utf_sequence_32_write(&to, result, do_swap);
        }
    }

    utf_32_t null = 0;
    array_push(result, &null);
}

void_t string_utf_16_le_to_16_be(const utf_16_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_big_endian();
        utf_sequence_16_t sequence;
        while (*it) {
            it += utf_sequence_16_read(&sequence, it, do_swap);
            utf_sequence_16_write(&sequence, result, !do_swap);
        }
    }

    utf_16_t null = 0;
    array_push(result, &null);
}

void_t string_utf_16_be_to_16_le(const utf_16_t* it, array_t* result)
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
    }

    utf_16_t null = 0;
    array_push(result, &null);
}

void_t string_utf_32_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_8_t));

    array_clear(result);

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            string_utf_32_le_to_8(it, result);
        } else {
            if (os_is_little_endian()) {
                string_utf_32_le_to_8(it, result);
            } else {
                string_utf_32_be_to_8(it, result);
            }
        }
    } else {
        utf_8_t null = 0;
        array_push(result, &null);
    }
}

void_t string_utf_32_le_to_8(const utf_32_t* it, array_t* result)
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
    }

    utf_8_t null = 0;
    array_push(result, &null);
}

void_t string_utf_32_be_to_8(const utf_32_t* it, array_t* result)
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
    }

    utf_8_t null = 0;
    array_push(result, &null);
}

void_t string_utf_32_to_16(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            string_utf_32_le_to_16_le(it, result);
        } else {
            if (os_is_little_endian()) {
                string_utf_32_le_to_16_le(it, result);
            } else {
                string_utf_32_be_to_16_be(it, result);
            }
        }
    } else {
        utf_16_t null = 0;
        array_push(result, &null);
    }
}

void_t string_utf_32_le_to_16_le(const utf_32_t* it, array_t* result)
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
    }

    utf_16_t null = 0;
    array_push(result, &null);
}

void_t string_utf_32_be_to_16_be(const utf_32_t* it, array_t* result)
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
    }

    utf_16_t null = 0;
    array_push(result, &null);
}

void_t string_utf_32_le_to_32_be(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_32_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_big_endian();
        utf_sequence_32_t sequence;
        while (*it) {
            it += utf_sequence_32_read(&sequence, it, do_swap);
            utf_sequence_32_write(&sequence, result, !do_swap);
        }
    }

    utf_32_t null = 0;
    array_push(result, &null);
}

void_t string_utf_32_be_to_32_le(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_type_size(result) == sizeof(utf_32_t));

    array_clear(result);

    if (*it) {
        bool_t do_swap = os_is_little_endian();
        utf_sequence_32_t sequence;
        while (*it) {
            it += utf_sequence_32_read(&sequence, it, do_swap);
            utf_sequence_32_write(&sequence, result, !do_swap);
        }
    }

    utf_32_t null = 0;
    array_push(result, &null);
}
