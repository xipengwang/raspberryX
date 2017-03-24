#ifndef _RPI_SPI_H
#define _RPI_SPI_H

#include "rpi.h"
#include "gpio.h"

typedef union {
    struct {
        uint32_t CS :2;
        uint32_t CPHA :1;
        uint32_t CPOL :1;
        uint32_t CLEAR :2;
        uint32_t CSPOL :1;
        uint32_t TA :1;
        uint32_t DMAEN :1;
        uint32_t INTD :1;
        uint32_t INTR :1;
        uint32_t ADCS :1;
        uint32_t REN :1;
        uint32_t LEN :1;
        uint32_t LMONO :1;
        uint32_t TE_EN :1;
        uint32_t DONE :1;
        uint32_t RXD :1;
        uint32_t TXD :1;
        uint32_t RXR :1;
        uint32_t RXF :1;
        uint32_t CSPOL0 :1;
        uint32_t CSPOL1 :1;
        uint32_t CSPOL2 :1;
        uint32_t DMA_LEN :1;
        uint32_t LEN_LONG :16;
        uint32_t r0 :6;
    } bit;
    uint32_t reg;
} RPI_SPI_CS_REG;

#define RPI_SPI_CS_CS_Pos 0
#define RPI_SPI_CS_CS_Msk 0x03 << RPI_SPI_CS_CS_Pos
#define RPI_SPI_CS_CPHA_Pos 2
#define RPI_SPI_CS_CPHA_Msk 0x01 << RPI_SPI_CS_CPHA_Pos
#define RPI_SPI_CS_CPOL_Pos 3
#define RPI_SPI_CS_CPOL_Msk 0x01 << RPI_SPI_CS_CPOL_Pos
#define RPI_SPI_CS_CLEAR_TX_Pos 4
#define RPI_SPI_CS_CLEAR_TX_Msk 0x01 << RPI_SPI_CS_CLEAR_TX_Pos
#define RPI_SPI_CS_CLEAR_RX_Pos 5
#define RPI_SPI_CS_CLEAR_RX_Msk 0x01 << RPI_SPI_CS_CLEAR_RX_Pos
#define RPI_SPI_CS_CSPOL_Pos 6
#define RPI_SPI_CS_CSPOL_Msk 0x01 << RPI_SPI_CS_CSPOL_Pos
#define RPI_SPI_CS_INTD_Pos 9
#define RPI_SPI_CS_INTD_Msk 0x01 << RPI_SPI_CS_INTD_Pos
#define RPI_SPI_CS_INTR_Pos 10
#define RPI_SPI_CS_INTR_Msk 0x01 << RPI_SPI_CS_INTR_Pos
#define RPI_SPI_CS_LEN_LONG_Pos 25
#define RPI_SPI_CS_LEN_LONG_Msk 0x01 << RPI_SPI_CS_LEN_LONG_Pos


typedef union {
    struct {
        uint32_t DATA :32;
    } bit;
    uint32_t reg;
} RPI_SPI_FIFO_REG;

typedef union {
    struct {
        uint32_t CDIV :16;
        uint32_t r0 :16;
    } bit;
    uint32_t reg;
} RPI_SPI_CLK_REG;

//Core clock 250MHz
typedef enum
{
    RPI_SPI_CLOCK_DIVIDER_65536 = 0, //3.814697260kHz, same as divider 4096
    RPI_SPI_CLOCK_DIVIDER_1024 = 0 //224.140625kHz, same as divider 4096
} Rpi_Spi_Clkdiv;

typedef union {
    struct {
        uint32_t LEN :16;
        uint32_t r0 :16;
    } bit;
    uint32_t reg;
} RPI_SPI_DLEN_REG;

typedef union {
    struct {
        uint32_t TOH :4;
        uint32_t r0 :28;
    } bit;
    uint32_t reg;
} RPI_SPI_LTOH_REG;

typedef union {
    struct {
        uint32_t TDREQ  :8;
        uint32_t TPANIC :8;
        uint32_t RDREQ  :8;
        uint32_t RPANIC :8;
    } bit;
    uint32_t reg;
} RPI_SPI_DC_REG;

typedef struct {
    __IO RPI_SPI_CS_REG CS;
    __IO RPI_SPI_FIFO_REG FIFO;
    __IO RPI_SPI_CLK_REG CLK;
    __IO RPI_SPI_DLEN_REG DLEN;
    __IO RPI_SPI_LTOH_REG LTOH;
    __IO RPI_SPI_DC_REG DC;
} rpi_spi_t;

extern volatile rpi_spi_t *rpi_spi0;

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif
