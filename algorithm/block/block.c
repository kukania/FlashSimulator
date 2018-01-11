#include <string.h>
#include <stdlib.h>
#include "block.h"

struct algorithm __block={
	.create=block_create,
	.destroy=block_destroy,
	.get=block_get,
	.set=block_set,
	.remove=block_remove
};

uint32_t block_create (lower_info* li,algorithm *algo){
	algo->li=li;
}
void block_destroy (lower_info* li, algorithm *algo){

}
uint32_t block_get(const request *req){
	block_params* params=(block_params*)malloc(sizeof(block_params));
	params->parents=req;
	params->test=-1;


	algo_req *my_req=(algo_req*)malloc(sizeof(algo_req));
	my_req->end_req=block_end_req;
	my_req->params=(void*)params;

	__block.li->pull_data(req->key,PAGESIZE,req->value,0,my_req,0);
}
bool block_set(const request *req){
	block_params* params=(block_params*)malloc(sizeof(block_params));
	params->parents=req;
	params->test=-1;



	algo_req *my_req=(algo_req*)malloc(sizeof(algo_req));
	my_req->end_req=block_end_req;
	my_req->params=(void*)params;

	__block.li->push_data(req->key,PAGESIZE,req->value,0,my_req,0);
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
