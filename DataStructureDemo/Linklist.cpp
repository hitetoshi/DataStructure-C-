#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl linklist_print_value(NODEELEMENT *elem)
{
	//elem->size==0为头节点
	if (elem->size >= sizeof(int) && elem->data)
	{
		printf("%d, ", *((int *)elem->data));
	}
	return OK;
}

void ShowLinklist()
{
	LINKLIST linklist1, linklist2;
	Status status;
	int structure_length;
	NODEELEMENT elem;
	LINKLISTPOSITION p1, p2;
	int v;
	size_t pos;
	int max = 0;

	printf("线性链表的演示\n\n");
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
			status = LinklistInsert(&linklist1, pos, &elem);
			if (v > max)
			{
				max = v;
			}
		}
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

		status = LinklistInit(&linklist2, CompareRoutine, AllocateRoutine, FreeRoutine);
		printf("初始化线性链表2: %d\n", status);
		if (status == OK)
		{
			structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//线性表长度10-30随机
			printf("随机线性链表2长度: %d\n", structure_length);
			for (int i = 0; i < structure_length; i++)
			{
				elem.size = sizeof(int);
				v = rand();
				elem.data = &v;
				pos = RangeRandom(0, (int)LinklistLength(&linklist2) + 1);	//插入线性表位置0-线性表长度随机
				status = LinklistInsert(&linklist2, pos, &elem);
				if (v > max)
				{
					max = v;
				}
			}
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
}