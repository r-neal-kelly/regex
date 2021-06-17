/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>

#include "regex/array_t.h"
#include "regex/os.h"
#include "regex/utf_sequence_t.h"

#define BYTE_ORDER_MARKER       0x0000FEFF
#define REPLACEMENT_CHARACTER   0x0000FFFD

/*
Well-formed UTF-8 Subsequences:
    00..7F
    C2..DF  80..BF
    E0      A0..BF  80..BF
    E1..EC  80..BF  80..BF
    ED      80..9F  80..BF
    EE..EF  80..BF  80..BF
    F0      90..BF  80..BF  80..BF
    F1..F3  80..BF  80..BF  80..BF
    F4      80..8F  80..BF  80..BF
*/

/*
Ill-formed Truncated UTF-8 Subsequences:
    C2..DF
    E0
    E0      A0..BF
    E1..EC
    E1..EC  80..BF
    ED
    ED      80..9F
    EE..EF
    EE..EF  80..BF
    F0
    F0      90..BF
    F0      90..BF  80..BF
    F1..F3
    F1..F3  80..BF
    F1..F3  80..BF  80..BF
    F4
    F4      80..8F
    F4      80..8F  80..BF
*/

/*
    Each invalid byte in an ill-formed UTF-8 sequence is replaced with a REPLACEMENT_CHARACTER
    except truncated UTF-8 subsequences, which are replaced with a single REPLACEMENT_CHARACTER.
    This holds true when reading a UTF-8 sequence backwards or forwards.
*/

u8_t utf_8_subsequence_from(utf_8_subsequence_t* it, const utf_8_t* from)
{
    assert(it);
    assert(from);

    #define read_1()            \
    {                           \
        it->a = *(from + 0);    \
        it->unit_count = 1;     \
        return 1;               \
    }

    #define read_2()            \
    {                           \
        it->a = *(from + 0);    \
        it->b = *(from + 1);    \
        it->unit_count = 2;     \
        return 2;               \
    }

    #define read_3()            \
    {                           \
        it->a = *(from + 0);    \
        it->b = *(from + 1);    \
        it->c = *(from + 2);    \
        it->unit_count = 3;     \
        return 3;               \
    }

    #define read_4()            \
    {                           \
        it->a = *(from + 0);    \
        it->b = *(from + 1);    \
        it->c = *(from + 2);    \
        it->d = *(from + 3);    \
        it->unit_count = 4;     \
        return 4;               \
    }

    #define replace(READ_UNIT_COUNT_)   \
    {                                   \
        it->a = 0xEF;                   \
        it->b = 0xBF;                   \
        it->c = 0xBD;                   \
        it->unit_count = 3;             \
        return READ_UNIT_COUNT_;        \
    }

    if (*(from + 0) >= 0x00 && *(from + 0) <= 0x7F) {
        read_1();
    } else if (*(from + 0) >= 0xC2 && *(from + 0) <= 0xDF) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            read_2();
        } else {
            replace(1);
        }
    } else if (*(from + 0) == 0xE0) {
        if (*(from + 1) >= 0xA0 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xE1 && *(from + 0) <= 0xEC) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) == 0xED) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0x9F) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) >= 0xEE && *(from + 0) <= 0xEF) {
        if (*(from + 1) >= 0x80 && *(from + 1) <= 0xBF) {
            if (*(from + 2) >= 0x80 && *(from + 2) <= 0xBF) {
                read_3();
            } else {
                replace(2);
            }
        } else {
            replace(1);
        }
    } else if (*(from + 0) == 0xF0) {
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
    } else if (*(from + 0) == 0xF4) {
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

    #undef read_1
    #undef read_2
    #undef read_3
    #undef read_4
    #undef replace
}

