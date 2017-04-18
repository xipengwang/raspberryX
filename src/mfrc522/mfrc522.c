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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/types.h>

#include "mfrc522.h"

int mfrc522_init()
{
    if(rpi_spi_init(rpi_spi0))
        return 0;
    //set spi
    rpi_spi_set_clk_divider(rpi_spi0, 4096);
    rpi_spi_chip_select(rpi_spi0, RPI_CHIP_SELECT_0);
    rpi_spi_set_chip_polarity(rpi_spi0, RPI_CHIP_SELECT_0, LOW);

    //set status as idle, cancel all running commands
    mfrc522_send_cmd(MFRC522_Cmd_Idle);

    //soft-reset
    mfrc522_send_cmd(MFRC522_Cmd_SoftReset);

    //CHECK status1 register reset value
    if(!mfrc522_check_spi_communication())
        return 0;

    //clears the internal FIFO buffer
    mfrc522_set_bit(MFRC522_FIFOLevelReg,FlushBuffer_MFRC522);

    //Set timer autostart
    mfrc522_timer_125us(4000); //500ms

    //Force100ASK to be 1
    mfrc522_write_data(MFRC522_TxASKReg,0x40);

    //clear interrupt
    mfrc522_clear_bit(MFRC522_ComIrqReg,0x80);

    //transmitter can only be started if an RF field is generated
    mfrc522_write_data(MFRC522_ModeReg,0x20);

    //Set antenna on
    uint8_t controlRegV = mfrc522_read_data(MFRC522_TxControlReg);
    mfrc522_write_data(MFRC522_TxControlReg, controlRegV | 0x03);

    printf("mfrc 522 is ready. \n");
    return 1;
}

void mfrc522_close()
{
    //set status as idle
    mfrc522_send_cmd(MFRC522_Cmd_Idle);
}

void mfrc522_send_cmd(char cmd)
{
    char buf[2] = {WRITE_CMD(MFRC522_CommandReg), cmd};
    //rpi_spi_transfernb(rpi_spi0, buf, NULL, 2);
    rpi_spi_writenb(rpi_spi0,buf,2);
}

void mfrc522_write_data(char addr, char data)
{
    char buf[2] = {WRITE_CMD(addr), data};
    //rpi_spi_transfernb(rpi_spi0, buf, NULL, 2);
    rpi_spi_writenb(rpi_spi0,buf,2);
}

void mfrc522_write_datanb(char addr, char *data, int len)
{
    for(int i=0; i<len; i++){
        mfrc522_write_data(addr,data[i]);
    }
}

uint8_t mfrc522_read_data(char addr)
{
    char in[2] = {READ_CMD(addr), 0x00};
    char out[2];
    rpi_spi_transfernb(rpi_spi0, in, out, 2);
    return out[1];
}

void mfrc522_read_datanb(char *addr, char *buf, int len)
{
    for(int i=0; i<len; i++){
        mfrc522_read_data(READ_CMD(addr[i]));
    }
}

void mfrc522_set_bit(char addr, uint8_t mask)
{
    uint8_t v = mfrc522_read_data(addr);
    v = SET_BIT(v,mask);
    mfrc522_write_data(addr,v);
}

void mfrc522_clear_bit(char addr, uint8_t mask)
{
    uint8_t v = mfrc522_read_data(addr);
    v = CLEAR_BIT(v,mask);
    mfrc522_write_data(addr,v);
}

void mfrc522_write_bits(char addr, char bitmask, char value)
{
    uint8_t v = mfrc522_read_data(addr);
    v = WRITE_BITS(v,bitmask,value);
    mfrc522_write_data(addr,v);
}

void mfrc522_timer_125us(uint16_t n)
{
    //PrescalerReg as 847: a delay time of 125 us
    mfrc522_write_data(MFRC522_TModeReg, 0x93);
    mfrc522_write_data(MFRC522_TPrescalerReg,0x4F);

    //set TRreload value as N
    mfrc522_write_data(MFRC522_TReloadRegH, (n>>8)&0xFF);
    mfrc522_write_data(MFRC522_TReloadRegL, (n&0xFF));
}

int mfrc522_check_authentication(void)
{
    //TODO: Send block data
    mfrc522_send_cmd(MFRC522_Cmd_MFAuthent);
    return 0;
}

int mfrc522_check_spi_communication(void)
{
    //
    uint8_t status1V = mfrc522_read_data(MFRC522_Status1Reg);
    if(status1V == 0x21)
        return 1;
    else{
        printf("Spi communication status1 reg: 0x%02X \n", status1V);
        return 0;
    }
}

