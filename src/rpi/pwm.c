#include "pwm.h"
volatile rpi_pwm_t *rpi_pwm;
volatile rpi_pwm_clk_t* rpi_pwm_clk;

void pwm_init(uint8_t channel,  uint8_t markspace, uint8_t enable)
{
    uint32_t control = rpi_pwm->CTL;
    if(channel == 0){
        if (markspace) {
            control |=  RPI_PWM_CTL_MSEN1;
        } else {
            control &= ~RPI_PWM_CTL_MSEN1;
        }
        if (enable) {
            control |= RPI_PWM_CTL_PWEN1;
        } else {
            control &= ~RPI_PWM_CTL_PWEN1;
        }
    } else if (channel == 1) {
        if (markspace) {
            control |= RPI_PWM_CTL_MSEN2;
        } else {
            control &= ~RPI_PWM_CTL_MSEN2;
        }
        if (enable) {
            control |= RPI_PWM_CTL_PWEN2;
        } else {
            control &= ~RPI_PWM_CTL_PWEN2;
        }
    }
    rpi_pwm->CTL = control;
}

void pwm_set_clock(uint32_t divisor)
{
    divisor &= 0xffff;
    // stop PWM clock
}

void pwm_set_range(uint8_t channel, uint32_t range)
{
    if (channel == 0) {
        rpi_pwm->RNG1 = range;
    } else if (channel == 1) {
        rpi_pwm->RNG2 = range;
    }
}

void pwm_set_data(uint8_t channel, uint32_t data)
{
    if(channel == 0) {
        rpi_pwm->DAT1 = data;
    }else if(channel == 1){
        rpi_pwm->DAT2 = data;
    }
}

void pwm_close(uint8_t channel)
{
    //disable PWM
}
