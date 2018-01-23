#ifndef _BM_HEAP_H_
#define _BM_HEAP_H_

#include "BM_common.h"
#define SWAP(a, b) {int32_t temp; temp=a; a=b; b=temp;} // 다형성을 위해 void*로 할 필요는 없을까?

uint32_t heap_maximum_extract(int* list, int size);
uint32_t heap_maximum(int* list);
void build_max_heap(int* list, int size);
void max_heapify(int* list, int count, int i);
void heapSort(int* list, int count);

#endif // !_BM_HEAP_H_
