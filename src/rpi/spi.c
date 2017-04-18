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

#include "rpi.h"
#include "gpio.h"
#include "spi.h"

volatile rpi_spi_t *rpi_spi0;

int rpi_spi_init(volatile rpi_spi_t *rpi_spi0)
{
    if (rpi_spi0 == MAP_FAILED)
        return -1;

    //reset CS, and select chip00
    rpi_spi0->CS.reg = 0;
    //clear FIFO
    rpi_spi0->CS.bit.CLEAR = 0x03;
    return 0;
}

void rpi_spi_close(Rpi_Gpio_Pin SS, Rpi_Gpio_Pin CLK, Rpi_Gpio_Pin MISO, Rpi_Gpio_Pin MOSI)
{
    rpi_gpio_fsel(SS, RPI_GPIO_FSEL_IN);
    rpi_gpio_fsel(CLK, RPI_GPIO_FSEL_IN);
    rpi_gpio_fsel(MISO, RPI_GPIO_FSEL_IN);
    rpi_gpio_fsel(MOSI, RPI_GPIO_FSEL_IN);
}

/* defaults to 0, which means a divider of 65536.
   The divisor must be a power of 2.
   The maximum SPI clock rate is of the APB clock
   RPI_SPI_CLOCK_DIVIDER_65536 = 0,       65536 = 262.144us = 3.814697260kHz
*/
void rpi_spi_set_clk_divider(volatile rpi_spi_t *rpi_spi0, uint16_t _divider)
{
    uint16_t divider = _divider;
    if(divider & (divider-1)) {
        //not a power of 2
        divider = 2;
        while (divider < _divider)
            divider *= 2;
    }
    rpi_spi0->CLK.bit.CDIV = divider;
}

void rpi_spi_chip_select(volatile rpi_spi_t *rpi_spi0, Rpi_Spi_Chip Chip)
{
    rpi_spi0->CS.bit.CS = Chip;
}

void rpi_spi_set_chip_polarity(volatile rpi_spi_t *rpi_spi0, Rpi_Spi_Chip Chip, int level)
{
    if (Chip == RPI_CHIP_SELECT_0) {
        rpi_spi0->CS.bit.CSPOL0 = level;
    } else if (Chip == RPI_CHIP_SELECT_1) {
        rpi_spi0->CS.bit.CSPOL1 = level;
    }
}

/* Writes (and reads) an number of bytes to SPI */
void rpi_spi_transfernb(volatile rpi_spi_t *rpi_spi0, const char* tbuf, char* rbuf, uint32_t len)
{

    uint32_t TXCnt = 0;
    uint32_t RXCnt = 0;

    //clear FIFO
    rpi_spi0->CS.bit.CLEAR = 0x03;

    // Set TA = 1; Transfer active.
    rpi_spi0->CS.bit.TA = 1;

    while ((TXCnt < len)||(RXCnt < len)) {
        // TX fifo not full, so add some more bytes
        // We write byte by byte into FIFO, not word by word
        while (rpi_spi0->CS.bit.TXD && (TXCnt < len)) {
            rpi_spi0->FIFO.reg = tbuf[TXCnt];
            TXCnt++;
        }
        // Rx fifo not empty, so get the next received bytes
        while( rpi_spi0->CS.bit.RXD && ( RXCnt < len )) {
            rbuf[RXCnt] = rpi_spi0->FIFO.reg;
            RXCnt++;
        }
    }
    // Wait for DONE to be set
    while (!rpi_spi0->CS.bit.DONE);

    // Set TA = 0
    rpi_spi0->CS.bit.TA = 0;
}

void rpi_spi_writenb(volatile rpi_spi_t *rpi_spi0, const char* tbuf, uint32_t len)
{
    uint32_t cnt = 0;
    //clear FIFO
    rpi_spi0->CS.bit.CLEAR = 0x03;
    // Set TA = 1; Transfer active.
    rpi_spi0->CS.bit.TA = 1;
    for (cnt = 0; cnt < len; cnt++) {
        while(!rpi_spi0->CS.bit.TXD);
        rpi_spi0->FIFO.reg = tbuf[cnt];
        while(rpi_spi0->CS.bit.RXD) {
            rpi_spi0->FIFO.reg;
        }
    }

    while (!rpi_spi0->CS.bit.DONE) {
        while(rpi_spi0->CS.bit.RXD) {
            rpi_spi0->FIFO.reg;
        }
    }
    // Set TA = 0
    rpi_spi0->CS.bit.TA = 0;
}

char rpi_spi_transfer(volatile rpi_spi_t *rpi_spi0, const char value)
{
    //clear FIFO
    rpi_spi0->CS.bit.CLEAR = 0x03;
    // Set TA = 1; Transfer active.
    rpi_spi0->CS.bit.TA = 1;

    /* Maybe wait for TXD */
    while (!rpi_spi0->CS.bit.TXD);

    rpi_spi0->FIFO.reg = value;

    while (!rpi_spi0->CS.bit.DONE);

    char ret = rpi_spi0->FIFO.reg;

    rpi_spi0->CS.bit.TA = 0;

    return ret;
}
