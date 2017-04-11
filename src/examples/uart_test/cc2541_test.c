#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "common/print_util.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"


// This is example is tested when PIN_08 is connected with PIN_10;
int main(int argc, char **args)
{
    if(rpi_init() ==-1){
        printf("initialization failed");
        return -1;
    }

    //configure GPIO pin
    rpi_gpio_fsel(PIN_08, RPI_GPIO_FSEL_ALT0);
    rpi_gpio_fsel(PIN_10, RPI_GPIO_FSEL_ALT0);

    //Default baudrate is 9600 bits/s
    rpi_uart_init(rpi_uart);

    //send string
    char *buf = "AT+NAME?";
    rpi_uart_transmit(rpi_uart, buf, strlen(buf));
    printf("Send string:  %s \n", buf);
    sleep(3);
    while(1){
        //receive string character by character
        printf("Received string:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
        sleep(1);
    }
    return 0;
}
