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

//RPI properties
#define RPI_CORE_CLK_HZ        250000000
#define RPI_GPIO_BASE          0x200000
#define RPI_SYS_TIMER          0x3000
#define RPI_PWM                0x20C000
#define RPI_PWM_CLK	       0x1010a0
#define RPI_I2C0               0x205000
#define RPI_I2C1               0x804000
#define RPI_SPI0               0x204000

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