u8_t utf_8_subsequence_from_previous(utf_8_subsequence_t* it, const utf_8_t* from, const utf_8_t* first)
{
    assert(it);
    assert(from);
    assert(first);
    assert(from > first);

    #define read_1()            \
    {                           \
        it->a = *(from - 1);    \
        it->unit_count = 1;     \
        return 1;               \
    }

    #define read_2()            \
    {                           \
        it->a = *(from - 1);    \
        it->b = *(from - 2);    \
        it->unit_count = 2;     \
        return 2;               \
    }

    #define read_3()            \
    {                           \
        it->a = *(from - 1);    \
        it->b = *(from - 2);    \
        it->c = *(from - 3);    \
        it->unit_count = 3;     \
        return 3;               \
    }

    #define read_4()            \
    {                           \
        it->a = *(from - 1);    \
        it->b = *(from - 2);    \
        it->c = *(from - 3);    \
        it->d = *(from - 4);    \
        it->unit_count = 4;     \
        return 4;               \
    }

    #define replace(READ_UNIT_COUNT_)   \
    {                                   \
        it->a = 0xEF;                   \
        it->b = 0xBF;                   \
        it->c = 0xBD;                   \
        it->unit_count = 3;             \
        return READ_UNIT_COUNT_;        \
    }

    if (*(from - 1) >= 0x00 && *(from - 1) <= 0x7F) {
        read_1(); // w[00..7F]
    } else if (*(from - 1) >= 0x80 && *(from - 1) <= 0xBF) {
        if (from - 2 >= first) {
            if (*(from - 2) >= 0xC2 && *(from - 2) <= 0xDF) {
                read_2(); // w[C2..DF, 80..BF]
            } else if (*(from - 2) >= 0xA0 && *(from - 2) <= 0xBF) {
                if (from - 3 >= first) {
                    if (*(from - 3) == 0xE0) {
                        read_3(); // w[E0, A0..BF, 80..BF]
                    }
                }
            } else if (*(from - 2) >= 0x80 && *(from - 2) <= 0xBF) {
                if (from - 3 >= first) {
                    if (*(from - 3) >= 0xE1 && *(from - 3) <= 0xEC) {
                        read_3(); // w[E1..EC, 80..BF, 80..BF]
                    } else if (*(from - 3) >= 0xEE && *(from - 3) <= 0xEF) {
                        read_3(); // w[EE..EF, 80..BF, 80..BF]
                    } else if (*(from - 3) >= 0x90 && *(from - 3) <= 0xBF) {
                        if (from - 4 >= first) {
                            if (*(from - 4) == 0xF0) {
                                read_4(); // w[F0, 90..BF, 80..BF, 80..BF]
                            }
                        }
                    } else if (*(from - 3) >= 0x80 && *(from - 3) <= 0xBF) {
                        if (from - 4 >= first) {
                            if (*(from - 4) >= 0xF1 && *(from - 4) <= 0xF3) {
                                read_4(); // w[F1..F3, 80..BF, 80..BF, 80..BF]
                            }
                        }
                    } else if (*(from - 3) >= 0x80 && *(from - 3) <= 0x8F) {
                        if (from - 4 >= first) {
                            if (*(from - 4) == 0xF4) {
                                read_4(); // w[F4, 80..8F, 80..BF, 80..BF]
                            }
                        }
                    } else if (*(from - 3) >= 0xF1 && *(from - 3) <= 0xF3) {
                        replace(3); // i[F1..F3, 80..BF, 80..BF]
                    }
                }
            } else if (*(from - 2) >= 0x80 && *(from - 2) <= 0x9F) {
                if (from - 3 >= first) {
                    if (*(from - 3) == 0xED) {
                        read_3(); // w[ED, 80..9F, 80..BF]
                    }
                }
            } else if (*(from - 2) >= 0xE1 && *(from - 2) <= 0xEC) {
                replace(2); // i[E1..EC, 80..BF]
            } else if (*(from - 2) >= 0xEE && *(from - 2) <= 0xEF) {
                replace(2); // i[EE..EF, 80..BF]
            } else if (*(from - 2) >= 0xF1 && *(from - 2) <= 0xF3) {
                replace(2); // i[F1..F3, 80..BF]
            } else if (*(from - 2) >= 0x90 && *(from - 2) <= 0xBF) {
                if (from - 3 >= first) {
                    if (*(from - 3) == 0xF0) {
                        replace(3); // i[F0, 90..BF, 80..BF]
                    }
                }
            } else if (*(from - 2) >= 0x80 && *(from - 2) <= 0x8F) {
                if (from - 3 >= first) {
                    if (*(from - 3) == 0xF4) {
                        replace(3); // i[F4, 80..8F, 80..BF]
                    }
                }
            }
        }
    } else if (*(from - 1) >= 0xA0 && *(from - 1) <= 0xBF) {
        if (from - 2 >= first) {
            if (*(from - 2) == 0xE0) {
                replace(2); // i[E0, A0..BF]
            }
        }
    } else if (*(from - 1) >= 0x80 && *(from - 1) <= 0x9F) {
        if (from - 2 >= first) {
            if (*(from - 2) == 0xED) {
                replace(2); // i[ED, 80..9F]
            }
        }
    } else if (*(from - 1) >= 0x90 && *(from - 1) <= 0xBF) {
        if (from - 2 >= first) {
            if (*(from - 2) == 0xF0) {
                replace(2); // i[F0, 90..BF]
            }
        }
    } else if (*(from - 1) >= 0x80 && *(from - 1) <= 0x8F) {
        if (from - 2 >= first) {
            if (*(from - 2) == 0xF4) {
                replace(2); // i[F4, 80..8F]
            }
        }
    } else {
        replace(1); // i[C2..DF], i[E0], i[E1..EC], i[ED], i[EE..EF], i[F0], i[F1..F3], i[F4], i[.]
    }

    replace(1); // i[.]

    #undef read_1
    #undef read_2
    #undef read_3
    #undef read_4
    #undef replace
}

