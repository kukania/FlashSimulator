#include "../../include/container.h"
typedef struct block_params{
	request *parents;
	int test;
}block_params;

uint32_t block_create (lower_info*, algorithm *);
void block_destroy (lower_info*,  algorithm *);
uint32_t block_get(const request *);
uint32_t block_set(const request *);
uint32_t block_remove(const request*);
void *block_end_req(algo_req*);
