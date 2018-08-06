#include "stdafx.h"

#include "DataStructureDemo.h"

Status __cdecl linklist_print_value(NODEELEMENT *elem)
{
	//elem->size==0Ϊͷ�ڵ�
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

	printf("�����������ʾ\n\n");
	srand((unsigned int)time(NULL));
	status = LinklistInit(&linklist1, CompareRoutine, AllocateRoutine, FreeRoutine);
	printf("��ʼ����������1: %d\n", status);
	if (status == OK)
	{
		structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//���Ա���10-30���
		printf("�����������1����: %d\n", structure_length);
		for (int i = 0; i < structure_length; i++)
		{
			elem.size = sizeof(int);
			v = rand();
			elem.data = &v;
			pos = RangeRandom(0, (int)LinklistLength(&linklist1) + 1);	//�������Ա�λ��0-���Ա������
			status = LinklistInsert(&linklist1, pos, &elem);
			if (v > max)
			{
				max = v;
			}
		}
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

		status = LinklistInit(&linklist2, CompareRoutine, AllocateRoutine, FreeRoutine);
		printf("��ʼ����������2: %d\n", status);
		if (status == OK)
		{
			structure_length = RangeRandom(STRUCTURE_SIZE_MIN, STRUCTURE_SIZE_MAX);	//���Ա���10-30���
			printf("�����������2����: %d\n", structure_length);
			for (int i = 0; i < structure_length; i++)
			{
				elem.size = sizeof(int);
				v = rand();
				elem.data = &v;
				pos = RangeRandom(0, (int)LinklistLength(&linklist2) + 1);	//�������Ա�λ��0-���Ա������
				status = LinklistInsert(&linklist2, pos, &elem);
				if (v > max)
				{
					max = v;
				}
			}
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
}