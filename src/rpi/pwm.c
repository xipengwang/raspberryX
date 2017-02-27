#include "pwm.h"
volatile rpi_pwm_t *rpi_pwm;


void pwm_init(uint8_t channel, Rpi_Gpio_Pin PWM_PIN, uint8_t markspace, uint8_t enable)
{
    rpi_gpio_fsel(PWM_PIN, RPI_GPIO_FSEL_ALT0);
  
    uint32_t control = rpi_pwm->CTL;   
    if(channel == 0){
        if (markspace) {
	    control|= RPI_PWM_CTL_MSEN1;
        } else {
            control &=~RPI_PWM_CTL_MSEN1;
        }
        if (enable) {
	    control |= RPI_PWM_CTL_PWEN1;
        } else {
            control &=~ RPI_PWM_CTL_PWEN1;
        }
    }else if(channel == 1) {
	if (markspace) {
	    control|= RPI_PWM_CTL_MSEN2;
        } else {
            control &=~RPI_PWM_CTL_MSEN2;
        }
        if (enable) {
	    control |= RPI_PWM_CTL_PWEN2;
        } else {
            control &=~ RPI_PWM_CTL_PWEN2;
        }
   }
    rpi_pwm->CTL = control;

}

void pwm_set_clock(uint32_t divider){

}

void pwm_set_range(uint8_t channel, uint32_t range){
    if (channel == 0) {
        mem_write_32_nb((uint32_t*)&rpi_pwm->RNG1, range);
    } else if (channel == 1) {
        mem_write_32_nb((uint32_t*)&rpi_pwm->RNG2, range);
    }
}

void pwm_set_data(uint8_t channel, uint32_t data){
    if(channel == 0) {
        mem_write_32_nb((uint32_t*)&rpi_pwm->DAT1, data);
    }else if(channel == 1){
        mem_write_32_nb((uint32_t*)&rpi_pwm->DAT2, data);
    }
}







