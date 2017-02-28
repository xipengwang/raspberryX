#ifndef _RPI_GPIO_H
#define _RPI_GPIO_H

#include "rpi.h"
typedef enum {
    /* RPi B+, RPi2, and RPi3 all use the same header 40 pins header*/
    /* pin header # = MPU GPIO pin # */
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
    RPI_GPIO_FSEL_IN    = 0x00,   /*!< Input 0b000 */
    RPI_GPIO_FSEL_OUT   = 0x01,   /*!< Output 0b001 */
    RPI_GPIO_FSEL_ALT0  = 0x04,   /*!< Alternate function 0 0b100 */
    RPI_GPIO_FSEL_ALT1  = 0x05,   /*!< Alternate function 1 0b101 */
    RPI_GPIO_FSEL_ALT2  = 0x06,   /*!< Alternate function 2 0b110, */
    RPI_GPIO_FSEL_ALT3  = 0x07,   /*!< Alternate function 3 0b111 */
    RPI_GPIO_FSEL_ALT4  = 0x03,   /*!< Alternate function 4 0b011 */
    RPI_GPIO_FSEL_ALT5  = 0x02,   /*!< Alternate function 5 0b010 */
    RPI_GPIO_FSEL_MASK  = 0x07    /*!< Function select bits mask 0b111 */
} Rpi_Function_Select;

typedef enum {
    RPI_GPIO_DISABLE_PUD  = 0x00,
    RPI_GPIO_PULLDOWN     = 0x01,
    RPI_GPIO_PULLUP       = 0x02,

} Rpi_Pud_Select;

typedef struct {
    uint32_t GPFSEL0;
    uint32_t GPFSEL1;
    uint32_t GPFSEL2;
    uint32_t GPFSEL3;
    uint32_t GPFSEL4;
    uint32_t GPFSEL5;
    uint32_t reserved0;
    uint32_t GPSET0;
    uint32_t GPSET1;
    uint32_t reserved1;
    uint32_t GPCLR0;
    uint32_t GPCLR1;
    uint32_t reserved2;
    uint32_t GPLEV0;
    uint32_t GPLEV1;
    uint32_t reserved3;
    uint32_t GPEDS0;
    uint32_t GPEDS1;
    uint32_t reserved4;
    uint32_t GPREN0;
    uint32_t GPREN1;
    uint32_t reserved5;
    uint32_t GPFEN0;
    uint32_t GPFEN1;
    uint32_t reserved6;
    uint32_t GPHEN0;
    uint32_t GPHEN1;
    uint32_t reserved7;
    uint32_t GPLEN0;
    uint32_t GPLEN1;
    uint32_t reserved8;
    uint32_t GPAREN0;
    uint32_t GPAREN1;
    uint32_t reserved9;
    uint32_t GPAFEN0;
    uint32_t GPAFEN1;
    uint32_t reserved10;
    uint32_t GPPUD;
    uint32_t GPPUDCLK0;
    uint32_t GPPUDCLK1;
} rpi_gpio_t;

extern volatile rpi_gpio_t *rpi_gpio;

#ifdef __cplusplus
extern "C" {
#endif

    void rpi_gpio_fsel(uint8_t pin, uint8_t mode);
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
    void rpi_gpio_pudclk(uint8_t pin);
#ifdef __cplusplus
}
#endif

#endif
