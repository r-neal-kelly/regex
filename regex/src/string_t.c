/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/charcoder_i.h"
#include "regex/os.h"
#include "regex/utf_sequence_t.h"
#include "regex/string_t.h"

static bool_t string_has_null(const string_t* it)
{
    assert(it);

    word_t unit_count = array_unit_count(&it->array);
    if (unit_count > 0) {
        byte_t* byte = array_access(&it->array, unit_count - 1);
        for (word_t idx = 0, end = it->charcoder->unit_size(); idx < end; idx += 1) {
            if (byte[idx] != 0) {
                return false;
            }
        }
        return true;
    } else {
        return false;
    }
}

static error_e string_push_null(string_t* it)
{
    assert(it);

    if (!string_has_null(it)) {
        error_e error = array_push_none(&it->array);
        if (error) {
            // we need to pop the last point and replace it with null.
            return error;
        }

        word_t unit_size = it->charcoder->unit_size();
        byte_t* bytes = (byte_t*)array_raw(&it->array) + ((array_unit_count(&it->array) - 1) * unit_size);
        for (word_t idx = 0, end = unit_size; idx < end; idx += 1) {
            bytes[idx] = 0;
        }

        it->point_count += 1;
    }

    return ERROR_NONE_e;
}

static void_t string_pop_null(string_t* it)
{
    assert(it);

    if (string_has_null(it)) {
        array_pop_none(&it->array);
        it->point_count -= 1;
    }
}

static error_e string_push_subsequence(string_t* it, const string_subsequence_t* subsequence)
{
    assert(it);
    assert(subsequence);
    assert(!string_has_null(it));

    word_t unit_size = string_unit_size(it);
    word_t unit_count = string_unit_count(it);

    error_e error;
    for (word_t idx = 0, end = subsequence->units_read; idx < end; idx += 1) {
        error = array_push(&it->array, (byte_t*)subsequence + (idx * unit_size));
        if (error) {
            while (array_unit_count(&it->array) > unit_count) {
                array_pop_none(&it->array);
            }
            return error;
        }
    }
    it->point_count += 1;

    return ERROR_NONE_e;
}

error_e string_create(string_t* it,
                      charcoder_i* charcoder,
                      allocator_i* allocator,
                      word_t reserve_unit_count,
                      float_t grow_rate)
{
    assert(it);
    assert(charcoder);
    assert(allocator);
    assert(reserve_unit_count > 0);

    error_e error;

    it->point_count = 0;
    it->charcoder = charcoder;

    error = array_create(
        &it->array,
        allocator,
        it->charcoder->unit_size(),
        reserve_unit_count,
        1,
        grow_rate
    );
    if (error) {
        it->point_count = 0;
        it->charcoder = 0;
        return error;
    }

    error = string_push_null(it);
    if (error) {
        it->point_count = 0;
        it->charcoder = 0;
        return error;
    }

    return ERROR_NONE_e;
}

error_e string_create_with_raw(string_t* it,
                               charcoder_i* charcoder,
                               const void_t* raw,
                               charcoder_i* raw_charcoder,
                               allocator_i* allocator,
                               word_t reserve_unit_count,
                               float_t grow_rate)
{
    assert(it);
    assert(charcoder);
    assert(raw);
    assert(raw_charcoder);
    assert(allocator);

    error_e error;

    error = string_create(it, charcoder, allocator, reserve_unit_count, grow_rate);
    if (error) {
        return error;
    }

    return string_push_raw(it, raw, raw_charcoder);
}

void_t string_destroy(string_t* it)
{
    assert(it);

    if (array_is_valid(&it->array)) {
        array_destroy(&it->array);
    }

    it->point_count = 0;
    it->charcoder = 0;
}

bool_t string_is_valid(const string_t* it)
{
    assert(it);

    return array_is_valid(&it->array) && it->point_count > 0 && it->charcoder && string_has_null(it);
}

charcoder_i* string_charcoder(const string_t* it)
{
    assert(it);

    return it->charcoder;
}

byte_t* string_raw(const string_t* it)
{
    assert(it);

    return array_raw(&it->array);
}

