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


int main(int argc, char **args)
{
    if(rpi_init() == -1){
        printf("initialization failed");
        return -1;
    }

    // PIN_33: PWM0 [ALT0];
    // PIN_32: PWM1 [ALT0];
    // PIN_12: PWM0 [ALT5];
    // PIN_35: PWM1 [ALT5];
    // Configure GPIO pin as ALT5, so it could be used for PWM function
    rpi_gpio_fsel(PIN_12, RPI_GPIO_FSEL_ALT5);


    // Initialize PWM Channel 0; Return 0 on success
    if(pwm_init(RPI_PWM_CHANNEL_0, 1, 1)) {
        printf("Initializing PWM fails \n");
        exit(-1);
    }

    // RPI_PWM_CLOCK_DIVIDER_2048 = 2048, //9.375khz
    // RPI_PWM_CLOCK_DIVIDER_1024 = 1024, //18.75khz
    // RPI_PWM_CLOCK_DIVIDER_512  = 512,  //37.5khz
    // RPI_PWM_CLOCK_DIVIDER_256  = 256,  //75khz
    // RPI_PWM_CLOCK_DIVIDER_128  = 128,  //150khz
    // RPI_PWM_CLOCK_DIVIDER_64   = 64,   //300khz
    // RPI_PWM_CLOCK_DIVIDER_32   = 32,   //600.0khz
    // RPI_PWM_CLOCK_DIVIDER_16   = 16,   //1.2Mhz
    // RPI_PWM_CLOCK_DIVIDER_8    = 8,    //2.4Mhz
    // RPI_PWM_CLOCK_DIVIDER_4    = 4,    //4.8Mhz
    // RPI_PWM_CLOCK_DIVIDER_2    = 2,    //9.6Mhz, fastest you can get
    // RPI_PWM_CLOCK_DIVIDER_1    = 1     //4.6875khz, same as divider 4096

    // Configure PWM clock = 19.2Mhz / 32 = 600kHz
    pwm_set_clock(32);

    // Set PWM Frequency = 19.2Mhz / 16 / 1000 = 600Hz
    pwm_set_range(0, 1000);

    // Set PWM duty cycle as 800 / 1000 = 80%
    pwm_set_data(0, 800);

    return 0;
}
