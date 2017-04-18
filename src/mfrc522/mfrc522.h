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

#ifndef _MFRC522_H
#define _MFRC522_H

#include <stdint.h>
#include "rpi/rpi_all.h"

/** Define MFRC522 registers
  */

#define MFRC522_CommandReg       0x01
#define MFRC522_ComIEnReg        0x02
#define MFRC522_DivlEnReg        0x03
#define MFRC522_ComIrqReg        0x04
#define MFRC522_DivIrqReg        0x05
#define MFRC522_ErrorReg         0x06
#define MFRC522_Status1Reg       0x07
#define MFRC522_Status2Reg       0x08
#define MFRC522_FIFODataReg      0x09
#define MFRC522_FIFOLevelReg     0x0A
#define MFRC522_WaterLevelReg    0x0B
#define MFRC522_ControlReg       0x0C
#define MFRC522_BitFramingReg    0x0D
#define MFRC522_CollReg          0x0E
#define MFRC522_ModeReg          0x11
#define MFRC522_TxModeReg        0x12
#define MFRC522_RxModeReg        0x13
#define MFRC522_TxControlReg     0x14
#define MFRC522_TxASKReg         0x15
#define MFRC522_TxSelReg         0x16
#define MFRC522_RxSelReg         0x17
#define MFRC522_RxThresholdReg   0x18
#define MFRC522_DemodReg         0x19
#define MFRC522_MfTxReg          0x1C
#define MFRC522_MfRxReg          0x1D
#define MFRC522_SerialSpeedReg   0x1F
#define MFRC522_CRCResultRegH    0x21 //2 bytes CRCresult
#define MFRC522_CRCResultRegL    0x22
#define MFRC522_ModWidthReg      0x24
#define MFRC522_RFCfgReg         0x26
#define MFRC522_GsNReg           0x27
#define MFRC522_CWGsPReg         0x28
#define MFRC522_ModGsPReg        0x29
#define MFRC522_TModeReg         0x2A
#define MFRC522_TPrescalerReg    0x2B
#define MFRC522_TReloadRegH      0x2C //2 bytes
#define MFRC522_TReloadRegL      0x2D
#define MFRC522_TCounterValRegH  0x2E //2 bytes
#define MFRC522_TCounterValRegL  0x2F

/** Define MFRC522 commands
 */
#define MFRC522_Cmd_Idle                   0x00
#define MFRC522_Cmd_Mem                    0x01
#define MFRC522_Cmd_GenerateRandomID       0x02
#define MFRC522_Cmd_CalcCRC                0x03
#define MFRC522_Cmd_Transmit               0x04
#define MFRC522_Cmd_NoCmdChange            0x07
#define MFRC522_Cmd_Revceive               0x08
#define MFRC522_Cmd_Transceive             0x0C
#define MFRC522_Cmd_MFAuthent              0x0E
#define MFRC522_Cmd_SoftReset              0x0F


//Mifare_One
#define PICC_REQIDL           0x26
#define PICC_REQALL           0x52
#define PICC_ANTICOLL         0x93 //+20h
#define PICC_SElECTTAG        0x93 //70h
#define PICC_AUTHENT1A        0x60
#define PICC_AUTHENT1B        0x61
#define PICC_READ             0x30
#define PICC_WRITE            0xA0
#define PICC_DECREMENT        0xC0
#define PICC_INCREMENT        0xC1
#define PICC_RESTORE          0xC2
#define PICC_TRANSFER         0xB0
#define PICC_HALT             0x50 //+00h

#define MFRC522_VERSION 10000 /* Version 1.00 */
#define MAX_LEN 16

#define WRITE_CMD(cmd)          (0x7F & (cmd<<1))
#define READ_CMD(cmd)           (0x80 | (cmd<<1))

#define SET_BIT(x,mask)         (x | mask)
#define CLEAR_BIT(x,mask)       (x & ~mask)

#define CHECK_BITS(x,mask)      (x & mask)
#define WRITE_BITS(x,mask,v)    ((x & ~mask) | (v & mask))

typedef enum{
    MFRC522_Return_Status_OK   = 0,
    MFRC522_Return_Status_FAIL = 1,
    MFRC522_Return_Status_NOTAuthenticated = 2,
    MFRC522_Return_Status_Timeout = 2,

}mfrc522_Return_Status;

