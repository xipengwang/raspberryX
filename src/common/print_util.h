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
