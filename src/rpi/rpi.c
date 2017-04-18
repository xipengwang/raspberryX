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

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "rpi.h"
#include "gpio.h"
#include "systimer.h"
#include "pwm.h"
#include "i2c.h"
#include "spi.h"
#include "uart.h"

uint32_t *rpi_peripherals_base;
uint32_t rpi_peripherals_size;
uint32_t *rpi_peripherals;

static void *mapmem(const char *msg, size_t size, int fd, off_t off);
static void unmapmem(void **pmem, size_t size);

int rpi_init(void)
{
    int  memfd;
    int  ok;
    FILE *fp;

    if ((fp = fopen("/proc/device-tree/soc/ranges" , "rb")))
    {
        unsigned char buf[4];
        fseek(fp, 4, SEEK_SET);
        if (fread(buf, 1, sizeof(buf), fp) == sizeof(buf))
            rpi_peripherals_base = (uint32_t *)(intptr_t)(buf[0] << 24
                                                          | buf[1] << 16
                                                          | buf[2] << 8
                                                          | buf[3] << 0);
        fseek(fp, 8, SEEK_SET);
        if (fread(buf, 1, sizeof(buf), fp) == sizeof(buf))
            rpi_peripherals_size = (buf[0] << 24 | buf[1] << 16 | buf[2] << 8 | buf[3] << 0);
        printf("rpi peripherals physical address:%p\n",(rpi_peripherals_base));
        fclose(fp);
    }

    memfd = -1;
    ok = -1;
    if (geteuid() == 0) {
        int x = RPI_UART_CR_UARTEN_Pos;
        if ((memfd = open("/dev/mem", O_RDWR | O_SYNC) ) < 0) {
            fprintf(stderr, "rpi_init: Unable to open /dev/mem: %s\n",
                    strerror(errno)) ;
            goto exit;
        }

        rpi_peripherals = mapmem("gpio",
                                 rpi_peripherals_size,
                                 memfd,
                                 (uint32_t)(intptr_t)rpi_peripherals_base);

        if (rpi_peripherals == MAP_FAILED) {
            goto exit;
        }
        rpi_gpio      = (rpi_gpio_t*)(rpi_peripherals + RPI_GPIO_BASE/4);
        rpi_sys_timer = (rpi_sys_timer_t*)(rpi_peripherals + RPI_SYS_TIMER/4);
        rpi_pwm       = (rpi_pwm_t*)(rpi_peripherals + RPI_PWM/4);
        rpi_pwm_clk   = (rpi_pwm_clk_t*) (rpi_peripherals + RPI_PWM_CLK/4);
        rpi_i2c0      = (rpi_i2c_t*)(rpi_peripherals + RPI_I2C0/4);
        rpi_i2c1      = (rpi_i2c_t*)(rpi_peripherals + RPI_I2C1/4);
        rpi_spi0      = (rpi_spi_t*)(rpi_peripherals + RPI_SPI0/4);
        rpi_uart      = (rpi_uart_t*) (rpi_peripherals + RPI_UART/4);

        //Examples of testing memory mapping:
        /*
          printf("data:0x%X \n", (char*)rpi_i2c1 - (char*)rpi_peripherals);
          puts("--------------");
          printf("C:0x%X \n", (char*)&rpi_i2c1->C - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->S - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->DLEN - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->A - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->FIFO - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->DIV - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->DEL - (char*)rpi_i2c1);
          printf("S:0x%X \n", (char*)&rpi_i2c1->CLKT - (char*)rpi_i2c1);
        */

        ok = 0;
    }
    else {
        printf("Please run with sudo!\n");
        return ok;
    }

  exit:
    if (memfd >= 0)
        close(memfd);

    if (ok)
        rpi_close();
    return ok;
}
int rpi_close(void)
{
    unmapmem((void**) &rpi_peripherals, rpi_peripherals_size);
    rpi_peripherals = MAP_FAILED;
    rpi_gpio = MAP_FAILED;
    return 0;
}


static void *mapmem(const char *msg, size_t size, int fd, off_t off)
{
    void *map = mmap(NULL, size, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, off);
    if (map == MAP_FAILED)
        fprintf(stderr, "rpi_init: %s mmap failed: %s\n", msg, strerror(errno));
    return map;
}
static void unmapmem(void **pmem, size_t size)
{
    if (*pmem == MAP_FAILED) return;
    munmap(*pmem, size);
    *pmem = MAP_FAILED;
}
