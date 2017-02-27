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
