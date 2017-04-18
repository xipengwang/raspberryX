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
