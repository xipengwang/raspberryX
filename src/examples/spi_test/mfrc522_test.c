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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <signal.h>

#include "mfrc522/mfrc522.h"

static void signal_handler(int signum)
{
    switch (signum) {
        case SIGINT:
            puts(" interrupt! \n");
            exit(1);    // XXX Cleanup later, if necessary
            break;
        default:
            break;
    }
}


int main(int argc, char **argv)
{

    setlinebuf(stdout);
    setlinebuf(stderr);
    signal(SIGINT, signal_handler);


    if (rpi_init() == -1) {
        printf("initialization failed");
        return -1;
    }

    //CE0 PIN is connected to SLAVE chip select PIN.
    rpi_gpio_fsel(PIN_24, RPI_GPIO_FSEL_ALT0); /* CE0 */
    rpi_gpio_fsel(PIN_21, RPI_GPIO_FSEL_ALT0); /* MISO */
    rpi_gpio_fsel(PIN_19, RPI_GPIO_FSEL_ALT0); /* MOSI */
    rpi_gpio_fsel(PIN_23, RPI_GPIO_FSEL_ALT0); /* CLK */

    if (!mfrc522_init())
    {
        printf("mfrc522_begin failed. SPI communication may fail. Check wire connections \n");
        return 1;
    }
    mfrc522_card_t card_data;
    while(1){
        //Query the card
        int v = mfrc522_query_card(&card_data);
        if(v == MFRC522_Return_Status_OK) {
            printf("backbits:0x%02X \n", card_data.numofbits);
            printf("successful!\n");
        } else if(v == MFRC522_Return_Status_Timeout) {
            printf("timeout!\n");
        }

        //You need query card before calling this function
        v = mfrc522_anticoll(&card_data);
        if(v == MFRC522_Return_Status_OK) {
            printf("Receive data:");
            for(int i = 0; i < card_data.len; i++){
                printf(" 0x%02X,", card_data.data[i]);
            }
            printf("\n");
            printf("len: %d \n", card_data.len);
        } else if(v == MFRC522_Return_Status_Timeout){
            printf("timeout!\n");
        }

    }

    return 0;
}
