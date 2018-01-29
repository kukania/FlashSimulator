#include "FAST.h"
/**
 * Function :
 * 
 * Description : 
 * 
 * Returns :
 *  No returns
 */

int fast_SwitchSWLogBlock(int log_block)
{
    SW_MappingTable* sw_MappingTable = tableInfo->sw_MappingTable;
    uint32_t data_block = BLOCK_TABLE(sw_MappingTable->data->logical_block);
    
    // Trim data block
    // Set log block as data block
    //fast_InitSWLogBlock();
}
