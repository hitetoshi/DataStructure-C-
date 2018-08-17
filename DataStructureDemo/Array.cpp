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
	Info.dwCursorPosition.X = (short)(MatrixCols(M)) * 4;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("＿\n");
	for (i = 0; i < MatrixRows(M); i++)
	{
		printf("｜");
		for (j = 0; j < MatrixCols(M); j++)
		{
			GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
			Info.dwCursorPosition.X = (short)j*4+2;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);

			v = 0;
			MatrixValue(M, i, j, &v);
			printf("%d", (int)v);
		}
		GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
		Info.dwCursorPosition.X = (short)j * 4+1;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
		printf("｜\n");
	}
	printf(" ￣");
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
	Info.dwCursorPosition.X = (short)(MatrixCols(M)) * 4;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("￣\n");
}

void PrintRLSMatrix(RLSMATRIX *M)
{
	MATRIX Matrix;

	if(MatrixCreate(&Matrix)==OK)
	{
		if (RLSMatrixDump(&Matrix, M) == OK)
		{
			PrintMatrix(&Matrix);
			
		}
		MatrixDestroy(&Matrix);
	}
}

void ShowMatrixTranspose()
{
	MATRIX M, T;
	size_t m, n;
	float v;

	printf_color(FOREGROUND_GREEN, "矩阵转置:\n\n");

	m = RangeRandom(MIN_MATRIX_ROW, MAX_MATRIX_ROW);
	n = RangeRandom(MIN_MATRIX_COL, MAX_MATRIX_COL);
	if (MatrixCreate(&M) != OK)
	{
		return;
	}
	if (MatrixCreate(&T) != OK)
	{
		MatrixDestroy(&M);
		return;
	}
	if (MatrixReset(&M, m, n) != OK)
	{
		MatrixDestroy(&T);
		MatrixDestroy(&M);
		return;
	}
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			v = (float)RangeRandom(0, MAX_MATRIX_VALUE) - MAX_MATRIX_VALUE / 2;
			MatrixAssign(&M, i, j, v);
		}
	}
	printf("随机生成%zd×%zd矩阵M:\n", m, n);
	PrintMatrix(&M);
	if (MatrixTranspose(&M, &T) == OK)
	{
		printf("M的转置矩阵:\n");
		PrintMatrix(&T);
	}
	MatrixDestroy(&T);
	MatrixDestroy(&M);
}

void ShowMatrixMulti()
{
	MATRIX M, N, Q;
	size_t m, n, n2;
	float v;

	system("cls");
	printf_color(FOREGROUND_GREEN, "矩阵乘法:\n\n");
	//生成两个较小的矩阵
	m = RangeRandom(MIN_MATRIX_ROW/2, MAX_MATRIX_ROW/2);
	n = RangeRandom(MIN_MATRIX_COL/2, MAX_MATRIX_COL/2);
	n2 = RangeRandom(MIN_MATRIX_COL / 2, MAX_MATRIX_COL / 2);
	if (MatrixCreate(&M) != OK)
	{
		return;
	}
	if (MatrixCreate(&N) != OK)
	{
		MatrixDestroy(&M);
		return;
	}
	if (MatrixCreate(&Q) != OK)
	{
		MatrixDestroy(&M);
		MatrixDestroy(&N);
		return;
	}
	if (MatrixReset(&M, m, n) != OK)
	{
		MatrixDestroy(&M);
		MatrixDestroy(&N);
		MatrixDestroy(&Q);
		return;
	}
	if (MatrixReset(&N, n, n2) != OK)
	{
		MatrixDestroy(&M);
		MatrixDestroy(&N);
		MatrixDestroy(&Q);
		return;
	}
	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			v = (float)RangeRandom(0, MAX_MATRIX_VALUE/10) - MAX_MATRIX_VALUE/10 / 2;
			MatrixAssign(&M, i, j, v);
		}
	}
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n2; j++)
		{
			v = (float)RangeRandom(0, MAX_MATRIX_VALUE/10) - MAX_MATRIX_VALUE/10 / 2;
			MatrixAssign(&N, i, j, v);
		}
	}
	printf("随机生成%zd×%zd矩阵M:\n", m, n);
	PrintMatrix(&M);
	printf("随机生成%zd×%zd矩阵N:\n", n, n2);
	PrintMatrix(&N);
	if (MatrixMult(&M, &N, &Q) == OK)
	{
		printf("M×N=\n");
		PrintMatrix(&Q);
	}
	MatrixDestroy(&M);
	MatrixDestroy(&N);
	MatrixDestroy(&Q);
}

