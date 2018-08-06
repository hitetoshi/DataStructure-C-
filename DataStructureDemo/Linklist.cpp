#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl linklist_print_value(NODEELEMENT *elem)
{
	printf("%d, ", *((int *)elem->data));
	return OK;
}

void ShowLinklist()
{
	LINKLIST linklist1;
	Status status;
	int structure_length;
	NODEELEMENT elem, elem2;
	int v;
	int max = 0;
	size_t pos;

	elem.data = &v;
	elem.size = sizeof(v);
	v = 1;

	srand((unsigned int)time(NULL));
	status = LinklistInit(&linklist1, CompareRoutine, AllocateRoutine, FreeRoutine);
	printf("初始化线性链表1: %d\n", status);
	if (status == OK)
	{
		structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//线性表长度10-30随机
		printf("随机线性链表1长度: %d\n", structure_length);
		for (int i = 0; i < structure_length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			pos = RangeRandom(0, (int)LinklistLength(&linklist1) + 1);	//插入线性表位置0-线性表长度随机
			status = LinklistInsert(&linklist1, LinklistLength(&linklist1), &elem);
			if (v > max)
			{
				max = v;
			}
		}
		printf("线性链表1长度: %d\n", LinklistLength(&linklist1));
		LinklistTraverse(&linklist1, linklist_print_value);
		printf("\b\b  \n");
		printf("线性链表1参考基准元素: %d\n", max);

		elem.data = &max;
		elem.size = sizeof(int);
		pos = LinklistLocate(&linklist1, &elem);
		printf("线性链表1元素 %d 位置: %d\n", max, pos);

		LinklistDestroy(&linklist1);
	}
}