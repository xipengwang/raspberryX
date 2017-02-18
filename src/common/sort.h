#ifndef _COMMON_SORT_H
#define _COMMON_SORT_H

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

    void g_swap(void *v, int a, int b, size_t size);
    void g_qsort(void *v, int total_elements, size_t size, int (*comp)(const void*, const void*));
    void g_shell_sort(void *v, int total_elements, size_t size, int (*comp)(const void*, const void*));
    void g_bubble_sort(void *v, int n, size_t size, int (*comp)(const void*, const void*));
    void g_heap_sort(void *v, int n, size_t size, int (*comp)(const void*, const void*));

#ifdef __cplusplus
}
#endif

#endif
