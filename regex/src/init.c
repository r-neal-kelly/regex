/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/init.h"
#include "regex/os.h"

bool_t  OS_IS_LITTLE_ENDIAN = false;
bool_t  OS_IS_BIG_ENDIAN    = false;

void_t init()
{
    OS_IS_LITTLE_ENDIAN = os_is_little_endian();
    OS_IS_BIG_ENDIAN = os_is_big_endian();
}
