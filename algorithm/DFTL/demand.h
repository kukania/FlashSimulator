#include "../../include/container.h"

#define TABLE_CAPACITY (PAGESIZE/8) //Number of table entries in a page
#define NTP (TOTALSIZE/TABLE_CAPACITY) //Number of Translation Page

struct page_map{
	int lpa;
	int ppa;
};

typedef struct demand_params{
	request *parents;
	int test;
}demand_params;

uint32_t demand_create(lower_info*, algorithm*);
void demand_destroy(lower_info*, algorithm*);
uint32_t demand_get(const request*);
uint32_t demand_set(const request*);
uint32_t demand_remove(const request*);
void *demand_end_req(algo_req*);
