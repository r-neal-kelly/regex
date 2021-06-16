/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/array_t.h"
#include "regex/os.h"
#include "regex/utf_sequence_t.h"

#define BYTE_ORDER_MARKER       0x0000FEFF
#define REPLACEMENT_CHARACTER   0x0000FFFD

u8_t utf_subsequence_8_next(utf_sequence_8_t* it, const utf_8_t* from)
{
    assert(it);
    assert(from);

    #define read_1()            \
    {                           \
        it->a = *(from + 0);    \
        it->count = 1;          \
        read_unit_count = 1;    \
    }

    #define read_2()            \
    {                           \
        it->a = *(from + 0);    \
        it->b = *(from + 1);    \
        it->count = 2;          \
        read_unit_count = 2;    \
    }

    #define read_3()            \
    {                           \
        it->a = *(from + 0);    \
        it->b = *(from + 1);    \
        it->c = *(from + 2);    \
        it->count = 3;          \
        read_unit_count = 3;    \
    }

    #define read_4()            \
    {                           \
        it->a = *(from + 0);    \
        it->b = *(from + 1);    \
        it->c = *(from + 2);    \
        it->d = *(from + 3);    \
        it->count = 4;          \
        read_unit_count = 4;    \
    }

    #define replace(READ_UNIT_COUNT_)       \
    {                                       \
        it->a = 0xEF;                       \
        it->b = 0xBF;                       \
        it->c = 0xBD;                       \
        it->count = 3;                      \
        read_unit_count = READ_UNIT_COUNT_; \
    }

    u8_t read_unit_count = 0;

    if (*(from + 0) >= 0x00 && *(from + 0) <= 0x7F) {
        read_1();
    } else if (*(from + 0) >= 0xC2 && *(from + 0) <= 0xDF) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            read_2();
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xE0) {
        if (*(from + 1) >= 0xA0 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xE1 && *(from + 0) >= 0xEC) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xED) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0x9F) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xEE && *(from + 0) >= 0xEF) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xF0) {
        if (*(from + 1) >= 0x90 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                if (*(from + 3) >= 0x80 && *(from + 3) <= 0xBF) {
                    read_4();
                } else {
                    replace(3);
                }
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xF1 && *(from + 0) <= 0xF3) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                if (*(from + 3) >= 0x80 && *(from + 3) <= 0xBF) {
                    read_4();
                } else {
                    replace(3);
                }
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xF4) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0x8F) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                if (*(from + 3) >= 0x80 && *(from + 3) <= 0xBF) {
                    read_4();
                } else {
                    replace(3);
                }
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else {
        replace(1);
    }

    return read_unit_count;

    #undef read_1
    #undef read_2
    #undef read_3
    #undef read_4
    #undef replace
}

u8_t utf_subsequence_8_previous(utf_sequence_8_t* it, const utf_8_t* from, const utf_8_t* first)
{
    assert(it);
    assert(from);
    assert(first);

    bool_t found_lead = false;
    bool_t do_replace = false;
    word_t read_count = 0;

    for (; from != first && !found_lead && !do_replace && read_count < 4; from -= 1) {
        if (((*from) >> 7) == 0x0) {
            found_lead = true;
            if (read_count == 0) {
                it->a = *from;
                it->count = 1;
                read_count += 1;
            } else {
                do_replace = true;
                read_count = read_count;
            }
        } else if (((*from) >> 6) == 0x2) {
            read_count += 1;
        } else if (((*from) >> 5) == 0x6) {
            found_lead = true;
            if (read_count == 1) {
                it->a = *from;
                it->b = *(from + 1);
                it->count = 2;
            } else {
                do_replace = true;
            }
            read_count += 1;
        } else if (((*from) >> 4) == 0xE) {
            found_lead = true;
            if (read_count == 2) {
                it->a = *from;
                it->b = *(from + 1);
                it->c = *(from + 2);
                it->count = 3;
            } else {
                do_replace = true;
            }
            read_count += 1;
        } else if (((*from) >> 3) == 0x1E) {
            found_lead = true;
            if (read_count == 3) {
                it->a = *from;
                it->b = *(from + 1);
                it->c = *(from + 2);
                it->d = *(from + 3);
                it->count = 4;
            } else {
                do_replace = true;
            }
            read_count += 1;
        } else {
            do_replace = true;
            read_count += 1;
        }
    }

    if (read_count == 4 && !found_lead) {
        do_replace = true;
        read_count = 1;
    }

    if (do_replace) {
        it->a = 0xEF;
        it->b = 0xBF;
        it->c = 0xBD;
        it->count = 3;
    }

    return read_count;
}

void_t utf_sequence_8_write(const utf_sequence_8_t* it, array_t* to)
{
    assert(it);
    assert(to);
    assert(utf_sequence_8_is_valid(it));
    assert(array_is_valid(to));
    assert(array_unit_size(to) == sizeof(utf_8_t));

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

    // use the above algorithm instead

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

u8_t utf_sequence_16_read(utf_sequence_16_t* it, const utf_16_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u16(*from) : *from;
    if (it->a >= UTF_SURROGATE_HIGH_FIRST && it->a <= UTF_SURROGATE_HIGH_LAST) {
        it->b = do_swap ? os_swap_bytes_u16(*(from + 1)) : *(from + 1);
        if (it->b >= UTF_SURROGATE_LOW_FIRST && it->b <= UTF_SURROGATE_LOW_LAST) {
            it->count = 2;
        } else {
            it->a = REPLACEMENT_CHARACTER;
            it->count = 1;
        }
    } else if (it->a >= UTF_SURROGATE_LOW_FIRST && it->a <= UTF_SURROGATE_LOW_LAST) {
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
    assert(array_unit_size(to) == sizeof(utf_16_t));

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
        return it->a < UTF_SURROGATE_HIGH_FIRST || it->a > UTF_SURROGATE_LOW_LAST;
    } else if (it->count == 2) {
        if (it->a >= UTF_SURROGATE_HIGH_FIRST &&
            it->a <= UTF_SURROGATE_HIGH_LAST &&
            it->b >= UTF_SURROGATE_LOW_FIRST &&
            it->b <= UTF_SURROGATE_LOW_LAST) {
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
        to->a = ((it->a - UTF_SURROGATE_HIGH_FIRST) << 10) + (it->b - UTF_SURROGATE_LOW_FIRST) + 0x10000;
        to->count = 1;
    }
}

u8_t utf_sequence_32_read(utf_sequence_32_t* it, const utf_32_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u32(*from) : *from;
    if (!utf_32_is_scalar_point(it->a)) {
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
    assert(array_unit_size(to) == sizeof(utf_32_t));

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

    if (it->count == 1 && utf_32_is_scalar_point(it->a)) {
        return true;
    } else {
        return false;
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
        to->a = ((it->a - 0x10000) >> 10) + UTF_SURROGATE_HIGH_FIRST;
        to->b = ((it->a - 0x10000) & 0x3FF) + UTF_SURROGATE_LOW_FIRST;
        to->count = 2;
    }
}
