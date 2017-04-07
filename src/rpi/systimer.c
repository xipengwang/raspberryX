#include "rpi.h"
#include "systimer.h"

volatile rpi_sys_timer_t *rpi_sys_timer;
void rpi_st_clr_status(Rpi_Sys_Timer timer)
{
    rpi_sys_timer->CS.reg |= timer;
}

uint32_t rpi_st_get_status(Rpi_Sys_Timer timer)
{
    return rpi_sys_timer->CS.reg & timer;
}

void rpi_st_set_cmp(Rpi_Sys_Timer timer, uint32_t value)
{
    switch(timer){
        case SYSTEM_TIMER_M0:
            rpi_sys_timer->C0.reg = value;
            break;
        case SYSTEM_TIMER_M1:
            rpi_sys_timer->C1.reg = value;
            break;
        case SYSTEM_TIMER_M2:
            rpi_sys_timer->C2.reg = value;
            break;
        case SYSTEM_TIMER_M3:
            rpi_sys_timer->C3.reg = value;
            break;
    }
}

uint64_t rpi_st_read()
{
    uint64_t v = rpi_sys_timer->CHI.reg;
    return (v << 32 | rpi_sys_timer->CLO.reg);
}
