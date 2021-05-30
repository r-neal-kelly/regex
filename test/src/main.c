/*
    Copyright 2021 r-neal-kelly
*/

#include <stdio.h>

#include "regex/allocator_t.h"
#include "regex/array_t.h"
#include "regex/intrinsic.h"

int main(int argument_count, char* arguments[])
{
    printf("sizeof pointer_t: %zu\n", sizeof(pointer_t));
    printf("sizeof allocator_t: %zu\n", sizeof(allocator_t));
    printf("sizeof memory_t: %zu\n", sizeof(memory_t));
    printf("sizeof array_t: %zu\n", sizeof(array_t));

    return 0;
}
