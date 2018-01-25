#include "FAST.h"
/*
 * Function: SW_Merge()
 * 
 * Description: 
 * 
 * 
 * Returns:
 *  error Code
 */

int fast_MergeSWLogBlock()
{
    int     e;      /* error number */
    SW_MappingTable* sw_MappingTable = tableInfo->sw_MappingTable;
    uint32_t data_block = BLOCK_TABLE(sw_MappingTable->data->logical_block);

    for(int i = 0; i < PAGE_PER_BLOCK; i++){
        if(GET_STATE(log_block, i) == VALID){
            // copy data from log block to tmp
        }
        else{
            //copy data from data block to log block
        }
    }
    // trim data_block
}