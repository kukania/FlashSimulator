#include "FAST.h"

/**
 * Function :
 * 
 * Description : 
 * 
 * Returns :
 *  No returns
 * 
 * Side effects :
 *  ???
 * 
 */

void* FAST_EndRequest(algo_req *input)
{
    FAST_Parameters* params = (FAST_Parameters*)input->params;

    request* req = params->parents;
    req->end_req(req);

    free(params);
    free(input);
}