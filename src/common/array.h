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

#ifndef _COMMON_ARRAY_H
#define _COMMON_ARRAY_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define DEFAULT_GARRAY_CAPACITY 8

#ifdef __cplusplus
extern "C" {
#endif

    struct garray {
        size_t el_sz;
        int used;
        int capacity;
        char *buf;
    };
    typedef struct garray garray_t;

    garray_t* garray_create(size_t el_sz);
    int garray_add(garray_t *garray, void* elem);
    int garray_get(garray_t *garray, int idx, void* elem);
    int garray_pop(garray_t *garray, void* elem);
    int garray_remove(garray_t *garray, int idx, void* elem);
    void garray_destroy(garray_t *garray);
    void garray_map(garray_t *garray, void (*f)(void*));
    void garray_sort(garray_t *garray, int (*comp)(const void*, const void*));

#ifdef __cplusplus
}
#endif

#endif
