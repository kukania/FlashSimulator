#include <string.h>
#include <stdlib.h>
#include "block.h"

//#define SIZEOFTABLE SOB/SOP
//#define SIZEOFTABLE NOB

/* Mapping table in RAM */
//int block_maptable[SIZEOFTABLE]; // Is this okay? int or uint32_t?
int32_t *block_maptable; // pointer to LPA->PPA table 
int8_t *exist_table; 
int8_t *block_valid_array;
uint32_t set_pointer = 0;

//#define PPB 4 // temporally
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

	exist_table = (int8_t*)malloc(sizeof(int8_t)*li->NOP); // 지금은 그냥 NOP짜리 char array인데, 나중에는 NOP/8만큼 하여 bit 연산으로 바꿀 수도 있을 것.
	for (i = 0; i < li->NOP; ++i)
		exist_table[i] = NONEXIST;

	block_valid_array = (int8_t*)malloc(sizeof(int8_t)*li->NOB); // 지금은 block_maptable과 같은 사이즈로 다른 array를 만들었지만, block_maptable의 item 중 MSB로 표기할 수도 있을 것
	for (i = 0; i < li->NOB; ++i)
		block_valid_array = ERASE; // 0 means ERASED, 1 means VALID
	// memset(block_valid_array, 0, li->NOB * li->SOB); 이래도 되지 않을까?

}
void block_destroy (lower_info* li, algorithm *algo){

	free(block_maptable);
	free(exist_table);
}
uint32_t block_get(const request *req){
	block_params* params=(block_params*)malloc(sizeof(block_params));
	params->parents=req;
	params->test=-1;

	uint32_t LBA = params->parents->key / __block.li->PPB;
	uint32_t offset = params->parents->key % __block.li->PPB;

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


	uint32_t LBA = params->parents->key / __block.li->PPB;
	uint32_t offset = params->parents->key % __block.li->PPB;
	uint32_t PBA;
	uint32_t PPA;
	
	// set_pointer를 현재부터 키우면서 가장 먼저 만나는 ERASE 자리로 두기
	while (block_valid_array[set_pointer] != ERASE) {
		set_pointer++;
	}

	if (block_maptable[LBA] == NIL)
	{
		// 그 block의 valid_array를 E에서 V로 교체
		block_valid_array[set_pointer] = VALID;

		// maptable에 그 mapping되는 PBA 쓰기
		block_maptable[LBA] = set_pointer;

		//PBA = block_maptable[LBA];
		//PPA = PBA + offset;
		PPA = set_pointer + offset; // 위 두 줄이랑 같다

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
		PPA = PBA + offset;

		if (exist_table[PPA] == NONEXIST)
		{
			exist_table[PPA] = EXIST;
			algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
			my_req->end_req = block_end_req;
			my_req->params = (void*)params;
			__block.li->push_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
		}
		else
		{
			// Cleaning

			// set_pointer를 가장 첫 번째 ERASE 자리로 맞춰놓기
			set_pointer = 0;
			while (block_valid_array[set_pointer] != ERASE) {
				set_pointer++;
			}

			// 새로 옮길 블록을 위한 maptable 업데이트
			block_maptable[LBA] = set_pointer;
			block_valid_array[set_pointer] = VALID;
			block_valid_array[PBA] = ERASE; // PBA means old_PBA

			uint32_t new_PBA = block_maptable[LBA];

			// 새로 옮길 블록으로 옮기기
			char* temp_block = (uint32_t*)malloc(sizeof(uint32_t)*__block.li->PPB); // 이거 vale를 제대로 담을 수 있는 걸로 만들어야..
			int i;
			for (i = 0; i<__block.li->PPB; ++i)
			{
				if (i == offset) {
					exist_table[PPA] = NONEXIST;
					exist_table[new_PBA + offset] = EXIST;

					algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
					my_req->end_req = block_end_req;
					my_req->params = (void*)params;
					__block.li->push_data(new_PBA + offset, PAGESIZE, req->value, 0, my_req, 0);
				}
				else if (exist_table[i] == EXIST) {
					temp_block[i] = read(PBA + i); // 제대로 된 READ로 바꾸기

					exist_table[PBA + i] = NONEXIST;
					exist_table[new_PBA + i] = EXIST;
					algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
					my_req->end_req = block_end_req;
					my_req->params = (void*)params;
					__block.li->push_data(new_PBA + i, PAGESIZE, temp_block+i, 0, my_req, 0);
				}
			}
			trim(PBA); // 제대로 된 TRIM으로 바꾸기
			free(temp_block);
		}
	}



	/*
	// set_pointer를 현재부터 키우면서 가장 먼저 만나는 ERASE 자리로 두기
	while (block_valid_array[set_pointer] != ERASE) {
		set_pointer++;
	}


	if (block_maptable[LBA] == NIL){
		// First write in the block
		//block_maptable[LBA] = LBA;
		block_maptable[set_pointer] = LBA;
		block_valid_array[set_pointer] = 1; // Change E -> V
		set_pointer++;

		

		// write
		algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
		my_req->end_req = block_end_req;
		my_req->params = (void*)params;

		PPA = LBA + offset;
		exist_table[PPA] = 1;
		__block.li->push_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
	}
	else if (block_maptable[LBA] != NIL) {
		PBA = block_maptable[LBA];
		PPA = PBA + offset;

		if (exist_table[PPA] == 0) {
			// This offset is first. just write

			algo_req *my_req = (algo_req*)malloc(sizeof(algo_req));
			my_req->end_req = block_end_req;
			my_req->params = (void*)params;

			exist_table[PPA] = 1;
			__block.li->push_data(PPA, PAGESIZE, req->value, 0, my_req, 0);
		}
		else {
			// Cleaning
			
		}
	}
	*/
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
