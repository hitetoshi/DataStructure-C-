#include "stdafx.h"

#include "DataStructureDemo.h"

void InitPolyn(POLYNOMAIL *polyn, int m)
{
	POLYN_ELEMENT_DATA *items = new POLYN_ELEMENT_DATA[m];
	if (items)
	{
		for (int i = 0; i < m; i++)
		{
			items[i].coef = (float)rand()/100;
			items[i].expn = RangeRandom(1, 11);	//次随机数1-11
		}
		PolynAddItem(polyn, items, m);
		delete[] items;
	}
}

void ShowPolynmail()
{
	POLYNOMAIL polynmail1,polynmail2,polynmail3;

	printf("一元多项式运算的演示\n\n");
	srand((unsigned int)time(NULL));
	if (PolynCreate(&polynmail1, AllocateRoutine, FreeRoutine) == OK)
	{
		InitPolyn(&polynmail1, RangeRandom(3, 10));	//项数随机3-10项
		if (PolynCreate(&polynmail2, AllocateRoutine, FreeRoutine) == OK)
		{
			InitPolyn(&polynmail2, RangeRandom(3, 10));	//项数随机3-10项
			printf("随机产生多项式:\n");
			printf("P(x) = ");
			PolynPrint(&polynmail1);
			printf("\n");
			printf("Q(x) = ");
			PolynPrint(&polynmail2);
			printf("\n");
			PolynAdd(&polynmail1, &polynmail2);
			printf("P(x)+Q(x) = ");
			PolynPrint(&polynmail1);
			printf("\n\n");
			PolynDestroy(&polynmail2);
		}
		PolynDestroy(&polynmail1);
	}

	if (PolynCreate(&polynmail1, AllocateRoutine, FreeRoutine) == OK)
	{
		InitPolyn(&polynmail1, RangeRandom(3, 10));	//项数随机3-10项
		if (PolynCreate(&polynmail2, AllocateRoutine, FreeRoutine) == OK)
		{
			InitPolyn(&polynmail2, RangeRandom(3, 10));	//项数随机3-10项
			printf("随机产生多项式:\n");
			printf("P(x) = ");
			PolynPrint(&polynmail1);
			printf("\n");
			printf("Q(x) = ");
			PolynPrint(&polynmail2);
			printf("\n");
			PolynSubtract(&polynmail1, &polynmail2);
			printf("P(x)-Q(x) = ");
			PolynPrint(&polynmail1);
			printf("\n\n");
			PolynDestroy(&polynmail2);
		}
		PolynDestroy(&polynmail1);
	}

	if (PolynCreate(&polynmail1, AllocateRoutine, FreeRoutine) == OK)
	{
		InitPolyn(&polynmail1, RangeRandom(3, 10));	//项数随机3-10项
		if (PolynCreate(&polynmail2, AllocateRoutine, FreeRoutine) == OK)
		{
			InitPolyn(&polynmail2, RangeRandom(3, 10));	//项数随机3-10项
			printf("随机产生多项式:\n");
			printf("P(x) = ");
			PolynPrint(&polynmail1);
			printf("\n");
			printf("Q(x) = ");
			PolynPrint(&polynmail2);
			printf("\n");
			if (PolynCreate(&polynmail3, AllocateRoutine, FreeRoutine) == OK)
			{
				PolynMulti(&polynmail1, &polynmail2,&polynmail3);
				printf("P(x)*Q(x) = ");
				PolynPrint(&polynmail3);
				printf("\n");
				PolynDestroy(&polynmail3);
			}
			PolynDestroy(&polynmail2);
		}
		PolynDestroy(&polynmail1);
	}
}