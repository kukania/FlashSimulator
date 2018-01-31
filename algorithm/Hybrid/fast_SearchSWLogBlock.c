#include "FAST.h"

/**
 * Function :
 * 
 * 
 * Description : 
 * 
 * 
 * Returns :
 *  No returns
 */

char fast_SearchSWLogBlock(uint32_t logical_address, uint32_t* physical_address)
{
    uint32_t logical_block = BLOCK(logical_address);
    uint32_t offset = OFFSET(logical_address);
    SW_MappingTable* sw_MappingTable = tableInfo->sw_MappingTable;

    uint32_t sw_log_block = sw_MappingTable->data->logical_block;
    uint32_t physical_block = sw_MappingTable->data->physical_block;
    uint32_t number_of_stored_sector = sw_MappingTable->data->number_of_stored_sector;

    if(logical_block == sw_log_block && offset < number_of_stored_sector){
        *physical_address = ADDRESS(physical_block, offset);
        printf("Log Block Found!\n");
        return (eNOERROR);
    }
    
    return (eNOTFOUND);
}

// 뒤에서부터 읽어옴
// 같은 값들을 가지고 있으면 그 값의 물리적 주소를 반환함.