#ifndef _RPI_SYSTIMER_H
#define _RPI_SYSTIMER_H

#include "rpi.h"

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
