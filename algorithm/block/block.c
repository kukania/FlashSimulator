#include <string.h>
#include <stdlib.h>
#include "block.h"

int32_t *block_maptable; // pointer to LPA->PPA table 
int8_t *exist_table; 
int8_t *block_valid_array;
uint32_t set_pointer = 0;

#define VALID 1
#define ERASE 0
#define NIL -1
#define EXIST 1
#define NONEXIST 0

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
		block_maptable[i] = NIL;
	}

	exist_table = (int8_t*)malloc(sizeof(int8_t)*li->NOP);
	for (i = 0; i < li->NOP; ++i)
		exist_table[i] = NONEXIST;

	block_valid_array = (int8_t*)malloc(sizeof(int8_t)*li->NOB);
	for (i = 0; i < li->NOB; ++i)
		block_valid_array[i] = ERASE; // 0 means ERASED, 1 means VALID
	// memset(block_valid_array, 0, li->NOB * li->SOB); 

}
void block_destroy (lower_info* li, algorithm *algo){

	free(block_maptable);
	free(exist_table);
	free(block_valid_array);
}
uint32_t block_get(const request *req){
	block_params* params=(block_params*)malloc(sizeof(block_params));
	params->parents=req;
	params->test=-1;

	uint32_t LBA = params->parents->key / __block.li->PPB;
	uint32_t offset = params->parents->key % __block.li->PPB;

	uint32_t PBA = block_maptable[LBA];
	uint32_t PPA = PBA * __block.li->PPB + offset;

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

	uint32_t LBA = params->parents->key / __block.li->PPB;
	uint32_t offset = params->parents->key % __block.li->PPB;
	uint32_t PBA;
	uint32_t PPA;
	//uint32_t PPB = __block.li->PPB;
	int8_t checker = 0;

	//Set set_pointer to first-meet ERASE index from current set_pointer
	for (; set_pointer < __block.li->NOB; ++set_pointer) {
		if (block_valid_array[set_pointer] == ERASE) {
			checker = 1;
			break;
		}
	}
	if (checker == 0) {
		for (set_pointer =0; set_pointer < __block.li->NOB; ++set_pointer)
			if (block_valid_array[set_pointer] == ERASE)
				break;
	}
	if (block_maptable[LBA] == NIL)
	{
		// Switch E to V of block_valid_array
		block_valid_array[set_pointer] = VALID;

		// Write PBA of mapping result in maptable
		block_maptable[LBA] = set_pointer;

		//PBA = block_maptable[LBA];
		//PPA = PBA * PPB + offset;
		PPA = set_pointer * __block.li->PPB + offset; // Equal to above 2 lines

		exist_table[PPA] = EXIST;

		// write
		algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
		my_req->end_req = block_end_req;
		my_req->params = (void*)params;
		__block.li->push_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
	}

	else
	{
		PBA = block_maptable[LBA];
		PPA = PBA  * __block.li->PPB + offset;

		if (exist_table[PPA] == NONEXIST && offset == 0)
		{
			exist_table[PPA] = EXIST;
			block_valid_array[PBA] = VALID;
			algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
			my_req->end_req = block_end_req;
			my_req->params = (void*)params;
			__block.li->push_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
		}
		else if (exist_table[PPA] == NONEXIST)
		{
			exist_table[PPA] = EXIST;
			algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
			my_req->end_req = block_end_req;
			my_req->params = (void*)params;
			__block.li->push_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
		}
		else if (exist_table[PPA] != NONEXIST)
		{
			// Cleaning
			// Maptable update for data moving
			block_maptable[LBA] = set_pointer;
			block_valid_array[set_pointer] = VALID;
			block_valid_array[PBA] = ERASE; // PBA means old_PBA

			uint32_t new_PBA = block_maptable[LBA];
			uint32_t new_PPA = new_PBA * __block.li->PPB + offset;

			// Data move to new block
			uint8_t* temp_block = (uint8_t*)malloc(sizeof(uint8_t)*__block.li->PPB); // 이거 vale를 제대로 담을 수 있는 걸로 만들어야..
			int i;

			/* Followings: ASC consideartion */

			for (i = 0; i < offset; ++i) {
				algo_req *temp_req=(algo_req*)malloc(sizeof(algo_req));
				temp_req->end_req=block_end_req;
				temp_req->params=(void*)params;
				__block.li->pull_data(PBA* __block.li->PPB + i, PAGESIZE, temp_block+i, 0, temp_req, 0);

				exist_table[PBA * __block.li->PPB + i] = NONEXIST;
				exist_table[new_PBA * __block.li->PPB + i] = EXIST;

				algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
				my_req->end_req = block_end_req;
				my_req->params = (void*)params;
				__block.li->push_data(new_PBA * __block.li->PPB + i, PAGESIZE, temp_block+i, 0, my_req, 0);
			}

			algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
			my_req->end_req = block_end_req;
			my_req->params = (void*)params;
			__block.li->push_data(new_PPA, PAGESIZE, req->value, 0, my_req, 0);
			
			if (offset < __block.li->PPB) {
				for (i = offset + 1; i < __block.li->PPB; ++i) {
					algo_req *temp_req = (algo_req*)malloc(sizeof(algo_req));
					temp_req->end_req = block_end_req;
					temp_req->params = (void*)params;
					__block.li->pull_data(PBA* __block.li->PPB + i, PAGESIZE, temp_block+i, 0, temp_req, 0);

					exist_table[PBA * __block.li->PPB + i] = NONEXIST;
					exist_table[new_PBA * __block.li->PPB + i] = EXIST;

					algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
					my_req->end_req = block_end_req;
					my_req->params = (void*)params;
					__block.li->push_data(new_PBA * __block.li->PPB + i, PAGESIZE, temp_block+i, 0, my_req, 0);
				}
			}

					

			/* Followings: No ASC consideration */
			/*
			for (i = 0; i<__block.li->PPB; ++i)
			{
				if (i == offset) {
					exist_table[PPA] = NONEXIST;
					exist_table[new_PPA] = EXIST;

					algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
					my_req->end_req = block_end_req;
					my_req->params = (void*)params;
					__block.li->push_data(new_PPA, PAGESIZE, req->value, 0, my_req, 0);
				}
				else if (exist_table[i] == EXIST) {
					//temp_block[i] = read(PBA + i);
					algo_req *temp_req=(algo_req*)malloc(sizeof(algo_req));
					temp_req->end_req=block_end_req;
					temp_req->params=(void*)params;
					__block.li->pull_data(PBA*__block.li->PPB+i, PAGESIZE, temp_block+i, 0, temp_req, 0);

					exist_table[PBA * __block.li->PPB + i] = NONEXIST;
					exist_table[new_PBA * __block.li->PPB + i] = EXIST;
					algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));

					my_req->end_req = block_end_req;	my_req->params = (void*)params;
					__block.li->push_data(new_PBA *__block.li->PPB + i, PAGESIZE, temp_block+i, 0, my_req, 0);
				}
			}
			*/
			//trim(PBA);
			__block.li->trim_block(PBA * __block.li->PPB + offset, false); // Is that right?
			free(temp_block);
		}
	}
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
