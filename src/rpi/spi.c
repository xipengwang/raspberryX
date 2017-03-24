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
        while (_divider < divider)
            divider *= 2;
    }
    rpi_spi0->CLK.bit.CDIV = divider;
}

/* Writes (and reads) an number of bytes to SPI */
void rpi_spi_transfernb(volatile rpi_spi_t *rpi_spi0, char* tbuf, char* rbuf, uint32_t len)
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
        if (rbuf) {
            while(rpi_spi0->CS.bit.RXD &&( RXCnt < len )) {
                rbuf[RXCnt] = rpi_spi0->FIFO.reg;
                RXCnt++;
            }
        } else {
            RXCnt = len;
        }
        if (rpi_spi0->CS.bit.DONE)
            break;
    }
    // Wait for DONE to be set
    while (rpi_spi0->CS.bit.DONE);

    // Set TA = 0
    rpi_spi0->CS.bit.TA = 0;
}
