#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl sqlist_print_value(NODEELEMENT *elem)
{
	if (elem->size>=sizeof(int) && elem->data)
	{
		printf("%d, ", *((int *)elem->data));
	}
	return OK;
}

void ShowSqlist()
{
	SQLIST sqlist1, sqlist2;
	NODEELEMENT elem, elem2;
	int v;
	int max = 0;
	size_t pos;
	Status status;
	int structure_length;

	printf("线性顺序表的演示\n\n");
	srand((unsigned int)time(NULL));
	status = SqlistInit(&sqlist1, CompareRoutine, AllocateRoutine, FreeRoutine);
	printf("初始化线性顺序表1: %d\n", status);
	if (status == OK)
	{
		structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//线性表长度10-30随机
		printf("随机线性顺序表1长度: %d\n", structure_length);
		for (int i = 0; i < structure_length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			pos = RangeRandom(0, (int)SqlistLength(&sqlist1) + 1);	//插入线性表位置0-线性表长度随机
			status = SqlistInsert(&sqlist1, pos, &elem);
			if (v > max)
			{
				max = v;
			}
		}
		printf("线性顺序表1长度: %zd\n", SqlistLength(&sqlist1));
		SqlistTraverse(&sqlist1, sqlist_print_value);
		printf("\b\b  \n");
		printf("线性顺序表1参考基准元素: %d\n", max);

		elem.data = &max;
		elem.size = sizeof(int);
		pos = SqlistLocate(&sqlist1, &elem);
		printf("线性顺序表1元素 %d 位置: %zd\n", max, pos);
		status = SqlistPriorElem(&sqlist1, &elem, &elem2);
		if (status == OK)
		{
			printf("线性顺序表1元素 %d 前驱元素: %d\n", *((int *)elem.data), *((int *)elem2.data));
		}
		else
		{
			printf("线性顺序表1元素 %d 没有前驱元素\n", *((int *)elem.data));
		}
		status = SqlistNextElem(&sqlist1, &elem, &elem2);
		if (status == OK)
		{
			printf("线性顺序表1元素 %d 后继元素: %d\n", *((int *)elem.data), *((int *)elem2.data));
		}
		else
		{
			printf("线性顺序表1元素 %d 没有后继元素\n", *((int *)elem.data));
		}
		elem2.data = &v;
		status = SqlistDelete(&sqlist1, pos, &elem2);
		if (status==OK)
		{
			printf("删除线性顺序表1位置 %zd 的元素 %d 后的顺序表:\n", pos, *((int *)elem2.data));
			SqlistTraverse(&sqlist1, sqlist_print_value);
			printf("\b\b  \n");
		}
		else
		{
			printf("删除线性顺序表1位置 %zd 的元素失败\n", pos);
		}
		printf("\n");
		status = SqlistInit(&sqlist2, CompareRoutine, AllocateRoutine, FreeRoutine);
		printf("初始化线性顺序表2: %d\n", status);
		if (status == OK)
		{
			structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//线性表长度10-30随机
			printf("随机线性顺序表2长度: %d\n", structure_length);
			for (int i = 0; i < structure_length; i++)
			{
				elem.size = sizeof(int);
				v = rand();
				elem.data = &v;
				pos = RangeRandom(0, (int)SqlistLength(&sqlist2) + 1);	//插入线性表位置0-线性表长度随机
				status = SqlistInsert(&sqlist2, SqlistLength(&sqlist2), &elem);
			}
			printf("线性顺序表2长度: %zd\n", SqlistLength(&sqlist2));
			SqlistTraverse(&sqlist2, sqlist_print_value);
			printf("\b\b  \n\n");

			SqlistUnio(&sqlist1, &sqlist2);

			printf("合并线性顺序表1和线性顺序表2后的长度: %zd\n", SqlistLength(&sqlist1));
			SqlistTraverse(&sqlist1, sqlist_print_value);
			printf("\b\b  \n");
			SqlistDestroy(&sqlist2);
		}

		SqlistDestroy(&sqlist1);
	}
}