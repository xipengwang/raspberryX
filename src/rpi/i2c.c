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
    //i2c->A.bit.ADDR = addr;
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


uint32_t bcm2835_peri_read(volatile uint32_t* paddr)
{
    uint32_t ret;

    __sync_synchronize();
    ret = *paddr;
    __sync_synchronize();
    return ret;

}


#define BCM2835_BSC_C_I2CEN 		0x00008000 /*!< I2C Enable, 0 = disabled, 1 = enabled */
#define BCM2835_BSC_C_INTR 		0x00000400 /*!< Interrupt on RX */
#define BCM2835_BSC_C_INTT 		0x00000200 /*!< Interrupt on TX */
#define BCM2835_BSC_C_INTD 		0x00000100 /*!< Interrupt on DONE */
#define BCM2835_BSC_C_ST 		0x00000080 /*!< Start transfer, 1 = Start a new transfer */
#define BCM2835_BSC_C_CLEAR_1 		0x00000020 /*!< Clear FIFO Clear */
#define BCM2835_BSC_C_CLEAR_2 		0x00000010 /*!< Clear FIFO Clear */
#define BCM2835_BSC_C_READ 		0x00000001 /*!<	Read transfer */

/* Register masks for BSC_S */
#define BCM2835_BSC_S_CLKT 		0x00000200 /*!< Clock stretch timeout */
#define BCM2835_BSC_S_ERR 		0x00000100 /*!< ACK error */
#define BCM2835_BSC_S_RXF 		0x00000080 /*!< RXF FIFO full, 0 = FIFO is not full, 1 = FIFO is full */
#define BCM2835_BSC_S_TXE 		0x00000040 /*!< TXE FIFO full, 0 = FIFO is not full, 1 = FIFO is full */
#define BCM2835_BSC_S_RXD 		0x00000020 /*!< RXD FIFO contains data */
#define BCM2835_BSC_S_TXD 		0x00000010 /*!< TXD FIFO can accept data */
#define BCM2835_BSC_S_RXR 		0x00000008 /*!< RXR FIFO needs reading (full) */
#define BCM2835_BSC_S_TXW 		0x00000004 /*!< TXW FIFO needs writing (full) */
#define BCM2835_BSC_S_DONE 		0x00000002 /*!< Transfer DONE */
#define BCM2835_BSC_S_TA 		0x00000001 /*!< Transfer Active */


void bcm2835_peri_write(volatile uint32_t* paddr, uint32_t value)
{

    __sync_synchronize();
    *paddr = value;
    __sync_synchronize();

}


void bcm2835_peri_set_bits(volatile uint32_t* paddr, uint32_t value, uint32_t mask)
{
    uint32_t v = bcm2835_peri_read(paddr);
    v = (v & ~mask) | (value & mask);
    bcm2835_peri_write(paddr, v);
}


RPI_I2C_RETURN_STATUS rpi_i2c_write(volatile rpi_i2c_t *i2c, const char * buf, uint32_t len)
{
    uint32_t remaining = len;
    uint32_t i = 0;
    uint8_t reason = RPI_I2C_OK;
    /* // Clear FIFO */
    /* __sync_synchronize(); */
    /* i2c->C.bit.CLEAR = 0x01; */
    /* __sync_synchronize(); */

    /* // Clear Status */
    /* __sync_synchronize(); */
    /* i2c->S.reg = RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk; */
    /* __sync_synchronize(); */

    /* // Set Data Length */
    /* __sync_synchronize(); */
    /* i2c->DLEN.reg = len; */
    /* __sync_synchronize(); */

    /* Clear FIFO */
    bcm2835_peri_set_bits((uint32_t*)(&(i2c->C)), BCM2835_BSC_C_CLEAR_1 , BCM2835_BSC_C_CLEAR_1 );
    /* Clear Status */
    bcm2835_peri_write((uint32_t*)(&(i2c->S)), BCM2835_BSC_S_CLKT | BCM2835_BSC_S_ERR | BCM2835_BSC_S_DONE);
    /* Set Data Length */
    bcm2835_peri_write((uint32_t*)(&(i2c->DLEN)), len);
    // pre-populate FIFO with max buffer; 16-byte FIFO

    while( remaining && ( i < 16 ) ) {

        bcm2835_peri_write((uint32_t*)(&(i2c->FIFO)), buf[i]);
        //i2c->FIFO.bit.DATA = buf[i];
        i++;
        remaining--;
    }
    // Start write
    //XXX: We cannot set bit separately.
    //i2c->C.bit.I2CEN = 1;
    //i2c->C.bit.ST = 1;
    bcm2835_peri_write((uint32_t*)(&(i2c->C)), BCM2835_BSC_C_I2CEN | BCM2835_BSC_C_ST);
    /* __sync_synchronize(); */
    /* mem_write_32((uint32_t*)(&(i2c->C)), RPI_I2C_C_I2CEN_Msk | RPI_I2C_C_ST_Msk); */
    /* //i2c->C.reg = RPI_I2C_C_I2CEN_Msk | RPI_I2C_C_ST_Msk; */
    /* __sync_synchronize(); */

    // Transfer is over
    while(!(bcm2835_peri_read((uint32_t*)(&(i2c->S))) & BCM2835_BSC_S_DONE )) {
          //while(!(mem_read_32((uint32_t*)(&(i2c->S))) & RPI_I2C_S_DONE_Msk)/*!(i2c->S.bit.DONE) */) {
        while ( remaining && (bcm2835_peri_read((uint32_t*)(&(i2c->S))) & BCM2835_BSC_S_TXD )) {
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
        i2c->C.bit.CLEAR = 0x01;
    }

    // Clear Status
    i2c->S.reg = RPI_I2C_S_DONE_Msk | RPI_I2C_S_ERR_Msk | RPI_I2C_S_CLKT_Msk;
    return reason;
}
