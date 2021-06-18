﻿/*
    Copyright 2021 r-neal-kelly
*/

#include <assert.h>
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
#include <string.h>

#include "regex/allocator_t.h"
#include "regex/array_t.h"
#include "regex/callocator_t.h"
#include "regex/charcoder_utf_8_i.h"
#include "regex/charcoder_utf_16_be_i.h"
#include "regex/charcoder_utf_16_le_i.h"
#include "regex/init.h"
#include "regex/intrinsic.h"
#include "regex/string_t.h"

#include "pointer_t.h"

int main(int argument_count, char* arguments[])
{
    if (_setmode(_fileno(stdin), _O_U16TEXT) == -1) {
        assert(0);
    }
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) {
        assert(0);
    }

    init();

    test_pointer();

    string_t string;
    string_create_with_raw(&string, &CHARCODER_UTF_8_i, u8"neal.νηαλ.נהאל", &CHARCODER_UTF_8_i, &CALLOCATOR, 16, 1.5f);

    wprintf(L"string unit_count: %zu\n", string_unit_count(&string));
    wprintf(L"string point_count: %zu\n", string_point_count(&string));
    wprintf(L"\n");

    for (string_itr itr = string_first(&string); !string_itr_is_postfix(&itr); string_itr_next(&itr)) {
        wprintf(L"unit_idx: %zu, point_idx: %zu is %8.8X\n",
                string_itr_unit_index(&itr),
                string_itr_point_index(&itr),
                string_itr_point(&itr));
    }
    wprintf(L"\n");

    for (string_itr itr = string_null(&string); !string_itr_is_prefix(&itr); string_itr_previous(&itr)) {
        wprintf(L"unit_idx: %zu, point_idx: %zu is %8.8X\n",
                string_itr_unit_index(&itr),
                string_itr_point_index(&itr),
                string_itr_point(&itr));
    }
    wprintf(L"\n");

    for (string_itr itr = string_first(&string); !string_itr_is_null(&itr); string_itr_next(&itr)) {
        wprintf(L"unit_idx: %zu, point_idx: %zu is %8.8X\n",
                string_itr_unit_index(&itr),
                string_itr_point_index(&itr),
                string_itr_point(&itr));
    }
    wprintf(L"\n");

    for (string_itr itr = string_last(&string); !string_itr_is_prefix(&itr); string_itr_previous(&itr)) {
        wprintf(L"unit_idx: %zu, point_idx: %zu is %8.8X\n",
                string_itr_unit_index(&itr),
                string_itr_point_index(&itr),
                string_itr_point(&itr));
    }
    wprintf(L"\n");

    string_destroy(&string);

    string_create_with_raw(&string, &CHARCODER_UTF_16_BE_i, u8"neal.νηαλ.נהאל", &CHARCODER_UTF_8_i, &CALLOCATOR, 16, 1.5f);
    wprintf(L"%s\n", (unsigned short*)string.array.memory.pointer.bytes);

    for (string_itr itr = string_first(&string); !string_itr_is_postfix(&itr); string_itr_next(&itr)) {
        wprintf(L"unit_idx: %zu, point_idx: %zu is %8.8X, literally: 0x%4.4X\n",
                string_itr_unit_index(&itr),
                string_itr_point_index(&itr),
                string_itr_point(&itr),
                *(u16_t*)itr.byte_pointer);
    }
    wprintf(L"\n");

    string_destroy(&string);

    string_create_with_raw(&string, &CHARCODER_UTF_16_LE_i, u8"neal.νηαλ.נהאל", &CHARCODER_UTF_8_i, &CALLOCATOR, 16, 1.5f);
    wprintf(L"%s\n", (unsigned short*)string.array.memory.pointer.bytes);

    for (string_itr itr = string_first(&string); !string_itr_is_postfix(&itr); string_itr_next(&itr)) {
        wprintf(L"unit_idx: %zu, point_idx: %zu is %8.8X, literally: 0x%4.4X\n",
                string_itr_unit_index(&itr),
                string_itr_point_index(&itr),
                string_itr_point(&itr),
                *(u16_t*)itr.byte_pointer);
    }
    wprintf(L"\n");

    string_destroy(&string);

    u16_t exit = getwc(stdin);

    return 0;
}
