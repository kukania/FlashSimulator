#include "FAST.h"

/**
 * Function :
 * 
 * Description : 
 * 
 * Returns :
 *  No returns
 */

void FAST_Destroy(lower_info* li, algorithm* algo)
{
    free(STATE);

    free(tableInfo->block_MappingTable->data);
    free(tableInfo->rw_MappingTable->data);
    free(tableInfo->sw_MappingTable->data);

    free(tableInfo->block_MappingTable);
    free(tableInfo->rw_MappingTable);
    free(tableInfo->sw_MappingTable);
    
    free(tableInfo);
}