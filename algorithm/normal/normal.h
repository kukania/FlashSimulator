#include "../../include/container.h"
typedef struct normal_params{
	request *parents;
	int test;
}normal_params;

uint32_t normal_create (lower_info*, algorithm *);
void normal_destroy (lower_info*,  algorithm *);
uint32_t normal_get(const request *);
uint32_t normal_set(const request *);
uint32_t normal_remove(const request*);
void *normal_end_req(algo_req*);
