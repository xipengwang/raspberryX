#include "print_util.h"

void print_marker(const char* marker, const char* descr)
{
    printf("%s : %s\n",marker,descr);
}

int printf_rate_limit(uint64_t limit, const char *format, ...)
{
    int ret = 0;
    static uint64_t last = 0;
    uint64_t now = utime_now();
    if(now > last + limit)
    {
        va_list args;
        va_start(args, format);
        last = now;
        ret = printf(format, args);
        va_end(args);
    }
    return ret;
}
