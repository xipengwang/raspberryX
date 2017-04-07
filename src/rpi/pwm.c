#include "common/time_util.h"

#include "rpi.h"
#include "gpio.h"
#include "pwm.h"
volatile rpi_pwm_t *rpi_pwm;
volatile rpi_pwm_clk_t* rpi_pwm_clk;

int pwm_init(Rpi_Pwm_Channel channel, uint8_t markspace, uint8_t enable)
{
    if (rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED) {
        return -1;
    }

    if (channel == RPI_PWM_CHANNEL_0) {
        if (markspace) {
            rpi_pwm->CTL.reg |=  RPI_PWM_CTL_PWEN1_Msk;
        } else {
            rpi_pwm->CTL.reg &=  ~RPI_PWM_CTL_PWEN1_Msk;
        }
        if (enable) {
            rpi_pwm->CTL.reg |= RPI_PWM_CTL_MSEN1_Msk;
        } else {
            rpi_pwm->CTL.reg &= ~RPI_PWM_CTL_MSEN1_Msk;
        }
    } else if (channel == RPI_PWM_CHANNEL_1) {
        if (markspace) {
            rpi_pwm->CTL.reg |=  RPI_PWM_CTL_PWEN2_Msk;
        } else {
            rpi_pwm->CTL.reg &=  ~RPI_PWM_CTL_PWEN2_Msk;
        }
        if (enable) {
            rpi_pwm->CTL.reg |= RPI_PWM_CTL_MSEN2_Msk;
        } else {
            rpi_pwm->CTL.reg &= ~RPI_PWM_CTL_MSEN2_Msk;
        }
    }
    return 0;
}

void pwm_set_clock(uint32_t divisor)
{
    if (rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED) {
        return;
    }

    divisor &= 0xffff;
    // stop PWM clock
    rpi_pwm_clk->CTL.reg = RPI_PWM_PASSWD | 0x01;
    timeutil_usleep(110);
    while(rpi_pwm_clk->CTL.bit.BUSY) {
        timeutil_usleep(1);
    }
    //set clock divider and enable pwm clock
    rpi_pwm_clk->DIV.reg = RPI_PWM_PASSWD | (divisor << 12);
    rpi_pwm_clk->CTL.reg = RPI_PWM_PASSWD | 0x11;
}

void pwm_set_range(Rpi_Pwm_Channel channel, uint32_t range)
{
    if(rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED) {
        return;
    }

    if (channel == RPI_PWM_CHANNEL_0) {
        rpi_pwm->RNG1.reg = range;
    } else if (channel == RPI_PWM_CHANNEL_1) {
        rpi_pwm->RNG2.reg = range;
    }
}

void pwm_set_data(Rpi_Pwm_Channel channel, uint32_t data)
{
    if (rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED) {
        return;
    }

    if (channel == RPI_PWM_CHANNEL_0) {
        rpi_pwm->DAT1.reg = data;
    } else if(channel == RPI_PWM_CHANNEL_1) {
        rpi_pwm->DAT2.reg = data;
    }
}

void pwm_close(Rpi_Pwm_Channel channel)
{
    if (rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED) {
        return;
    }
    if (channel == RPI_PWM_CHANNEL_0) {
        rpi_pwm->CTL.reg &= ~RPI_PWM_CTL_MSEN1_Msk;
    } else if (channel == RPI_PWM_CHANNEL_1) {
        rpi_pwm->CTL.reg &= ~RPI_PWM_CTL_MSEN2_Msk;
    }
}
