#include <stdlib.h>
#include <stdint.h>
#include "demand.h"

struct algorithm __demand={
	.create=demand_create,
	.destroy=demand_destroy,
	.get=demand_get,
	.set=demand_set,
	.remove=demand_remove
};

page_map *CMT;
page_map *GTD;

uint32_t demand_create(lower_info *li, algorithm *algo){

	CMT = (page_map*)malloc(sizeof(page_map)*
	algo->li=li;
	
}
