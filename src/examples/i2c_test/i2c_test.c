#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common/print_util.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"

#define pin_out PIN_07
int main(int argc, char **args)
{
    print_marker("XRobot","Hello World!");
    if(rpi_init() != -1){
        print_version();
        rpi_gpio_fsel(pin_out, RPI_GPIO_FSEL_OUT);

        //i2c test
        rpi_gpio_fsel(PIN_03, RPI_GPIO_FSEL_ALT0); /* SDA */
        rpi_gpio_fsel(PIN_05, RPI_GPIO_FSEL_ALT0); /* SCL */
        if(rpi_i2c_init(rpi_i2c1)) {
            printf("I2C error \n");
            exit(-1);
        }

        //7 bit addressing.
        rpi_i2c_setslave(rpi_i2c1, 0x77);
        rpi_i2c_setclockdivider(rpi_i2c1, 1024);

        char buf[] = { 0xD0 }; // Data to send; Chip-ID register
        char len = sizeof(buf) / sizeof(char);
        double i2c_bytes_wait_us = len *
            ((float)rpi_i2c1->DIV.bit.CDIV / RPI_CORE_CLK_HZ) * 1000000 * 9;
        printf("wait_us:%f \n", i2c_bytes_wait_us);
        for(int i = 0; i < 1; i++) {

            int w_return = rpi_i2c_write(rpi_i2c1, buf, len);
            if(w_return == RPI_I2C_OK){
                printf("Write to I2C: %02X \n", buf[0]);
            } else if (w_return == RPI_I2C_ERROR_NACK){
                printf("NACK\n");
            } else if (w_return == RPI_I2C_ERROR_CLKT) {
                printf("TIME OUT\n");
            } else if (w_return == RPI_I2C_ERROR_DATA) {
                printf("DATA REMAINING\n");
            }

            int r_return = rpi_i2c_read(rpi_i2c1, buf, len);
            if (r_return == RPI_I2C_OK) {
                printf("Read from I2C: %02X\n", buf[0]); //Should be 0x55 for MP180
            } else if (r_return == RPI_I2C_ERROR_NACK){
                printf("NACK\n");
            } else if (r_return == RPI_I2C_ERROR_CLKT) {
                printf("TIME OUT\n");
            } else if (r_return == RPI_I2C_ERROR_DATA) {
                printf("DATA REMAINING\n");
            }

            timeutil_usleep(i2c_bytes_wait_us);
        }

        /* rpi_i2c_close(PIN_03, PIN_05); */

        /* while(1) { */
        /*     rpi_gpio_write(pin_out, 0); */
        /*     timeutil_usleep(100000); */
        /*     rpi_gpio_write(pin_out, 1); */
        /*     timeutil_usleep(100000); */
        /* } */
    }


    return 0;
}
