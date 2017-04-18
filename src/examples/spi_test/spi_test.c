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

#include "rpi/rpi_all.h"

int main(int argc, char **argv)
{

    setlinebuf(stdout);
    setlinebuf(stderr);

    if (rpi_init())
        return 1;

    if(rpi_spi_init(rpi_spi0))
        return 0;

    //Set UP SPI PINs.
    rpi_gpio_fsel(PIN_26, RPI_GPIO_FSEL_ALT0); /* CE1 */
    rpi_gpio_fsel(PIN_24, RPI_GPIO_FSEL_ALT0); /* CE0 */
    rpi_gpio_fsel(PIN_21, RPI_GPIO_FSEL_ALT0); /* MISO */
    rpi_gpio_fsel(PIN_19, RPI_GPIO_FSEL_ALT0); /* MOSI */
    rpi_gpio_fsel(PIN_23, RPI_GPIO_FSEL_ALT0); /* CLK */

    //set spi
    rpi_spi_set_clk_divider(rpi_spi0, 4096);

    //If CE0 PIN is connected to SLAVE CHIP SS PIN.
    rpi_spi_chip_select(rpi_spi0, RPI_CHIP_SELECT_0);
    rpi_spi_set_chip_polarity(rpi_spi0, RPI_CHIP_SELECT_0, LOW);

    char send_data = 0x88;
    char read_data;

    read_data = rpi_spi_transfer(rpi_spi0, send_data);
    printf("Sent to SPI: 0x%02X. Read back from SPI: 0x%02X.\n", send_data, read_data);
    if (send_data != read_data)
        printf("Do you have the loopback from MOSI to MISO connected?\n");

    char tbuf[] = { 0x11, 0x22, 0x33, 0x55, 0xFF }; // Data to send
    char buf[4];
    rpi_spi_transfernb(rpi_spi0, tbuf, buf, 5);
    // buf will now be filled with the data that was read from the slave
    printf("Read from SPI: %02X  %02X  %02X  %02X \n", buf[0], buf[1], buf[2], buf[3]);

    return 0;
}
