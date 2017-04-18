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

#include "common/print_util.h"
#include "common/time_util.h"
#include "rpi/rpi_all.h"

//BMP180 default address.
#define BMP180_I2CADDR            0x77

//Operating Modes
//Always us BMP180_ULTRALOWPOWER in this example
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
#define BMP180_DATA               0xF6
#define BMP180_ID                 0xD0

//Commands
#define BMP180_READTEMPCMD        0x2E
#define BMP180_READPRESSURECMD    0x34

//example: BMP180

//This function read a byte back from the register address specified by buf.
uint8_t read_8(volatile rpi_i2c_t *rpi_i2c, char *buf);

//This function read two byte back from the register address specified by buf.
uint16_t read_16(volatile rpi_i2c_t *rpi_i2c, char *buf);

//Get the calibration value of the sensor.
void get_cal(volatile rpi_i2c_t *rpi_i2c, uint16_t *cal_AC1, uint16_t *cal_AC2, uint16_t *cal_AC3,
             uint16_t *cal_AC4, uint16_t *cal_AC5, uint16_t *cal_AC6,
             uint16_t *cal_B1, uint16_t *cal_B2,
             uint16_t *cal_MB, uint16_t *cal_MC, uint16_t *cal_MD);

int main(int argc, char **args)
{
    print_marker("XRobot","Hello World!");
    if(rpi_init() != -1){
        print_version();
        //Set up the PIN functions. This is important!!!!!
        rpi_gpio_fsel(PIN_03, RPI_GPIO_FSEL_ALT0); /* SDA */
        rpi_gpio_fsel(PIN_05, RPI_GPIO_FSEL_ALT0); /* SCL */

        if(rpi_i2c_init(rpi_i2c1)) {
            printf("I2C error \n");
            exit(-1);
        }

        //7 bit addressing.
        rpi_i2c_setslave(rpi_i2c1, BMP180_I2CADDR);
        rpi_i2c_setclockdivider(rpi_i2c1, 2500);

        //Get default chipID, it should be 0X55.
        char buf[] = { BMP180_ID }; // Data to send; Chip-ID register
        char len = sizeof(buf) / sizeof(char);
        double i2c_bytes_wait_us = len *
            ((float)rpi_i2c1->DIV.bit.CDIV / RPI_CORE_CLK_HZ) * 1000000 * 9;

        /* Basic example demonstrating using rpi_i2c_write and rpi_i2c_read */
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

        /* Example demonstrating interacting with BMP180 to get temperature and pressure */

        //soft reset
        //char reset[2] = {0xE0, 0xB6};
        //rpi_i2c_write(rpi_i2c1, reset, 2);

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

        int16_t AC1 = (int16_t)cal_AC1;
        int16_t AC2 = (int16_t)cal_AC2;
        int16_t AC3 = (int16_t)cal_AC3;
        uint16_t AC4 = cal_AC4;
        uint16_t AC5 = cal_AC5;
        uint16_t AC6 = cal_AC6;
        int16_t B1 = (int16_t)cal_B1;
        int16_t B2 = (int16_t)cal_B2;
        int16_t MB = (int16_t)cal_MB;
        int16_t MC = (int16_t)cal_MC;
        int16_t MD = (int16_t)cal_MD;

        //get temperature and pressure
        char t_command[2] = {BMP180_CONTROL , BMP180_READTEMPCMD}; //temperature cmd
        char p_command[2] = {BMP180_CONTROL , BMP180_READPRESSURECMD}; //pressure cmd
        char t_reg[1] = {BMP180_DATA};
        char pressure_reg[3] = {BMP180_DATA, BMP180_DATA+1, BMP180_DATA+2};
        rpi_i2c_write(rpi_i2c1, t_command, 2);
        usleep(5000);
        long T_raw = read_16(rpi_i2c1, t_reg);

        rpi_i2c_write(rpi_i2c1, p_command, 2);
        usleep(10000);
        uint8_t msb = read_8(rpi_i2c1, &pressure_reg[0]);
        uint8_t lsb = read_8(rpi_i2c1, &pressure_reg[1]);
        uint8_t xlsb = read_8(rpi_i2c1, &pressure_reg[2]);
        long P_raw = ((msb << 16) + (lsb << 8) + xlsb) >> 8;

        long X1 = ((T_raw - AC6) * AC5) >> 15;
        long X2 = (MC << 11) / (X1 + MD);
        long B5 = X1 + X2;
        double temp = ((B5 + 8) >> 4) / 10.0;

        printf("temperature %f *C \n", temp);

        long p;
        long B6 = B5 - 4000;
        X1 = (B2 * (B6*B6) >> 12) >> 11;
        X2 = (AC2 * B6) >> 11;
        long X3 = X1 + X2;
        long B3 = ((AC1*4+X3)+2)/4;
        X1 = (AC3 * B6) >> 13;
        X2 = (B1*(B6*B6)>>12)>>16;
        X3 = (X1+X2+2) >> 2;
        unsigned long B4 = (AC4 * (unsigned long)(X3 + 32768)) >> 15;
        unsigned long B7 = (unsigned long)(P_raw-B3)*50000;
        if(B7 < 0x80000000) {
            p = (B7 * 2) / B4;
        } else {
            p = (B7 / B4) * 2;
        }
        X1 = (p >> 8) * (p >> 8);
        X1 = (X1 * 3038) >> 16;
        X2 = (-7357 * p) >> 16;
        p = p + ((X1 + X2 + 3791) >> 4);
        double pressure = p * 0.01;
        printf("pressure %fhPa \n", pressure);

        rpi_i2c_close(PIN_03, PIN_05);
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

uint8_t read_8(volatile rpi_i2c_t *rpi_i2c, char *buf)
{
    uint8_t ret;
    char rbuf[1];
    rpi_i2c_write(rpi_i2c, buf, 1);
    rpi_i2c_read(rpi_i2c, rbuf, 1);
    ret = rbuf[0];
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
