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
		printf("[%d]:\t新客户到来\n", en->OccurTime);
	}
	else
	{
		printf("[%d]:\t客户从 %d 号柜台离开\n",en->OccurTime, en->NType);
	}
}

void ShowQueue()
{
	int TotalTime, CustomerNum;
	Bank_Simulation(480, OnEvent,&TotalTime,&CustomerNum);
	printf("总计处理客户: %d, 总计消耗时间: %d, 平均每用户时间: %.2f\n",CustomerNum, TotalTime, (float)TotalTime/(float)CustomerNum);
}