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
    //These bits indicate the
    //number of data bits transmitted or received
    rpi_uart->LCRH.bit.WLEN = 0x03; // 8bits data
    //enable FIFO
    rpi_uart->LCRH.bit.FEN = 1;
    //transmit enable
    rpi_uart->CR.bit.TXE = 1;
    //receive enable
    rpi_uart->CR.bit.RXE = 1;
    //CTS
    rpi_uart->FR.bit.CTS = 1;
    //RTS
    rpi_uart->CR.bit.RTS = 1;
    //set freq
    rpi_uart_set_baudrate(rpi_uart, RPI_UART_BAUD_RATE_9600);
    //enable uart
    rpi_uart->CR.bit.UARTEN = 1;
    return 0;
}

void rpi_uart_set_baudrate(volatile rpi_uart_t* rpi_uart, uint16_t baudrate)
{
    rpi_uart->CR.bit.UARTEN = 0;
    if (baudrate == RPI_UART_BAUD_RATE_9600) {
        rpi_uart->IBRD.reg = 325;
    } else if (baudrate == RPI_UART_BAUD_RATE_11520) {
        rpi_uart->IBRD.reg = 271;
    } else {
        //50MHz / (16 * baudrate)
        rpi_uart->IBRD.reg = (50000000 >> 4) / baudrate;
    }
    rpi_uart->FBRD.reg = 0;
    rpi_uart->CR.bit.UARTEN = 1;
}

void rpi_uart_putc(volatile rpi_uart_t* rpi_uart, char c)
{
    while(rpi_uart->FR.bit.TXFF);
    rpi_uart->DR.reg = c;
    while(rpi_uart->FR.bit.BUSY);
}

void rpi_uart_transmit(volatile rpi_uart_t* rpi_uart, char* tbuf, uint32_t len)
{
    uint32_t TXCnt =0;

    while (TXCnt < len){
        if(!rpi_uart->FR.bit.TXFF){
            rpi_uart->DR.reg = tbuf[TXCnt];
            TXCnt++;
        }
    }
    while(rpi_uart->FR.bit.BUSY);
}

char rpi_uart_getc(volatile rpi_uart_t* rpi_uart)
{
    char c = '\0';
    if(!rpi_uart->FR.bit.RXFE) {
         c = rpi_uart->DR.bit.DATA;
        /* if (rpi_uart->DR.bit.FE) { */
        /*     printf("frame error\n"); */
        /* } */
        /* if (rpi_uart->DR.bit.PE) { */
        /*     printf("parity error\n"); */
        /* } */
        /* if (rpi_uart->DR.bit.BE) { */
        /*     printf("break error\n"); */
        /* } */
        /* if (rpi_uart->DR.bit.OE) { */
        /*     printf("Overrun error\n"); */
        /* } */
    }
    return c;
}

int rpi_uart_receive(volatile rpi_uart_t* rpi_uart, char* rbuf, uint32_t len)
{
    uint32_t RXCnt = 0;
    while(RXCnt < len && !rpi_uart->FR.bit.RXFE){
        rbuf[RXCnt] = rpi_uart->DR.bit.DATA;
        RXCnt++;
        /* if (rpi_uart->DR.bit.FE) { */
        /*     printf("frame error\n"); */
        /* } */
        /* if (rpi_uart->DR.bit.PE) { */
        /*     printf("parity error\n"); */
        /* } */
        /* if (rpi_uart->DR.bit.BE) { */
        /*     printf("break error\n"); */
        /* } */
        /* if (rpi_uart->DR.bit.OE) { */
        /*     printf("Overrun error\n"); */
        /* } */
    }
    while(!rpi_uart->FR.bit.RXFE) {
        rbuf[RXCnt] = rpi_uart->DR.bit.DATA;
        printf("ALERT: buffer remaining chars:%c \n", rbuf[RXCnt]);
    }
    return RXCnt;
}
