#ifndef _RPI_PWM_H
#define _RPI_PWM_H

#include "rpi.h"
#include "gpio.h"
#include "common/time_util.h"
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

typedef enum
{
    BCM_PWM_CLOCK_DIVIDER_2048 = 2048, //9.375khz
    BCM_PWM_CLOCK_DIVIDER_1024 = 1024, //18.75khz
    BCM_PWM_CLOCK_DIVIDER_512 = 512, //37.5khz
    BCM_PWM_CLOCK_DIVIDER_256 = 256, //75khz
    BCM_PWM_CLOCK_DIVIDER_128 = 128, //150khz
    BCM_PWM_CLOCK_DIVIDER_64 = 64, //300khz
    BCM_PWM_CLOCK_DIVIDER_32 = 32, //600.0khz
    BCM_PWM_CLOCK_DIVIDER_16 = 16, //1.2Mhz
    BCM_PWM_CLOCK_DIVIDER_8 = 8, //2.4Mhz
    BCM_PWM_CLOCK_DIVIDER_4 = 4, //4.8Mhz
    BCM_PWM_CLOCK_DIVIDER_2 = 2, //9.6Mhz, fastest you can get
    BCM_PWM_CLOCK_DIVIDER_1 = 1 //4.6875khz, same as divider 4096
} rpi_pwm_clkdiv_t;


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
    //31-24 passwd
    //23-11 unused
    //10-9 mash control
    //8 flip, 7 busy, 6 unused, 5 kill, 4 enable
    //3-0 src
    uint32_t CTL;
    //31-24: passwd   5a
    //23-12: DIVI Integer part of divisor
    //11-0: DIVF: Fractional part of divisor
    uint32_t DIV;
} rpi_pwm_clk_t;

extern volatile rpi_pwm_t *rpi_pwm;
extern volatile rpi_pwm_clk_t *rpi_pwm_clk;

#ifdef __cplusplus
extern "C" {
#endif
void pwm_init(uint8_t channel, uint8_t markspace, uint8_t enable);

void pwm_set_clock(uint32_t divisor);

void pwm_set_range(uint8_t channel, uint32_t range);

void pwm_set_data(uint8_t channel, uint32_t data);

void pwm_close(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif
