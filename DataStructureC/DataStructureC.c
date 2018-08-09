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

size_t _strlen(char *s)
{
	size_t i = 0;

	while (s[i++]);
	return i;
}

Status _isdigit(int c)
{
	return (c >= '0' && c <= '9');
}
