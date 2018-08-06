#include "stdafx.h"

#include "DataStructureDemo.h"

void ShowPolynmail()
{
	int m;
	POLYNOMAIL polynmail1,polynmail2;

	printf("一元多项式运算的演示\n\n");
	printf("输入多项式P的项数: ");
	scanf_s("%d", &m);

	if (PolynCreate(&polynmail1, m, AllocateRoutine, FreeRoutine) == OK)
	{
		printf("输入多项式Q的项数: ");
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