byte_t* string_raw_null(const string_t* it)
{
    assert(it);

    return array_access(&it->array, array_unit_count(&it->array) - 1);
}

byte_t* string_raw_prefix(const string_t* it)
{
    assert(it);

    return (byte_t*)array_raw(&it->array) - (1 * string_unit_size(it));
}

byte_t* string_raw_postfix(const string_t* it)
{
    return (byte_t*)array_raw(&it->array) + (string_unit_count(it) * string_unit_size(it));
}

word_t string_unit_size(const string_t* it)
{
    assert(it);

    return it->charcoder->unit_size();
}

word_t string_unit_count(const string_t* it)
{
    assert(it);

    return array_unit_count(&it->array);
}

word_t string_unit_length(const string_t* it)
{
    assert(it);

    return array_unit_count(&it->array) - 1;
}

word_t string_point_count(const string_t* it)
{
    assert(it);

    return it->point_count;
}

word_t string_point_length(const string_t* it)
{
    assert(it);

    return it->point_count - 1;
}

error_e string_push_point(string_t* it, u32_t point)
{
    assert(it);

    if (point == 0) {
        return ERROR_INVALID_ARGUMENT_e;
    }

    error_e error;

    string_pop_null(it);

    string_subsequence_t subsequence;
    it->charcoder->to_subsequence(point, &subsequence);

    error = string_push_subsequence(it, &subsequence);
    if (error) {
        string_push_null(it);
        return error;
    }

    error = string_push_null(it);
    if (error) {
        return error;
    }

    return ERROR_NONE_e;
}

error_e string_push_raw(string_t* it, const void_t* raw, charcoder_i* raw_charcoder)
{
    assert(it);
    assert(raw);
    assert(raw_charcoder);

    error_e error;

    string_pop_null(it);

    if (raw_charcoder == string_charcoder(it)) {
        word_t unit_size = string_unit_size(it);
        string_subsequence_t subsequence;
        for (; !string_has_null(it); (byte_t*)raw += subsequence.units_read * unit_size) {
            raw_charcoder->read_forward(raw, &subsequence);
            error = string_push_subsequence(it, &subsequence);
            if (error) {
                string_push_null(it);
                return error;
            }
        }
    } else {
        word_t unit_size = string_unit_size(it);
        string_subsequence_t subsequence;
        u32_t point;
        for (; !string_has_null(it); (byte_t*)raw += subsequence.units_read * unit_size) {
            raw_charcoder->read_forward(raw, &subsequence);
            raw_charcoder->to_point(&subsequence, &point);
            it->charcoder->to_subsequence(point, &subsequence);
            error = string_push_subsequence(it, &subsequence);
            if (error) {
                string_push_null(it);
                return error;
            }
        }
    }

    return ERROR_NONE_e;
}

void_t string_clear(string_t* it)
{
    assert(it);

    array_clear(&it->array);
    it->point_count = 0;
    
    error_e error = string_push_null(it);
    assert(error == ERROR_NONE_e);
}

string_itr string_first(const string_t* it)
{
    assert(it);

    string_itr iterator;
    iterator.owner = it;

    byte_t* byte_pointer = string_raw(it);
    it->charcoder->read_forward(byte_pointer, &iterator.subsequence);
    iterator.byte_pointer = byte_pointer;
    iterator.point_index = 0;

    return iterator;
}

string_itr string_last(const string_t* it)
{
    assert(it);
    assert(string_unit_length(it) > 0);

    string_itr iterator;
    iterator.owner = it;

    byte_t* byte_pointer = string_raw_null(it);
    it->charcoder->read_reverse(byte_pointer, string_raw(it), &iterator.subsequence);
    iterator.byte_pointer = byte_pointer - iterator.subsequence.units_read * string_unit_size(it);
    iterator.point_index = string_point_length(it) - 1;

    return iterator;
}

string_itr string_null(const string_t* it)
{
    assert(it);

    string_itr iterator;
    iterator.owner = it;

    byte_t* byte_pointer = string_raw_null(it);
    it->charcoder->read_forward(byte_pointer, &iterator.subsequence);
    iterator.byte_pointer = byte_pointer;
    iterator.point_index = string_point_count(it) - 1;

    return iterator;
}

