#include "stdafx.h"
#include "DataStructureDemo.h"

Status __cdecl queue_visit(ELEMENT *e, void *param)
{
	printf("%d ",*((int *)e->data));
	return TRUE;
}

void __cdecl OnEvent(BM_EVENT *en)
{
	if (en->NType == 0)
	{
		printf("[%d]:\t�¿ͻ�����\n", en->OccurTime);
	}
	else
	{
		printf("[%d]:\t�ͻ��� %d �Ź�̨�뿪\n",en->OccurTime, en->NType);
	}
}

void ShowQueue()
{
	int TotalTime, CustomerNum;
	Bank_Simulation(480, OnEvent,&TotalTime,&CustomerNum);
	printf("\n�ͻ������������ʱ����: %d\n"\
		"ÿλ�ͻ�����ҵ���ʱ��: %d\n"\
		"���й�̨��: %d\n"\
		"ģ��ʱ��: %d\n"\
		"�ܼƴ���ͻ�: %d, �ܼ�����ʱ��: %d, ƽ��ÿ�û�ʱ��: %.2f\n", MAX_INTERTIME, MAX_DURTIME,
		QUEUE_COUNT, 480, CustomerNum, TotalTime, (float)TotalTime/(float)CustomerNum);
}