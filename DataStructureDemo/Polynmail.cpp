#include "stdafx.h"

#include "DataStructureDemo.h"

void ShowPolynmail()
{
	int m;
	POLYNOMAIL polynmail1,polynmail2;

	printf("һԪ����ʽ�������ʾ\n\n");
	printf("�������ʽP������: ");
	scanf_s("%d", &m);

	if (PolynCreate(&polynmail1, m, AllocateRoutine, FreeRoutine) == OK)
	{
		printf("�������ʽQ������: ");
		scanf_s("%d", &m);

		if (PolynCreate(&polynmail2, m, AllocateRoutine, FreeRoutine) == OK)
		{

		}
	}

	printf("P(x) = ");
	PolynPrint(&polynmail1);
	printf("\n");
	printf("Q(x) = ");
	PolynPrint(&polynmail2);
	printf("\n");
}