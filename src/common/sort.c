#include "sort.h"


void g_swap(void *v, int a, int b, size_t size)
{
    char *temp = calloc(1, size);
    memcpy(temp, (char*)v+a*size, size);
    memcpy((char*)v+a*size, (char*)v+b*size, size);
    memcpy((char*)v+b*size, temp, size);
    free(temp);
}

/* deprecated
void g_qsort(void *v, int left, int right, size_t size, int (*comp)(void*, void*))
{

    if(left >= right)
        return;
    int last = left;
    for(int i = left + 1; i <= right; i++) {
        if(comp((char*)v+i*size,(char*)v+left*size)<0) {
            g_swap(v,i,++last,size);
        }
    }
    g_swap(v, left, last, size);
    g_qsort(v, left, last-1, size, comp);
    g_qsort(v, last+1, right, size, comp);
}
*/

void g_qsort(void *v, int n, size_t size, int (*comp)(const void*, const void*))
{

    if(n <= 0)
        return;
    int last = 0;
    for(int i = 1; i < n; i++) {
        if(comp((char*)v+i*size,(char*)v)<0) {
            g_swap(v,i,++last,size);
        }
    }
    g_swap(v, 0, last, size);
    g_qsort(v, last, size, comp);
    g_qsort((void*)((char*)v+(last+1)*size), n - last -1,size, comp);
}

void g_shell_sort(void *v, int n, size_t size, int (*comp)(const void*, const void*))
{
    for(int gap = n/2; gap > 0; gap /= 2) {
        for(int i = gap; i < n; i++) {
            for (int j = i - gap; j >= 0; j -= gap) {
                if(comp((char*)v+j*size,(char*)v+(j+gap)*size) < 0)
                    break;
                g_swap(v, j, j+gap, size);
            }
        }
    }
}

void g_bubble_sort(void *v, int n, size_t size, int (*comp)(const void*, const void*))
{
    for(int i = 0; i < n; i++){
        for(int j = n-1; j > i; j--) {
            if(comp((char*)v+j*size,(char*)v+(j-1)*size) < 0)
                g_swap(v, j, j-1, size);
        }
    }
}

static void max_heapify(void *v, int n, int node_k, size_t size, int (*comp)(const void*, const void*))
{
    int l_child = node_k * 2 + 1;
    int r_child = node_k * 2 + 2;
    int largest_idx = node_k;

    if(l_child < n &&
       comp((char*)v + largest_idx*size, (char*)v + l_child*size) < 0)
        largest_idx = l_child;

    if(r_child < n &&
       comp((char*)v + largest_idx*size, (char*)v + r_child*size) < 0)
        largest_idx = r_child;

    if(largest_idx == node_k)
        return;

    g_swap(v, largest_idx, node_k, size);
    max_heapify(v, n, largest_idx, size, comp);
}

static void heap_build(void *v, int n, size_t size, int (*comp)(const void*, const void*))
{
    for(int i = n/2 - 1; i >= 0; i--) {
        max_heapify(v, n, i, size, comp);
    }
}

void g_heap_sort(void *v, int n, size_t size, int (*comp)(const void*, const void*))
{
    heap_build(v, n, size, comp);
    for(int i = n - 1; i >= 0; i--) {
        g_swap(v, 0, i, size);
        max_heapify(v, i, 0, size, comp);
    }
}
