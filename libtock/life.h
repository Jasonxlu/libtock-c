#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define DRIVER_NUM_LIFE 0x00090008

    // Returns the meaning of life.
    int life_meaning(int *life);

    // If 42 is not the meaning of life, this function will error.
    int define_life(int life_num);

#ifdef __cplusplus
}
#endif
