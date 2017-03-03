#include "i2c.h"

volatile rpi_i2c_t *rpi_i2c0;
volatile rpi_i2c_t *rpi_i2c1;
volatile rpi_i2c_t *rpi_i2c2;

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
    i2c->A = addr;
}

// SCL = core clock / DIV
// The reset value is 0x5dc which is 1500.
// core clock = 2500kHz, which results in SCL reset value as 166khz.
void rpi_i2c_setclockdivider(volatile rpi_i2c_t *i2c, uint16_t divider)
{
    i2c->DIV = divider;
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
    mem_set_bits_32((uint32_t*)&i2c->C, RPI_I2C_C_CLEAR, RPI_I2C_C_CLEAR);
    // Clear Status
    mem_set_bits_32((uint32_t*)&i2c->S, RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT,
                    RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT);
    // Set Data Length
    i2c->DLEN = len;
    // Start read
    mem_write_32_nb((uint32_t*)&i2c->C, RPI_I2C_C_I2CEN | RPI_I2C_C_ST | RPI_I2C_C_READ);

    while (!(i2c->S & RPI_I2C_S_DONE)) {
        while (i2c->S & RPI_I2C_S_RXD) {
            // Read from FIFO
            buf[i] = i2c->FIFO;
            i++;
            remaining--;
    	}
    }

    // transfer has finished - grab any remaining stuff in FIFO
    while (remaining && i2c->S & RPI_I2C_S_RXD) {
        /* Read from FIFO, no barrier */
        buf[i] = i2c->FIFO;
        i++;
        remaining--;
    }

    if (i2c->S & RPI_I2C_S_ERR) {
        // Received a NACK
        reason = RPI_I2C_ERROR_NACK;
    } else if (i2c->S & RPI_I2C_S_CLKT) {
        // Received Clock Stretch Timeout
        reason = RPI_I2C_ERROR_CLKT;
    } else if (remaining) {
        // Not all data is received
        reason = RPI_I2C_ERROR_DATA;
    }
    mem_set_bits_32((uint32_t*)&i2c->S, RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT,
                    RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT);
    return reason;
}

RPI_I2C_RETURN_STATUS rpi_i2c_write(volatile rpi_i2c_t *i2c, const char * buf, uint32_t len)
{
    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = RPI_I2C_OK;

    // Clear FIFO
    mem_set_bits_32((uint32_t*)&i2c->C, RPI_I2C_C_CLEAR, RPI_I2C_C_CLEAR);
    // Clear Status
    mem_set_bits_32((uint32_t*)&i2c->S, RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT,
                    RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT);
    // Set Data Length
    i2c->DLEN = len;
    // pre-populate FIFO with max buffer; 16-byte FIFO
    while( remaining && ( i < 16 ) ) {
        mem_write_32_nb((uint32_t*)&i2c->FIFO, (uint32_t)buf[i]);
        i++;
        remaining--;
    }
    // Start write
    mem_write_32_nb((uint32_t*)&i2c->C, RPI_I2C_C_I2CEN | RPI_I2C_C_ST);

    // Transfer is over
    while(!(i2c->S & RPI_I2C_S_DONE )) {
        while ( remaining && (i2c->S & RPI_I2C_S_TXD )) {
            // Write to FIFO
            mem_write_32_nb((uint32_t*)&i2c->FIFO, (uint32_t)buf[i]);
            i++;
            remaining--;
    	}
    }

    if (i2c->S & RPI_I2C_S_ERR) {
        // Received a NACK
        reason = RPI_I2C_ERROR_NACK;
    } else if (i2c->S & RPI_I2C_S_CLKT) {
        // Received Clock Stretch Timeout
        reason = RPI_I2C_ERROR_CLKT;
    } else if (remaining) {
        // Not all data is received
        reason = RPI_I2C_ERROR_DATA;
        // Clear FIFO
        mem_set_bits_32((uint32_t*)&i2c->C, RPI_I2C_C_CLEAR, RPI_I2C_C_CLEAR);
    }

    // Clear Status
    mem_set_bits_32((uint32_t*)&i2c->S, RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT,
                    RPI_I2C_S_DONE | RPI_I2C_S_ERR | RPI_I2C_S_CLKT);
    return reason;
}