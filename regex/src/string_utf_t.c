/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/os.h"
#include "regex/utf_sequence_t.h"
#include "regex/string_utf_i.h"
#include "regex/string_utf_t.h"

error_e string_utf_create(string_utf_t* it, string_utf_i* interface, allocator_i* allocator, word_t reserve_unit_count, float_t grow_rate)
{
    assert(it);
    assert(interface);
    assert(allocator);

    error_e error;

    it->interface = interface;

    error = array_create(&it->array, allocator, it->interface->unit_size(), reserve_unit_count, grow_rate);
    if (error) {
        return error;
    }

    it->point_count = 0;

    error = it->interface->terminate(it);
    if (error) {
        return error;
    }

    return ERROR_NONE_e;
}

error_e string_utf_copy(string_utf_t* it, const string_utf_t* other, allocator_i* allocator, float_t grow_rate)
{
    assert(it);
    assert(other);
    assert(allocator);
    assert(string_utf_is_valid(other));

    it->interface = other->interface;

    error_e error = array_create(&it->array, allocator, it->interface->unit_size(), string_utf_unit_count(other), grow_rate);
    if (error) {
        return error;
    }

    //it->interface.copy(it, other); // we may be able to do this on this level now that we have the iterator

    return ERROR_NONE_e;
}

void_t string_utf_destroy(string_utf_t* it)
{
    assert(it);

    it->interface = 0;

    if (array_is_valid(&it->array)) {
        array_destroy(&it->array);
    }

    it->point_count = 0;
}

bool_t string_utf_is_valid(const string_utf_t* it)
{
    assert(it);

    return array_is_valid(&it->array) && it->interface && it->interface->has_terminator(it) && it->point_count > 0;
}

string_utf_i* string_utf_interface(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    return it->interface;
}

word_t string_utf_unit_size(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    return it->interface->unit_size();
}

word_t string_utf_unit_count(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    return array_unit_count(&it->array);
}

word_t string_utf_unit_length(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    return array_unit_count(&it->array) - 1;
}

word_t string_utf_point_count(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    return it->point_count;
}

word_t string_utf_point_length(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    return it->point_count - 1;
}

void_t string_utf_clear(string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    array_clear(&it->array);
    it->point_count = 0;
    
    error_e error = it->interface->terminate(it);
    assert(error == ERROR_NONE_e);
}

string_utf_iterator_t string_utf_first(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    string_utf_iterator_t iterator;
    it->interface->read(it, 0, &iterator.units_read, &iterator.point);
    iterator.unit_index = 0;
    iterator.point_index = 0;

    return iterator;
}

string_utf_iterator_t string_utf_last(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));
    assert(string_utf_unit_length(it) > 0);

    string_utf_iterator_t iterator;
    word_t terminator_unit_index = it->interface->terminator_unit_index(it);
    it->interface->read_reverse(it, terminator_unit_index, &iterator.units_read, &iterator.point);
    iterator.unit_index = terminator_unit_index - iterator.units_read;
    iterator.point_index = string_utf_point_length(it) - 1;

    return iterator;
}

string_utf_iterator_t string_utf_end(const string_utf_t* it)
{
    assert(it);
    assert(string_utf_is_valid(it));

    string_utf_iterator_t iterator;
    word_t terminator_unit_index = it->interface->terminator_unit_index(it);
    it->interface->read(it, terminator_unit_index, &iterator.units_read, &iterator.point);
    iterator.unit_index = terminator_unit_index;
    iterator.point_index = string_utf_point_length(it);

    return iterator;
}

bool_t string_utf_next(const string_utf_t* it, string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);
    assert(string_utf_is_valid(it));
    assert(string_utf_is_valid_iterator(it, iterator));

    if (!string_utf_is_end(it, iterator)) {
        iterator->unit_index += iterator->units_read;
        iterator->point_index += 1;
        it->interface->read(it, iterator->unit_index, &iterator->units_read, &iterator->point);

        return true;
    } else {
        return false;
    }
}

bool_t string_utf_previous(const string_utf_t* it, string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);
    assert(string_utf_is_valid(it));
    assert(string_utf_is_valid_iterator(it, iterator));

    if (!string_utf_is_first(it, iterator)) {
        it->interface->read_reverse(it, iterator->unit_index, &iterator->units_read, &iterator->point);
        iterator->unit_index -= iterator->units_read;
        iterator->point_index -= 1;

        return true;
    } else {
        return false;
    }
}

bool_t string_utf_is_first(const string_utf_t* it, const string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);
    assert(string_utf_is_valid(it));
    assert(string_utf_is_valid_iterator(it, iterator));

    return iterator->unit_index == 0;
}

bool_t string_utf_is_last(const string_utf_t* it, const string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);
    assert(string_utf_is_valid(it));
    assert(string_utf_is_valid_iterator(it, iterator));

    return iterator->unit_index + iterator->units_read == it->interface->terminator_unit_index(it);
}

bool_t string_utf_is_end(const string_utf_t* it, const string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);
    assert(string_utf_is_valid(it));
    assert(string_utf_is_valid_iterator(it, iterator));

    return iterator->unit_index == it->interface->terminator_unit_index(it);
}

bool_t string_utf_is_valid_iterator(const string_utf_t* it, const string_utf_iterator_t* iterator)
{
    assert(it);
    assert(iterator);

    if (string_utf_is_valid(it) &&
        iterator->unit_index <= it->interface->terminator_unit_index(it) &&
        iterator->point_index <= string_utf_point_length(it) &&
        utf_32_is_scalar_point(iterator->point)) {
        return true;
    } else {
        return false;
    }
}










/*void_t string_utf_8_to_16_le(const utf_8_t* it, array_t* result)
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

void_t string_utf_8_to_16_be(const utf_8_t* it, array_t* result)
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

void_t string_utf_8_to_32_le(const utf_8_t* it, array_t* result)
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

void_t string_utf_8_to_32_be(const utf_8_t* it, array_t* result)
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

void_t string_utf_16_le_to_16_be(const utf_16_t* it, array_t* result)
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

void_t string_utf_16_be_to_16_le(const utf_16_t* it, array_t* result)
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

void_t string_utf_32_to_8(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_8_t));

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

void_t string_utf_32_be_to_8(const utf_32_t* it, array_t* result)
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

void_t string_utf_32_to_16(const utf_32_t* it, array_t* result)
{
    assert(it);
    assert(result);
    assert(array_is_valid(result));
    assert(array_unit_size(result) == sizeof(utf_16_t));

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

void_t string_utf_32_be_to_16_be(const utf_32_t* it, array_t* result)
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

void_t string_utf_32_le_to_32_be(const utf_32_t* it, array_t* result)
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

void_t string_utf_32_be_to_32_le(const utf_32_t* it, array_t* result)
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
