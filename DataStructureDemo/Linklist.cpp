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
			status = LinklistInsert(&linklist1, LinklistLength(&linklist1), &elem);
			if (v > max)
			{
				max = v;
			}
		}
		printf("��������1����: %d\n", LinklistLength(&linklist1));
		LinklistTraverse(&linklist1, linklist_print_value);
		printf("\b\b  \n");
		printf("��������1�ο���׼Ԫ��: %d\n", max);

		elem.data = &max;
		elem.size = sizeof(int);
		pos = LinklistLocate(&linklist1, &elem);
		printf("��������1Ԫ�� %d λ��: %d\n", max, pos);

		LinklistDestroy(&linklist1);
	}
}