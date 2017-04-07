#ifndef _RPI_GPIO_H
#define _RPI_GPIO_H

/* RPi B+, RPi2, and RPi3 all use the same header 40 pins header
 * Number on left side is the pin number on RasPi.
 * Number on right side is the GPIO number of the CHIP.
 */
typedef enum {
    PIN_03     =  2,
    PIN_05     =  3,
    PIN_07     =  4,
    PIN_08     = 14,
    PIN_10     = 15,
    PIN_11     = 17,
    PIN_12     = 18,
    PIN_13     = 27,
    PIN_15     = 22,
    PIN_16     = 23,
    PIN_18     = 24,
    PIN_19     = 10,
    PIN_21     =  9,
    PIN_22     = 25,
    PIN_23     = 11,
    PIN_24     =  8,
    PIN_26     =  7,
    PIN_29     =  5,
    PIN_31     =  6,
    PIN_32     = 12,
    PIN_33     = 13,
    PIN_35     = 19,
    PIN_36     = 16,
    PIN_37     = 26,
    PIN_38     = 20,
    PIN_40     = 21
} Rpi_Gpio_Pin;

typedef enum {
    RPI_GPIO_FSEL_IN    = 0x00,   /* Input 0b000 */
    RPI_GPIO_FSEL_OUT   = 0x01,   /* Output 0b001 */
    RPI_GPIO_FSEL_ALT0  = 0x04,   /* Alternate function 0 0b100 */
    RPI_GPIO_FSEL_ALT1  = 0x05,   /* Alternate function 1 0b101 */
    RPI_GPIO_FSEL_ALT2  = 0x06,   /* Alternate function 2 0b110 */
    RPI_GPIO_FSEL_ALT3  = 0x07,   /* Alternate function 3 0b111 */
    RPI_GPIO_FSEL_ALT4  = 0x03,   /* Alternate function 4 0b011 */
    RPI_GPIO_FSEL_ALT5  = 0x02,   /* Alternate function 5 0b010 */
    RPI_GPIO_FSEL_MASK  = 0x07    /* Function select bits mask  */
} Rpi_Gpio_Function_Select;

typedef enum {
    RPI_GPIO_DISABLE_PUD  = 0x00, /* Disable pull up or pull down */
    RPI_GPIO_PULLDOWN     = 0x01,
    RPI_GPIO_PULLUP       = 0x02,
} Rpi_Gpio_Pud_Select;

typedef union {
    struct {
        uint32_t r0 :32;
    } bit;
    uint32_t reg;
} RPI_GPIO_SHARE_REG;

typedef struct {
    __IO RPI_GPIO_SHARE_REG GPFSEL0;
    __IO RPI_GPIO_SHARE_REG GPFSEL1;
    __IO RPI_GPIO_SHARE_REG GPFSEL2;
    __IO RPI_GPIO_SHARE_REG GPFSEL3;
    __IO RPI_GPIO_SHARE_REG GPFSEL4;
    __IO RPI_GPIO_SHARE_REG GPFSEL5;
    __IO RPI_GPIO_SHARE_REG reserved0;
    __IO RPI_GPIO_SHARE_REG GPSET0;
    __IO RPI_GPIO_SHARE_REG GPSET1;
    __IO RPI_GPIO_SHARE_REG reserved1;
    __IO RPI_GPIO_SHARE_REG GPCLR0;
    __IO RPI_GPIO_SHARE_REG GPCLR1;
    __IO RPI_GPIO_SHARE_REG reserved2;
    __IO RPI_GPIO_SHARE_REG GPLEV0;
    __IO RPI_GPIO_SHARE_REG GPLEV1;
    __IO RPI_GPIO_SHARE_REG reserved3;
    __IO RPI_GPIO_SHARE_REG GPEDS0;
    __IO RPI_GPIO_SHARE_REG GPEDS1;
    __IO RPI_GPIO_SHARE_REG reserved4;
    __IO RPI_GPIO_SHARE_REG GPREN0;
    __IO RPI_GPIO_SHARE_REG GPREN1;
    __IO RPI_GPIO_SHARE_REG reserved5;
    __IO RPI_GPIO_SHARE_REG GPFEN0;
    __IO RPI_GPIO_SHARE_REG GPFEN1;
    __IO RPI_GPIO_SHARE_REG reserved6;
    __IO RPI_GPIO_SHARE_REG GPHEN0;
    __IO RPI_GPIO_SHARE_REG GPHEN1;
    __IO RPI_GPIO_SHARE_REG reserved7;
    __IO RPI_GPIO_SHARE_REG GPLEN0;
    __IO RPI_GPIO_SHARE_REG GPLEN1;
    __IO RPI_GPIO_SHARE_REG reserved8;
    __IO RPI_GPIO_SHARE_REG GPAREN0;
    __IO RPI_GPIO_SHARE_REG GPAREN1;
    __IO RPI_GPIO_SHARE_REG reserved9;
    __IO RPI_GPIO_SHARE_REG GPAFEN0;
    __IO RPI_GPIO_SHARE_REG GPAFEN1;
    __IO RPI_GPIO_SHARE_REG reserved10;
    __IO RPI_GPIO_SHARE_REG GPPUD;
    __IO RPI_GPIO_SHARE_REG GPPUDCLK0;
    __IO RPI_GPIO_SHARE_REG GPPUDCLK1;
} rpi_gpio_t;

extern volatile rpi_gpio_t *rpi_gpio;

#ifdef __cplusplus
extern "C" {
#endif

    void rpi_gpio_fsel(uint8_t pin, Rpi_Gpio_Function_Select mode);

    void rpi_gpio_write(uint8_t pin, uint8_t level);
    uint32_t rpi_gpio_read(uint8_t pin);
    uint32_t rpi_gpio_status(uint8_t pin);

    void rpi_gpio_enable_Redge(uint8_t pin);
    void rpi_gpio_disable_Redge(uint8_t pin);
    void rpi_gpio_enable_Fedge(uint8_t pin);
    void rpi_gpio_disable_Fedge(uint8_t pin);
    void rpi_gpio_enable_high(uint8_t pin);
    void rpi_gpio_disable_high(uint8_t pin);
    void rpi_gpio_enable_low(uint8_t pin);
    void rpi_gpio_disable_low(uint8_t pin);
    void rpi_gpio_disable_low(uint8_t pin);
    void rpi_gpio_disable_low(uint8_t pin);
    void rpi_gpio_enable_async_Fedge(uint8_t pin);
    void rpi_gpio_enable_async_Fedge(uint8_t pin);
    void rpi_gpio_enable_async_Fedge(uint8_t pin);

    void rpi_gpio_pud(Rpi_Gpio_Pud_Select mode);
    void rpi_gpio_pudclk(uint8_t pin);

#ifdef __cplusplus
}
#endif

#endif
