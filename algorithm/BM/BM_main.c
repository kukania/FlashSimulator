#include <stdio.h>
#include "BM.h"

void print_arr(int32_t *arr, uint32_t size)
{
	int i = 0;
	for (; i < size; ++i)
		printf("%d ", arr[i]);
	printf("\n");
}
int32_t main(void)
{
	int32_t arr[10] = { 4,1,2,5,8,6,3,12,62,43 };

	print_arr(arr, 10);
	//max_heapify(arr, 10, 0);
	//heapSort(arr, 10);
	build_max_heap(arr, 10);
	print_arr(arr, 10);

	//realloc ½ÇÇè
	int* numarray = (int*)malloc(sizeof(int) * 10);
	for (int i = 0; i < 10; ++i)
		numarray[i] = i;
	print_arr(numarray, 10);
	numarray = (int*)realloc(numarray, sizeof(int) * 9);
	print_arr(numarray, 10);

	//ERR(eHEAPUNDERFLOW_BM);

	build_max_heap(numarray, 9);
	print_arr(numarray, 9);
	printf("Maximum value: %d\n", heap_maximum_extract(numarray, 9));
	print_arr(numarray, 9);
}