#include <string.h>
#include <stdlib.h>
#include "block.h"

//#define SIZEOFTABLE SOB/SOP
//#define SIZEOFTABLE NOB

/* Mapping table in RAM */
//int block_maptable[SIZEOFTABLE]; // Is this okay? int or uint32_t?
int32_t *block_maptable; // pointer to LPA->PPA table 
#define PPB 4 // temporally


struct algorithm __block={
	.create=block_create,
	.destroy=block_destroy,
	.get=block_get,
	.set=block_set,
	.remove=block_remove
};

uint32_t block_create (lower_info* li,algorithm *algo){
	algo->li=li;

	block_maptable = (int32_t*)malloc(sizeof(int32_t) * li->NOB);
	int32_t i=0;
	for (; i<li->NOB; ++i){ // maptable initialization
		block_maptable[i] = -1;
	}

}
void block_destroy (lower_info* li, algorithm *algo){

	free(block_maptable);
}
uint32_t block_get(const request *req){
	//__block->li->NOB;
	block_params* params=(block_params*)malloc(sizeof(block_params));
	params->parents=req;
	params->test=-1;

	uint32_t LBA = params->parents->key / __block->li->PPB;
	uint32_t offset = params->parents->key % __block->li->PPB;

	uint32_t PBA = block_maptable[LBA];
	uint32_t PPA = PBA + offset;

	//KEYT key_ = PPA;

	algo_req *my_req=(algo_req*)malloc(sizeof(algo_req));
	my_req->end_req=block_end_req;
	my_req->params=(void*)params;

	//__block.li->pull_data(req->key,PAGESIZE,req->value,0,my_req,0);
	__block.li->pull_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
}
uint32_t block_set(const request *req){
	block_params* params=(block_params*)malloc(sizeof(block_params));
	params->parents=req;
	params->test=-1;


	uint32_t LBA = params->parents->key / PPB;
	uint32_t offset = params->parents->key % PPB;

	if (block_maptable[LBA] == -1){
		// First write
		block_maptable[LBA] = LBA;

		// write
		algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
		my_req->end_req = block_end_req;
		my_req->params = (void*)params;

		__block.li->push_data(LBA, PAGESIZE, req->value, 0, my_req, 0);
	}
	// Follwings are should be considered with ERASE operation
	/*
	else if (block_maptable[LBA] != LBA){ // HOW?


	algo_req *my_req=(algo_req*)malloc(sizeof(algo_req));
	my_req->end_req=block_end_req;
	my_req->params=(void*)params;

	__block.li->push_data(req->key,PAGESIZE,req->value,0,my_req,0);
	*/
}
bool block_remove(const request *req){
//	block->li->trim_block()
}

void *block_end_req(algo_req* input){
	block_params* params=(block_params*)input->params;
	
	request *res=params->parents;
	res->end_req(res);

	free(params);
	free(input);
}
