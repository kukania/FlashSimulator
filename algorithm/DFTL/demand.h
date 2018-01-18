#include "../../include/container.h"
#include "../../algorithm/page/page.h"

#define TABLE_CAPACITY (PAGESIZE / 8) //Number of table entries in a page
#define NTP (TOTALSIZE / TABLE_CAPACITY) //Number of Translation Page
#define	GTDSIZE 8 * NTP
#define CMTSIZE CACHESIZE - GTDSIZE

typedef struct cached_table{
	int lpa;
	int ppa;
}cached_table;

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
int CMT_check(int lpa, int *ppa);
uint32_t demand_eviction(int *CMT_i);
uint32_t demand_GC();
