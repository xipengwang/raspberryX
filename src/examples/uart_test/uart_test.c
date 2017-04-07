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

    //change baudrate 11520:
    //rpi_uart_set_baudrate(rpi_uart, RPI_UART_BAUD_RATE_11520);

    //change baudrate to 4800:
    //rpi_uart_set_baudrate(rpi_uart, 4800);

    while(1){
        //send string
        char *buf = "Hello world!";
        rpi_uart_transmit(rpi_uart, buf, 12);
        printf("Send string:  %s \n", buf);

        //receive string character by character
        printf("Received string:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");

        //Test send character by characters and read a fixed length string.
        //Interesting thing will happen if use fixed receive length is smaller than the sending length; Try it!
        //Hint: Most time, you don't know received data length, so try to use rpi_uart_getc().
        if(0) {
            for (int i = 0; i < 12; i++) {
                rpi_uart_putc(rpi_uart, buf[i]);
            }

            char rbuf[15] = { '\0' };
            int len = rpi_uart_receive(rpi_uart, rbuf, 10);
            printf("Receive Data:%d, %s \n", len, rbuf);
            timeutil_sleep(1);
        }

    }
    return 0;
}
