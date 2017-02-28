#include "pwm.h"

volatile rpi_pwm_t *rpi_pwm;
volatile rpi_pwm_clk_t* rpi_pwm_clk;

void pwm_init(uint8_t channel,  uint8_t markspace, uint8_t enable)
{
    if(rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED)
	return;
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
    if(rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED)
	return;
    divisor &= 0xffff;
    // stop PWM clock
    rpi_pwm_clk->CTL = RPI_PWM_PASSWD | 0x01;
    timeutil_usleep(110);
    while((rpi_pwm_clk->CTL & 0x80) !=0){
	timeutil_usleep(1);
    }
        //set clock divider and enable pwm clock
    rpi_pwm_clk->DIV = RPI_PWM_PASSWD | (divisor <<12);
    rpi_pwm_clk->CTL = RPI_PWM_PASSWD | 0x11;
    
    
}

void pwm_set_range(uint8_t channel, uint32_t range)
{
    if(rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED)
	return;   
    if (channel == 0) {
        rpi_pwm->RNG1 = range;
    } else if (channel == 1) {
        rpi_pwm->RNG2 = range;
    }
}

void pwm_set_data(uint8_t channel, uint32_t data)
{
    if(rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED)
	return;
    if(channel == 0) {
        rpi_pwm->DAT1 = data;
    }else if(channel == 1){
        rpi_pwm->DAT2 = data;
    }
}

void pwm_close(uint8_t channel)
{
    if(rpi_pwm == MAP_FAILED || rpi_pwm_clk == MAP_FAILED)
	return;
    uint32_t control = rpi_pwm->CTL;
    //disable PWM
    if (channel == 0){
	control &= ~RPI_PWM_CTL_PWEN1;
    } else if (channel == 1){
	control &= ~RPI_PWM_CTL_PWEN2;
    }
    rpi_pwm->CTL = control;
}
