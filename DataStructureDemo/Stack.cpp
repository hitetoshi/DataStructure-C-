#include "stdafx.h"

#include "DataStructureDemo.h"

//�������x*y�Թ�
//point[xn][yn]=0��ʾ�հ�
//point[xn][yn]=1��ʾǽ
void CreateRandomMaze(int **point, int x, int y)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			point[i][j] = (RangeRandom(0, 5)>2?1:0);	//�ÿհײ��ֱ�ǽ��1.5��
		}
	}
}

//���Թ�ջ�в���ָ�������Ƿ��Ѿ�����
Status MazeFindPos(ELEMENT *node, void *param)
{
	if (((COORDINATE *)param)->x == ((COORDINATE *)node->data)->x && ((COORDINATE *)param)->y == ((COORDINATE *)node->data)->y)
	{
		//param=����������
		//��ջ�����Ĺ�����,�κ�һ��ջԪ�ص����������������Ⱥ�������FALSE,��StackTraverse������ERROR�������̽���
		//����,���ȫջ��������Ȼû��ջԪ��������������StackTraverse������OK
		return FALSE;
	}
	return TRUE;
}

void PrintMaze(int **point, int x, int y, SQSTACK *path)
{
	COORDINATE pos;
	WORD color = 0;

	printf("  ");
	for (int i = 0; i < y; i++)
	{
		printf("%d", i);
		if (i < 10)
		{
			printf(" ");
		}
	}

	for (int i = 0; i < x; i++)
	{
		printf("\n%d",i);
		if (i < 10)
		{
			printf(" ");
		}
		for (int j = 0; j < y; j++)
		{
			if (point[i][j] == 0)
			{
				if (path)
				{
					pos.x = i;
					pos.y = j;
					if (StackTraverse(path, MazeFindPos, &pos) == ERROR)
					{
						color = ConsoleTextColor(FOREGROUND_GREEN);
					}
				}
				printf("��");
				if (color)
				{
					ConsoleTextColor(color);
					color = 0;
				}
			}
			else
			{
				printf("��");
			}
		}
	}
	printf("\n");
}

void GetPoint(COORDINATE *coord, int **point, int x, int y)
{
	char c;

	while (TRUE)
	{
		if (scanf_s("%d,%d", &coord->x, &coord->y) != 2)
		{
			printf("�����ʽ����,����������:");
			while ((c = getchar()) != EOF && c != '\n');
		}
		else
		{
			if ((coord->x >= 0 && coord->x < x) &&
				(coord->y >= 0 && coord->y<y) &&
				point[coord->x][coord->y] == 0)
			{
				break;
			}
			else
			{
				printf("����(%d.%d)δ������ǿհ׵�,����������:", coord->x, coord->y);
				while ((c = getchar()) != EOF && c != '\n');
			}
		}
	}
}

void PrintMazePath(int **point, int x, int y, SQSTACK *path)
{
	ELEMENT e;
	COORDINATE pos;

	PrintMaze(point, x, y, path);
	e.data = &pos;
	e.size = sizeof(pos);
	while (!StackEmpty(path))
	{
		StackPop(path, &e);
		printf("(%d,%d)", pos.x, pos.y);
		if (!StackEmpty(path))
		{
			printf("->");
		}
	}
	printf("\n");
}

void ShowMaze()
{
	int x, y;
	int **point;
	COORDINATE start;
	COORDINATE end;
	SQSTACK path;

	x = RangeRandom(MAZE_MIN, MAZE_MAX);	//x->10-20���
	y = RangeRandom(MAZE_MIN, MAZE_MAX);	//y->10-20���
	printf_color(FOREGROUND_GREEN, "�Թ������ʾ:\n");
	printf("�������%d��%d�Թ�\r\n", x, y);
	point = new int *[x];
	if (point)
	{
		for (int i = 0; i < x; i++)
		{
			point[i] = new int[y];
		}
		CreateRandomMaze(point, x, y);
		PrintMaze(point, x, y, NULL);

		printf("\n�����Թ��������(��ʽ:x,y):");
		GetPoint(&start, point, x, y);
		printf("�����Թ��յ�����(��ʽ:x,y):");
		GetPoint(&end, point, x, y);

		printf("\n");
		if (StackInit(&path, AllocateRoutine, FreeRoutine) == OK)
		{
			if (MazePath(point, x, y, &start, &end, &path))
			{
				PrintMazePath(point, x, y, &path);
			}
			else
			{
				printf("���Թ��޽�\n");
			}
			StackDestroy(&path);
		}

		for (int i = 0; i < x; i++)
		{
			delete[] point[i];
		}
		delete[] point;
	}
}

void ShowConversion()
{
	int num;
	char c;

	printf_color(FOREGROUND_GREEN,"����ת����ʾ:\n������һ��ʮ��������س�:");
	while (scanf_s("%d", &num) != 1)
	{
		printf("�������,����������:");
		while ((c = getchar()) != EOF && c != '\n');
	}
	conversion(num, 8);
	conversion(num, 16);
}

void ShowBrackets()
{
	char ch[1024];

	printf_color(FOREGROUND_GREEN, "����ƥ����ʾ:\n��������������()[]{}�Իس�����:");
	scanf_s("%s", ch, 1024);
	brackets(ch);
}

void ShowLineEdit()
{
	char ch[1024];

	printf_color(FOREGROUND_GREEN, "�б༭��ʾ:\n");
	printf("���ն������ַ�, \"#\"��ʾ�˸�,\"@\"��ʾ����,����EOF�����༭\nWindows������EOF�ķ���������һ������Ctrl-Z���س�:\n");
	memset(ch, 0, sizeof(ch));
	LineEdit(ch, _countof(ch));
	printf("\n���༭���û�����:\n");
	printf("%s\n", ch); 
}

void ShowEvaluateExpression()
{
	char expression[256];
	int result;

	printf_color(FOREGROUND_GREEN, "���ʽ������ʾ:\n");
	printf("������Ҫ����ı��ʽ, ֧�� + -*/ ()�����:");
	memset(expression, 0, sizeof(expression));
	scanf_s("%s", expression, 256);
	//���Ͻ�����#
	strcat_s(expression, _countof(expression), "#");
	if (EvaluateExpression(expression, &result) == OK)
	{
		expression[strlen(expression)-1] = 0;
		printf("%s = %d\n", expression, result);
	}
	else
	{
		printf("�޷�������ʽ��ֵ,�����Ǳ��ʽ���﷨����\n");
	}
}

void ShowStack(int cat)
{
	srand((int)time(NULL));

	switch (cat)
	{
	case 1:
		ShowConversion();
		break;
	case 2:
		ShowBrackets();
		break;
	case 3:
		ShowLineEdit();
		break;
	case 4:
		ShowMaze();
		break;
	case 5:
		ShowEvaluateExpression();
		break;
	default:
		break;
	}
}