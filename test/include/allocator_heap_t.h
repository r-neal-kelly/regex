/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "regex/allocator_heap_t.h"
#include "regex/pointer_t.h"

#include "intrinsic.h"

inline void_t test_allocator_heap_i()
{
    pointer_t ptr;

    test_begin(L"allocator_heap_i: can allocate 0xFFF");
    {
        bool_t worked = ALLOCATOR_HEAP.allocate(&ptr, 0xFFF);
        test(worked);
        test(pointer_is_valid(&ptr));
        test(ptr.byte_count == 0xFFF);
    }
    test_end();

    test_begin(L"allocator_heap_i: can deallocate");
    {
        ALLOCATOR_HEAP.deallocate(&ptr);
        test(!pointer_is_valid(&ptr));
    }
    test_end();

    test_begin(L"allocator_heap_i: cannot allocate MAX_WORD, and thus should invalidate pointer.");
    {
        ptr.bytes = (byte_t*)1;
        ptr.byte_count = 1;
        bool_t worked = ALLOCATOR_HEAP.allocate(&ptr, MAX_WORD);
        test(!worked);
        test(!pointer_is_valid(&ptr));
        test(ptr.bytes == 0);
        test(ptr.byte_count == 0);
    }
    test_end();

    test_begin(L"allocator_heap_i: deallocate should gracefully fail to deallocate invalid pointer");
    {
        test(!pointer_is_valid(&ptr));
        ALLOCATOR_HEAP.deallocate(&ptr);
        test(!pointer_is_valid(&ptr));
        test(ptr.bytes == 0);
        test(ptr.byte_count == 0);
    }
    test_end();

    test_begin(L"allocator_heap_i: can reallocate and set the new byte_count");
    {
        pointer_t ptr;
        ALLOCATOR_HEAP.allocate(&ptr, 0xFF);
        test(pointer_is_valid(&ptr));
        test(ptr.bytes);
        test(ptr.byte_count == 0xFF);
        bool_t did_reallocate = ALLOCATOR_HEAP.reallocate(&ptr, 0xFFFF);
        test(did_reallocate);
        test(pointer_is_valid(&ptr));
        test(ptr.byte_count == 0xFFFF);
        ALLOCATOR_HEAP.deallocate(&ptr);
        test(!pointer_is_valid(&ptr));
    }
    test_end();
}
