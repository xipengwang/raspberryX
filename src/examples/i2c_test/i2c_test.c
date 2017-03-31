#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "common/print_util.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"

//BMP180 default address.
#define BMP180_I2CADDR            0x77

//Operating Modes
#define BMP180_ULTRALOWPOWER      0
#define BMP180_STANDARD           1
#define BMP180_HIGHRES            2
#define BMP180_ULTRAHIGHRES       3

//BMP180 Registers
#define BMP180_CAL_AC1            0xAA //R   Calibration data (16 bits)
#define BMP180_CAL_AC2            0xAC
#define BMP180_CAL_AC3            0xAE
#define BMP180_CAL_AC4            0xB0
#define BMP180_CAL_AC5            0xB2
#define BMP180_CAL_AC6            0xB4
#define BMP180_CAL_B1             0xB6
#define BMP180_CAL_B2             0xB8
#define BMP180_CAL_MB             0xBA
#define BMP180_CAL_MC             0xBC
#define BMP180_CAL_MD             0xBE
#define BMP180_CONTROL            0xF4
#define BMP180_TEMPDATA           0xF6
#define BMP180_PRESSUREDATA       0xF6
#define BMP180_ID                 0xD0

//Commands
#define BMP180_READTEMPCMD        0x2E
#define BMP180_READPRESSURECMD    0x34

//example: BMP180

uint16_t read_16(volatile rpi_i2c_t *rpi_i2c, char *buf);
void get_cal(volatile rpi_i2c_t *rpi_i2c, uint16_t *cal_AC1, uint16_t *cal_AC2, uint16_t *cal_AC3,
             uint16_t *cal_AC4, uint16_t *cal_AC5, uint16_t *cal_AC6,
             uint16_t *cal_B1, uint16_t *cal_B2,
             uint16_t *cal_MB, uint16_t *cal_MC, uint16_t *cal_MD);

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
        rpi_i2c_setslave(rpi_i2c1, BMP180_I2CADDR);
        rpi_i2c_setclockdivider(rpi_i2c1, 148);

        //Get default chipID, it should be 0X55.
        char buf[] = { BMP180_ID }; // Data to send; Chip-ID register
        char len = sizeof(buf) / sizeof(char);
        double i2c_bytes_wait_us = len *
            ((float)rpi_i2c1->DIV.bit.CDIV / RPI_CORE_CLK_HZ) * 1000000 * 9;

        //Write register address 0xD0
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

        //Read chip ID.
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

        //Get Temperature, Pressure, and Altitude.
        //Only cal_AC4,5,6 are unsigned short
        uint16_t cal_AC1;
        uint16_t cal_AC2;
        uint16_t cal_AC3;
        uint16_t cal_AC4;
        uint16_t cal_AC5;
        uint16_t cal_AC6;
        uint16_t cal_B1;
        uint16_t cal_B2;
        uint16_t cal_MB;
        uint16_t cal_MC;
        uint16_t cal_MD;

        get_cal(rpi_i2c1, &cal_AC1, &cal_AC2, &cal_AC3,
                &cal_AC4, &cal_AC5, &cal_AC6,
                &cal_B1, &cal_B2,
                &cal_MB, &cal_MC, &cal_MD);




        char command[2] = {BMP180_CONTROL , BMP180_READTEMPCMD};
        rpi_i2c_write(rpi_i2c1, command, 2);
        //rpi_i2c_write(rpi_i2c1, &command[0], 1);
        //rpi_i2c_write(rpi_i2c1, &command[1], 1);
        timeutil_usleep(30);
        char reg[2] = {BMP180_TEMPDATA, BMP180_TEMPDATA+1};
        uint16_t T_raw = read_16(rpi_i2c1, reg);

        long X1 = ((T_raw - cal_AC6) * cal_AC5) >> 15;
        long X2 = ((int16_t)cal_MC << 11) / (X1 + (int16_t)cal_MD);
        long B5 = X1 + X2;
        double temp = ((B5 + 8) >> 4) / 10.0;

        printf("%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d %d\n",
               (int16_t)cal_AC1, (int16_t)cal_AC2, (int16_t)cal_AC3,
               cal_AC4, cal_AC5, cal_AC6,
               (int16_t)cal_B1, (int16_t)cal_B2,
               (int16_t)cal_MB, (int16_t)cal_MC, (int16_t)cal_MD, T_raw);

        printf("temperature %f \n", temp);

        //rpi_i2c_close(PIN_03, PIN_05);

        /* while(1) { */
        /*     rpi_gpio_write(pin_out, 0); */
        /*     timeutil_usleep(100000); */
        /*     rpi_gpio_write(pin_out, 1); */
        /*     timeutil_usleep(100000); */
        /* } */
    }


    return 0;
}

uint16_t read_16(volatile rpi_i2c_t *rpi_i2c, char *buf)
{
    uint16_t ret;
    char rbuf[2];
    rpi_i2c_write(rpi_i2c, buf, 1);
    rpi_i2c_read(rpi_i2c, rbuf, 2);
    ret = rbuf[0] << 8 | rbuf[1];
    return ret;
}

void get_cal(volatile rpi_i2c_t *rpi_i2c, uint16_t *cal_AC1, uint16_t *cal_AC2, uint16_t *cal_AC3,
             uint16_t *cal_AC4, uint16_t *cal_AC5, uint16_t *cal_AC6,
             uint16_t *cal_B1, uint16_t *cal_B2,
             uint16_t *cal_MB, uint16_t *cal_MC, uint16_t *cal_MD)
{
    char buf[1];
    buf[0] = BMP180_CAL_AC1;
    *cal_AC1 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_AC2;
    *cal_AC2 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_AC3;
    *cal_AC3 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_AC4;
    *cal_AC4 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_AC5;
    *cal_AC5 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_AC6;
    *cal_AC6 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_B1;
    *cal_B1 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_B2;
    *cal_B2 = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_MB;
    *cal_MB = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_MC;
    *cal_MC = read_16(rpi_i2c, buf);

    buf[0] = BMP180_CAL_MD;
    *cal_MD = read_16(rpi_i2c, buf);

}
