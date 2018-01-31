#include "FAST.h"
/**
 * Function :
 * 
 * Description : 
 * 
 * Returns :
 *  No returns
 */

char fast_SwitchSWLogBlock(uint32_t log_block)
{
    SW_MappingTable* sw_MappingTable = tableInfo->sw_MappingTable;
    uint32_t data_block = BLOCK_TABLE(sw_MappingTable->data->logical_block);
    
    // Trim data block
    // Set log block as data block
    //fast_InitSWLogBlock();

    return (eNOERROR);
}
