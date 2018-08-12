#include "stdafx.h"

#include "DataStructureDemo.h"

//随机创建x*y迷宫
//point[xn][yn]=0表示空白
//point[xn][yn]=1表示墙
void CreateRandomMaze(int **point, int x, int y)
{
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			point[i][j] = (RangeRandom(0, 5)>2?1:0);	//让空白部分比墙多1.5倍
		}
	}
}

//从迷宫栈中查找指定坐标是否已经存在
Status MazeFindPos(ELEMENT *node, void *param)
{
	if (((COORDINATE *)param)->x == ((COORDINATE *)node->data)->x && ((COORDINATE *)param)->y == ((COORDINATE *)node->data)->y)
	{
		//param=给定的坐标
		//在栈遍历的过程中,任何一个栈元素的坐标与给定坐标相等函数返回FALSE,则StackTraverse将返回ERROR遍历立刻结束
		//否则,完成全栈遍历后仍然没有栈元素与给定坐标相等StackTraverse将返回OK
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
				printf("■");
				if (color)
				{
					ConsoleTextColor(color);
					color = 0;
				}
			}
			else
			{
				printf("□");
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
			printf("输入格式有误,请重新输入:");
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
				printf("坐标(%d.%d)未定义或不是空白点,请重新输入:", coord->x, coord->y);
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

	x = RangeRandom(MAZE_MIN, MAZE_MAX);	//x->10-20随机
	y = RangeRandom(MAZE_MIN, MAZE_MAX);	//y->10-20随机
	printf_color(FOREGROUND_GREEN, "迷宫求解演示:\n");
	printf("随机生成%d×%d迷宫\r\n", x, y);
	point = new int *[x];
	if (point)
	{
		for (int i = 0; i < x; i++)
		{
			point[i] = new int[y];
		}
		CreateRandomMaze(point, x, y);
		PrintMaze(point, x, y, NULL);

		printf("\n输入迷宫起点坐标(格式:x,y):");
		GetPoint(&start, point, x, y);
		printf("输入迷宫终点坐标(格式:x,y):");
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
				printf("此迷宫无解\n");
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

	printf_color(FOREGROUND_GREEN,"进制转换演示:\n请输入一个十进制数后回车:");
	while (scanf_s("%d", &num) != 1)
	{
		printf("输入错误,请重新输入:");
		while ((c = getchar()) != EOF && c != '\n');
	}
	conversion(num, 8);
	conversion(num, 16);
}

void ShowBrackets()
{
	char ch[1024];

	printf_color(FOREGROUND_GREEN, "括号匹配演示:\n请任意输入括号()[]{}以回车结束:");
	scanf_s("%s", ch, 1024);
	brackets(ch);
}

void ShowLineEdit()
{
	char ch[1024];

	printf_color(FOREGROUND_GREEN, "行编辑演示:\n");
	printf("从终端输入字符, \"#\"表示退格,\"@\"表示退行,输入EOF结束编辑\nWindows下输入EOF的方法是新起一行输入Ctrl-Z并回车:\n");
	memset(ch, 0, sizeof(ch));
	LineEdit(ch, _countof(ch));
	printf("\n经编辑的用户输入:\n");
	printf("%s\n", ch); 
}

void ShowEvaluateExpression()
{
	char expression[256];
	int result;

	printf_color(FOREGROUND_GREEN, "表达式计算演示:\n");
	printf("输入需要计算的表达式, 支持 + -*/ ()运算符:");
	memset(expression, 0, sizeof(expression));
	scanf_s("%s", expression, 256);
	//加上结束符#
	strcat_s(expression, _countof(expression), "#");
	if (EvaluateExpression(expression, &result) == OK)
	{
		expression[strlen(expression)-1] = 0;
		printf("%s = %d\n", expression, result);
	}
	else
	{
		printf("无法计算表达式的值,可能是表达式有语法错误\n");
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