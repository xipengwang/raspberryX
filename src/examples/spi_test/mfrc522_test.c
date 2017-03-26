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

    rpi_gpio_fsel(PIN_26, RPI_GPIO_FSEL_ALT0); /* CE1 */
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
        //check timer
        /* uint8_t v = mfrc522_read_data(MFRC522_ComIrqReg); */
        /* if(CHECK_BITS(v,TimerIRq_MFRC522)){ */
        /*     printf("Receive data: 0x%02X \n", v); */
        /*     mfrc522_clear_bit(MFRC522_ComIrqReg,SET1_MFRC522); */
        /*     //break; */
        /* } */


        int v = mfrc522_query_card(&card_data);
        if(v == MFRC522_Return_Status_OK){
            printf("backbits:0x%02X \n",card_data.numofbits);
            printf("successful!\n");
        }
        else if(v == MFRC522_Return_Status_Timeout){
            printf("timeout!\n");
        }

        v = mfrc522_anticoll(&card_data);
        if(v == MFRC522_Return_Status_OK){
            printf("Receive data:");
            for(int i=0;i<card_data.len;i++){
                printf(" 0x%02X,", card_data.data[i]);
            }
            printf("\n");
            printf("len: %d \n", card_data.len);
            //printf("successful!\n");
        }
        else if(v == MFRC522_Return_Status_Timeout){
            printf("timeout!\n");
        }

    }

    return 0;
}
