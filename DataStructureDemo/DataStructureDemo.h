#pragma once

#include "..\DataStructureC\list.h"

#define	STRUCTURE_SIZE_MIN	10
#define	STRUCTURE_SIZE_MAX	31

int __cdecl CompareRoutine(NODEELEMENT *FirstElement, NODEELEMENT* SecondElement);
void __cdecl FreeRoutine(void *Buffer);
void *__cdecl AllocateRoutine(size_t ByteSize);

int RangeRandom(int range_min, int range_max);
