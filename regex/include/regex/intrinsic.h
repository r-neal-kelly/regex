/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include "stdint.h"

#if _WIN64 || _WIN32
    #if _WIN64
        #define _REGEX_64
    #elif _WIN32
        #define _REGEX_32
    #else
        #error "Cannot compile with this version of windows."
    #endif
#else
    #error "Cannot compile with this architecture."
#endif

typedef int8_t      s8_t;
typedef int16_t     s16_t;
typedef int32_t     s32_t;
typedef int64_t     s64_t;

typedef uint8_t     u8_t;
typedef uint16_t    u16_t;
typedef uint32_t    u32_t;
typedef uint64_t    u64_t;

typedef float       f32_t;
typedef double      f64_t;

typedef void        void_t;
typedef uint8_t     bool_t;

typedef uint8_t     byte_t;

#if defined(_REGEX_64)
typedef u64_t       word_t;
typedef f64_t       fword_t;
#elif defined(_REGEX_32)
typedef u32_t       word_t;
typedef f32_t       fword_t;
#endif
