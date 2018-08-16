// DataStructureDemo.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DataStructureDemo.h"

void *__cdecl AllocateRoutine(size_t ByteSize)
{
	static int count = 0;
	count++;
	return new unsigned char[ByteSize];
}

void __cdecl FreeRoutine(void *Buffer)
{
	delete[] Buffer;
}

int __cdecl CompareRoutine(ELEMENT *FirstElement, ELEMENT* SecondElement)
{
	return (FirstElement->size >= sizeof(int) && FirstElement->data && SecondElement->size >= sizeof(int) && SecondElement->data) ?
		(*((int *)FirstElement->data)) - (*((int *)SecondElement->data)) : -1;
}

WORD ConsoleTextColor(WORD wColor)
{
	CONSOLE_SCREEN_BUFFER_INFO csbinfo;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	
	memset(&csbinfo, 0, sizeof(csbinfo));
	if (hStdout)
	{
		GetConsoleScreenBufferInfo(hStdout, &csbinfo);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
	}
	return csbinfo.wAttributes;
}

void printf_color(WORD wColor, const char *format,...)
{
	WORD old = ConsoleTextColor(wColor);

	va_list args;
	va_start(args, format);

	_vcprintf(format, args);
	ConsoleTextColor(old);
}

void print_usage()
{
	printf("选择要演示的数据结构:\n\n");
	printf("1:线性表(线性顺序表,线性链表,一元多项式运算)\n"\
		"2:栈(进制转换,括号匹配,行编辑,迷宫求解,表达式计算)\n"\
		"3:队列(离散事件模拟)\n"\
		"4:串(模式匹配,词索引表)\n"\
		"5:数组(稀疏矩阵,普通矩阵)\n"
		"\n0:退出程序\n");
}

void print_list()
{
	printf("选择要演示的线性表结构:\n");
	printf("1:线性顺序表\n2:线性链表\n3:一元多项式运算\n");
}

void print_stack()
{
	printf("选择要演示的栈结构:\n");
	printf("1:进制转换\n2:括号匹配\n3:行编辑\n4:迷宫求解\n5:表达式计算\n");
}

void print_hstring()
{
	printf("选择要演示的串结构:\n");
	printf("1:模式匹配\n2:关键词索引表\n");
}

void print_array()
{
	printf("选择要演示的数组结构:\n");
	printf("1:稀疏矩阵\n2:普通矩阵\n");
}

int main(int argc, char *argv[])
{
	int cmd = 0;
	int cat = 0;
	char c;

	setlocale(LC_ALL, "zh-cn");
	if (argc >= 2)
	{
		cmd = atoi(argv[1]);
		if (cmd < 0 || cmd>5)
		{
			print_usage();
			while (scanf_s("%d", &cmd) != 1 && cmd<0 || cmd>5)
			{
				printf("输入错误,请重新输入:");
				while ((c = getchar()) != EOF && c != '\n');
			}
		}
	}
	else
	{
		print_usage();
		while (scanf_s("%d", &cmd) != 1 && cmd<0 || cmd>5)
		{
			printf("输入错误,请重新输入:");
			while ((c = getchar()) != EOF && c != '\n');
		}
	}

	if (argc >= 3)
	{
		cat = atoi(argv[2]);
	}

	do
	{
		switch (cmd)
		{
		case 0:
			return 0;
		case 1:
			if (!cat)
			{
				printf("\n");
				print_list();
				while (scanf_s("%d", &cat) != 1 && cat < 1 || cat>3)
				{
					printf("输入错误,请重新输入:");
					while ((c = getchar()) != EOF && c != '\n');
				}
			}
			ShowList(cat);
			break;
		case 2:
			if (!cat)
			{
				printf("\n");
				print_stack();
				while (scanf_s("%d", &cat) != 1 && cat < 1 || cat>5)
				{
					printf("输入错误,请重新输入:");
					while ((c = getchar()) != EOF && c != '\n');
				}
			}
			ShowStack(cat);
			break;
		case 3:
			ShowQueue();
			break;
		case 4:
			if (!cat)
			{
				printf("\n");
				print_hstring();
				while (scanf_s("%d", &cat) != 1 && cat < 1 || cat>2)
				{
					printf("输入错误,请重新输入:");
					while ((c = getchar()) != EOF && c != '\n');
				}
			}
			ShowHString(cat);
			break;
		case 5:
			if (!cat)
			{
				printf("\n");
				print_array();
				while (scanf_s("%d", &cat) != 1 && cat < 1 || cat>2)
				{
					printf("输入错误,请重新输入:");
					while ((c = getchar()) != EOF && c != '\n');
				}
			}
			ShowArray(cat);
			break;
		default:
			break;
		}

		printf("按任意键继续\n");
		_getch();
		printf("\n");
		print_usage();
		while (scanf_s("%d", &cmd) != 1 && cmd<0 || cmd>4)
		{
			printf("输入错误,请重新输入:");
			while ((c = getchar()) != EOF && c != '\n');
		}
		cat = 0;
	} while (cmd != 0);
    return 0;
}

