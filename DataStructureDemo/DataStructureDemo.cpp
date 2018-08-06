// DataStructureDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DataStructureDemo.h"

void *__cdecl AllocateRoutine(size_t ByteSize)
{
	static int count = 0;
	count++;
	return new unsigned char[ByteSize];
}

void __cdecl FreeRoutine(void *Buffer)
{
	delete[] Buffer;
}

int __cdecl CompareRoutine(NODEELEMENT *FirstElement, NODEELEMENT* SecondElement)
{
	return (*((int *)FirstElement->data)) - (*((int *)SecondElement->data));
}

int RangeRandom(int range_min, int range_max)
{
	int i = rand();
	return (int)((double)rand() / (RAND_MAX + 1.0)*(range_max - range_min) + range_min);
}

int main(int argc, char *argv[])
{
	int cmd;

	if (argc >= 2)
	{
		cmd = atoi(argv[1]);
	}
	else
	{

	}
	switch (cmd)
	{
	case 1:
		ShowSqlist();
		break;
	case 2:
		ShowLinklist();
	case 3:
		ShowPolynmail();
	default:
		break;
	}
    return 0;
}

