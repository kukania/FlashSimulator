#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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
D_OOB *demand_OOB; // PLEASE USE OOB
D_SRAM *d_sram;

uint32_t DPA_status = 0;
uint32_t TPA_status = _NOP;

extern LINKED_LIST *head;
extern LINKED_LIST *tail;

uint32_t demand_create(lower_info *li, algorithm *algo){

	/* Table Alloc */
	GTD = (D_TABLE*)malloc(GTDSIZE);
	CMT = (C_TABLE*)malloc(CMTSIZE);
	demand_OOB = (D_OOB*)malloc(sizeof(D_OOB) * _NOB);
	d_sram = (D_SRAM*)malloc(sizeof(D_SRAM) * _NOB);
	algo->li = li;

	/* SRAM Init */
	for(int i = 0; i < GTDENT; i++){
		GTD[i].ppa = -1;
	}
	for(int i = 0; i < CMTENT; i++){
		CMT[i] = (C_TABLE){-1, -1, 0, NULL};
	}
	for(int i = 0; i < _NOB; i++){
		d_sram->lpa_RAM = -1;
		d_sram->VPTR_RAM = NULL;
	memset(demand_OOB, 0, sizeof(D_OOB) * _NOB);
	}
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
	D_TABLE* p_table;
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	params->parents = req;
	params->test = -1;
	
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
		t_ppa = GTD[D_IDX].ppa;
		__demand.li->pull_data(t_ppa, PAGESIZE, (V_PTR)p_table, 0, my_req, 0); 
		ppa = p_table[P_IDX].ppa;
		CMT[CMT_i] = (C_TABLE){lpa, ppa, 0, queue_insert((void*)(CMT + CMT_i))};
		__demand.li->pull_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		free(p_table);
	}
}

uint32_t demand_set(const request *req){
	int lpa; //lpa of data page
	int ppa; //ppa of data page
	int CMT_i; //index of CMT
	D_TABLE *p_table;
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	params->parents = req;
	params->test = -1;

	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->params = (void*)params;

	lpa = req->key;
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){ // check CACHE
		demand_OOB[ppa].valid_checker = 0;
		dp_alloc(&ppa);//please add data_page_allocation function
		__demand.li->push_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		CMT[CMT_i] = (C_TABLE){.ppa = ppa, .flag = 1}; // Is it possible???
		queue_update(CMT[CMT_i].queue_ptr);
	}
	else{
		__demand.li->pull_data(ppa, PAGESIZE, (V_PTR)p_table, 0, my_req, 0);
		ppa = p_table[P_IDX].ppa;
		demand_OOB[ppa].valid_checker = 0;
		demand_eviction(&CMT_i); //Handling initial cycle in eviction
		dp_alloc(&ppa);
		__demand.li->push_data(ppa, PAGESIZE, req->value, 0, my_req, 0);
		CMT[CMT_i] = (C_TABLE){lpa, ppa, 1, queue_insert((void*)(CMT + CMT_i))};
		demand_OOB[ppa] = (D_OOB){lpa, 1};
		free(p_table);
	}
}

bool demand_remove(const request *req){
	int lpa;
	int ppa;
	int t_ppa;
	int CMT_i;
	D_TABLE *p_table;
	
	demand_params *params = (demand_params*)malloc(sizeof(demand_params));
	params->parents = req;
	params->test = -1;

	algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
	my_req->end_req = demand_end_req;
	my_req->params = (void*)params;

	lpa = req->key;
	if((CMT_i = CMT_check(lpa, &ppa)) != -1){
		queue_delete(CMT[CMT_i].queue_ptr);
		CMT[CMT_i] = (C_TABLE){-1, -1, 0, NULL};
	}
	t_ppa = GTD[D_IDX].ppa;
	__demand.li->pull_data(t_ppa, PAGESIZE, (V_PTR)p_table, 0, my_req, 0);
	ppa = p_table[P_IDX].ppa;
	demand_OOB[ppa].valid_checker = 0;
	p_table[P_IDX].ppa = -1;
	tp_alloc(&t_ppa);
	GTD[D_IDX].ppa = t_ppa;
	__demand.li->push_data(t_ppa, PAGESIZE, (V_PTR)p_table, 0, my_req, 0);
}

void *demand_end_req(algo_req* input){
	demand_params* params = (demand_params*)input->params;

	request *res = params->parents;
	res->end_req(res);

	free(params);
	free(input);
}

int CMT_check(int lpa, int *ppa){
	for(int i = 0; i < CMTENT; i++){
		if(CMT[i].lpa==lpa){
			*ppa = CMT[i].ppa;
			return i; //CMT_i
		}
	}
	return -1; //No such lpa in CMT
}

//Handling when cache is empty or first t_page write
uint32_t demand_eviction(int *CMT_i){
	int lpa;
	int ppa;
	int t_ppa;
	D_TABLE *p_table;

	/* Check empty entry */
	for(int i = 0; i < CMTENT; i++){
		if(CMT[i].lpa == -1){
			*CMT_i = i;
			return 0;
		}
	}

	/* Eviction */
	*CMT_i = (int)(CMT - (C_TABLE*)(tail->DATA)); //Use tail of queue
	lpa = CMT[*CMT_i].lpa;
	ppa = CMT[*CMT_i].ppa;
	if(CMT[*CMT_i].flag != 0){
		if((t_ppa = GTD[D_IDX].ppa) != -1){	// Check it's first t_page
			__demand.li->pull_data(t_ppa, PAGESIZE, (V_PTR)p_table, 0, NULL, 0);
			p_table[P_IDX].ppa = ppa;
		}
		tp_alloc(&t_ppa);
		__demand.li->push_data(t_ppa, PAGESIZE, (V_PTR)p_table, 0, NULL, 0);
		GTD[D_IDX].ppa = t_ppa;
	}
	queue_delete(tail);
	CMT[*CMT_i] = (C_TABLE){-1, -1, 0, NULL};
	demand_OOB[ppa].valid_checker = 0;
	free(p_table);
}

uint32_t demand_GC(){
	return 0;
}

uint32_t dp_alloc(int *ppa){
	return 0;
}

uint32_t tp_alloc(int *t_ppa){
	return 0;	
}
