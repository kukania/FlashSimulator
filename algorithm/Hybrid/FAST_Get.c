#include "FAST.h"

/**
 * Function : FAST_Get(const request *req)
 * 
 * Description : 
 *  Make pull request using imformation in req.
 *  First, convert physical sector number in req to logical page number.
 *  In this process, we will search log block table first.
 *  If data is not located in log block, we will search data block table. 
 *  Using converted logical block, make posix_push_data request.
 * 
 * Returns :
 *  No returns
 */

bool FAST_Get(const request *req)
{
    //algo_req            temp;
    //char                type;
    uint32_t            key;
    char*               value;
    uint32_t            physical_address;
    FAST_Parameters*    params;
    algo_req*           my_req;

    key = req->key;
    value = req->value;

	//printf("Physical_Address: %d\n", physical_address);
    if(fast_SearchSWLogBlock(key, &physical_address) == eNOTFOUND){
        //printf("Physical_Address: %d\n", physical_address);
		if(fast_SearchRWLogBlock(key, &physical_address) == eNOTFOUND){
			//printf("Physical_Address: %d\n", physical_address);
            fast_SearchDataBlock(key, &physical_address);
        }
    }
	//printf("Test\n");
	//printf("Key: %d Translated Address: %d\n", key, physical_address);

/*    
    if(!FAST_SearchLogBlockTable(req, &physical_address)){
        FAST_SearchDataBlockTable(req, &physical_address);
    }
*/

    // Pull data using translated address
    params = (FAST_Parameters*)malloc(sizeof(FAST_Parameters));
    params->parents = req;

    my_req = (algo_req*)malloc(sizeof(algo_req));
    my_req->end_req = FAST_EndRequest;
    my_req->params = (void*)params;
    
    FAST_Algorithm.li->pull_data(physical_address, PAGESIZE, value, 0, my_req, 0);
}
