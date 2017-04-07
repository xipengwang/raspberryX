#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//This includes print_util functions in common library.
#include "common/print_util.h"

//This includes time_util functions in common library.
//Now you can use all functions in the time_util.h
#include "common/time_util.h"

//This include all rpi library functions such as pwm, i2c, spi, uart, etc.
#include "rpi/rpi_all.h"


int main(int argc, char **args)
{
    print_marker("XRobot","Hello World!");
    if(rpi_init() != -1){
        print_version();
    }


    return 0;
}
