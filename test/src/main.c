/*
    Copyright 2021 r-neal-kelly
*/

#include <stdio.h>

#include "regex/allocator_t.h"
#include "regex/callocator_t.h"
#include "regex/array_t.h"
#include "regex/intrinsic.h"

int main(int argument_count, char* arguments[])
{
    printf("sizeof pointer_t: %zu\n", sizeof(pointer_t));
    printf("sizeof allocator_t: %zu\n", sizeof(allocator_t));
    printf("sizeof callocator_t: %zu\n", sizeof(callocator_t));
    printf("sizeof memory_t: %zu\n", sizeof(memory_t));
    printf("sizeof array_t: %zu\n", sizeof(array_t));

    printf("address of allocator.allocate: %p\n", ALLOCATOR.allocate);
    printf("address of allocator.reallocate: %p\n", ALLOCATOR.reallocate);
    printf("address of allocator.deallocate: %p\n", ALLOCATOR.deallocate);

    array_t arr;
    array_create(&arr, &CALLOCATOR, sizeof(u64_t), 16, 1.5f);

    printf("arr type_size: %zu\n", arr.type_size);
    for (word_t idx = 0, end = arr.memory.pointer.byte_count / 8; idx < end; idx += 1) {
        printf("    idx %zu: %zu\n", idx, ((u64_t*)arr.memory.pointer.bytes)[idx]);
    }

    for (word_t idx = 0, end = 24; idx < end; idx += 1) {
        u64_t value = idx * 5;
        array_push(&arr, &value);
        printf("arr capacity: %zu\n", array_capacity(&arr));
    }

    for (word_t idx = 0, end = array_count(&arr); idx < end; idx += 1) {
        u64_t* value_ptr = (u64_t*)array_access(&arr, idx);
        printf("    idx %zu: %zu\n", idx, *value_ptr);
    }

    array_destroy(&arr);

    int c = getc(stdin);

    return 0;
}
