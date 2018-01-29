#include "FAST.h"

/**
 * Function : uint32_t FAST_Create(lower_info* li, algorithm* algo)
 * 
 * Description : 
 *  Initiate FAST FTL.
 *  Allocate memory for mapping table (Block Level, Page Level - SW & RW)
 * 
 *  Sector mapping table for SW (Sequential Write) keeps information 
 *  for physical & logical block number, number of page stored in the log block.
 *  Sector mapping table for RW (Random Write) keeps information
 *  for which sectors are written, the offset of RW log blocks.
 * 
 *  Block mapping table keeps information for data block which logical block corresponds to.
 *  Because we search Log blocks first, we don't need to keep information for log blocks.
 * 
 * Returns :
 *  No returns
 * 
 * Side effects:
 *  Memeory allocation for mapping table
 */

struct algorithm FAST_Algorithm = {
    .create = FAST_Create,
    .destroy = FAST_Destroy,
    .get = FAST_Get,
    .set = FAST_Set,
    .remove = FAST_Remove
};

TableInfo* tableInfo;

char* STATE;

/* Definition of constant value */
uint32_t NUMBER_OF_BLOCK;
uint32_t NUMBER_OF_PAGE;
uint32_t SIZE_OF_KEY_TYPE;
uint32_t SIZE_OF_BLOCK;
uint32_t PAGE_PER_BLOCK;
uint32_t TOTAL_SIZE;

const char ERASED = 0;
const char VALID = 1;
const char INVALID = 2;

const char eNOERROR = 0;
const char eNOTFOUND = -1;
const char UNEXPECTED = -2;
const char eNOTWRITTEN = -3;
const char eOVERWRITTED = -4;
const char eUNEXPECTED = -5;
const char eNOTSEQUENTIAL = -6;
const char NUMBER_OF_RW_LOG_BLOCK = 15;

const char NIL = -1;

uint32_t FAST_Create(lower_info* li, algorithm* algo)
{
    NUMBER_OF_BLOCK = li->NOB;
    NUMBER_OF_PAGE = li->NOP;
    SIZE_OF_KEY_TYPE = li->SOK;
    SIZE_OF_BLOCK = li->SOB;
    PAGE_PER_BLOCK = li->PPB;
    TOTAL_SIZE = li->TS;

    algo->li = li;      /* li means Lower Info */



    /* Definition of Global Variables */
    /*
    NUMBER_OF_BLOCK = li->NOB;
    NUMBER_OF_PAGE = li->NOP;
    SIZE_OF_KEY_TYPE = li->SOK;
    SIZE_OF_BLOCK = li->SOB;
    PAGE_PER_BLOCK = li->PPB;
    TOTAL_SIZEZ = li->TS;
    */
    tableInfo = (TableInfo*)malloc(sizeof(TableInfo));

    tableInfo->sw_MappingTable = (SW_MappingTable*)malloc(sizeof(SW_MappingTable));
    tableInfo->rw_MappingTable = (RW_MappingTable*)malloc(sizeof(RW_MappingTable) * NUMBER_OF_RW_LOG_BLOCK);
    tableInfo->block_MappingTable = (Block_MappingTable*)malloc(sizeof(Block_MappingTable));

    tableInfo->sw_MappingTable->data = 
            (SW_MappingInfo*)malloc(sizeof(SW_MappingInfo) * PAGE_PER_BLOCK);
    memset(tableInfo->sw_MappingTable->data, 0, sizeof(SW_MappingInfo) * PAGE_PER_BLOCK);
    tableInfo->rw_MappingTable->data = 
            (RW_MappingInfo*)malloc(sizeof(RW_MappingInfo) * PAGE_PER_BLOCK);
    memset(tableInfo->rw_MappingTable->data, 0, sizeof(RW_MappingInfo) * PAGE_PER_BLOCK);
    tableInfo->block_MappingTable->data = 
            (Block_MappingInfo*)malloc(sizeof(Block_MappingInfo) * NUMBER_OF_BLOCK);
    memset(tableInfo->block_MappingTable->data, 0, sizeof(Block_MappingInfo) * PAGE_PER_BLOCK);

	for(int i = 0; i < NUMBER_OF_BLOCK; i++){
		tableInfo->block_MappingTable->data[i].physical_block = i;
	}

	tableInfo->sw_MappingTable->data->physical_block = 1000; // @TODO : should change dependent on log block size

    STATE = (char*)malloc(sizeof(char)*NUMBER_OF_BLOCK);
    memset(STATE, ERASED, NUMBER_OF_BLOCK);
    printf("FAST FTL Creation Finished!\n");
	printf("Page Per Block : %d\n", PAGE_PER_BLOCK);
	printf("Total_Size : %d\n", TOTAL_SIZE);
	printf("NUMBER_OF_PAGE : %d\n", NUMBER_OF_PAGE);
}
