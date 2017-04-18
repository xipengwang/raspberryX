/*
 * Copyright (C) <2017>  <Xipeng Wang>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rpi.h"
#include "gpio.h"

volatile rpi_gpio_t *rpi_gpio;
void rpi_gpio_fsel(uint8_t pin, Rpi_Gpio_Function_Select mode)
{
    uint8_t shift = (pin % 10) * 3;
    uint32_t mask = RPI_GPIO_FSEL_MASK << shift;
    uint32_t value = mode << shift;
    uint32_t v;
    switch(pin/10){
        case 0:
            v = rpi_gpio->GPFSEL0.reg;
            rpi_gpio->GPFSEL0.reg = (v & ~mask) | (value & mask);
            break;
        case 1:
            v = rpi_gpio->GPFSEL1.reg;
            rpi_gpio->GPFSEL1.reg = (v & ~mask) | (value & mask);
            break;
        case 2:
            v = rpi_gpio->GPFSEL2.reg;
            rpi_gpio->GPFSEL2.reg = (v & ~mask) | (value & mask);
            break;
        case 3:
            v = rpi_gpio->GPFSEL3.reg;
            rpi_gpio->GPFSEL3.reg = (v & ~mask) | (value & mask);
            break;
        case 4:
            v = rpi_gpio->GPFSEL4.reg;
            rpi_gpio->GPFSEL4.reg = (v & ~mask) | (value & mask);
            break;
        case 5:
            v = rpi_gpio->GPFSEL5.reg;
            rpi_gpio->GPFSEL5.reg = (v & ~mask) | (value & mask);
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
                rpi_gpio->GPCLR0.reg = (1 << shift);
                break;
            case 1:
                rpi_gpio->GPCLR1.reg = (1 << shift);
                break;
            default:
                printf("Pin number must be <= 53 \n");
                exit(0);
        }

    } else {
        switch(pin/32){
            case 0:
                rpi_gpio->GPSET0.reg = (1 << shift);
                break;
            case 1:
                rpi_gpio->GPSET1.reg = (1 << shift);
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
            return (rpi_gpio->GPLEV0.reg & (1 << shift));
        case 1:
            return (rpi_gpio->GPLEV1.reg & (1 << shift));
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
            return (rpi_gpio->GPEDS0.reg & (1 << shift));
        case 1:
            return (rpi_gpio->GPEDS1.reg & (1 << shift));
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
            v = rpi_gpio->GPREN0.reg;
            rpi_gpio->GPREN0.reg = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPREN1.reg;
            rpi_gpio->GPREN1.reg = v | mask;
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
            v = rpi_gpio->GPREN0.reg;
            rpi_gpio->GPREN0.reg = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPREN1.reg;
            rpi_gpio->GPREN1.reg = v & ~mask;
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
            v = rpi_gpio->GPFEN0.reg;
            rpi_gpio->GPFEN0.reg = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPFEN1.reg;
            rpi_gpio->GPFEN1.reg = v | mask;
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
            v = rpi_gpio->GPFEN0.reg;
            rpi_gpio->GPFEN0.reg = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPFEN1.reg;
            rpi_gpio->GPFEN1.reg = v & ~mask;
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
            v = rpi_gpio->GPHEN0.reg;
            rpi_gpio->GPHEN0.reg = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPHEN1.reg;
            rpi_gpio->GPHEN1.reg = v | mask;
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
            v = rpi_gpio->GPHEN0.reg;
            rpi_gpio->GPHEN0.reg = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPHEN1.reg;
            rpi_gpio->GPHEN1.reg = v & ~mask;
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
            v = rpi_gpio->GPLEN0.reg;
            rpi_gpio->GPLEN0.reg = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPLEN1.reg;
            rpi_gpio->GPLEN1.reg = v | mask;
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
            v = rpi_gpio->GPLEN0.reg;
            rpi_gpio->GPLEN0.reg = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPLEN1.reg;
            rpi_gpio->GPLEN1.reg = v & ~mask;
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
            v = rpi_gpio->GPAREN0.reg;
            rpi_gpio->GPAREN0.reg = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPAREN1.reg;
            rpi_gpio->GPAREN1.reg = v | mask;
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
            v = rpi_gpio->GPAREN0.reg;
            rpi_gpio->GPAREN0.reg = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPAREN1.reg;
            rpi_gpio->GPAREN1.reg = v & ~mask;
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
            v = rpi_gpio->GPAFEN0.reg;
            rpi_gpio->GPAFEN0.reg = v | mask;
            break;
        case 1:
            v = rpi_gpio->GPAFEN1.reg;
            rpi_gpio->GPAFEN1.reg = v | mask;
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
            v = rpi_gpio->GPAFEN0.reg;
            rpi_gpio->GPAFEN0.reg = v & ~mask;
            break;
        case 1:
            v = rpi_gpio->GPAFEN1.reg;
            rpi_gpio->GPAFEN1.reg = v & ~mask;
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }

}

void rpi_gpio_pud(Rpi_Gpio_Pud_Select mode)
{
    rpi_gpio->GPPUD.reg = mode;
}

void rpi_gpio_pudclk(uint8_t pin)
{
    uint8_t shift = pin % 32;
    switch(pin/32){
        case 0:
            rpi_gpio->GPPUDCLK0.reg = (1 << shift);
            break;
        case 1:
            rpi_gpio->GPPUDCLK1.reg = (1 << shift);
            break;
        default:
            printf("Pin number must be <= 53 \n");
            exit(0);
    }
}
