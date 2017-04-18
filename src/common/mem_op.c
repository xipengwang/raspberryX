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
