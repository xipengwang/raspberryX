#ifdef _RPI_UART_H
#define _RPI_SPI_H

#include "rpi.h"
#include "gpio.h"

typedef union{
    struct{
    	uint32_t UARTEN:1;
	uint32_t SIREN: 1;
	uint32_t SIRLP: 1;
	uint32_t reserved: 4;
        uint32_t LBE :1;
        uint32_t TXE :1;
        uint32_t RXE :1;
        uint32_t DTR: 1;
        uint32_t RTS: 1;
        uint32_t OUT1: 1;
        uint32_t OUT2: 1;
        uint32_t RTSEN: 1;
        uint32_t CTSEN: 1;
        uint32_t reserved: 16;
    } bit;
    uint32_t reg;
} RPI_UART_CR_REG;//control register
#define RPI_UART_CR_UARTEN_Pos 0
#define RPI_UART_CR_UARTEN_Msk 0x01 << RPI_UART_CR_UARTEN_pos
#define RPI_UART_CR_SIREN_Pos 1
#define RPI_UART_CR_SIREN_Msk 0x01 << RPI_UART_CR_SIREN_pos
#define RPI_UART_CR_SIRLP_Pos 2
#define RPI_UART_CR_SIRLP_Msk 0x01 << RPI_UART_CR_SIRLP_pos
#define RPI_UART_CR_LBE_Pos 7
#define RPI_UART_CR_LBE_Msk 0x01 << RPI_UART_CR_LBE_pos
#define RPI_UART_CR_TXE_Pos 8
#define RPI_UART_CR_TXE_Msk 0x01 << RPI_UART_CR_TXE_pos
#define RPI_UART_CR_RXE_Pos 9
#define RPI_UART_CR_RXE_Msk 0x01 << RPI_UART_CR_RXE_pos
#define RPI_UART_CR_DTR_Pos 10
#define RPI_UART_CR_DTR_Msk 0x01 << RPI_UART_CR_DTR_pos
#define RPI_UART_CR_RTS_Pos 11
#define RPI_UART_CR_RTS_Msk 0x01 << RPI_UART_CR_RTS_pos
#define RPI_UART_CR_OUT1_Pos 12
#define RPI_UART_CR_OUT1_Msk 0x01 << RPI_UART_CR_OUT1_pos
#define RPI_UART_CR_OUT2_Pos 13
#define RPI_UART_CR_OUT2_Msk 0x01 << RPI_UART_CR_OUT2_pos
#define RPI_UART_CR_RTSEN_Pos 14
#define RPI_UART_CR_RTSEN_Msk 0x01 << RPI_UART_CR_RTSEN_pos
#define RPI_UART_CR_CTSEN_Pos 15
#define RPI_UART_CR_CTSEN_Msk 0x01 << RPI_UART_CR_CTSEN_pos


typedef union{
    struct{
	uint32_t CTS :1;
	uint32_t DSR :1;
	uint32_t DCD :1;
	uint32_t BUSY:1;
	uint32_t RXFE:1;
	uint32_t TXFF:1;
        uint32_t RXFF:1;
        uint32_t TXFE:1;
        uint32_t RI:1;
        uint32_t reserved: 23;
    } bit;
    uint32_t reg;
} RPI_UART_FR_REG;//flag register

typedef union{
    struct{
	uint32_t DATA :8;
	uint32_t FE :1;
	uint32_t PE :1;
	uint32_t BE :1;
	uint32_t OE :1;
	uint32_t reserved: 20;	  } bit;
    uint32_t reg;
} RPI_UART_DR_REG;//data register


typedef union{
    struct{
	uint32_t IBRD: 16;
	uint32_t reserved: 16;
    } bit;
    uint32_t reg;
} RPI_UART_IBRD_REG; //integer part baudrate divisor


typedef union{
    struct{
	uint32_t FBRD: 6;
        uint32_t reserved: 26;    } bit;
    uint32_t reg;
} RPI_UART_FBRD_REG;//fractional baud rate divisor

typedef union{
    struct{
         uint32_t r0: 32;
    } bit;
    uint32_t reg;
} RPI_UART_SHARE_REG;


typedef struct{
    
}rpi_uart_t
