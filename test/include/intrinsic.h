/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>

#include "regex/intrinsic.h"

#define test_begin(NAME_)       \
{                               \
    wprintf(L"test %s", NAME_); \
}

#define test(BOOL_EXPRESSION_)                              \
{                                                           \
    if (!(BOOL_EXPRESSION_)) {                              \
        wprintf(L"\nfailed: " L#BOOL_EXPRESSION_ L"\n");    \
        wprintf(L"%s: %i", __FILEW__, __LINE__);            \
        wchar_t unused = getwc(stdin);                      \
        exit(1);                                            \
    }                                                       \
}

#define test_end()          \
{                           \
    wprintf(L"... ok!\n");  \
}
