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

#ifndef _RPI_PWM_H
#define _RPI_PWM_H

#define RPI_PWM_PASSWD (0x5a<<24)

typedef union {
    struct {
        uint32_t PWEN1 :1;
        uint32_t MODE1 :1;
        uint32_t RPTL1 :1;
        uint32_t SBIT1 :1;
        uint32_t POLA1 :1;
        uint32_t USEF1 :1;
        uint32_t CLRF1 :1;
        uint32_t MSEN1 :1;
        uint32_t PWEN2 :1;
        uint32_t MODE2 :1;
        uint32_t RPTL2 :1;
        uint32_t SBIT2 :1;
        uint32_t POLA2 :1;
        uint32_t USEF2 :1;
        uint32_t r0    :1;
        uint32_t MSEN2 :1;
        uint32_t r1    :16;
    } bit;
    uint32_t reg;
} RPI_PWM_CTL_REG;

#define RPI_PWM_CTL_PWEN1_Pos 0
#define RPI_PWM_CTL_PWEN1_Msk 1 << RPI_PWM_CTL_PWEN1_Pos
#define RPI_PWM_CTL_MODE1_Pos 1
#define RPI_PWM_CTL_MODE1_Msk 1 << RPI_PWM_CTL_MODE1_Pos
#define RPI_PWM_CTL_RPLT1_Pos 2
#define RPI_PWM_CTL_RPLT1_Msk 1 << RPI_PWM_CTL_RPLT1_Pos
#define RPI_PWM_CTL_SBIT1_Pos 3
#define RPI_PWM_CTL_SBIT1_Msk 1 << RPI_PWM_CTL_SBIT1_Pos
#define RPI_PWM_CTL_POLA1_Pos 4
#define RPI_PWM_CTL_POLA1_Msk 1 << RPI_PWM_CTL_POLA1_Pos
#define RPI_PWM_CTL_FIFO1_Pos 5
#define RPI_PWM_CTL_FIFO1_Msk 1 << RPI_PWM_CTL_FIFO1_Pos
#define RPI_PWM_CTL_CLRF1_Pos 6
#define RPI_PWM_CTL_CLRF1_Msk 1 << RPI_PWM_CTL_CLRF1_Pos
#define RPI_PWM_CTL_MSEN1_Pos 7
#define RPI_PWM_CTL_MSEN1_Msk 1 << RPI_PWM_CTL_MSEN1_Pos
#define RPI_PWM_CTL_PWEN2_Pos 8
#define RPI_PWM_CTL_PWEN2_Msk 1 << RPI_PWM_CTL_PWEN2_Pos
#define RPI_PWM_CTL_MODE2_Pos 9
#define RPI_PWM_CTL_MODE2_Msk 1 << RPI_PWM_CTL_MODE2_Pos
#define RPI_PWM_CTL_RPLT2_Pos 10
#define RPI_PWM_CTL_RPLT2_Msk 1 << RPI_PWM_CTL_RPLT2_Pos
#define RPI_PWM_CTL_SBIT2_Pos 11
#define RPI_PWM_CTL_SBIT2_Msk 1 << RPI_PWM_CTL_SBIT2_Pos
#define RPI_PWM_CTL_POLA2_Pos 12
#define RPI_PWM_CTL_POLA2_Msk 1 << RPI_PWM_CTL_POLA2_Pos
#define RPI_PWM_CTL_USEF2_Pos 13
#define RPI_PWM_CTL_USEF2_Msk 1 << RPI_PWM_CTL_USEF2_Pos
#define RPI_PWM_CTL_MSEN2_Pos 15
#define RPI_PWM_CTL_MSEN2_Msk 1 << RPI_PWM_CTL_MSEN2_Pos

typedef enum {
    RPI_PWM_CHANNEL_0,
    RPI_PWM_CHANNEL_1
} Rpi_Pwm_Channel;

//19.2Mhz clock for PWM
typedef enum {
    RPI_PWM_CLOCK_DIVIDER_2048 = 2048, //9.375khz
    RPI_PWM_CLOCK_DIVIDER_1024 = 1024, //18.75khz
    RPI_PWM_CLOCK_DIVIDER_512  = 512,  //37.5khz
    RPI_PWM_CLOCK_DIVIDER_256  = 256,  //75khz
    RPI_PWM_CLOCK_DIVIDER_128  = 128,  //150khz
    RPI_PWM_CLOCK_DIVIDER_64   = 64,   //300khz
    RPI_PWM_CLOCK_DIVIDER_32   = 32,   //600.0khz
    RPI_PWM_CLOCK_DIVIDER_16   = 16,   //1.2Mhz
    RPI_PWM_CLOCK_DIVIDER_8    = 8,    //2.4Mhz
    RPI_PWM_CLOCK_DIVIDER_4    = 4,    //4.8Mhz
    RPI_PWM_CLOCK_DIVIDER_2    = 2,    //9.6Mhz, fastest you can get
    RPI_PWM_CLOCK_DIVIDER_1    = 1     //4.6875khz, same as divider 4096
} Rpi_Pwm_Clkdiv;

typedef union {
    struct {
        uint32_t r0 :32;
    } bit;
    uint32_t reg;
} RPI_PWM_SHARE_REG;

typedef struct {
    __IO RPI_PWM_CTL_REG CTL;
    __IO RPI_PWM_SHARE_REG STA;
    __IO RPI_PWM_SHARE_REG DMAC;
    __IO RPI_PWM_SHARE_REG reverved0;
    __IO RPI_PWM_SHARE_REG RNG1;
    __IO RPI_PWM_SHARE_REG DAT1;
    __IO RPI_PWM_SHARE_REG FIF1;
    __IO RPI_PWM_SHARE_REG reverved1;
    __IO RPI_PWM_SHARE_REG RNG2;
    __IO RPI_PWM_SHARE_REG DAT2;
} rpi_pwm_t;

typedef union {
    struct {
        uint32_t SRC  :4;
        uint32_t ENAB :1;
        uint32_t KILL :1;
        uint32_t r0   :1;
        uint32_t BUSY :1;
        uint32_t FLIP :1;
        uint32_t MASH :2;
        uint32_t r1   :13;
        uint32_t PASSWD  :8;
    } bit;
    uint32_t reg;
} RPI_PWM_CLK_CTL_REG;

typedef union {
    struct {
        uint32_t DIVF  :12;
        uint32_t DIVI  :12;
        uint32_t PASSWD  :8;
    } bit;
    uint32_t reg;
} RPI_PWM_CLK_DIV_REG;

typedef struct{
    __IO RPI_PWM_CLK_CTL_REG CTL;
    __IO RPI_PWM_CLK_DIV_REG DIV;
} rpi_pwm_clk_t;

extern volatile rpi_pwm_t *rpi_pwm;
extern volatile rpi_pwm_clk_t *rpi_pwm_clk;

#ifdef __cplusplus
extern "C" {
#endif

    int pwm_init(Rpi_Pwm_Channel channel, uint8_t markspace, uint8_t enable);
    void pwm_set_clock(uint32_t divisor);
    void pwm_set_range(Rpi_Pwm_Channel channel, uint32_t range);
    void pwm_set_data(Rpi_Pwm_Channel channel, uint32_t data);
    void pwm_close(Rpi_Pwm_Channel channel);

#ifdef __cplusplus
}
#endif

#endif
