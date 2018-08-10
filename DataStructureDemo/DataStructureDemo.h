#pragma once

#include "DataStructureC.h"
#include "List.h"
#include "Stack.h"
#include "Queue.h"
#include "HString.h"

#define	STRUCTURE_SIZE_MIN	10
#define	STRUCTURE_SIZE_MAX	30

#define	MAZE_MIN	15
#define	MAZE_MAX	20

int __cdecl CompareRoutine(ELEMENT *FirstElement, ELEMENT* SecondElement);
void __cdecl FreeRoutine(void *Buffer);
void *__cdecl AllocateRoutine(size_t ByteSize);

WORD ConsoleTextColor(WORD wColor);

void ShowList(int cat);
void ShowStack(int i);
void ShowQueue();
void ShowHString();

