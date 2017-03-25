#include "uart.h"

volatile rpi_uart_t *rpi_uart;

int rpi_uart_init(volatile rpi_uart_t *rpi_uart){

    if(rpi_uart == MAP_FAILED){
	return -1;
    }
    //disable UART
    rpi_uart->CR.reg =0;
    //wait for the end of tx/rx of current char
    //flush trans FIFO  UART_LCRH.FEN =0
    rpi_uart->LCRH.bit.FEN=0; 
    //transmit enable
    rpi_uart->CR.bit.TXE = 1;
    //receive enable
    rpi_uart->CR.bit.RXE = 1;
    //request to send
    rpi_uart->CR.bit.RTS = 1;
    //data is requested when there is space in the receive FIFO
    rpi_uart->CR.bit.RTSEN = 1;
    
    rpi_uart->CR.bit.CTSEN = 1;

    //enable FIFO 
    rpi_uart->LCRH.bit.FEN =1;
    //enable parity
    rpi_uart->LCRH.bit.PEN =1;
    //enable uart
    rpi_uart->CR.bit.UARTEN =1;
    return 0;
}

void rpi_uart_set_clock(volatile rpi_uart_t* rpi_uart, uint16_t _divider){
    uint16_t divider = _divider;
    if(divider & (divider -1)) {
      //not a power of two
      divider = 2;
      while (_divider < divider){
	   divider *=2;
      }
    }
    rpi_uart->IBRD.bit.IBRD = divider;
}


void rpi_uart_transmit(volatile rpi_uart_t* rpi_uart, char* tbuf, uint32_t len){
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

void rpi_uart_receive(volatile rpi_uart_t* rpi_uart, char* rbuf, uint32_t len){
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



  
