#include "array.h"

garray_t* garray_create(size_t el_sz)
{
    garray_t *garray = (garray_t*)calloc(1, sizeof(garray_t));
    garray->buf = (char*)calloc(DEFAULT_GARRAY_CAPACITY, el_sz);
    if(garray->buf == NULL) {
        free(garray);
        return NULL;
    }
    garray->capacity = DEFAULT_GARRAY_CAPACITY;
    garray->used = 0;
    garray->el_sz = el_sz;
    return garray;

}

int garray_add(garray_t *garray, void* elem)
{
    if(garray->used == garray->capacity) {
        garray->capacity *= 2;
        garray->buf = (char*)realloc(garray->buf, garray->capacity);
        if(garray->buf == NULL)
            return 0;
    }
    memcpy((char*)garray->buf + garray->used*garray->el_sz,
           (char*)elem, garray->el_sz);
    garray->used++;
    return 1;
}

int garray_get(garray_t *garray, int idx, void* elem)
{
    if(idx >= garray->used)
        return 0;

    if(elem == NULL)
        return 0;

    memcpy((char*)elem,
           (char*)garray->buf + idx*garray->el_sz,
           garray->el_sz);
    return 1;
}

int garray_pop(garray_t *garray, void* elem)
{
    if(garray->used == 0)
        return 0;
    garray->used--;
    if(elem != NULL)
        memcpy((char*)elem,
               (char*)garray->buf + (garray->used)*garray->el_sz,
               garray->el_sz);
    return 1;
}

int garray_remove(garray_t *garray, int idx, void* elem)
{
    if(idx >= garray->used)
        return 0;

    if(elem != NULL)
        memcpy((char*)elem,
               (char*)garray->buf + idx*garray->el_sz,
               garray->el_sz);

    garray->used--;
    if(idx == garray->used)
        return 1;

    memmove((char*)garray->buf + idx*garray->el_sz,
            (char*)garray->buf + (idx+1)*garray->el_sz,
            (garray->used - idx)*garray->el_sz);
    return 1;
}

void garray_destroy(garray_t *garray)
{
    if(garray == NULL)
        return;

    free(garray->buf);
    free(garray);
}

void garray_map(garray_t *garray, void (*f)(void*))
{
    for(int i = 0; i < garray->used; i++) {
        f((char*)garray->buf + i*garray->el_sz);
    }
}

void garray_sort(garray_t *garray, int (*comp)(const void*, const void*))
{
    qsort(garray->buf, garray->used, garray->el_sz, comp);
}
