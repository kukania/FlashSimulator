#include "FAST.h"

/**
 * Function :
 * 
 * Description :
 *  Branch 0: Is SW Log Block is full
 *   true : Switch SW Log Block
 *
 *  Branch 1: Is offset is 0
 *   true : Merge SW Log Block and make allocation with given key
 *   false : go to Branch 2
 *  Branch 2: Is block is same
 *   true : go to Branch 3 
 *   false : return eNOTSEQUENTIAL 
 *  Branch 3: Is offset if sequential
 *   true : Allocate 
 *   false : return eNOTSEQUENTIAL
 *
 * Returns :
 *  No returns
 */

char fast_AllocSWLogBlockEntry(KEYT key, uint32_t* physical_address)
{
    int physical_block;
    int logical_block;
	int offset;
   	int sw_logical_block;

   	//SW_MappingTable* sw_MappingTable = tableInfo->sw_MappingTable;
    SW_MappingInfo* sw_MappingInfo = tableInfo->sw_MappingTable->data; 
	logical_block = BLOCK(key);
    sw_logical_block = sw_MappingInfo->logical_block;
	offset = OFFSET(key);
   	
	
	if(sw_MappingInfo->number_of_stored_sector == PAGE_PER_BLOCK){
		fast_SwitchSWLogBlock();
		sw_MappingInfo->number_of_stored_sector = 0;
	}
	
	if(offset == 0){
		if(fast_SwitchSWLogBlock(logical_block) != eNOERROR){
			fast_MergeSWLogBlock(logical_block);
		}
		sw_MappingInfo->logical_block = logical_block;
	}
	else if(logical_block == sw_logical_block){
		fast_SwitchSWLogBlock(logical_block);

		if(offset != sw_MappingInfo->number_of_stored_sector){
			return (eNOTSEQUENTIAL);
		}
	}
	else{
		return (eNOTSEQUENTIAL);
	}

	*physical_address = ADDRESS(sw_MappingInfo->physical_block, offset);
	sw_MappingInfo->number_of_stored_sector++;
	printf("%d %d %d\t", sw_MappingInfo->physical_block, offset, *physical_address);
	printf("Why So Slow?\n");

	return (eNOERROR);
	/*
	if(BLOCK(key) != sw_MappingTable->data->logical_block){
		// TRIM & MAKE_NEW_ALLOCATION
		if(offset == 0){
			//if(sw_MappingTable->data->number_of_stored_sector == PAGE_PER_BLOCK){
			if(fast_SwitchSWLogBlock() != eNOERROR){
				fast_MergeSWLogBlock();
			}
			//else{
			//	fast_MergeSWLogBlock();
			//}
			// TRIM
			//
			return (eNOERROR);
		}
		else{
			return (eNOTSEQUENTIAL);
		}
	}
	*/
	
    // @TODO : Trim
	/*
    if(sw_MappingTable->data->number_of_stored_sector == PAGE_PER_BLOCK){
        // @TODO: TRIM
        // Optimized with Switch-Merge
        // fast_InitSWLogBlock();

        // Initiate Sector Mapping for SW Log Block
        memset(sw_MappingTable->data, 0, sizeof(SW_MappingInfo));
        //return FULL
		
        return (eNOERROR);
    }
	*/

}
