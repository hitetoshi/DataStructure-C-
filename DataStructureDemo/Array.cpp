#include "stdafx.h"
#include "DataStructureDemo.h"

#define	MIN_MATRIX_ROW	10
#define	MAX_MATRIX_ROW	20
#define	MIN_MATRIX_COL	10
#define	MAX_MATRIX_COL	20

#define	MAX_MATRIX_VALUE	100

void PrintMatrix(MATRIX *M)
{
	float v;
	CONSOLE_SCREEN_BUFFER_INFO Info;
	size_t i, j;

	printf(" ＿");
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
	Info.dwCursorPosition.X = (int)(MatrixCols(M)) * 4 - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("＿\n");
	for (i = 0; i < MatrixRows(M); i++)
	{
		printf("｜");
		for (j = 0; j < MatrixCols(M); j++)
		{
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
			Info.dwCursorPosition.X = (int)j*4+2;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);

			v = 0;
			MatrixValue(M, i, j, &v);
			printf("%d", (int)v);
		}
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
		Info.dwCursorPosition.X = (int)j * 4;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
		printf("｜\n");
	}
	printf(" ￣");
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
	Info.dwCursorPosition.X = (int)(MatrixCols(M)) * 4 - 1;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("￣\n");
}

void PrintRLSMatrix(RLSMATRIX *M)
{
	MATRIX Matrix;

	if (MatrixCreate(&Matrix, RLSMatrixRows(M), RLSMatrixCols(M)) == OK)
	{
		if (RLSMatrixDump(&Matrix, M) == OK)
		{
			PrintMatrix(&Matrix);
		}
		MatrixDestroy(&Matrix);
	}
}

void ShowMatrix()
{
	MATRIX M,T;
	size_t m, n;
	float v;

	m = RangeRandom(MIN_MATRIX_ROW, MAX_MATRIX_ROW);
	n = RangeRandom(MIN_MATRIX_COL, MAX_MATRIX_COL);
	if (MatrixCreate(&M, m, n))
	{
		for (size_t i = 0; i < m; i++)
		{
			for (size_t j = 0; j < n; j++)
			{
				v = (float)RangeRandom(0, MAX_MATRIX_VALUE) - MAX_MATRIX_VALUE/2;
				MatrixAssign(&M, i, j, v);
			}
		}
		printf("随机生成%zd×%zd矩阵M:\n", m, n);
		PrintMatrix(&M);
		printf("按任意键求转置矩阵\n");
		_getch();
		if (MatrixCreate(&T, n, m) == OK)
		{
			if (MatrixTranspose(&M, &T) == OK)
			{
				printf("M的转置矩阵:\n");
				PrintMatrix(&T);
			}
			MatrixDestroy(&T);
		}
		MatrixDestroy(&M);
	}
}

void ShowRLSMatrix()
{
	RLSMATRIX M,T;
	size_t m, n;
	size_t num;
	size_t i, j;
	float v;

	m = RangeRandom(MIN_MATRIX_ROW, MAX_MATRIX_ROW);
	n = RangeRandom(MIN_MATRIX_COL, MAX_MATRIX_COL);
	if (RLSMatrixCreate(&M, m, n) != OK)
	{
		return;
	}
	num = m*n * 5 / 100;	//生成m*n*5/100个非零元
	for (size_t p = 0; p < num; p++)
	{
		i = RangeRandom(0, (int)m);
		j = RangeRandom(0, (int)n);
		v = (float)RangeRandom(0, MAX_MATRIX_VALUE) - MAX_MATRIX_VALUE / 2;
		RLSMatrixAssign(&M, i, j, v);
	}
	printf("随机生成%zd×%zd稀疏矩阵M:\n", m, n);
	PrintRLSMatrix(&M);
	printf("按任意键求转置矩阵\n");
	_getch();
	if (RLSMatrixCreate(&T, n, m) == OK)
	{
		if (RLSMatrixTranspose(&M, &T) == OK)
		{
			printf("M的转置矩阵:\n");
			PrintRLSMatrix(&T);
		}
		RLSMatrixDestroy(&T);
	}
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
		ShowMatrix();
		break;
	default:
		break;
	}
}