#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common/print_util.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"


int main(int argc, char **args)
{
    if(rpi_init() ==-1){
	printf("initialization failed");
	return -1;
    }
   //configure GPIO pin
    print_version();
    rpi_gpio_fsel(PIN_12, RPI_GPIO_FSEL_ALT5);

    pwm_init(RPI_PWM_CHANNEL_0, 1, 1);
    pwm_set_range(0, 1024);
    pwm_set_data(0, 512);

    return 0;
}

