/*
 * Copyright (C) <2017>  <Xipeng Wang>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

    /*
     * wake up
     in sleep mode, you can send a long string (Length > 80 or more),
     that string can made module wake up, and you will receive;
     “OK+WAKE” string through UART. That string can’t include any AT commands; */
    if(0) {
        char *buf = "Please wake up! Please wake up! Please wake up! Please wake up! Please wake up! Please wake up!";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        printf("Send string:  %s \n", buf);
        sleep(3);
    }

    {
        //check name
        char *buf = "AT+NAME?";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
        printf("Device name:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
    }

    if(1){
        //set name
        char *buf = "AT+NAMEROBOTX";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
        printf("Device name:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
    }

    if(1){
        //check mac address
        char *buf = "AT+ADDR?";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
        printf("Device MAC:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
    }

    if(1){
        //check type
        char *buf = "AT+TYPE?";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
        printf("Device Type:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
    }

    if(1){
        //Set as type 2 to require passwd; if TYPE0, then no passwd need for connection
        char *buf = "AT+TYPE2";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
        printf("Device Type:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
    }

    if(1){
        //Set passwd as 666666
        char *buf = "AT+PASS666666";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
        printf("Passwd:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
    }

    while(1){
        //receive string character by character
        printf("Received string:  ");
        char c;
        while ((c = rpi_uart_getc(rpi_uart))) {
            printf("%c", c);
        }
        printf("\n");
        char *buf = "Hello World";
        rpi_uart_transmit(rpi_uart, buf, strlen(buf));
        sleep(1);
    }
    return 0;
}
