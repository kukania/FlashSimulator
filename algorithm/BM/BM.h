#ifndef _BM_H_
#define _BM_H_

#include "BM_common.h"
#include "BM_Block.h"

void		BM_invalidate_ppa(uint32_t PPA);
int8_t		BM_is_invalid_ppa(uint32_t PPA);
uint32_t	BM_get_gc_victim(Block *Block_list);
uint32_t	BM_get_weared_block(Block *Block_list);


#endif // !_BM_H_
