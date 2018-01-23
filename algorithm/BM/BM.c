/* Badblock Manager */
#include "BM.h"

// 기본 4가지 인터페이스

void		BM_invalidate_ppa(uint32_t PPA)
{
	// array를 통해 invalid 여부를 표시
	// parameter로 받은 PPA를 INVALID에서 VALID로 바꾸는 함수인듯

}
int8_t		BM_is_invalid_ppa(uint32_t PPA)
{
	// parameter로 받은 PPA가 VALID인지 INVALID인지 반환하는 함수인듯
	// 1bit로 하게 해도 되겠지만 status가 VALID INVALID 외에 더 있을수도 있으므로 일단 char로 반환
}
uint32_t	BM_get_gc_victim(Block *Block_list)
{
	/* victim block의 PPA를 반환하는 함수 */
	/* 
	 * Parameter: Array(Heap) of Block structure
	 * Parameter가 Heap으로 주어지므로, Heap 연산을 이용하여 cnt(P/E)가 max인 node를(max heap의 root) 찾아서 그 PPA(PBA?)를 반환한다.
	 */
}
uint32_t	BM_get_weared_block(Block *Block_list)
{
	// 이거 뭐지
}