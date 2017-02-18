#ifndef _RPI_SYSTIMER_H
#define _RPI_SYSTIMER_H

#include "rpi.h"

typedef enum {
    SYSTEM_TIMER_M0 = 0x01,
    SYSTEM_TIMER_M1 = 0x02,
    SYSTEM_TIMER_M2 = 0x04,
    SYSTEM_TIMER_M3 = 0x08
} Rpi_Sys_Timer;

typedef struct {
    uint32_t CS;
    uint32_t CLO;
    uint32_t CHI;
    uint32_t C0;
    uint32_t C1;
    uint32_t C2;
    uint32_t C3;
    uint32_t M3;
    uint32_t M2;
    uint32_t M1;
    uint32_t M0;
} rpi_sys_timer_t;


#ifdef __cplusplus
extern "C" {
#endif
    void rpi_st_clr_status(uint8_t timer);
    uint32_t rpi_st_get_status(uint8_t timer);
    void rpi_st_set_cmp(uint8_t timer, uint32_t value);
    uint64_t rpi_st_read();

#ifdef __cplusplus
}
#endif

#endif