void ShowRLSMatrixTranspose()
{
	RLSMATRIX M, T;
	size_t m, n;
	size_t num;
	size_t i, j;
	float v;

	printf_color(FOREGROUND_GREEN, "稀疏矩阵转置:\n\n");
	m = RangeRandom(MIN_MATRIX_ROW, MAX_MATRIX_ROW);
	n = RangeRandom(MIN_MATRIX_COL, MAX_MATRIX_COL);
	if (RLSMatrixCreate(&M) != OK)
	{
		return;
	}
	if (RLSMatrixReset(&M, m, n) != OK)
	{
		return;
	}
	num = m*n / 5;	//生成m*n*/5个非零元
	for (size_t p = 0; p < num; p++)
	{
		i = RangeRandom(0, (int)m);
		j = RangeRandom(0, (int)n);
		v = (float)RangeRandom(0, MAX_MATRIX_VALUE) - MAX_MATRIX_VALUE / 2;
		RLSMatrixAssign(&M, i, j, v);
	}
	printf("随机生成%zd×%zd稀疏矩阵M:\n", m, n);
	PrintRLSMatrix(&M);
	if (RLSMatrixCreate(&T) == OK)
	{
		if (RLSMatrixFastTranspose2(&M, &T) == OK)
		{
			printf("M的转置矩阵:\n");
			PrintRLSMatrix(&T);
		}
		RLSMatrixDestroy(&T);
	}
	RLSMatrixDestroy(&M);
}

void ShowRLSMatrixMulti()
{
	RLSMATRIX M, N, Q;
	size_t m, n, n2;
	float v;
	size_t num;
	size_t i, j;

	system("cls");
	printf_color(FOREGROUND_GREEN, "稀疏矩阵乘法:\n\n");

	//生成两个较小的矩阵
	m = 2;//RangeRandom(MIN_MATRIX_ROW / 2, MAX_MATRIX_ROW / 2);
	n = 1;// RangeRandom(MIN_MATRIX_COL / 2, MAX_MATRIX_COL / 2);
	n2 = 2;// RangeRandom(MIN_MATRIX_COL / 2, MAX_MATRIX_COL / 2);
	if (RLSMatrixCreate(&M) != OK)
	{
		return;
	}
	if (RLSMatrixCreate(&N) != OK)
	{
		RLSMatrixDestroy(&M);
		return;
	}
	if (RLSMatrixCreate(&Q) != OK)
	{
		RLSMatrixDestroy(&M);
		RLSMatrixDestroy(&N);
		return;
	}
	if (RLSMatrixReset(&M, m, n) != OK)
	{
		RLSMatrixDestroy(&M);
		RLSMatrixDestroy(&N);
		RLSMatrixDestroy(&Q);
		return;
	}
	if (RLSMatrixReset(&N, n, n2) != OK)
	{
		RLSMatrixDestroy(&M);
		RLSMatrixDestroy(&N);
		RLSMatrixDestroy(&Q);
		return;
	}

	for (size_t i = 0; i < m; i++)
	{
		for (size_t j = 0; j < n; j++)
		{
			v = (float)RangeRandom(0, MAX_MATRIX_VALUE / 10) - MAX_MATRIX_VALUE / 10 / 2;
			RLSMatrixAssign(&M, i, j, v);
		}
	}
	for (size_t i = 0; i < n; i++)
	{
		for (size_t j = 0; j < n2; j++)
		{
			v = (float)RangeRandom(0, MAX_MATRIX_VALUE / 10) - MAX_MATRIX_VALUE / 10 / 2;
			RLSMatrixAssign(&N, i, j, v);
		}
	}

	/*num = m*n / 5;
	for (size_t p = 0; p < num; p++)
	{
		i = RangeRandom(0, (int)m);
		j = RangeRandom(0, (int)n);
		v = (float)RangeRandom(0, MAX_MATRIX_VALUE/10) - MAX_MATRIX_VALUE/10 / 2;
		RLSMatrixAssign(&M, i, j, v);
	}
	num = n*n2 / 5;
	for (size_t p = 0; p < num; p++)
	{
		i = RangeRandom(0, (int)n);
		j = RangeRandom(0, (int)n2);
		v = (float)RangeRandom(0, MAX_MATRIX_VALUE/10) - MAX_MATRIX_VALUE/10 / 2;
		RLSMatrixAssign(&N, i, j, v);
	}*/
	printf("随机生成%zd×%zd稀疏矩阵M:\n", m, n);
	PrintRLSMatrix(&M);
	printf("随机生成%zd×%zd稀疏矩阵N:\n", n, n2);
	PrintRLSMatrix(&N);
	if (RLSMatrixMult(&M, &N, &Q) == OK)
	{
		printf("M×N=\n");
		PrintRLSMatrix(&Q);
	}
	RLSMatrixDestroy(&M);
	RLSMatrixDestroy(&N);
	RLSMatrixDestroy(&Q);
}

void ShowRLSMatrix()
{
	ShowRLSMatrixTranspose();
	printf("按任意键演示稀疏矩阵乘法\n");
	_getch();
	ShowRLSMatrixMulti();
}

void ShowMatrix()
{
	ShowMatrixTranspose();
	printf("按任意键演示矩阵乘法\n");
	_getch();
	ShowMatrixMulti();
	return;
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