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
		printf("%d������:", base);
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
			printf("����ƥ��ʧ��,�ڵ� %d ���ַ� '%c' \n", i, ch[i-1]);
		}
		else
		{
			printf(StackEmpty(&s) ? "����ƥ��ɹ�\n" : "����ƥ��ʧ��\n");
		}
		StackDestroy(&s);
	}
}

void ShowStack()
{
	int num;
	char ch[100];

	printf("ջ����ʾ\n\n");

	printf("����ת����ʾ:\n������һ��ʮ��������س�:");
	scanf_s("%d", &num);
	Conversion(num, 8);
	Conversion(num, 16);
	printf("\n����ƥ����ʾ:\n��������������()[]{}�Իس�����:");
	scanf_s("%s", ch, 100);
	Brackets(ch);
}