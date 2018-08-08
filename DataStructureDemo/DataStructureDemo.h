#pragma once

#include "..\DataStructureC\DataStructureC.h"

#define	STRUCTURE_SIZE_MIN	10
#define	STRUCTURE_SIZE_MAX	31

#define	MAZE_MIN	15
#define	MAZE_MAX	21

int __cdecl CompareRoutine(ELEMENTNODE *FirstElement, ELEMENTNODE* SecondElement);
void __cdecl FreeRoutine(void *Buffer);
void *__cdecl AllocateRoutine(size_t ByteSize);

int RangeRandom(int range_min, int range_max);

WORD ConsoleTextColor(WORD wColor);

void ShowSqlist();
void ShowLinklist();
void ShowPolynmail();
void ShowStack(int i);

