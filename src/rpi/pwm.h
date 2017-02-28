#ifndef _RPI_PWM_H
#define _RPI_PWM_H

#include "rpi.h"
#include "gpio.h"

#define RPI_PWM_PASSWD 0x5a

typedef enum{
    RPI_PWM_CTL_PWEN1 = 1,
    RPI_PWM_CTL_MODE1 = 1<<1,
    RPI_PWM_CTL_RPLT1 = 1<<2,
    RPI_PWM_CTL_SBIT1 = 1<<3,
    RPI_PWM_CTL_POLA1 = 1<<4,
    RPI_PWM_CTL_FIFO1 = 1<<5,
    RPI_PWM_CTL_CLRF1 = 1<<6,
    RPI_PWM_CTL_MSEN1 = 1<<7,

    RPI_PWM_CTL_PWEN2 = 1<<8,
    RPI_PWM_CTL_MODE2 = 1<<9,
    RPI_PWM_CTL_RPLT2 = 1<<10,
    RPI_PWM_CTL_SBIT2 = 1<<11,
    RPI_PWM_CTL_POLA2 = 1<<12,
    RPI_PWM_CTL_FIFP2 = 1<<13,
   //RESERVE 0
    RPI_PWM_CTL_MSEN2 = 1<<15

} rpi_pwm_CTL_t;


typedef struct {
    uint32_t CTL;
    uint32_t STA;
    uint32_t DMAC;
    uint32_t reverved0;
    uint32_t RNG1;
    uint32_t DAT1;
    uint32_t FIF1;
    uint32_t reverved1;
    uint32_t RNG2;
    uint32_t DAT2;
} rpi_pwm_t;

typedef struct{
    //31-24: passwd   5a
    //23-12: DIVI Integer part of divisor
    //11-0: DIVF: Fractional part of divisor
    uint32_t DATA;
} rpi_pwm_clk_t;

#ifdef __cplusplus
extern "C" {
#endif
void pwm_init(uint8_t channel, uint8_t markspace, uint8_t enable);

void pwm_set_clock(uint32_t divisor);

void pwm_set_range(uint8_t channel, uint32_t range);

void pwm_set_data(uint8_t channel, uint32_t data);

void pwm_close(Rpi_Gpio_Pin PWM_PIN);

#ifdef __cplusplus
}
#endif

#endif
