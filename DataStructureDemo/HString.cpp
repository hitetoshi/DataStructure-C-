#include "stdafx.h"
#include "DataStructureDemo.h"

char string_table[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789~!@#$%^&*()_+`-=|\\<>?,./";

size_t RandomString(HString *table, int len, HString *s)
{
	size_t table_len;

	StrClear(s);
	table_len = StrLength(table);

	for (int i = 0; i<len; i++)
	{
		StrCatc(s, StrElem(table, RangeRandom(0, (int)table_len)));
	}
	return StrLength(s);
}

void ShowStringIndex(HString *t1,HString *t2)
{
	LARGE_INTEGER end_time;
	LARGE_INTEGER begin_time;
	LARGE_INTEGER freq;
	double us_time;
	size_t pos;

	QueryPerformanceFrequency(&freq);

	QueryPerformanceCounter(&begin_time);
	pos = StrIndex(t1, t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("\n模式匹配 位置: %zd 时间: %f 微秒\n", pos, us_time);

	QueryPerformanceCounter(&begin_time);
	pos = StrIndexKMP(t1, t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("模式匹配(KMP算法) 位置: %zd 时间: %f 微秒\n", pos, us_time);

	QueryPerformanceCounter(&begin_time);
	pos = StrIndexKMPOpt(t1, t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("模式匹配(KMP算法优化) 位置: %zd 时间: %f 微秒\n", pos, us_time);
}

void ShowIndex()
{
	HString t1, t2, table;
	int str_len;
	size_t pos;
	size_t substr_len;

	printf_color(FOREGROUND_GREEN, "模式匹配演示:\n");

	srand((int)time(NULL));
	StrInit(&t2);
	StrInit(&t1);
	StrInit(&table);

	StrAssign(&table, string_table);

	str_len = RangeRandom(300, 500);	//生成300-500随机长度随机串
	RandomString(&table, str_len, &t1);
	printf("随机生成长度 %d 串:\n%s\n", str_len, StrPointer(&t1));

	substr_len = RangeRandom(30, 50);	//随机生成30-50串长子串
	pos = RangeRandom(0, str_len - substr_len);
	StrSubString(&t2, &t1, pos, substr_len);
	printf("\n随机截取长度 %zd 位置 %zd 模式串:\n%s\n", substr_len, pos, StrPointer(&t2));

	
	ShowStringIndex(&t1, &t2);
	printf_color(FOREGROUND_RED , "如果在这里你发现KMP算法运行时间比较普通算法运行时间还要长,请参看HString.h文件关于KMP算法效率的讨论\n\n");
	printf("按任意键生成KMP算法友好的主串和模式串\n");
	_getch();
	StrAssign(&table, "0000000000");
	StrCopy(&t2, &table);
	StrCatc(&t2, '1');
	StrClear(&t1);
	for (pos = 0; pos < 50; pos++)
	{
		StrCat(&t1, &table);
	}
	StrCatc(&t1, '1');
	printf("生成KMP算法友好长度 %zd 主串:\n%s\n", StrLength(&t1), StrPointer(&t1));
	printf("生成KMP算法友好长度 %zd 模式串:\n%s\n", StrLength(&t2), StrPointer(&t2));
	ShowStringIndex(&t1, &t2);

	StrDestroy(&t1);
	StrDestroy(&t2);
	StrDestroy(&table);
}

Status __cdecl traverse_print_bno(ELEMENT *elem)
{
	printf("%03d,", *((int *)elem->data));
	return OK;
}

Status __cdecl traverse_print_idxlist(ELEMENT *elem)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);

	Info.dwCursorPosition.X += 2;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("%s",StrPointer(&((IDXTERMTYPE *)elem->data)->key));
	Info.dwCursorPosition.X += 2+15;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	LinklistTraverse(&((IDXTERMTYPE *)elem->data)->bnolist, traverse_print_bno);
	printf("\b \n");
	return OK;
}

void print_idxlist(SQLIST *idxlist)
{
	CONSOLE_SCREEN_BUFFER_INFO Info;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Info);
	Info.dwCursorPosition.X += 5;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("关键词");
	Info.dwCursorPosition.X += 15;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Info.dwCursorPosition);
	printf("书号索引\n");
	SqlistTraverse(idxlist, traverse_print_idxlist);
}

void ShowKeywordIndex()
{
	SQLIST idxlist;
	char text[1024];
	IDXTERMTYPE idx;
	ELEMENT e;
	size_t pos;
	
	printf_color(FOREGROUND_GREEN, "关键词索引表演示:\n\n");

	if (CreateIdxList(&idxlist) == OK)
	{
		print_idxlist(&idxlist);
		StrInit(&idx.key);
		do
		{
			printf("输入关键词检索书号(\"exit\"退出):\n");
			scanf_s("%s", text, 1024);
			StrAssign(&idx.key, text);
			e.data = &idx;
			e.size = sizeof(idx);
			pos = SqlistLocate(&idxlist, &e);
			if (pos != -1)
			{
				SqlistGetElem(&idxlist, pos, &e);
				traverse_print_idxlist(&e);
			}
		} while (_stricmp(text, "exit") != 0);
		StrDestroy(&idx.key);
		DestroyIdxList(&idxlist);
	}
}

void ShowHString(int cat)
{
	switch (cat)
	{
	case 1:
		ShowIndex();
		break;
	case 2:
		ShowKeywordIndex();
		break;
	default:
		break;
	}
}