typedef enum{
    WrErr_MFRC522 =       BITMASK_7,
    TempErr_MFRC522 =     BITMASK_6,
    BufferOvfl_MFRC522 =  BITMASK_4,
    CollErr_MFRC522 =     BITMASK_3,
    CRCErr_MFRC522 =      BITMASK_2,
    ParityErr_MFRC522 =   BITMASK_1,
    ProtocolErr_MFRC522 = BITMASK_0,
}mfrc522_ErrorMasks;

typedef enum{
    CRCOk_MFRC522 =     BITMASK_6,
    CRCReady_MFRC522 =  BITMASK_5,
    IRq_MFRC522 =       BITMASK_4,
    TRunning_MFRC522 =  BITMASK_3,
    HiAlert_MFRC522 =   BITMASK_1,
    LoAlert_MFRC522 =   BITMASK_0,
}mfrc522_Status1Masks;

typedef enum{
    MFCrypto1On = BITMASK_3,
}mfrc522_Status2Masks;


typedef enum{
    TStopNow_MFRC522 = BITMASK_7,
    TStartNow_MFRC522 = BITMASK_6,
    RxLastBits_MFRC522 = BITMASK_2 | BITMASK_1 | BITMASK_0,
}mfrc522_ControlRegMasks;

typedef enum{
    IRqInv_MFRC522 = BITMASK_7,
    TxIEn_MFRC522 = BITMASK_6,
    RxIEn_MFRC522 = BITMASK_5,
    IdleIEn_MFRC522 = BITMASK_4,
    HiAlertIEn_MFRC522 = BITMASK_3,
    LoAlertIEn_MFRC522 = BITMASK_2,
    ErrIEn_MFRC522 = BITMASK_1,
    TimerIEn_MFRC522 = BITMASK_0,
}mfrc522_ComIEnRegMasks;

typedef enum{
    SET1_MFRC522 = BITMASK_7,
    TxIRq_MFRC522 = BITMASK_6,
    RxIRq_MFRC522 = BITMASK_5,
    IdleIRq_MFRC522 = BITMASK_4,
    HiAlertIRq_MFRC522 = BITMASK_3,
    LoAlertIRq_MFRC522 = BITMASK_2,
    ErrIRq_MFRC522 = BITMASK_1,
    TimerIRq_MFRC522 = BITMASK_0,
}mfrc522_ComIrqRegMasks;

typedef enum{
    FlushBuffer_MFRC522 = BITMASK_7,
    FIFOLevel_MFRC522 = 0x7F,
}mfrc522_FIFOLevelRegMasks;

typedef enum{
    StartSend_MFRC522 = BITMASK_7,
    RxAlign_MFRC522 = BITMASK_6 | BITMASK_5 | BITMASK_4,
    TxLastBits = BITMASK_2 | BITMASK_1 | BITMASK_0,
}mfrc522_BitFramingRegMasks;

struct mfrc522_card{
    mfrc522_Return_Status status;
    int len;
    int numofbits;
    char data[MAX_LEN];
};
typedef struct mfrc522_card mfrc522_card_t;

#ifdef __cplusplus
extern "C" {
#endif

    inline unsigned int mfrc522_version(void)
    {
        return MFRC522_VERSION;
    }

    void mfrc522_send_cmd(char cmd);
    void mfrc522_write_data(char addr, char data);
    void mfrc522_write_datanb(char addr, char *data, int len);
    uint8_t mfrc522_read_data(char addr);
    void mfrc522_read_datanb(char *addr, char *buf, int len);

    void mfrc522_timer_125us(uint16_t n);
    int mfrc522_check_authentication(void);

    void mfrc522_set_bit(char addr, uint8_t mask);
    void mfrc522_clear_bit(char addr, uint8_t mask);
    void mfrc522_write_bits(char addr, char mask, char value);

    int mfrc522_query_card(mfrc522_card_t *card_data);
    int mfrc522_anticoll(mfrc522_card_t *card_data);
    int mfrc522_check_spi_communication(void);

    // successful return 1, else 0
    int mfrc522_init();
    void mfrc522_close();



#ifdef __cplusplus
}
#endif

#endif