bool_t utf_8_subsequence_is_well_formed(const utf_8_subsequence_t* it)
{
    assert(it);

    if (it->unit_count == 1) {
        if (it->a >= 0x00 && it->a <= 0x7F) {
            return true;
        } else {
            return false;
        }
    } else if (it->unit_count == 2) {
        if (it->a >= 0xC2 && it->a <= 0xDF &&
            it->b >= 0x80 && it->b <= 0xBF) {
            return true;
        } else {
            return false;
        }
    } else if (it->unit_count == 3) {
        if (it->a == 0xE0 &&
            it->b >= 0xA0 && it->b <= 0xBF &&
            it->c >= 0x80 && it->c <= 0xBF) {
            return true;
        } else if (it->a >= 0xE1 && it->a <= 0xEC &&
                   it->b >= 0x80 && it->b <= 0xBF &&
                   it->c >= 0x80 && it->c <= 0xBF) {
            return true;
        } else if (it->a == 0xED &&
                   it->b >= 0x80 && it->b <= 0x9F &&
                   it->c >= 0x80 && it->c <= 0xBF) {
            return true;
        } else if (it->a >= 0xEE && it->a <= 0xEF &&
                   it->b >= 0x80 && it->b <= 0xBF &&
                   it->c >= 0x80 && it->c <= 0xBF) {
            return true;
        } else {
            return false;
        }
    } else if (it->unit_count == 4) {
        if (it->a == 0xF0 &&
            it->b >= 0x90 && it->b <= 0xBF &&
            it->c >= 0x80 && it->c <= 0xBF &&
            it->d >= 0x80 && it->d <= 0xBF) {
            return true;
        } else if (it->a >= 0xF1 && it->a <= 0xF3 &&
                   it->b >= 0x80 && it->b <= 0xBF &&
                   it->c >= 0x80 && it->c <= 0xBF &&
                   it->d >= 0x80 && it->d <= 0xBF) {
            return true;
        } else if (it->a == 0xF4 &&
                   it->b >= 0x80 && it->b <= 0x8F &&
                   it->c >= 0x80 && it->c <= 0xBF &&
                   it->d >= 0x80 && it->d <= 0xBF) {
            return true;
        } else {
            return false;
        }
    } else {
        return false;
    }
}

void_t utf_8_subsequence_to_16(const utf_8_subsequence_t* it, utf_16_subsequence_t* to)
{
    assert(it);
    assert(to);
    assert(utf_8_subsequence_is_well_formed(it));

    utf_32_subsequence_t intermediate;
    utf_8_subsequence_to_32(it, &intermediate);
    utf_32_subsequence_to_16(&intermediate, to);
}

