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

#ifndef _RPI_SYSTIMER_H
#define _RPI_SYSTIMER_H

typedef enum {
    SYSTEM_TIMER_M0 = 0x01,
    SYSTEM_TIMER_M1 = 0x02,
    SYSTEM_TIMER_M2 = 0x04,
    SYSTEM_TIMER_M3 = 0x08
} Rpi_Sys_Timer;

typedef union {
    struct {
        uint32_t r0 :1;
        uint32_t r1 :31;
    } bit;
    uint32_t reg;
} RPI_SYSTEM_TIMER_SHARE_REG;

typedef struct {
    __IO RPI_SYSTEM_TIMER_SHARE_REG CS;
    __IO RPI_SYSTEM_TIMER_SHARE_REG CLO;
    __IO RPI_SYSTEM_TIMER_SHARE_REG CHI;
    __IO RPI_SYSTEM_TIMER_SHARE_REG C0;
    __IO RPI_SYSTEM_TIMER_SHARE_REG C1;
    __IO RPI_SYSTEM_TIMER_SHARE_REG C2;
    __IO RPI_SYSTEM_TIMER_SHARE_REG C3;
    __IO RPI_SYSTEM_TIMER_SHARE_REG M3;
    __IO RPI_SYSTEM_TIMER_SHARE_REG M2;
    __IO RPI_SYSTEM_TIMER_SHARE_REG M1;
    __IO RPI_SYSTEM_TIMER_SHARE_REG M0;
} rpi_sys_timer_t;

extern volatile rpi_sys_timer_t *rpi_sys_timer;

#ifdef __cplusplus
extern "C" {
#endif

    void rpi_st_clr_status(Rpi_Sys_Timer timer);
    uint32_t rpi_st_get_status(Rpi_Sys_Timer timer);
    void rpi_st_set_cmp(Rpi_Sys_Timer timer, uint32_t value);
    uint64_t rpi_st_read();

#ifdef __cplusplus
}
#endif

#endif
