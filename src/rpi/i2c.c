#include "rpi.h"
#include "gpio.h"
#include "i2c.h"

volatile rpi_i2c_t *rpi_i2c0;
volatile rpi_i2c_t *rpi_i2c1;

int rpi_i2c_init(volatile rpi_i2c_t *i2c)
{
    if (i2c == MAP_FAILED) {
        return -1;
    }
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
// core clock = 250MHz, which results in SCL reset value as 166khz.
void rpi_i2c_setclockdivider(volatile rpi_i2c_t *i2c, uint16_t _divider)
{
    uint16_t divider = _divider;
    i2c->DIV.reg = divider;
    /*
      Note: Care must be taken in choosing values for FEDL and REDL
      as it is possible to cause the BSC master to malfunction by
      setting values of CDIV/2 or greater.
      Therefore the delay values should always be set to less than CDIV/2.

     */
    i2c->DEL.bit.FEDL = 0x30;
    i2c->DEL.bit.REDL = 0x30;
    i2c->CLKT.bit.TOUT = 0x40;
    //printf("DEL: %d, %d\n", i2c->DEL.bit.FEDL, i2c->DEL.bit.REDL);
    //printf("CDIV:%d\n", i2c->DIV.bit.CDIV / 2 );
    //printf("CLKT:%d\n", i2c->CLKT.bit.TOUT);
    assert(i2c->DIV.bit.CDIV / 2 > i2c->DEL.bit.FEDL);
    assert(i2c->DIV.bit.CDIV / 2 > i2c->DEL.bit.REDL);
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
    i2c->C.bit.CLEAR = 0x01;

    // Clear Status
    i2c->S.reg = RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk;
    i2c->DLEN.reg = len;

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
    i2c->S.reg = RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk;
    return reason;
}

RPI_I2C_RETURN_STATUS rpi_i2c_write(volatile rpi_i2c_t *i2c, const char * buf, uint32_t len)
{
    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = RPI_I2C_OK;
    // Clear FIFO
    i2c->C.bit.CLEAR = 0x01;
    // Clear Status
    //__sync_synchronize();
    i2c->S.reg = RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk;
    //__sync_synchronize();

    // Set Data Length
    i2c->DLEN.reg = len;
    // pre-populate FIFO with max buffer; 16-byte FIFO

    while( remaining && ( i < 16 ) ) {
        //XXX: Don't use i2c->FIFO.bit.DATA, since this may cause reading before writing.
        i2c->FIFO.reg = buf[i];
        i++;
        remaining--;
    }
    // Start write
    //XXX!!!: We cannot set bit separately.
    //i2c->C.bit.I2CEN = 1;
    //i2c->C.bit.ST = 1;
    i2c->C.reg = RPI_I2C_C_I2CEN_Msk | RPI_I2C_C_ST_Msk;

    // Transfer is over
    while(!(i2c->S.bit.DONE)) {
        while (remaining && (i2c->S.bit.TXD)) {
            // Write to FIFO
            i2c->FIFO.reg = buf[i];
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
        i2c->C.bit.CLEAR = 0x01;
    }

    // Clear Status
    i2c->S.reg = RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk;
    return reason;
}
