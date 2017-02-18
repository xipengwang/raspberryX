#include "systimer.h"
volatile rpi_sys_timer_t *rpi_sys_timer;

void rpi_st_clr_status(uint8_t timer)
{
    rpi_sys_timer->CS |= timer;
}

uint32_t rpi_st_get_status(uint8_t timer)
{
    return rpi_sys_timer->CS & timer;
}

void rpi_st_set_cmp(uint8_t timer, uint32_t value)
{
    switch(timer){
        case SYSTEM_TIMER_M0:
            rpi_sys_timer->C0 = value;
            break;
        case SYSTEM_TIMER_M1:
            rpi_sys_timer->C1 = value;
            break;
        case SYSTEM_TIMER_M2:
            rpi_sys_timer->C2 = value;
            break;
        case SYSTEM_TIMER_M3:
            rpi_sys_timer->C3 = value;
            break;
    }
}

uint64_t rpi_st_read()
{
    uint64_t v = rpi_sys_timer->CHI;
    return (v << 32 | rpi_sys_timer->CLO);
}
