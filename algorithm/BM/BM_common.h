#ifndef _BM_COMMON_H_
#define _BM_COMMON_H_

#include <stdint.h>



/* error macro */
#define ERR(e)	printf("Error: %d\n", (int32_t)e); return e;








/*
* Macro Definitions
*/
#define ERR_ENCODE_ERROR_CODE(base,no)      ( -1 * (((base) << 16) | no) )

/*
* Error Base Definitions
*/
#define OM_ERR_BASE                              6

/*
* Error Definitions for OM_ERR_BASE
*/
#define eHEAPUNDERFLOW_BM                        ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,0)

/*
#define eBADOBJECTID_OM                          ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,1)
#define eBADCATALOGOBJECT_OM                     ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,2)
#define eBADLENGTH_OM                            ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,3)
#define eBADSTART_OM                             ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,4)
#define eBADFILEID_OM                            ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,5)
#define eBADUSERBUF_OM                           ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,6)
#define eBADPAGEID_OM                            ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,7)
#define eTOOLARGESORTKEY_OM                      ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,8)
#define eCANTALLOCEXTENT_BL_OM                   ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,9)
#define NUM_ERRORS_OM_ERR_BASE                   10
#define eNOTSUPPORTED_EDUOM			             ERR_ENCODE_ERROR_CODE(OM_ERR_BASE,11)
*/


#endif // !_BM_COMMON_H_