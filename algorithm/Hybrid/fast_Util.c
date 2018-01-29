#include "FAST.h"

uint32_t BLOCK(uint32_t logical_address)             
        { return logical_address / PAGE_PER_BLOCK; }
uint32_t OFFSET(uint32_t logical_address)            
        { return logical_address % PAGE_PER_BLOCK; }
uint32_t ADDRESS(uint32_t block, uint32_t offset)    
        { return block * PAGE_PER_BLOCK + offset; }
uint32_t BLOCK_TABLE(uint32_t logical_block)
        { return tableInfo->block_MappingTable->data[logical_block].physical_block; }
/*
inline int SW_LOG_BLOCK(uint32_t logical_block)
        { return (logical_block == sw_MappingInfo) ? sw_MappingInfo->physical_block : INVALID; }
inline int RW_LOG_BLOCK(uint32_t logical_block)
        { return ()}
*/
char GET_STATE(uint32_t physical_address)
        { return *(STATE + physical_address); }
void SET_STATE(uint32_t physical_address, char state)
        { *(STATE + physical_address) = state; }
