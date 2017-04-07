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

    //change baudrate: default is 9600
    //rpi_uart_set_baudrate(rpi_uart, RPI_UART_BAUD_RATE_11520);

    while(1){
        //send string
        char *buf = "Hello-world!";
        rpi_uart_transmit(rpi_uart, buf, 12);
        printf("Send Data: %s \n", buf);

        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");

        /* char rbuf[15] = "\0"; */
        /* rbuf[14] = '\0'; */
        /* int len = rpi_uart_receive(rpi_uart, rbuf, 12); */
        /* printf("Receive Data:%d, %s \n", len, rbuf); */
        timeutil_sleep(1);
    }
    return 0;
}
