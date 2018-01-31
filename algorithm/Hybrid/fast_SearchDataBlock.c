#include "FAST.h"

/**
 * Function : int fast_SearchDataBlock(uint32_t logical_address, uint32_t* physical_address)
 * 
 * Description : 
 *  Convert logical addrerss into physical address using block mapping table.
 *  Returns state of data written in physical address.
 * 
 * Returns :
 *  error code
 *   eNOERROR
 *   eNOTWRITTEN
 *   eOVERWRITTED
 *   eUNEXPECTED
 * 
 * Side effects :
 *  Converted physical address is saved in physical_address
 * 
 */

char fast_SearchDataBlock(uint32_t logical_address, uint32_t* physical_address)
{
    //SW_MappingTable*    sw_MappingTable;
    //RW_MappingTable*    rw_MappingTable;
    uint32_t logical_block = BLOCK(logical_address);
    uint32_t physical_block = BLOCK_TABLE(logical_block);
    uint32_t offset = OFFSET(logical_address);
    
    *physical_address = ADDRESS(physical_block, offset);

    char state = GET_STATE(*physical_address);
    if(state == VALID){
        return (eNOERROR);
    }
    else if(state == ERASED){
        return (eNOTWRITTEN);
    }
    else if(state == INVALID){
        return (eOVERWRITTED);
    }

    return (eUNEXPECTED);
    // Block_MappingTable block_MappingTable = tableInfo->block_MappingTable;

//    char state = GET_STATE(physical_address);

//    return state;
}