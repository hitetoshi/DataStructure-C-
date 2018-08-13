#include "DataStructureC.h"

int RangeRandom(int range_min, int range_max)
{
	int i = rand();
	return (int)((double)rand() / (RAND_MAX + 1.0)*(range_max - range_min) + range_min);
}

void * __cdecl CommonAllocRotuine(size_t bytes)
{
	return CMEM_ALOC(bytes);
}

void __cdecl CommonFreeRoutine(void *block)
{
	CMEM_FREE(block);
}

int __cdecl CommonIntCompareRoutine(ELEMENT *first, ELEMENT *second)
{
	return *((int *)first->data) - *((int *)second->data);
}