void_t utf_8_subsequence_to_32(const utf_8_subsequence_t* it, utf_32_subsequence_t* to)
{
    assert(it);
    assert(to);
    assert(utf_8_subsequence_is_well_formed(it));

    if (it->unit_count == 1) {
        to->a = it->a;
    } else if (it->unit_count == 2) {
        to->a =
            (((u32_t)it->a & 0x1F) << 6) |
            ((u32_t)it->b & 0x3F);
    } else if (it->unit_count == 3) {
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

    to->unit_count = 1;
}

u8_t utf_16_subsequence_from(utf_16_subsequence_t* it, const utf_16_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u16(*from) : *from;
    if (utf_16_is_high_surrogate(it->a)) {
        it->b = do_swap ? os_swap_bytes_u16(*(from + 1)) : *(from + 1);
        if (utf_16_is_low_surrogate(it->b)) {
            it->unit_count = 2;
        } else {
            it->a = REPLACEMENT_CHARACTER;
            it->unit_count = 1;
        }
    } else if (utf_16_is_low_surrogate(it->a)) {
        it->a = REPLACEMENT_CHARACTER;
        it->unit_count = 1;
    } else {
        it->unit_count = 1;
    }

    return it->unit_count;
}

u8_t utf_16_subsequence_from_previous(utf_16_subsequence_t* it, const utf_16_t* from, const utf_16_t* first, bool_t do_swap)
{
    assert(it);
    assert(from);
    assert(first);
    assert(from > first);

    utf_16_t read_a = do_swap ? os_swap_bytes_u16(*(from - 1)) : *(from - 1);
    if (utf_16_is_low_surrogate(read_a)) {
        if (from - 2 >= first) {
            utf_16_t read_b = do_swap ? os_swap_bytes_u16(*(from - 2)) : *(from - 2);
            if (utf_16_is_high_surrogate(read_b)) {
                it->a = read_b;
                it->b = read_a;
                it->unit_count = 2;
            } else {
                it->a = REPLACEMENT_CHARACTER;
                it->unit_count = 1;
            }
        } else {
            it->a = REPLACEMENT_CHARACTER;
            it->unit_count = 1;
        }
    } else if (utf_16_is_high_surrogate(read_a)) {
        it->a = REPLACEMENT_CHARACTER;
        it->unit_count = 1;
    } else {
        it->a = read_a;
        it->unit_count = 1;
    }

    return it->unit_count;
}

bool_t utf_16_subsequence_is_well_formed(const utf_16_subsequence_t* it)
{
    assert(it);

    if (it->unit_count == 1) {
        return !utf_16_is_surrogate(it->a);
    } else if (it->unit_count == 2) {
        return utf_16_is_high_surrogate(it->a) && utf_16_is_low_surrogate(it->b);
    } else {
        return false;
    }
}

void_t utf_16_subsequence_to_8(const utf_16_subsequence_t* it, utf_8_subsequence_t* to)
{
    assert(it);
    assert(to);
    assert(utf_16_subsequence_is_well_formed(it));

    utf_32_subsequence_t intermediate;
    utf_16_subsequence_to_32(it, &intermediate);
    utf_32_subsequence_to_8(&intermediate, to);
}

void_t utf_16_subsequence_to_32(const utf_16_subsequence_t* it, utf_32_subsequence_t* to)
{
    assert(it);
    assert(to);
    assert(utf_16_subsequence_is_well_formed(it));

    if (it->unit_count == 1) {
        to->a = it->a;
        to->unit_count = 1;
    } else {
        to->a = ((it->a - UTF_SURROGATE_HIGH_FIRST) << 10) + (it->b - UTF_SURROGATE_LOW_FIRST) + 0x10000;
        to->unit_count = 1;
    }
}

u8_t utf_32_subsequence_from(utf_32_subsequence_t* it, const utf_32_t* from, bool_t do_swap)
{
    assert(it);
    assert(from);

    it->a = do_swap ? os_swap_bytes_u32(*from) : *from;
    if (!utf_32_is_scalar_point(it->a)) {
        it->a = REPLACEMENT_CHARACTER;
    }
    it->unit_count = 1;

    return it->unit_count;
}

u8_t utf_32_subsequence_from_previous(utf_32_subsequence_t* it, const utf_32_t* from, const utf_32_t* first, bool_t do_swap)
{
    assert(it);
    assert(from);
    assert(first);
    assert(from > first);

    it->a = do_swap ? os_swap_bytes_u32(*(from - 1)) : *(from - 1);
    if (!utf_32_is_scalar_point(it->a)) {
        it->a = REPLACEMENT_CHARACTER;
    }
    it->unit_count = 1;

    return it->unit_count;
}

bool_t utf_32_subsequence_is_well_formed(const utf_32_subsequence_t* it)
{
    assert(it);

    return it->unit_count == 1 && utf_32_is_scalar_point(it->a);
}

void_t utf_32_subsequence_to_8(const utf_32_subsequence_t* it, utf_8_subsequence_t* to)
{
    assert(it);
    assert(to);
    assert(utf_32_subsequence_is_well_formed(it));

    if (it->a <= 0x7F) {
        to->a = it->a;
        to->unit_count = 1;
    } else if (it->a <= 0x7FF) {
        to->a = ((it->a >> 6) & 0x1F) | 0xC0;
        to->b = (it->a & 0x3F) | 0x80;
        to->unit_count = 2;
    } else if (it->a <= 0xFFFF) {
        to->a = ((it->a >> 12) & 0x0F) | 0xE0;
        to->b = ((it->a >> 6) & 0x3F) | 0x80;
        to->c = (it->a & 0x3F) | 0x80;
        to->unit_count = 3;
    } else {
        to->a = ((it->a >> 18) & 0x07) | 0xF0;
        to->b = ((it->a >> 12) & 0x3F) | 0x80;
        to->c = ((it->a >> 6) & 0x3F) | 0x80;
        to->d = (it->a & 0x3F) | 0x80;
        to->unit_count = 4;
    }
}

void_t utf_32_subsequence_to_16(const utf_32_subsequence_t* it, utf_16_subsequence_t* to)
{
    assert(it);
    assert(to);
    assert(utf_32_subsequence_is_well_formed(it));

    if (it->a <= 0xFFFF) {
        to->a = it->a;
        to->unit_count = 1;
    } else {
        to->a = ((it->a - 0x10000) >> 10) + UTF_SURROGATE_HIGH_FIRST;
        to->b = ((it->a - 0x10000) & 0x3FF) + UTF_SURROGATE_LOW_FIRST;
        to->unit_count = 2;
    }
}
