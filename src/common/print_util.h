#ifndef _PRINT_UTIL_H
#define _PRINT_UTIL_H

#include <stdarg.h>

#include "std_util.h"
#include "time_util.h"

#ifdef __cplusplus
extern "C" {
#endif
    void print_marker(const char* marker, const char* descr);
    int printf_rate_limit(uint64_t limit, const char *format, ...);
#ifdef __cplusplus
}
#endif

#endif
