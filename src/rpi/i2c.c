#include "i2c.h"

volatile rpi_i2c_t *rpi_i2c0;
volatile rpi_i2c_t *rpi_i2c1;

int rpi_i2c_init(volatile rpi_i2c_t *i2c)
{
    if (i2c == MAP_FAILED)
        return -1;

    return 0;
}

void rpi_i2c_close(Rpi_Gpio_Pin SDA_PIN, Rpi_Gpio_Pin SCL_PIN)
{
    rpi_gpio_fsel(SDA_PIN, RPI_GPIO_FSEL_IN); /* SDA */
    rpi_gpio_fsel(SCL_PIN, RPI_GPIO_FSEL_IN); /* SCL */
}

void rpi_i2c_setslave(volatile rpi_i2c_t *i2c, uint8_t addr)
{
    i2c->A.reg = addr;
}

// SCL = core clock / DIV
// The reset value is 0x5dc which is 1500.
// core clock = 2500kHz, which results in SCL reset value as 166khz.
void rpi_i2c_setclockdivider(volatile rpi_i2c_t *i2c, uint16_t divider)
{
    i2c->DIV.reg = divider;
}

void rpi_i2c_set_baudrate(volatile rpi_i2c_t *i2c, uint32_t baudrate)
{
	uint16_t divider;
    //round down any odd number
	divider = (RPI_CORE_CLK_HZ / baudrate) & 0xFFFE;
	rpi_i2c_setclockdivider(i2c, divider);
}

RPI_I2C_RETURN_STATUS rpi_i2c_read(volatile rpi_i2c_t *i2c, char* buf, uint32_t len)
{
    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = RPI_I2C_OK;

    // Clear FIFO
    mem_set_bits_32((uint32_t*)&(i2c->C), RPI_I2C_C_CLEAR_Msk, RPI_I2C_C_CLEAR_Msk);
    // Clear Status
    mem_set_bits_32((uint32_t*)&(i2c->S),
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk,
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk);
    // Set Data Length
    i2c->DLEN.reg = len;
    // Start read
    i2c->C.reg = RPI_I2C_C_I2CEN_Msk | RPI_I2C_C_ST_Msk | RPI_I2C_C_READ_Msk;

    while (!(i2c->S.bit.DONE)) {
        while (i2c->S.bit.RXD) {
            // Read from FIFO
            buf[i] = i2c->FIFO.reg;
            i++;
            remaining--;
    	}
    }

    // transfer has finished - grab any remaining stuff in FIFO
    while (remaining && i2c->S.bit.RXD) {
        /* Read from FIFO, no barrier */
        buf[i] = i2c->FIFO.reg;
        i++;
        remaining--;
    }

    if (i2c->S.bit.ERR) {
        // Received a NACK
        reason = RPI_I2C_ERROR_NACK;
    } else if (i2c->S.bit.CLKT) {
        // Received Clock Stretch Timeout
        reason = RPI_I2C_ERROR_CLKT;
    } else if (remaining) {
        // Not all data is received
        reason = RPI_I2C_ERROR_DATA;
    }
    mem_set_bits_32((uint32_t*)&(i2c->S),
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk,
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk);
    return reason;
}

RPI_I2C_RETURN_STATUS rpi_i2c_write(volatile rpi_i2c_t *i2c, const char * buf, uint32_t len)
{
    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = RPI_I2C_OK;

    // Clear FIFO
    mem_set_bits_32((uint32_t*)&(i2c->C), RPI_I2C_C_CLEAR_Msk, RPI_I2C_C_CLEAR_Msk);
    // Clear Status
    mem_set_bits_32((uint32_t*)&(i2c->S),
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk,
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk);
    // Set Data Length
    i2c->DLEN.reg = len;
    // pre-populate FIFO with max buffer; 16-byte FIFO
    while( remaining && ( i < 16 ) ) {
        i2c->FIFO.bit.DATA = buf[i];
        i++;
        remaining--;
    }
    // Start write
    i2c->C.reg =  RPI_I2C_C_I2CEN_Msk | RPI_I2C_C_ST_Msk;

    // Transfer is over
    while(!(i2c->S.bit.DONE)) {
        while ( remaining && i2c->S.bit.TXD) {
            // Write to FIFO
            i2c->FIFO.bit.DATA = buf[i];
            i++;
            remaining--;
    	}
    }

    if (i2c->S.bit.ERR) {
        // Received a NACK
        reason = RPI_I2C_ERROR_NACK;
    } else if (i2c->S.bit.CLKT) {
        // Received Clock Stretch Timeout
        reason = RPI_I2C_ERROR_CLKT;
    } else if (remaining) {
        // Not all data is received
        reason = RPI_I2C_ERROR_DATA;
        // Clear FIFO
        mem_set_bits_32((uint32_t*)&(i2c->C), RPI_I2C_C_CLEAR_Msk, RPI_I2C_C_CLEAR_Msk);
    }
    // Clear Status
    mem_set_bits_32((uint32_t*)&(i2c->S),
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk,
                    RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk);
    return reason;
}
