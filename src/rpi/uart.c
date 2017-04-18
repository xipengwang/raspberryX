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
    uint32_t dr;
    if(!rpi_uart->FR.bit.RXFE) {
        dr = rpi_uart->DR.reg;
        c = 0xFF & dr;
        if (dr & RPI_UART_DR_FE_Msk) {
            printf("ALERT: frame error\n");
        }
        if (dr & RPI_UART_DR_PE_Msk) {
            printf("ALERT: parity error\n");
        }
        if (dr & RPI_UART_DR_BE_Msk) {
            printf("ALERT: break error\n");
        }
        if (dr & RPI_UART_DR_OE_Msk) {
            printf("ALERT Overrun error\n");
        }
    }
    return c;
}

int rpi_uart_receive(volatile rpi_uart_t* rpi_uart, char* rbuf, uint32_t len)
{
    uint32_t RXCnt = 0;
    uint32_t dr;
    while(RXCnt < len && !rpi_uart->FR.bit.RXFE){
        dr = rpi_uart->DR.reg;
        rbuf[RXCnt] = 0xFF & dr;
        RXCnt++;
        if (dr & RPI_UART_DR_FE_Msk) {
            printf("ALERT: frame error\n");
        }
        if (dr & RPI_UART_DR_PE_Msk) {
            printf("ALERT: parity error\n");
        }
        if (dr & RPI_UART_DR_BE_Msk) {
            printf("ALERT: break error\n");
        }
        if (dr & RPI_UART_DR_OE_Msk) {
            printf("ALERT Overrun error\n");
        }
    }
    while(!rpi_uart->FR.bit.RXFE) {
        rbuf[RXCnt] = rpi_uart->DR.bit.DATA;
        printf("### ALERT: buffer remaining chars:%c ###\n", rbuf[RXCnt]);
    }
    return RXCnt;
}
