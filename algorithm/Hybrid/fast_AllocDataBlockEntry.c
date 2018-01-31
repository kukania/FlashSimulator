#include "FAST.h"

/**
 * Function : fast_AllocDataBlockEntry(int key, int* physical_address)
 * 
 * Description : 
 *  Find a physical page.
 * 
 * Returns :
 *  physical_address
 * 
 */

char fast_AllocDataBlockEntry(KEYT key, uint32_t* physical_address)
{
    // RW_MappingTable* rw_MappingTable = tableInfo->rw_MappingTable;

    int logical_block;
    int physical_block;
    int offset;
    char check;

    /* Should Use Block Mapping Table */
    logical_block = BLOCK(key);
    offset = OFFSET(key);
	physical_block = BLOCK_TABLE(logical_block);
	*physical_address = ADDRESS(physical_block, offset);
    check = GET_STATE(*physical_address);
	if(check == VALID){
		SET_STATE(*physical_address, INVALID);
	}
	else if(check == ERASED){
		SET_STATE(*physical_address, VALID);
	}
    return check;
    /*
    if(check == ERASED){
        return (eNOERROR);
    }
    else if(check == VALID){
        // We should write to log block 
        SET_STATE(physical_address, INVALID);
        return (VALID);
    }
    else if(check == INVALID){
        return (INVALID);
    }

    return (UNEXPECTED);
    */
}
