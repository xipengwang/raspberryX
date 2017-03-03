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
        rpi_gpio_fsel(PIN_12, RPI_GPIO_FSEL_ALT5);

        //PWM test
        pwm_init(0, 1, 1);
        pwm_set_range(0, 1024);
        pwm_set_data(0, 512);

        //i2c test
        rpi_gpio_fsel(PIN_03, RPI_GPIO_FSEL_ALT0); /* SDA */
        rpi_gpio_fsel(PIN_05, RPI_GPIO_FSEL_ALT0); /* SCL */
        rpi_i2c_setslave(rpi_i2c0, 0xC4);
        char buf[] = { 0x01, 0x02, 0x11, 0x33 }; // Data to send

        int w_return = rpi_i2c_write(rpi_i2c0, buf, sizeof(buf));
        if(w_return == RPI_I2C_OK){
            printf("Write to I2C: %02X  %02X  %02X  %02X \n", buf[0], buf[1], buf[2], buf[3]);
        } else if (w_return == RPI_I2C_ERROR_NACK){
            printf("NACK\n");
        } else if (w_return == RPI_I2C_ERROR_CLKT) {
            printf("TIME OUT\n");
        } else if (w_return == RPI_I2C_ERROR_DATA) {
            printf("DATA REMAINING\n");
        }

        int r_return = rpi_i2c_write(rpi_i2c0, buf, sizeof(buf));
        if(r_return == RPI_I2C_OK){
            printf("Read from I2C: %02X  %02X  %02X  %02X \n", buf[0], buf[1], buf[2], buf[3]);
        } else if (r_return == RPI_I2C_ERROR_NACK){
            printf("NACK\n");
        } else if (r_return == RPI_I2C_ERROR_CLKT) {
            printf("TIME OUT\n");
        } else if (r_return == RPI_I2C_ERROR_DATA) {
            printf("DATA REMAINING\n");
        }

        rpi_i2c_close(PIN_03, PIN_05);


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
