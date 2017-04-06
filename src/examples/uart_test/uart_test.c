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
    //define GPIO 14 as tx
    rpi_gpio_fsel(PIN_08, RPI_GPIO_FSEL_ALT0);
    //define GPIO 15 as rx
    rpi_gpio_fsel(PIN_10, RPI_GPIO_FSEL_ALT0);

    rpi_uart_init(rpi_uart);
    rpi_uart_set_clock(rpi_uart, 813); //250MHz / (16 *10000) = 19200

    //send string
    while(1){
    	char array[6] = "U";
    	rpi_uart_transmit(rpi_uart, array, 1);
        printf("%s \n", array);
        printf("%d\n",rpi_uart->IBRD.reg);
        printf("%X\n",rpi_uart->LCRH.reg);
        printf("%X\n",rpi_uart->CR.reg);
        timeutil_sleep(1);
    }
    return 0;
}
