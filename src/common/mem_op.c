#include "mem_op.h"

#define _MASK(shiftbits) 1<<shitfbits

/**
 * Read with memory barrier
 */
uint32_t mem_read_32(volatile uint32_t* paddr)
{
    uint32_t ret;
    __sync_synchronize();
    ret = *paddr;
    __sync_synchronize();
    return ret;
}

uint8_t mem_read_8(volatile uint8_t* paddr)
{
    uint32_t ret;
    __sync_synchronize();
    ret = *paddr;
    __sync_synchronize();
    return ret;
}

uint32_t mem_read_32_nb(volatile uint32_t* paddr)
{
    return *paddr;
}

uint8_t mem_read_8_nb(volatile uint8_t* paddr)
{
    return *paddr;
}

void mem_write_32(volatile uint32_t* paddr, uint32_t value)
{
        __sync_synchronize();
        *paddr = value;
        __sync_synchronize();
}

void mem_write_8(volatile uint8_t* paddr, uint8_t value)
{
    __sync_synchronize();
    *paddr = value;
    __sync_synchronize();
}

/* write to peripheral without the write barrier */
void mem_write_32_nb(volatile uint32_t* paddr, uint32_t value)
{
        *paddr = value;
}

void mem_write_8_nb(volatile uint8_t* paddr, uint8_t value)
{
    *paddr = value;
}

void mem_set_bits_32(volatile uint32_t* paddr, uint32_t value, uint32_t mask)
{
    uint32_t v = mem_read_32(paddr);
    v = (v & ~mask) | (value & mask);
    mem_write_32(paddr, v);
}

void mem_set_bits_8(volatile uint8_t* paddr, uint8_t value, uint8_t mask)
{
    uint32_t v = mem_read_8(paddr);
    v = (v & ~mask) | (value & mask);
    mem_write_8(paddr, v);
}