bool_t string_itr_is_valid(const string_itr* it)
{
    assert(it);

    if (it->owner &&
        string_is_valid(it->owner) &&
        it->byte_pointer >= string_raw_prefix(it->owner) &&
        it->byte_pointer <= string_raw_postfix(it->owner)) {
        return it->point_index <= string_point_count(it->owner) || it->point_index == (word_t)0 - 1;
    } else {
        return false;
    }
}

bool_t string_itr_is_prefix(const string_itr* it)
{
    assert(it);

    return it->byte_pointer == string_raw_prefix(it->owner);
}

bool_t string_itr_is_null(const string_itr* it)
{
    assert(it);

    return it->byte_pointer == string_raw_null(it->owner);
}

bool_t string_itr_is_postfix(const string_itr* it)
{
    assert(it);

    return it->byte_pointer == string_raw_postfix(it->owner);
}

bool_t string_itr_next(string_itr* it)
{
    assert(it);

    if (!string_itr_is_postfix(it)) {
        it->byte_pointer += it->subsequence.units_read * string_unit_size(it->owner);
        it->point_index += 1;
        it->owner->charcoder->read_forward(it->byte_pointer, &it->subsequence);
        return true;
    } else {
        return false;
    }
}

bool_t string_itr_previous(string_itr* it)
{
    assert(it);

    if (!string_itr_is_prefix(it)) {
        if (it->byte_pointer != string_raw(it->owner)) {
            it->owner->charcoder->read_reverse(it->byte_pointer, string_raw(it->owner), &it->subsequence);
            it->byte_pointer -= it->subsequence.units_read * string_unit_size(it->owner);
            it->point_index -= 1;
        } else {
            it->byte_pointer = string_raw_prefix(it->owner);
            it->point_index -= 1;
        }
        return true;
    } else {
        return false;
    }
}

u32_t string_itr_point(const string_itr* it)
{
    assert(it);
    assert(!string_itr_is_prefix(it));
    assert(!string_itr_is_postfix(it));

    u32_t point;
    it->owner->charcoder->to_point(&it->subsequence, &point);
    return point;
}

word_t string_itr_unit_index(const string_itr* it)
{
    assert(it);
    assert(!string_itr_is_prefix(it));
    assert(!string_itr_is_postfix(it));

    return (it->byte_pointer - string_raw(it->owner)) / string_unit_size(it->owner);
}

word_t string_itr_point_index(const string_itr* it)
{
    assert(it);
    assert(!string_itr_is_prefix(it));
    assert(!string_itr_is_postfix(it));

    return it->point_index;
}










/*void_t string_8_to_16_le(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_8_to_16_be(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_8_to_32_le(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_32_t));

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

void_t string_8_to_32_be(const utf_8_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_32_t));

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

void_t string_16_le_to_16_be(const utf_16_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_16_be_to_16_le(const utf_16_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_32_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_8_t));

    array_clear(result);

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            string_32_le_to_8(it, result);
        } else {
            if (os_is_little_endian()) {
                string_32_le_to_8(it, result);
            } else {
                string_32_be_to_8(it, result);
            }
        }
    } else {
        utf_8_t null = 0;
        array_push(result, &null);
    }
}

void_t string_32_le_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_8_t));

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

void_t string_32_be_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_8_t));

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

void_t string_32_to_16(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

    array_clear(result);

    if (*it) {
        byte_t* bytes = (byte_t*)it;
        if (bytes[0] == 0xFF && bytes[1] == 0xFE && bytes[2] == 0x00 && bytes[3] == 0x00) {
            string_32_le_to_16_le(it, result);
        } else {
            if (os_is_little_endian()) {
                string_32_le_to_16_le(it, result);
            } else {
                string_32_be_to_16_be(it, result);
            }
        }
    } else {
        utf_16_t null = 0;
        array_push(result, &null);
    }
}

void_t string_32_le_to_16_le(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_32_be_to_16_be(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_32_le_to_32_be(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_32_t));

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

void_t string_32_be_to_32_le(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_32_t));

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
}*/
