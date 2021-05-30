/*
    Copyright 2021 r-neal-kelly
*/

#include "regex/math.h"

bool_t is_power_of_two(word_t word)
{
    return word != 0 && !(word & (word - 1));
}
