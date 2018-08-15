#include "stdafx.h"
#include "DataStructureDemo.h"

#define	MIN_MATRIX_ROW	1	//10
#define	MAX_MATRIX_ROW	3	//21
#define	MIN_MATRIX_COL	1	//10
#define	MAX_MATRIX_COL	3	//21

void ShowRLSMatrix()
{
	RLSMATRIX M;
	size_t m, n;
	size_t num;
	size_t i, j;
	float v;

	m = 3;//RangeRandom(MIN_MATRIX_ROW, MAX_MATRIX_ROW);
	n = 3;//RangeRandom(MIN_MATRIX_COL, MAX_MATRIX_COL);
	if (RLSMatrixCreate(&M, m, n) != OK)
	{
		return;
	}
	num = m*n * /*5*/100 / 100;	//生成m*n*5/100给非零元
	for (size_t p = 0; p < num; p++)
	{
		i = RangeRandom(0, (int)m);
		j = RangeRandom(0, (int)n);
		v = (float)rand();
		printf("%d(%zd,%zd) ", (int)v,i,j);
		RLSMatrixAssign(&M, i, j, v);
	}
	printf("\n");
	RLSMatrixPrintNoneZero(&M);
	RLSMatrixDestroy(&M);
}

void ShowArray(int cat)
{
	srand((int)time(NULL));

	switch (cat)
	{
	case 1:
		ShowRLSMatrix();
		break;
	case 2:
		break;
	default:
		break;
	}
}