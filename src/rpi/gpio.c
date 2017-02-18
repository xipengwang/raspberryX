#include "gpio.h"

volatile rpi_gpio_t *rpi_gpio;
void rpi_gpio_fsel(uint8_t pin, uint8_t mode)
{
    uint8_t   shift = (pin % 10) * 3;
    uint32_t  mask = RPI_GPIO_FSEL_MASK << shift;
    uint32_t  value = mode << shift;
    uint32_t v;
    switch(pin/10){
        case 0:
            v = rpi_gpio->GPFSEL0;
            rpi_gpio->GPFSEL0 = (v & ~mask) | (value & mask);
            break;
        case 1:
            v = rpi_gpio->GPFSEL1;
            rpi_gpio->GPFSEL1 = (v & ~mask) | (value & mask);
            break;
        case 2:
            v = rpi_gpio->GPFSEL2;
            rpi_gpio->GPFSEL2 = (v & ~mask) | (value & mask);
            break;
        case 3:
            v = rpi_gpio->GPFSEL3;
            rpi_gpio->GPFSEL3 = (v & ~mask) | (value & mask);
            break;
        case 4:
            v = rpi_gpio->GPFSEL4;
            rpi_gpio->GPFSEL4 = (v & ~mask) | (value & mask);
            break;
        case 5:
            v = rpi_gpio->GPFSEL5;
            rpi_gpio->GPFSEL5 = (v & ~mask) | (value & mask);
            break;
        default:
            printf("Pin number must be <= 53 \n");
    }
}

void rpi_gpio_write(uint8_t pin, uint8_t level)
{

    uint8_t shift = pin % 32;
    if(level == LOW) {
        switch(pin/32){
            case 0:
                rpi_gpio->GPCLR0 = (1 << shift);
                break;
            case 1:
                rpi_gpio->GPCLR1 = (1 << shift);
                break;
            default:
                printf("Pin number must be <= 53 \n");
                exit(0);
        }

    } else {
        switch(pin/32){
            case 0:
                rpi_gpio->GPSET0 = (1 << shift);
                break;
            case 1:
                rpi_gpio->GPSET1 = (1 << shift);
                break;
            default:
                printf("Pin number must be <= 53 \n");
                exit(0);
        }
    }
}

uint32_t rpi_gpio_read(uint8_t pin)
{
    uint8_t shift = pin % 32;
    switch(pin/32){
        case 0:
            return (rpi_gpio->GPLEV0 & (1 << shift));
        case 1:
            return (rpi_gpio->GPLEV1 & (1 << shift));
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }
}

uint32_t rpi_gpio_status(uint8_t pin)
{
    uint8_t shift = pin % 32;
    switch(pin/32){
        case 0:
            return (rpi_gpio->GPEDS0 & (1 << shift));
        case 1:
            return (rpi_gpio->GPEDS1 & (1 << shift));
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }
}

void rpi_gpio_enable_Redge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPREN0;
            rpi_gpio->GPREN0 = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPREN1;
            rpi_gpio->GPREN1 = v | mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_disable_Redge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPREN0;
            rpi_gpio->GPREN0 = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPREN1;
            rpi_gpio->GPREN1 = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_enable_Fedge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPFEN0;
            rpi_gpio->GPFEN0 = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPFEN1;
            rpi_gpio->GPFEN1 = v | mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_disable_Fedge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPFEN0;
            rpi_gpio->GPFEN0 = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPFEN1;
            rpi_gpio->GPFEN1 = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_enable_high(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPHEN0;
            rpi_gpio->GPHEN0 = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPHEN1;
            rpi_gpio->GPHEN1 = v | mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_disable_high(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPHEN0;
            rpi_gpio->GPHEN0 = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPHEN1;
            rpi_gpio->GPHEN1 = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_enable_low(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPLEN0;
            rpi_gpio->GPLEN0 = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPLEN1;
            rpi_gpio->GPLEN1 = v | mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_disable_low(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPLEN0;
            rpi_gpio->GPLEN0 = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPLEN1;
            rpi_gpio->GPLEN1 = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_enable_async_Redge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPAREN0;
            rpi_gpio->GPAREN0 = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPAREN1;
            rpi_gpio->GPAREN1 = v | mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_disable_async_Redge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPAREN0;
            rpi_gpio->GPAREN0 = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPAREN1;
            rpi_gpio->GPAREN1 = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_enable_async_Fedge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPAFEN0;
            rpi_gpio->GPAFEN0 = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPAFEN1;
            rpi_gpio->GPAFEN1 = v | mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_disable_async_Fedge(uint8_t pin)
{
    uint32_t  mask = 1 << (pin % 32);
    uint32_t v;
    switch(pin/32){
        case 0:
            v = rpi_gpio->GPAFEN0;
            rpi_gpio->GPAFEN0 = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPAFEN1;
            rpi_gpio->GPAFEN1 = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_pud(uint8_t mode)
{
    rpi_gpio->GPPUD = mode;
}

void rpi_gpio_pudclk(uint8_t pin)
{
    uint8_t shift = pin % 32;
    switch(pin/32){
        case 0:
            rpi_gpio->GPPUDCLK0 = (1 << shift);
            break;
        case 1:
            rpi_gpio->GPPUDCLK1 = (1 << shift);
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }
}
