#ifndef _RPI_I2C_H
#define _RPI_I2C_H
/*
  Add "dtparam=i2c_vc=on" to /boot/config.txt;
  So you can see both i2c0 and i2c1 interface.
  However, be aware that i2c0 or i2c1 may not work. Use oscilloscope to check the clock signals.
 */


typedef enum {
    RPI_I2C_OK = 0,
    RPI_I2C_ERROR_NACK,
    RPI_I2C_ERROR_CLKT,
    RPI_I2C_ERROR_DATA
} RPI_I2C_RETURN_STATUS;

typedef union {
    struct {
        uint32_t READ :1;
        uint32_t r0 :3;
        uint32_t CLEAR :2;
        uint32_t r1 :1;
        uint32_t ST :1;
        uint32_t INTD :1;
        uint32_t INTT :1;
        uint32_t INTR :1;
        uint32_t r2 :4;
        uint32_t I2CEN :1;
        uint32_t r3 :16;
    } bit;
    uint32_t reg;
} RPI_I2C_C_REG;
#define RPI_I2C_C_I2CEN_Pos 15
#define RPI_I2C_C_I2CEN_Msk 1 << RPI_I2C_C_I2CEN_Pos
#define RPI_I2C_C_INTR_Pos 10
#define RPI_I2C_C_INTR_Msk 1 << RPI_I2C_C_INTR_Pos
#define RPI_I2C_C_INTT_Pos 9
#define RPI_I2C_C_INTT_Msk 1 << RPI_I2C_C_INTT_Pos
#define RPI_I2C_C_INTD_Pos 8
#define RPI_I2C_C_INTD_Msk 1 << RPI_I2C_C_INTD_Pos
#define RPI_I2C_C_ST_Pos 7
#define RPI_I2C_C_ST_Msk 1 << RPI_I2C_C_ST_Pos
#define RPI_I2C_C_CLEAR_H_Pos 5
#define RPI_I2C_C_CLEAR_L_Pos 4
#define RPI_I2C_C_CLEAR_Msk (1 << RPI_I2C_C_CLEAR_H_Pos) | (1 << RPI_I2C_C_CLEAR_L_Pos)
#define RPI_I2C_C_READ_Pos 0
#define RPI_I2C_C_READ_Msk 1 << RPI_I2C_C_READ_Pos

typedef union {
    struct {
        uint32_t TA :1;
        uint32_t DONE :1;
        uint32_t TXW :1;
        uint32_t RXR :1;
        uint32_t TXD :1;
        uint32_t RXD :1;
        uint32_t TXE :1;
        uint32_t RXF :1;
        uint32_t ERR :1;
        uint32_t CLKT :1;
        uint32_t r0 :22;
    } bit;
    uint32_t reg;
} RPI_I2C_S_REG;
#define RPI_I2C_S_CLKT_Pos 9
#define RPI_I2C_S_CLKT_Msk 1 << RPI_I2C_S_CLKT_Pos
#define RPI_I2C_S_ERR_Pos 8
#define RPI_I2C_S_ERR_Msk 1 << RPI_I2C_S_ERR_Pos
#define RPI_I2C_S_RXF_Pos 7
#define RPI_I2C_S_RXF_Msk 1 << RPI_I2C_S_RXF_Pos
#define RPI_I2C_S_TXE_Pos 6
#define RPI_I2C_S_TXE_Msk 1 << RPI_I2C_S_TXE_Pos
#define RPI_I2C_S_RXD_Pos 5
#define RPI_I2C_S_RXD_Msk 1 << RPI_I2C_S_RXD_Pos
#define RPI_I2C_S_TXD_Pos 4
#define RPI_I2C_S_TXD_Msk 1 << RPI_I2C_S_TXD_Pos
#define RPI_I2C_S_RXR_Pos 3
#define RPI_I2C_S_RXR_Msk 1 << RPI_I2C_S_RXR_Pos
#define RPI_I2C_S_TXW_Pos 2
#define RPI_I2C_S_TXW_Msk 1 << RPI_I2C_S_TXW_Pos
#define RPI_I2C_S_DONE_Pos 1
#define RPI_I2C_S_DONE_Msk 1 << RPI_I2C_S_DONE_Pos
#define RPI_I2C_S_TA_Pos 0
#define RPI_I2C_S_TA_Msk 1 << RPI_I2C_S_TA_Pos

typedef union {
    struct {
        uint32_t DLEN :16;
        uint32_t r0 :16;
    } bit;
    uint32_t reg;
} RPI_I2C_DLEN_REG;

typedef union {
    struct {
        uint32_t ADDR :7;
        uint32_t r0 :25;
    } bit;
    uint32_t reg;
} RPI_I2C_A_REG;

typedef union {
    struct {
        uint32_t DATA :8;
        uint32_t r0 :16;
    } bit;
    uint32_t reg;
} RPI_I2C_FIFO_REG;

typedef union {
    struct {
        uint32_t CDIV :16;
        uint32_t r0 :16;
    } bit;
    uint32_t reg;
} RPI_I2C_DIV_REG;

typedef union {
    struct {
        uint32_t REDL :16;
        uint32_t FEDL :16;
    } bit;
    uint32_t reg;
} RPI_I2C_DEL_REG;

typedef union {
    struct {
        uint32_t TOUT :16;
        uint32_t r0 :16;
    } bit;
    uint32_t reg;
} RPI_I2C_CLKT_REG;

typedef struct {
    __IO RPI_I2C_C_REG C;
    __IO RPI_I2C_S_REG S;
    __IO RPI_I2C_DLEN_REG DLEN;
    __IO RPI_I2C_A_REG A;
    __IO RPI_I2C_FIFO_REG FIFO;
    __IO RPI_I2C_DIV_REG DIV;
    __IO RPI_I2C_DEL_REG DEL;
    __IO RPI_I2C_CLKT_REG CLKT;
} rpi_i2c_t;

extern volatile rpi_i2c_t *rpi_i2c0;
extern volatile rpi_i2c_t *rpi_i2c1;

#ifdef __cplusplus
extern "C" {
#endif

    /**
     * Initialize i2c.
     * @param i2c A rpi_i2c_t pointer. It will be one of [rpi_i2c0, rpi_i2c1, rpi_i2c2].
     * @return Return 0 on success
     */
    int rpi_i2c_init(volatile rpi_i2c_t *rpi_i2c);
    void rpi_i2c_close(Rpi_Gpio_Pin SDA_PIN, Rpi_Gpio_Pin SCL_PIN);
    void rpi_i2c_setslave(volatile rpi_i2c_t *i2c, uint8_t addr);
    void rpi_i2c_setclockdivider(volatile rpi_i2c_t *i2c, uint16_t divider);
    void rpi_i2c_set_baudrate(volatile rpi_i2c_t *i2c, uint32_t baudrate);
    RPI_I2C_RETURN_STATUS rpi_i2c_read(volatile rpi_i2c_t *i2c, char* buf, uint32_t len);
    RPI_I2C_RETURN_STATUS rpi_i2c_write(volatile rpi_i2c_t *i2c, const char * buf, uint32_t len);

#ifdef __cplusplus
}
#endif

#endif
