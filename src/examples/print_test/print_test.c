#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common/print_util.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"

#define pin_out PIN_07
#define pin_in PIN_08
int main(int argc, char **args)
{
    print_marker("XRobot","Hello World!");
    if(rpi_init() != -1){
        print_version();
        rpi_gpio_fsel(pin_out, RPI_GPIO_FSEL_OUT);
        rpi_gpio_fsel(pin_in, RPI_GPIO_FSEL_IN);
        if(rpi_gpio_read(pin_in))
            printf("HIGH!\n");
        else
            printf("LOW!\n");
        while(1){
            rpi_gpio_write(pin_out, 0);
            timeutil_usleep(100000);
            rpi_gpio_write(pin_out, 1);
            timeutil_usleep(100000);
        }
    }


    return 0;
}
