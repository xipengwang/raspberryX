#include "uart.h"

volatile rpi_uart_t *rpi_uart;

int rpi_uart_init(volatile rpi_uart_t *rpi_uart)
{

    if(rpi_uart == MAP_FAILED){
        return -1;
    }
    //disable UART
    rpi_uart->CR.bit.UARTEN = 0;

    //flush trans FIFO  UART_LCRH.FEN =0
    rpi_uart->LCRH.bit.FEN = 0;
    //one stop bit
    rpi_uart->LCRH.bit.STP2 = 0;
    //disable parity
    rpi_uart->LCRH.bit.PEN = 0;
    //transmit disable
    rpi_uart->CR.bit.TXE = 0;
    //receive disable
    rpi_uart->CR.bit.RXE = 0;

    return 0;
}

void rpi_uart_set_clock(volatile rpi_uart_t* rpi_uart, uint16_t divider)
{
    rpi_uart->IBRD.reg = divider;
}

void rpi_uart_transmit(volatile rpi_uart_t* rpi_uart, char* tbuf, uint32_t len)
{
    uint32_t TXCnt =0;

    while (TXCnt < len){

        if(!rpi_uart->FR.bit.TXFF){
            rpi_uart->DR.bit.DATA = tbuf[TXCnt];
            TXCnt++;
            if(rpi_uart->DR.bit.FE){
                printf("frame error\n");
            }
            if(rpi_uart->DR.bit.PE){
                printf("parity error\n");
            }
            if(rpi_uart->DR.bit.BE){
                printf("break error\n");
            }
            if(rpi_uart->DR.bit.OE){
                printf("Overrun error\n");
            }
        }
    }
}

void rpi_uart_receive(volatile rpi_uart_t* rpi_uart, char* rbuf, uint32_t len)
{
    uint32_t RXCnt = 0;

    while(RXCnt < len){
        if(!rpi_uart->FR.bit.RXFF){
            rbuf[RXCnt] = rpi_uart->DR.bit.DATA;
            RXCnt++;
            if(rpi_uart->DR.bit.FE){
                printf("frame error\n");
            }
            if(rpi_uart->DR.bit.PE){
                printf("parity error\n");
            }
            if(rpi_uart->DR.bit.BE){
                printf("break error\n");
            }
            if(rpi_uart->DR.bit.OE){
                printf("Overrun error\n");
            }
        }
    }
}
