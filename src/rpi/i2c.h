#ifndef _RPI_I2C_H
#define _RPI_I2C_H

#include "rpi.h"
#include "gpio.h"

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
typedef enum {
    RPI_I2C_C_I2CEN = 1 << 15,
    RPI_I2C_C_INTR = 1 << 10,
    RPI_I2C_C_INTT = 1 << 9,
    RPI_I2C_C_INTD = 1 << 8,
    RPI_I2C_C_ST = 1 << 7,
    RPI_I2C_C_CLEAR = 1 << 5 | 1 << 4,
    RPI_I2C_C_READ = 1
} RPI_I2C_C;

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
typedef enum {
    RPI_I2C_S_CLKT = 1 << 9,
    RPI_I2C_S_ERR = 1 << 8,
    RPI_I2C_S_RXF = 1 << 7,
    RPI_I2C_S_TXE = 1 << 6,
    RPI_I2C_S_RXD = 1 << 5,
    RPI_I2C_S_TXD = 1 << 4,
    RPI_I2C_S_RXR = 1 << 3,
    RPI_I2C_S_TXW = 1 << 2,
    RPI_I2C_S_DONE = 1 << 1,
    RPI_I2C_S_TA = 1
} RPI_I2C_S;

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
        uint32_t r0 :24;
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
