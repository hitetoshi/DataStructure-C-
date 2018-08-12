#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl linklist_print_value(ELEMENT *elem)
{
	//elem->size==0为头节点
	if (elem->size >= sizeof(int) && elem->data)
	{
		printf("%d, ", *((int *)elem->data));
	}
	return OK;
}

bool InitOrderLinklist(LINKLIST *list, int length)
{
	bool ret = false;
	ELEMENT elem;
	int v;

	if (LinklistInit(list, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
	{
		for (int i = 0; i < length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			LinklistOrderInsert(list, &elem);
		}
		ret = true;
	}
	return ret;
}

bool InitRandomLinklist(LINKLIST *list, int length, int *max)
{
	ELEMENT elem;
	int v;
	bool ret = false;
	size_t pos;

	if (max)
	{
		*max = 0;
	}
	if (LinklistInit(list, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
	{
		for (int i = 0; i < length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			pos = RangeRandom(0, (int)LinklistLength(list) + 1);	//插入线性表位置0-线性表长度随机
			if (LinklistInsert(list, pos, &elem) == OK)
			{
				if (max)
				{
					if (v > *max)
					{
						*max = v;
					}
				}
			}
		}
		ret = true;
	}
	return ret;
}

void ShowLinklist()
{
	LINKLIST linklist1, linklist2, linklist3;
	Status status;
	ELEMENT elem;
	LINKLISTPOSITION p1, p2;
	int max = 0;

	printf_color(FOREGROUND_GREEN,"线性链表的演示\n\n");
	srand((unsigned int)time(NULL));

	if (InitRandomLinklist(&linklist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), &max))
	{
		printf("线性链表1长度: %zd\n", LinklistLength(&linklist1));
		LinklistTraverse(&linklist1, linklist_print_value);
		printf("\b\b  \n");
		printf("线性链表1参考基准元素: %d\n", max);

		elem.data = &max;
		elem.size = sizeof(int);
		p1 = LinklistLocateElem(&linklist1, &elem);
		if(p1)
		{
			printf("线性链表1元素 %d 定位: %d\n", max, *((int *)p1->elem.data));

			p2 = LinklistPriorPos(&linklist1, p1);
			if (p2)
			{
				if (p2 == LinklistHeader(&linklist1))
				{
					printf("线性链表1元素 %d 没有前驱元素\n", *((int *)p1->elem.data));
				}
				else
				{
					printf("线性链表1元素 %d 前驱元素: %d\n", *((int *)p1->elem.data), *((int *)p2->elem.data));
				}
			}
			else
			{
				printf("线性链表1元素 %d 没有前驱元素\n", *((int *)p1->elem.data));
			}
			p2 = LinklistNextPos(&linklist1, p1);
			if (p2)
			{
				printf("线性链表1元素 %d 后继元素: %d\n", *((int *)p1->elem.data), *((int *)p2->elem.data));
			}
			else
			{
				printf("线性链表1元素 %d 没有后继元素\n", *((int *)p1->elem.data));
			}

			status=LinklistDeletePos(&linklist1, p1);
			if (status == OK)
			{
				printf("删除线性链表1位置的元素 %d 后的链表:\n", max);
				LinklistTraverse(&linklist1, linklist_print_value);
				printf("\b\b  \n");
			}
			else
			{
				printf("删除线性链表1的元素 %d 失败\n", max);
			}
		}
		printf("\n");

		if (InitRandomLinklist(&linklist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), NULL))
		{
			printf("线性链表2长度: %zd\n", LinklistLength(&linklist2));
			LinklistTraverse(&linklist2, linklist_print_value);
			printf("\b\b  \n\n");
			LinklistUnio(&linklist1, &linklist2);
			printf("合并线性链表1和线性链表2后的长度: %zd\n", LinklistLength(&linklist1));
			LinklistTraverse(&linklist1, linklist_print_value);
			printf("\b\b  \n");
			LinklistDestroy(&linklist2);
		}

		LinklistDestroy(&linklist1);
	}

	printf("\n");
	if (InitOrderLinklist(&linklist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
	{
		printf("构造有序线性链表1, 长度: %zd\n", LinklistLength(&linklist1));
		LinklistTraverse(&linklist1, linklist_print_value);
		printf("\b\b  \n");

		if (InitOrderLinklist(&linklist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
		{
			printf("\n构造有序线性链表2, 长度: %zd\n", LinklistLength(&linklist2));
			LinklistTraverse(&linklist2, linklist_print_value);
			printf("\b\b  \n");

			if (LinklistInit(&linklist3, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
			{
				LinklistMerge(&linklist1, &linklist2, &linklist3);
				printf("\n归并有序线性链表1和2, 新有序链表长度: %zd\n", LinklistLength(&linklist3));
				LinklistTraverse(&linklist3, linklist_print_value);
				printf("\b\b  \n");
				LinklistDestroy(&linklist3);
			}
			LinklistDestroy(&linklist2);
		}
		LinklistDestroy(&linklist1);
	}
}