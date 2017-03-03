#ifndef _RPI_SPI_H
#define _RPI_SPI_H

#include "rpi.h"
#include "gpio.h"

typedef struct {
    uint32_t CS;
    uint32_t FIFO;
    uint32_t CLK;
    uint32_t DLEN;
    uint32_t LTOH;
    uint32_t DC;
} rpi_spi_t;

extern volatile rpi_spi_t *rpi_spi0;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
