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
	printf("ѡ��Ҫ��ʾ�����ݽṹ:\n\n");
	printf("1:���Ա�(����˳���,��������,һԪ����ʽ����)\n"\
		"2:ջ(����ת��,����ƥ��,�б༭,�Թ����,���ʽ����)\n"\
		"3:����(��ɢ�¼�ģ��)\n"\
		"4:��(ģʽƥ��,��������)\n"\
		"5:����(ϡ�����,��ͨ����)\n"
		"\n0:�˳�����\n");
}

void print_list()
{
	printf("ѡ��Ҫ��ʾ�����Ա�ṹ:\n");
	printf("1:����˳���\n2:��������\n3:һԪ����ʽ����\n");
}

void print_stack()
{
	printf("ѡ��Ҫ��ʾ��ջ�ṹ:\n");
	printf("1:����ת��\n2:����ƥ��\n3:�б༭\n4:�Թ����\n5:���ʽ����\n");
}

void print_hstring()
{
	printf("ѡ��Ҫ��ʾ�Ĵ��ṹ:\n");
	printf("1:ģʽƥ��\n2:�ؼ���������\n");
}

void print_array()
{
	printf("ѡ��Ҫ��ʾ������ṹ:\n");
	printf("1:ϡ�����\n2:��ͨ����\n");
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
				printf("�������,����������:");
				while ((c = getchar()) != EOF && c != '\n');
			}
		}
	}
	else
	{
		print_usage();
		while (scanf_s("%d", &cmd) != 1 && cmd<0 || cmd>5)
		{
			printf("�������,����������:");
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
					printf("�������,����������:");
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
					printf("�������,����������:");
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
					printf("�������,����������:");
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
					printf("�������,����������:");
					while ((c = getchar()) != EOF && c != '\n');
				}
			}
			ShowArray(cat);
			break;
		default:
			break;
		}

		printf("�����������\n");
		_getch();
		printf("\n");
		print_usage();
		while (scanf_s("%d", &cmd) != 1 && cmd<0 || cmd>4)
		{
			printf("�������,����������:");
			while ((c = getchar()) != EOF && c != '\n');
		}
		cat = 0;
	} while (cmd != 0);
    return 0;
}

