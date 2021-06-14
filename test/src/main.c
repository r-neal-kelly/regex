/*
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
#include "regex/intrinsic.h"
#include "regex/utf.h"

int main(int argument_count, char* arguments[])
{
    if (_setmode(_fileno(stdin), _O_U16TEXT) == -1) {
        assert(0);
    }
    if (_setmode(_fileno(stdout), _O_U16TEXT) == -1) {
        assert(0);
    }

    wprintf(L"sizeof pointer_t: %zu\n", sizeof(pointer_t));
    wprintf(L"sizeof allocator_t: %zu\n", sizeof(allocator_t));
    wprintf(L"sizeof callocator_t: %zu\n", sizeof(callocator_t));
    wprintf(L"sizeof memory_t: %zu\n", sizeof(memory_t));
    wprintf(L"sizeof array_t: %zu\n", sizeof(array_t));

    wprintf(L"address of allocator.allocate: %p\n", ALLOCATOR.allocate);
    wprintf(L"address of allocator.reallocate: %p\n", ALLOCATOR.reallocate);
    wprintf(L"address of allocator.deallocate: %p\n", ALLOCATOR.deallocate);

    array_t arr;
    array_create(&arr, &CALLOCATOR, sizeof(u64_t), 16, 1.5f);

    wprintf(L"arr type_size: %zu\n", arr.type_size);
    for (word_t idx = 0, end = arr.memory.pointer.byte_count / 8; idx < end; idx += 1) {
        wprintf(L"    idx %zu: %zu\n", idx, ((u64_t*)arr.memory.pointer.bytes)[idx]);
    }

    for (word_t idx = 0, end = 26; idx < end; idx += 1) {
        u64_t value = idx * 5;
        array_push(&arr, &value);
        wprintf(L"arr capacity: %zu\n", array_capacity(&arr));
    }

    for (word_t idx = 0, end = array_count(&arr); idx < end; idx += 1) {
        u64_t* value_ptr = (u64_t*)array_access(&arr, idx);
        wprintf(L"    idx %zu: %zu\n", idx, *value_ptr);
    }

    array_destroy(&arr);

    wprintf(L"Testing\n");

    array_t buffer;
    array_create(&buffer, &CALLOCATOR, sizeof(u8_t), 16, 1.5f);
    FILE* file = 0;
    fopen_s(&file, "./test.txt", "rb");
    if (file) {
        u8_t byte;
        while (fread(&byte, sizeof(u8_t), 1, file) > 0) {
            if (feof(file) == 0) {
                array_push(&buffer, &byte);
            } else {
                break;
            }
        }
    } else {
        wprintf(L"Couldn't open file!");
    }
    for (word_t idx = 0, end = sizeof(utf_32_t); idx < end; idx += 1) {
        u8_t zero = 0;
        array_push(&buffer, &zero);
    }

    wprintf(L"Read file: %s\n", (wchar_t*)buffer.memory.pointer.bytes);
    for (word_t idx = 0, end = array_count(&buffer) / 2; idx < end; idx += 1) {
        wprintf(L"0x%X ", ((wchar_t*)buffer.memory.pointer.bytes)[idx]);
    }
    wprintf(L"\n");

    array_t buffer_2;
    array_create(&buffer_2, &CALLOCATOR, sizeof(u16_t), 16, 1.5f);
    //utf_string_16_be_to_16_le((utf_16_t*)buffer.memory.pointer.bytes, &buffer_2);
    utf_string_8_to_16_le((utf_8_t*)buffer.memory.pointer.bytes, &buffer_2);

    wprintf(L"After conversion: %s\n", (wchar_t*)buffer_2.memory.pointer.bytes);
    for (word_t idx = 0, end = array_count(&buffer_2); idx < end; idx += 1) {
        wprintf(L"0x%X ", ((wchar_t*)buffer_2.memory.pointer.bytes)[idx]);
    }
    wprintf(L"\n");

    array_destroy(&buffer);
    array_destroy(&buffer_2);

    u16_t exit = getwc(stdin);

    return 0;
}
