#include "life.h"

int life_meaning(int *life)
{
    syscall_return_t rval = command(DRIVER_NUM_LIFE, 0, 0, 0);
    return tock_command_return_u32_to_returncode(rval, (uint32_t *)life);
}

int define_life(int life_num)
{
    syscall_return_t rval = command(DRIVER_NUM_LIFE, 1, life_num, 0);
    return tock_command_return_novalue_to_returncode(rval);
}
