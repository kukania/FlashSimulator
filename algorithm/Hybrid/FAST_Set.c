#include "FAST.h"

/**
 * Function :
 * 
 * Description : 
 * 
 * Returns :
 *  No returns
 */

bool FAST_Set(const request* req)
{
    FAST_Parameters*    params;
    algo_req*           my_req;
    KEYT                key;
    char*               value;
    uint32_t            physical_address;

    key = req->key;
    value = req->value;    

    // Find page to write
    if(fast_AllocDataBlockEntry(key, &physical_address) == 0){
        if(fast_AllocSWLogBlockEntry(key, &physical_address) == 0){
            fast_AllocRWLogBlockEntry(key, &physical_address);
        }
    }

    // Push data using translated address
    params = (FAST_Parameters*)malloc(sizeof(FAST_Parameters));
    params->parents = req;
    
    my_req = (algo_req*)malloc(sizeof(algo_req));
    my_req->end_req = FAST_EndRequest;
    my_req->params = (void*)params;

    FAST_Algorithm.li->push_data(physical_address, PAGESIZE, value, 0, my_req, 0);
}