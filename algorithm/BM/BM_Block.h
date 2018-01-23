#ifndef _BM_BLOCK_H_
#define _BM_BLOCK_H_

#include "BM_common.h"
#include "BM_Heap.h"



struct _Block {
	uint32_t PBA;
	uint8_t bit;
	uint8_t cnt;
	uint32_t PE_cycle;
};

typedef struct _Block Block;

#endif // !_BM_BLOCK_H_


