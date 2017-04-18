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

#ifndef _COMMON_HASH_H
#define _COMMON_HASH_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#define GHASH_FACTOR 2
#define DEFAULT_GHASH_CAPACITY 8

#ifdef __cplusplus
extern "C" {
#endif

    struct ghash {
        size_t key_sz;
        size_t value_sz;
        int entry_sz; //1+key_sz+value_sz;

        int used;
        int capacity;
        char *buf;

        uint32_t(*hash)(const void*);
        int (*equals)(const void*, const void*);
    };
    typedef struct ghash ghash_t;


    struct ghash_iter {
        const ghash_t *ghash;
        int last_entry_idx;
    };
    typedef struct ghash_iter ghash_iter_t;


    ghash_t* ghash_create(size_t key_sz, size_t value_sz,
                          uint32_t hash(const void*),
                          int equals(const void*, const void*));


    void ghash_destroy(ghash_t *ghash);
    int ghash_put(ghash_t *ghash, void *key, void *value, void *old_key, void *old_value);
    int ghash_get(ghash_t *ghash, void *key, void *out_value);

    ghash_iter_t *ghash_iter_create(ghash_t *ghash);
    int ghash_iter_next(ghash_iter_t *ghash_iter, void *key, void *value);
    void ghash_iter_destroy(ghash_iter_t *ghash_iter);

    uint32_t ghash_uint32_hash(const void *_a);
    int ghash_uint32_equals(const void *_a, const void *_b);
    uint32_t ghash_uint64_hash(const void *_a);
    int ghash_uint64_equals(const void *_a, const void *_b);
    uint32_t ghash_ptr_hash(const void *_a);
    int ghash_ptr_equals(const void *_a, const void *_b);
    int ghash_str_equals(const void *_a, const void *_b);
    uint32_t ghash_str_hash(const void *_a);

#ifdef __cplusplus
}
#endif

#endif
