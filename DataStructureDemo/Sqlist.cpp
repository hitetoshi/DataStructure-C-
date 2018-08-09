#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl sqlist_print_value(ELEMENT *elem)
{
	if (elem->size>=sizeof(int) && elem->data)
	{
		printf("%d, ", *((int *)elem->data));
	}
	return OK;
}

bool InitOrderSqlist(SQLIST *list, int length)
{
	ELEMENT elem;
	int v;
	bool ret = false;
	
	if (SqlistInit(list, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
	{
		for (int i = 0; i < length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			SqlistOrderInsert(list, &elem);
		}
		ret = true;
	}
	return ret;
}

bool InitRandomSqlist(SQLIST *list, int length, int *max)
{
	ELEMENT elem;
	int v;
	size_t pos;
	bool ret = false;

	if (max)
	{
		*max = 0;
	}
	if (SqlistInit(list, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
	{
		for (int i = 0; i < length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			pos = RangeRandom(0, (int)SqlistLength(list) + 1);	//插入线性表位置0-线性表长度随机
			if (SqlistInsert(list, pos, &elem) == OK)
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

void ShowSqlist()
{
	SQLIST sqlist1, sqlist2, sqlist3;
	ELEMENT elem, elem2;
	int v;
	int max = 0;
	size_t pos;
	Status status;

	printf("线性顺序表的演示\n\n");
	srand((unsigned int)time(NULL));

	if(InitRandomSqlist(&sqlist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), &max))
	{
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

		if(InitRandomSqlist(&sqlist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), NULL))
		{
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

	printf("\n");
	if(InitOrderSqlist(&sqlist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
	{
		printf("构造有序线性顺序表1, 长度: %zd\n", SqlistLength(&sqlist1));
		SqlistTraverse(&sqlist1, sqlist_print_value);
		printf("\b\b  \n");

		if (InitOrderSqlist(&sqlist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
		{
			printf("构造有序线性顺序表2, 长度: %zd\n", SqlistLength(&sqlist2));
			SqlistTraverse(&sqlist2, sqlist_print_value);
			printf("\b\b  \n");

			if (SqlistInit(&sqlist3, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
			{
				if (SqlistMerge(&sqlist1, &sqlist2, &sqlist3) == OK)
				{
					printf("归并线性顺序表1和表2后新表长度: %zd\n", SqlistLength(&sqlist3));
					SqlistTraverse(&sqlist3, sqlist_print_value);
					printf("\b\b  \n");
				}
				else
				{
					printf("归并线性顺序表失败\n");
				}
				SqlistDestroy(&sqlist3);
			}

			SqlistDestroy(&sqlist2);
		}
		SqlistDestroy(&sqlist1);
	}
}