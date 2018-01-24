#include "../../include/container.h"
#include "demand_queue.h"

#define CACHESIZE (4*M)
#define EPP (PAGESIZE / sizeof(D_TABLE)) //Number of table entries per page
#define NTP (_NOP / EPP) //Number of Translation Page
#define	GTDSIZE (sizeof(D_TABLE) * NTP)
#define CMTSIZE (CACHESIZE - GTDSIZE)
//#define CMTSIZE (sizeof(C_TABLE) * ((CACHESIZE - GTDSIZE) / sizeof(C_TABLE)))
#define D_IDX (lpa/EPP)	// Idx of directory table
#define P_IDX (lpa%EPP)	// Idx of page table
#define GTDENT (GTDSIZE/sizeof(D_TABLE))	// Num of GTD entries
#define CMTENT (CMTSIZE/sizeof(C_TABLE))	// Num of CMT entries

typedef struct cached_table{
	int32_t lpa;
	int32_t ppa;
	unsigned char flag; // 0: unchanged, 1: changed
	LINKED_LIST *queue_ptr;
}C_TABLE;

typedef struct demand_mapping_table{
	int32_t ppa; //Index = lpa
}D_TABLE;

typedef struct demand_OOB{
	int32_t reverse_table;
	unsigned char valid_checker; // 0: invalid, 1: valid
}D_OOB;

typedef struct demand_SRAM{
	int32_t lpa_RAM;
	PTR PTR_RAM;
}D_SRAM;

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
int CMT_check(uint32_t lpa, uint32_t *ppa);
uint32_t demand_eviction(int *CMT_i);
char btype_check(uint32_t PBA_status);
void batch_update();
void demand_GC(uint32_t PBA_status);
void dp_alloc(uint32_t *ppa);
void tp_alloc(uint32_t *t_ppa);
