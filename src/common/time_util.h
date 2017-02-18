/* $COPYRIGHT_UM
   $LICENSE_BSD */

#ifndef _TIME_UTIL_H
#define _TIME_UTIL_H

#include <stdint.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct timeutil_rest timeutil_rest_t;
    timeutil_rest_t *timeutil_rest_create();

    void timeutil_rest_destroy(timeutil_rest_t * rest);

    int64_t utime_now(); // blacklist-ignore
    int64_t utime_get_seconds(int64_t v);
    int64_t utime_get_useconds(int64_t v);

    int32_t  timeutil_usleep(int64_t useconds);
    uint32_t timeutil_sleep(unsigned int seconds);
    int32_t  timeutil_running_hz(timeutil_rest_t *rest, double hz);

#ifdef __cplusplus
}
#endif

#endif
