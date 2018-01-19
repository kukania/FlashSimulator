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

C_TABLE *CMT;
D_TABLE *GTD;
D_OOB *demand_OOB;
D_SRAM *d_sram;

uint32_t demand_create(lower_info *li, algorithm *algo){

	/* Table Alloc */
	GTD = (D_TABLE*)malloc(GTDSIZE);
	CMT = (C_TABLE*)malloc(CMTSIZE);
	demand_OOB = (D_OOB*)malloc(sizeof(D_OOB) * _NOB);
	d_sram = (D_SRAM*)malloc(sizeof(D_SRAM) * _NOB);
	algo->li = li;

	/* SRAM Init */
	for(int i = 0; i < GTDSIZE / sizeof(D_TALBE); i++){
		GTD[i].ppa = -1;
	}
	for(int i = 0; i < CMTSIZE / sizeof(C_TABLE); i++){
		CMT[i].lpa = -1;
		CMT[i].ppa = -1;
		CMT[i].flag = 0;
		CMT[i].queue_ptr = NULL;
	}
	for(int i = 0; i < _NOB; i++){
		d_sram->lpa_RAM = -1;
		d_sram->VPTR_RAM = NULL;
	memset(demand_OOB, 0, sizeof(D_OOB) * _NOB);
}

void demand_destroy(lower_info *li, algorithm *algo){
	free(CMT);
	free(GTD);
	free(demand_OOB);
}

uint32_t demand_get(const request *req){
	int lpa;
	int ppa;
	int t_ppa;
	int CMT_i;
	D_TABLE* d_table;
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	demand_params->parents = req;
	demand_params->test = -1;
	
	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->params = (void*)params;

	lpa = req->key;
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){
		queue_update(CMT[CMT_i].queue_ptr);
		__demand.li->pull_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
	}
	else{
		demand_eviction(&CMT_i);
		t_ppa = GTD[lpa / EPP].ppa;
		__demand.li->pull_data(t_ppa, PAGESIZE, (V_PTR)d_table, 0, my_req, 0); 
		ppa = d_table[lpa % EPP].ppa;
		CMT[CMT_i]={lpa, ppa, 0, queue_insert((void*)(CMT + CMT_i))}; //Is it possible ???
		__demand.li->pull_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		free(d_table);
	}
}

uint32_t demand_set(const request *req){
	int lpa; //lpa of data page
	int ppa; //ppa of data page
	int CMT_i; //index of CMT
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	demand_params->parents = req;
	demand_params->test = -1;

	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->params = (void*)params;

	lpa = req->key;
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){ // check CACHE
		ppa = dp_alloc();//please add data_page_allocation function
		__demand.li->push_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		CMT[CMT_i] = {, ppa, 1, } // Is it possible???
		queue_update(CMT[CMT_i].queue_ptr);
	}
	else{
		demand_eviction(&CMT_i); //Handling initial cycle in eviction
		ppa = dp_alloc();
		__demand.li->push_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		CMT[CMT_i] = {lpa, ppa, 1, queue_insert((void*)(CMT + CMT_i))
	}
}

bool demand_remove(const request *req){
	int lpa;
	int t_ppa;
	D_TABLE *d_table;
	
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	demand_params->parents = req;
	demand_params->test = -1;

	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->parmas = (void*)params;

	lpa = req->key;
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){
		queue_delete(CMT[CMT_i].queue_ptr);
		CMT[CMT_i] = {-1, -1, 0, NULL};
		t_ppa = GTD[lpa / EPP].ppa;
		__demand.li->pull_data(t_ppa, PAGESIZE, (V_PTR)d_table, 0, NULL, 0);
		d_table[lpa % EPP].ppa = -1;
		t_ppa = tp_alloc();
		__demand.li->push_data(t_ppa, PAGESIZE, (V_PTR)d_table, 0, NULL, 0);
}

void *demand_end_req(algo_req* input){
	demand_params* params = (demand_params*)input->params;

	request *res = params->parents;
	res->end_req(res);

	free(params);
	free(input);
}

int CMT_check(int lpa, int *ppa){
	for(int i = 0; i < CMTSIZE / sizeof(C_TABLE); i++){
		if(CMT[i].lpa==lpa){
			*ppa = CMT[i].ppa;
			return i;
		}
	}
	return -1;
}

//Handling when cache is empty or first t_page write
uint32_t demand_eviction(int *CMT_i){
	int lpa;
	int ppa;
	int t_ppa;
	D_TABLE *d_table;
	LRU(CMT_i);
	lpa = CMT[CMT_i].lpa;
	ppa = CMT[CMT_i].ppa;
	t_ppa = GTD[lpa / EPP]
	__demand.li->pull_data(t_ppa, PAGESIZE, (V_PTR)d_table, 0, NULL, 0); // NULL?????
	if(d_table[lpa % EPP] != ppa){
		d_table[lpa % EPP] = ppa;
		tp_alloc(&t_ppa);
		__demand.li->push_data(t_ppa, PAGESIZE, (V_PTR)d_table, 0, NULL, 0);
		GTD[lpa / EPP] = t_ppa;
	}
	CMT[CMT_i].lpa = -1;
	CMT[CMT_i].ppa = -1;
	free(d_table);
}

uint32_t demand_GC(){
	
}
