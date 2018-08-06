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

	printf("����˳������ʾ\n\n");
	srand((unsigned int)time(NULL));
	status = SqlistInit(&sqlist1, CompareRoutine, AllocateRoutine, FreeRoutine);
	printf("��ʼ������˳���1: %d\n", status);
	if (status == OK)
	{
		structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//���Ա���10-30���
		printf("�������˳���1����: %d\n", structure_length);
		for (int i = 0; i < structure_length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			pos = RangeRandom(0, (int)SqlistLength(&sqlist1) + 1);	//�������Ա�λ��0-���Ա������
			status = SqlistInsert(&sqlist1, pos, &elem);
			if (v > max)
			{
				max = v;
			}
		}
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
		status = SqlistInit(&sqlist2, CompareRoutine, AllocateRoutine, FreeRoutine);
		printf("��ʼ������˳���2: %d\n", status);
		if (status == OK)
		{
			structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//���Ա���10-30���
			printf("�������˳���2����: %d\n", structure_length);
			for (int i = 0; i < structure_length; i++)
			{
				elem.size = sizeof(int);
				v = rand();
				elem.data = &v;
				pos = RangeRandom(0, (int)SqlistLength(&sqlist2) + 1);	//�������Ա�λ��0-���Ա������
				status = SqlistInsert(&sqlist2, SqlistLength(&sqlist2), &elem);
			}
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
}