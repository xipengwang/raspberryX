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


#define pin_out PIN_07
#define pin_in PIN_08
#define pin_event PIN_10

int main(int argc, char **args)
{
    if (rpi_init() == -1) {
        printf("initialization failed");
        return -1;
    }

    // Configure pin_in as input pin
    rpi_gpio_fsel(pin_in, RPI_GPIO_FSEL_IN);
    rpi_gpio_fsel(pin_in, RPI_GPIO_FSEL_IN);

    // Configure pin_out as output pin.
    rpi_gpio_fsel(pin_out, RPI_GPIO_FSEL_OUT);

    // Configure pin_event to detect rising edge event
    rpi_gpio_enable_Redge(pin_event);

    // Read pin_in and check it is LOW or HIGH
    if (rpi_gpio_read(pin_in) == LOW) {
        printf("PIN_07 is low, and we set PIN_08 as HIGH \n");
        // Set pin_out to be HIGH
        rpi_gpio_write(pin_out, HIGH);
    } else {
        printf("PIN_07 is high, and we set PIN_08 as LOW \n");
        // Set pin_out to be LOW
        rpi_gpio_write(pin_out, LOW);
    }

    timeutil_usleep(100000);

    while(1) {
        timeutil_usleep(10);
        // Check the event, if the rising edge event on pin_event happens,
        // we print out something.
        if (rpi_gpio_status(pin_event)) {
            printf("Capture the pin rising event \n");
        }
    }
    return 0;
}
