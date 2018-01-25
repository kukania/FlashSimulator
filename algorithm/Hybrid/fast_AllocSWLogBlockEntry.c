#include "FAST.h"

/**
 * Function :
 * 
 * Description : 
 * 
 * Returns :
 *  No returns
 */

int fast_AllocSWLogBlockEntry(KEYT key, uint32_t* physical_address)
{
    int physical_block;
    int offset;
    SW_MappingTable* sw_MappingTable = tableInfo->sw_MappingTable;
    
    physical_block = sw_MappingTable->data->physical_block;
    // Update Sector mapping for SW Log BLock
    
    /*
    rw_MappingTable->data[number_of_written_page] = {BLOCK(physical_address), logical_block, offset, VALID};
    rw_MappingTable->offset++;
    if(offset == PAGE_PER_BLOCK){
        rw_MappingTable->offset = 0;
        rw_MappingTable->number_of_full_log_block++;
    }
    */
    // @TODO : Trim
    if(sw_MappingTable->data->number_of_stored_sector == PAGE_PER_BLOCK){
        // @TODO: TRIM
        // Optimized with Switch-Merge
        // fast_InitSWLogBlock();

        // Initiate Sector Mapping for SW Log Block
        memset(sw_MappingTable->data, 0, sizeof(SW_MappingInfo));
        //return FULL
        return (0);
    }

    return (eNOERROR);
}