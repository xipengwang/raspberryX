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

#ifndef _MEM_OP_H
#define _MEM_OP_H

#include "std_util.h"


// Define MEM_BitMarks
#define BITMASK_7 0x80
#define BITMASK_7 0x80
#define BITMASK_6 0x40
#define BITMASK_5 0x20
#define BITMASK_4 0x10
#define BITMASK_3 0x08
#define BITMASK_2 0x04
#define BITMASK_1 0x02
#define BITMASK_0 0x01


#ifdef __cplusplus
extern "C" {
#endif

    uint32_t mem_read_32(volatile uint32_t* paddr);
    uint8_t mem_read_8(volatile uint8_t* paddr);
    uint32_t mem_read_32_nb(volatile uint32_t* paddr);
    uint8_t mem_read_8_nb(volatile uint8_t* paddr);

    void mem_write_32(volatile uint32_t* paddr, uint32_t value);
    void mem_write_8(volatile uint8_t* paddr, uint8_t value);
    void mem_write_32_nb(volatile uint32_t* paddr, uint32_t value);
    void mem_write_8_nb(volatile uint8_t* paddr, uint8_t value);

    void mem_set_bits_32(volatile uint32_t* paddr,
                         uint32_t value, uint32_t mask);
    void mem_set_bits_8(volatile uint8_t* paddr,
                        uint8_t value, uint8_t mask);
#ifdef __cplusplus
}
#endif

#endif
