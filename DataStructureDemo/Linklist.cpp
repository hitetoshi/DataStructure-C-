#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl linklist_print_value(ELEMENT *elem)
{
	//elem->size==0Ϊͷ�ڵ�
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
			pos = RangeRandom(0, (int)LinklistLength(list) + 1);	//�������Ա�λ��0-���Ա������
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

	printf_color(FOREGROUND_GREEN,"�����������ʾ\n\n");
	srand((unsigned int)time(NULL));

	if (InitRandomLinklist(&linklist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), &max))
	{
		printf("��������1����: %zd\n", LinklistLength(&linklist1));
		LinklistTraverse(&linklist1, linklist_print_value);
		printf("\b\b  \n");
		printf("��������1�ο���׼Ԫ��: %d\n", max);

		elem.data = &max;
		elem.size = sizeof(int);
		p1 = LinklistLocateElem(&linklist1, &elem);
		if(p1)
		{
			printf("��������1Ԫ�� %d ��λ: %d\n", max, *((int *)p1->elem.data));

			p2 = LinklistPriorPos(&linklist1, p1);
			if (p2)
			{
				if (p2 == LinklistHeader(&linklist1))
				{
					printf("��������1Ԫ�� %d û��ǰ��Ԫ��\n", *((int *)p1->elem.data));
				}
				else
				{
					printf("��������1Ԫ�� %d ǰ��Ԫ��: %d\n", *((int *)p1->elem.data), *((int *)p2->elem.data));
				}
			}
			else
			{
				printf("��������1Ԫ�� %d û��ǰ��Ԫ��\n", *((int *)p1->elem.data));
			}
			p2 = LinklistNextPos(&linklist1, p1);
			if (p2)
			{
				printf("��������1Ԫ�� %d ���Ԫ��: %d\n", *((int *)p1->elem.data), *((int *)p2->elem.data));
			}
			else
			{
				printf("��������1Ԫ�� %d û�к��Ԫ��\n", *((int *)p1->elem.data));
			}

			status=LinklistDeletePos(&linklist1, p1);
			if (status == OK)
			{
				printf("ɾ����������1λ�õ�Ԫ�� %d �������:\n", max);
				LinklistTraverse(&linklist1, linklist_print_value);
				printf("\b\b  \n");
			}
			else
			{
				printf("ɾ����������1��Ԫ�� %d ʧ��\n", max);
			}
		}
		printf("\n");

		if (InitRandomLinklist(&linklist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX), NULL))
		{
			printf("��������2����: %zd\n", LinklistLength(&linklist2));
			LinklistTraverse(&linklist2, linklist_print_value);
			printf("\b\b  \n\n");
			LinklistUnio(&linklist1, &linklist2);
			printf("�ϲ���������1����������2��ĳ���: %zd\n", LinklistLength(&linklist1));
			LinklistTraverse(&linklist1, linklist_print_value);
			printf("\b\b  \n");
			LinklistDestroy(&linklist2);
		}

		LinklistDestroy(&linklist1);
	}

	printf("\n");
	if (InitOrderLinklist(&linklist1, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
	{
		printf("����������������1, ����: %zd\n", LinklistLength(&linklist1));
		LinklistTraverse(&linklist1, linklist_print_value);
		printf("\b\b  \n");

		if (InitOrderLinklist(&linklist2, RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX)))
		{
			printf("\n����������������2, ����: %zd\n", LinklistLength(&linklist2));
			LinklistTraverse(&linklist2, linklist_print_value);
			printf("\b\b  \n");

			if (LinklistInit(&linklist3, CompareRoutine, AllocateRoutine, FreeRoutine) == OK)
			{
				LinklistMerge(&linklist1, &linklist2, &linklist3);
				printf("\n�鲢������������1��2, ������������: %zd\n", LinklistLength(&linklist3));
				LinklistTraverse(&linklist3, linklist_print_value);
				printf("\b\b  \n");
				LinklistDestroy(&linklist3);
			}
			LinklistDestroy(&linklist2);
		}
		LinklistDestroy(&linklist1);
	}
}