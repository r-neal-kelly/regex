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
    int c;
    array_t arr;

    printf("sizeof pointer_t: %zu\n", sizeof(pointer_t));
    printf("sizeof allocator_t: %zu\n", sizeof(allocator_t));
    printf("sizeof callocator_t: %zu\n", sizeof(callocator_t));
    printf("sizeof memory_t: %zu\n", sizeof(memory_t));
    printf("sizeof array_t: %zu\n", sizeof(array_t));

    printf("address of allocator.allocate: %p\n", ALLOCATOR.allocate);
    printf("address of allocator.reallocate: %p\n", ALLOCATOR.reallocate);
    printf("address of allocator.deallocate: %p\n", ALLOCATOR.deallocate);

    array_create(&arr, &CALLOCATOR, sizeof(u64_t), 16, 1.7f);
    printf("arr type_size: %zu", arr.type_size);
    for (word_t idx = 0, end = arr.memory.pointer.byte_count / 8; idx < end; idx += 1) {
        printf("    idx %zu: %zu\n", idx, ((u64_t*)arr.memory.pointer.bytes)[idx]);
    }
    array_destroy(&arr);

    c = getc(stdin);

    return 0;
}
