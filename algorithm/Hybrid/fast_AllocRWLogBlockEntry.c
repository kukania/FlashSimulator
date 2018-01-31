#include "FAST.h"

/**
 * Function :
 * 
 * Description : 
 *  Set physical_address for empty RW Log Block Page.
 *  
 *  First, find empty entry in the table.
 *  Second, initiate table entry using given information.
 *  Third, update mapping table information (How many page are written in the log block).
 *  If needed, trim operation can be done to make erased log block.
 * 
 * Returns :
 *  No returns
 */

// @TODO : Please use Circular Queue
char fast_AllocRWLogBlockEntry(KEYT key, uint32_t* physical_address)
{
    uint32_t physical_block;
    uint32_t offset;
    uint32_t number_of_written_page;
    RW_MappingTable* rw_MappingTable = tableInfo->rw_MappingTable;

    uint32_t number_of_full_log_block = rw_MappingTable->number_of_full_log_block;

    number_of_written_page = number_of_full_log_block + rw_MappingTable->offset;

    // Update Sector mapping for RW Log BLock
    rw_MappingTable->data[number_of_written_page].physical_block = BLOCK(*physical_address);
    rw_MappingTable->data[number_of_written_page].logical_block = BLOCK_TABLE(BLOCK(*physical_address));
    rw_MappingTable->data[number_of_written_page].physical_offset = OFFSET(number_of_written_page); 
    rw_MappingTable->data[number_of_written_page].logical_offset = offset;
    rw_MappingTable->data[number_of_written_page].state =  VALID;
    rw_MappingTable->offset++;
    if(offset == PAGE_PER_BLOCK){
        rw_MappingTable->offset = 0;
        rw_MappingTable->number_of_full_log_block++;
    }

    // @TODO : Should think of memory operation (간략하게나마 구현할 수는 있지만, 최적화되지 않음)
    // 원형 큐를 이용해서 최적화를 할 수 있을 거라 생각됨.
    if(number_of_full_log_block == NUMBER_OF_RW_LOG_BLOCK){
        // @TODO: TRIM
    }

    printf("Check\n");
}