#include "stdafx.h"

#include "DataStructureDemo.h"

void Conversion(int num,int base)
{
	ELEMENTNODE e;
	int N1, N2;
	SQSTACK s;

	if (StackInit(&s, AllocateRoutine, FreeRoutine) == OK)
	{
		N1 = num;
		e.data = &N2;
		e.size = sizeof(N2);
		while (N1)
		{
			N2 = N1 % base;
			StackPush(&s, &e);
			N1 = N1 / base;
		}
		printf("%d进制数:", base);
		while (!StackEmpty(&s))
		{
			StackPop(&s, &e);
			if (N2 <= 9)
			{
				printf("%d", N2);
			}
			else
			{
				printf("%c", N2 - 10 + 'A');
			}
		}
		printf("\n");
		StackDestroy(&s);
	}
}

void Brackets(char *ch)
{
	SQSTACK s;
	ELEMENTNODE e;
	char v;
	bool failure=false;
	int i;

	e.data = &v;
	e.size = sizeof(v);

	if (StackInit(&s, AllocateRoutine, FreeRoutine))
	{
		for (i = 0; i < (int)strlen(ch) && !failure; i++)
		{
			if (ch[i] == '(' || ch[i] == '[' || ch[i] == '{')
			{
				v = ch[i];
				StackPush(&s, &e);
			}
			else
			{
				if (StackGetTop(&s, &e) == OK)
				{
					if (ch[i] == ')')
					{
						if (v == '(')
						{
							StackPop(&s, &e);
						}
						else
						{
							failure = true;
						}
					}
					else if (ch[i] == ']')
					{
						if (v == '[')
						{
							StackPop(&s, &e);
						}
						else
						{
							failure = true;
						}
					}
					else if (ch[i] == '}')
					{
						if (v == '{')
						{
							StackPop(&s, &e);
						}
						else
						{
							failure = true;
						}
					}
					else
					{
						failure = true;
					}
				}
				else
				{
					failure = true;
				}
			}
		}
		if (failure)
		{
			printf("括号匹配失败,在第 %d 个字符 '%c' \n", i, ch[i-1]);
		}
		else
		{
			printf(StackEmpty(&s) ? "括号匹配成功\n" : "括号匹配失败\n");
		}
		StackDestroy(&s);
	}
}

void ShowStack()
{
	int num;
	char ch[100];

	printf("栈的演示\n\n");

	printf("进制转换演示:\n请输入一个十进制数后回车:");
	scanf_s("%d", &num);
	Conversion(num, 8);
	Conversion(num, 16);
	printf("\n括号匹配演示:\n请任意输入括号()[]{}以回车结束:");
	scanf_s("%s", ch, 100);
	Brackets(ch);
}