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

void print_usage()
{
	printf("ѡ��Ҫ��ʾ�����ݽṹ:\n\n");
	printf("1:����˳���\n2:��������\n3:һԪ����ʽ����\n4:ջ(����ת��,����ƥ��,�б༭,�Թ����,���ʽ����)\n5:����(��ɢ�¼�ģ��)0:�˳�����\n");
}

void print_stack()
{
	printf("ѡ��Ҫ��ʾ��ջ�ṹ:\n");
	printf("1:����ת��\n2:����ƥ��\n3:�б༭\n4:�Թ����\n5:���ʽ����\n");
}

int main(int argc, char *argv[])
{
	int cmd = 0;
	int cat = 0;
	char c;

	if (argc >= 2)
	{
		cmd = atoi(argv[1]);
	}
	else
	{
		print_usage();
		while (scanf_s("%d", &cmd) != 1 && cmd<0 || cmd>4)
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
			ShowSqlist();
			break;
		case 2:
			ShowLinklist();
			break;
		case 3:
			ShowPolynmail();
			break;
		case 4:
			if(!cat)
			{
				printf("\n");
				print_stack();
				while (scanf_s("%d", &cat) != 1 && cat < 1 || cat>4)
				{
					printf("�������,����������:");
					while ((c = getchar()) != EOF && c != '\n');
				}
			}
			ShowStack(cat);
		case 5:
			ShowQueue();
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

