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
			pos = RangeRandom(0, (int)SqlistLength(list) + 1);	//�������Ա�λ��0-���Ա������
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

	printf("����˳������ʾ\n\n");
	srand((unsigned int)time(NULL));

	if(InitRandomSqlist(&sqlist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), &max))
	{
		printf("����˳���1����: %zd\n", SqlistLength(&sqlist1));
		SqlistTraverse(&sqlist1, sqlist_print_value);
		printf("\b\b  \n");
		printf("����˳���1�ο���׼Ԫ��: %d\n", max);

		elem.data = &max;
		elem.size = sizeof(int);
		pos = SqlistLocate(&sqlist1, &elem);
		printf("����˳���1Ԫ�� %d λ��: %zd\n", max, pos);
		status = SqlistPriorElem(&sqlist1, &elem, &elem2);
		if (status == OK)
		{
			printf("����˳���1Ԫ�� %d ǰ��Ԫ��: %d\n", *((int *)elem.data), *((int *)elem2.data));
		}
		else
		{
			printf("����˳���1Ԫ�� %d û��ǰ��Ԫ��\n", *((int *)elem.data));
		}
		status = SqlistNextElem(&sqlist1, &elem, &elem2);
		if (status == OK)
		{
			printf("����˳���1Ԫ�� %d ���Ԫ��: %d\n", *((int *)elem.data), *((int *)elem2.data));
		}
		else
		{
			printf("����˳���1Ԫ�� %d û�к��Ԫ��\n", *((int *)elem.data));
		}
		elem2.data = &v;
		status = SqlistDelete(&sqlist1, pos, &elem2);
		if (status==OK)
		{
			printf("ɾ������˳���1λ�� %zd ��Ԫ�� %d ���˳���:\n", pos, *((int *)elem2.data));
			SqlistTraverse(&sqlist1, sqlist_print_value);
			printf("\b\b  \n");
		}
		else
		{
			printf("ɾ������˳���1λ�� %zd ��Ԫ��ʧ��\n", pos);
		}
		printf("\n");

		if(InitRandomSqlist(&sqlist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), NULL))
		{
			printf("����˳���2����: %zd\n", SqlistLength(&sqlist2));
			SqlistTraverse(&sqlist2, sqlist_print_value);
			printf("\b\b  \n\n");

			SqlistUnio(&sqlist1, &sqlist2);

			printf("�ϲ�����˳���1������˳���2��ĳ���: %zd\n", SqlistLength(&sqlist1));
			SqlistTraverse(&sqlist1, sqlist_print_value);
			printf("\b\b  \n");
			SqlistDestroy(&sqlist2);
		}

		SqlistDestroy(&sqlist1);
	}

	printf("\n");
	if(InitOrderSqlist(&sqlist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
	{
		printf("������������˳���1, ����: %zd\n", SqlistLength(&sqlist1));
		SqlistTraverse(&sqlist1, sqlist_print_value);
		printf("\b\b  \n");

		if (InitOrderSqlist(&sqlist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
		{
			printf("������������˳���2, ����: %zd\n", SqlistLength(&sqlist2));
			SqlistTraverse(&sqlist2, sqlist_print_value);
			printf("\b\b  \n");

			if (SqlistInit(&sqlist3, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
			{
				if (SqlistMerge(&sqlist1, &sqlist2, &sqlist3) == OK)
				{
					printf("�鲢����˳���1�ͱ�2���±���: %zd\n", SqlistLength(&sqlist3));
					SqlistTraverse(&sqlist3, sqlist_print_value);
					printf("\b\b  \n");
				}
				else
				{
					printf("�鲢����˳���ʧ��\n");
				}
				SqlistDestroy(&sqlist3);
			}

			SqlistDestroy(&sqlist2);
		}
		SqlistDestroy(&sqlist1);
	}
}