/*
    Copyright 2021 r-neal-kelly
*/

#pragma once

#include <stdio.h>

#include "regex/intrinsic.h"

#define test(BOOL_EXPRESSION_)                          \
{                                                       \
    if (!BOOL_EXPRESSION_) {                            \
        wprintf(L"failed: " L#BOOL_EXPRESSION_ L"\n");  \
        wprintf(L"%s: %i", __FILEW__, __LINE__);        \
        wchar_t unused = getwc(stdin);                  \
    }                                                   \
}
