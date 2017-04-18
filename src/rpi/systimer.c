/*
 * Copyright (C) <2017>  <Xipeng Wang>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