int write_to_card(mfrc522_card_t *card_data, int MFRC522_Cmd)
{
    uint8_t irqEn = IRqInv_MFRC522 | TxIEn_MFRC522 | RxIEn_MFRC522 |
        IdleIEn_MFRC522 | ErrIEn_MFRC522 | TimerIEn_MFRC522;

    uint8_t waitIrq;
    if(MFRC522_Cmd == MFRC522_Cmd_Transceive){
        waitIrq = RxIRq_MFRC522|IdleIRq_MFRC522;
    }
    else if(MFRC522_Cmd == MFRC522_Cmd_MFAuthent){
        waitIrq = IdleIRq_MFRC522;
    }
    else{
        printf("Wrong command!\n");
        exit(-1);
    }

    mfrc522_write_data(MFRC522_ComIEnReg,irqEn);
    mfrc522_clear_bit(MFRC522_ComIrqReg,SET1_MFRC522);
    mfrc522_set_bit(MFRC522_FIFOLevelReg,FlushBuffer_MFRC522);

    mfrc522_send_cmd(MFRC522_Cmd_Idle);

    //write data
    card_data->status = MFRC522_Return_Status_FAIL;
    for(int i=0; i<card_data->len; i++){
        mfrc522_write_data(MFRC522_FIFODataReg,card_data->data[i]);
    }

    mfrc522_send_cmd(MFRC522_Cmd_Transceive);

    //starts the transmission of data
    mfrc522_set_bit(MFRC522_BitFramingReg, StartSend_MFRC522);

    int i = 2000;
    uint8_t v;
    while(1){
        v = mfrc522_read_data(MFRC522_ComIrqReg);
        i--;
        //end of loop || time out || end of command and receiver gets data
        if((i==0) || (v&TimerIRq_MFRC522) || (CHECK_BITS(v, waitIrq)))
            break;
    }
    //stop the transmission of data
    mfrc522_clear_bit(MFRC522_BitFramingReg, StartSend_MFRC522);
    if(i!=0){
        uint8_t errV = mfrc522_read_data(MFRC522_ErrorReg);
        if(CHECK_BITS(v,TimerIRq_MFRC522)){
            //clears the internal FIFO buffer
            return MFRC522_Return_Status_Timeout;
        }
        if(!(CHECK_BITS(errV,(BufferOvfl_MFRC522|CollErr_MFRC522|
                              ParityErr_MFRC522|ProtocolErr_MFRC522)) ==0x00)){
            return MFRC522_Return_Status_FAIL;
        }
        // get return data
        int len = mfrc522_read_data(MFRC522_FIFOLevelReg);
        int numofbits = 0;
        uint8_t lastBits = mfrc522_read_data(MFRC522_ControlReg);
        if(len==0)
            len=1;
        if(len>MAX_LEN)
            len=MAX_LEN;

        if(lastBits & RxLastBits_MFRC522){
            numofbits = (len-1)*8 + lastBits;
        }else{
            numofbits = len*8;
        }

        for(int i=0; i<len; i++){
            card_data->data[i] = mfrc522_read_data(MFRC522_FIFODataReg);
        }
        card_data->status = MFRC522_Return_Status_OK;
        card_data->len = len;
        card_data->numofbits = numofbits;
        return MFRC522_Return_Status_OK;
    }
    return MFRC522_Return_Status_FAIL;
}

int mfrc522_query_card(mfrc522_card_t *card_data)
{
    // request is a 7 bits command, so only transmit 7 bits of last byte.
    mfrc522_write_data(MFRC522_BitFramingReg, TxLastBits);
    card_data->len = 1;
    card_data->data[0] = PICC_REQALL;
    int status = write_to_card(card_data, MFRC522_Cmd_Transceive);
    if(card_data->numofbits!=16)
        return MFRC522_Return_Status_FAIL;
    return status;
}

int mfrc522_anticoll(mfrc522_card_t *card_data)
{
    card_data->len = 2;
    card_data->data[0] = PICC_ANTICOLL;
    card_data->data[1] = 0x20;
    mfrc522_write_data(MFRC522_BitFramingReg, 0x00);
    mfrc522_Return_Status status = write_to_card(card_data,
                                                 MFRC522_Cmd_Transceive);
    if(status== MFRC522_Return_Status_OK){
        if(card_data->len == 5){
            uint8_t check;
            for(int i=0;i<4;i++){
                check = check ^ card_data->data[i];
            }
            if(check == card_data->data[4]){
                // only the first 4 bytes are UID
                card_data->len = 4;
                card_data->numofbits = 4*8;
                return status;
            }else{
                return MFRC522_Return_Status_FAIL;
            }
        }else{
            return MFRC522_Return_Status_FAIL;
        }

    }else{
        return status;
    }


}
