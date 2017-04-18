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

#ifndef _RPI_H
#define _RPI_H

#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "common/std_util.h"
#include "common/mem_op.h"

#define __IO volatile

#define RPI_VERSION 10000

#define HIGH 0x01
#define LOW  0x00

//RPI peripheral addresses
#define RPI_CORE_CLK_HZ        250000000 //NOTE: UART0 use UART_CLK_HZ = 50000000
#define RPI_GPIO_BASE          0x200000
#define RPI_SYS_TIMER          0x3000
#define RPI_PWM                0x20C000
#define RPI_PWM_CLK	           0x1010a0
#define RPI_I2C0               0x205000
#define RPI_I2C1               0x804000
#define RPI_SPI0               0x204000
#define RPI_UART               0x201000

#ifdef __cplusplus
extern "C" {
#endif

    int rpi_init(void);
    int rpi_close(void);

    static inline void print_version(void)
    {
        printf("Version: %d \n", RPI_VERSION);
    }

#ifdef __cplusplus
}
#endif

#endif /* RPI_H */
