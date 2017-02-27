#ifndef _RPI_PWM_H
#define _RPI_PWM_H

#include "rpi.h"


typedef struct {
    uint32_t CTL;
    uint32_t STA;
    uint32_t DMAC;
    uint32_t RNG1;
    uint32_t DAT1;
    uint32_t FIF1;
    uint32_t RNG2;
    uint32_t DAT2;
} rpi_pwm_t;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
