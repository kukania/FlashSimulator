#include <stdlib.h>
#include <stdint.h>
#include "demand.h"

struct algorithm __demand={
	.create = demand_create,
	.destroy = demand_destroy,
	.get = demand_get,
	.set = demand_set,
	.remove = demand_remove
};

page_map *CMT;
page_map *GTD;
int pivot = 0;
int *block_OOB = (int*)malloc(sizeof(int) * NOB);
memset(block_OOB, 0, sizeof(int) * NOB);

uint32_t demand_create(lower_info *li, algorithm *algo){
	/* Table Alloc */
	GTD = (page_map*)malloc(GTDSIZE);
	CMT = (page_map*)malloc(CMTSIZE);
	algo->li = li;

	/* SRAM Init */
	for(int i = 0; i < GTDSIZE / 8; i++){
		GTD[i].lpa = -1;
		GTD[i].ppa = -1;
	}
	for(int i = 0; i < CMTSIZE / 8; i++){
		CMT[i].lpa = -1;
		CMT[i].ppa = -1;
	}
}

void demand_destroy(lower_info *li, algorithm *algo){
	//??
}

uint32_t demand_get(const request *req){
	int lpa;
	int ppa;
	int t_ppa;
	int CMT_i;
	char* page_table;
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	demand_params->parents = req;
	demand_params->test = -1;
	
	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->params = (void*)params;

	lpa = req->key;
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){
		__demand.li->pull_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
	}
	else{
		demand_eviction(&CMT_i);
		t_ppa = GTD[lpa / TABLE_CAPACITY].ppa;
		__demand.li->pull_data(t_ppa, PAGESIZE, page_table, 0, my_req, 0); 
		ppa = page_table[lpa % TABLE_CAPACITY].ppa;
		CMT[CMT_i].lpa = lpa;
		CMT[CMT_i].ppa = ppa;
		__demand.li->pull_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
	}
}

uint32_t demand_set(const request *req){
	int lpa; //lpa of data page
	int ppa; //ppa of data page
	int t_ppa; //ppa of translation page
	int CMT_i; //index of CMT
	int GTD_i; //index of GTD
	char* page_table;
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	demand_params->parents = req;
	demand_params->test = -1;

	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->params = (void*)params;

	lpa = req->key
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){ // check CACHE
		ppa = dp_alloc();//please add data_page_allocation function
		__demand.li->push_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		CMT[CMT_i].ppa = ppa;
		/*GTD_i = (lpa)/TABLE_CAPACITY;
		t_ppa = GTD[GTD_i].ppa;
		__demand.li->pull_data(t_ppa, PAGESIZE, page_table, 0. my_req, 0);
		page_table[lpa % TABLE_CAPACITY].ppa = ppa;
		t_ppa = tp_alloc(); //please add new translation page allocation function
		__demand.li->push_data(t_ppa, PAGESIZE, page_table, 0, my_req, 0);
		GTD[GTD_i].ppa = t_ppa;*/
	}
	else{
		demand_eviction(&CMT_i); //please add new eviction function
		ppa = dp_alloc();
		__demand.li->push_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		CMT[CMT_i].ppa = ppa;
	}
}

bool demand_remove(const request *req){
	//??
}

void *demand_end_req(algo_req* input){
	demand_params* params = (demand_params*)input->params;

	request *res = params->parents;
	res->end_req(res);

	free(params);
	free(input);
}

int CMT_check(int lpa, int *ppa){
	for(int i = 0; i < CMTSIZE / 8; i++){
		if(CMT[i].lpa==lpa){
			*ppa = CMT[i].ppa;
			return i;
		}
	}
	return -1;
}

uint32_t demand_eviction(int *CMT_i){
	int lpa;
	int ppa;
	int t_ppa;
	int *page_table;
	LRU(CMT_i);
	lpa = CMT[CMT_i].lpa;
	ppa = CMT[CMT_i].ppa;
	t_ppa = GTD[lpa / TABLE_CAPACITY].ppa
	__demand.li->pull_data(t_ppa, PAGESIZE, page_table, 0, NULL, 0); // NULL?????
	if(page_table[lpa % TABLE_CAPACITY].ppa != ppa){
		page_table[lpa % TABLE_CAPACITY].ppa = ppa;
		tp_alloc(&t_ppa);
		__demand.li->push_data(t_ppa, PAGESIZE, page_table, 0, NULL, 0);
		GTD[lpa / TABLE_CAPACITY].ppa = t_ppa;
	}
	CMT[CMT_i].lpa = -1;
	CMT[CMT_i].ppa = -1;
}

uint32_t demand_GC(){
	
}
