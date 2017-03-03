#ifndef _RPI_I2C_H
#define _RPI_I2C_H

#include "rpi.h"
#include "gpio.h"

typedef enum {
    RPI_I2C_C_I2CEN = 1 << 15,
    RPI_I2C_C_INTR = 1 << 10,
    RPI_I2C_C_INTT = 1 << 9,
    RPI_I2C_C_INTD = 1 << 8,
    RPI_I2C_C_ST = 1 << 7,
    RPI_I2C_C_CLEAR = 1 << 5 | 1 << 4,
    RPI_I2C_C_READ = 1
} RPI_I2C_C;

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

typedef enum {
    RPI_I2C_OK = 0,
    RPI_I2C_ERROR_NACK,
    RPI_I2C_ERROR_CLKT,
    RPI_I2C_ERROR_DATA
} RPI_I2C_RETURN_STATUS;

typedef struct {
    uint32_t C;
    uint32_t S;
    uint32_t DLEN;
    uint32_t A;
    uint32_t FIFO;
    uint32_t DIV;
    uint32_t DEL;
    uint32_t CLKT;
} rpi_i2c_t;

extern volatile rpi_i2c_t *rpi_i2c0;
extern volatile rpi_i2c_t *rpi_i2c1;
extern volatile rpi_i2c_t *rpi_i2c2;

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
