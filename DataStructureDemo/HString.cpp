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
	printf("\nģʽƥ�� λ��: %zd ʱ��: %f ΢��\n", pos, us_time);

	QueryPerformanceCounter(&begin_time);
	pos = StrIndexKMP(t1, t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("ģʽƥ��(KMP�㷨) λ��: %zd ʱ��: %f ΢��\n", pos, us_time);

	QueryPerformanceCounter(&begin_time);
	pos = StrIndexKMPOpt(t1, t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("ģʽƥ��(KMP�㷨�Ż�) λ��: %zd ʱ��: %f ΢��\n", pos, us_time);
}

void ShowIndex()
{
	HString t1, t2, table;
	int str_len;
	size_t pos;
	size_t substr_len;

	printf_color(FOREGROUND_GREEN, "ģʽƥ����ʾ:\n");

	srand((int)time(NULL));
	StrInit(&t2);
	StrInit(&t1);
	StrInit(&table);

	StrAssign(&table, string_table);

	str_len = RangeRandom(300, 500);	//����300-500������������
	RandomString(&table, str_len, &t1);
	printf("������ɳ��� %d ��:\n%s\n", str_len, StrPointer(&t1));

	substr_len = RangeRandom(30, 50);	//�������30-50�����Ӵ�
	pos = RangeRandom(0, str_len - substr_len);
	StrSubString(&t2, &t1, pos, substr_len);
	printf("\n�����ȡ���� %zd λ�� %zd ģʽ��:\n%s\n", substr_len, pos, StrPointer(&t2));

	
	ShowStringIndex(&t1, &t2);
	printf_color(FOREGROUND_RED , "����������㷢��KMP�㷨����ʱ��Ƚ���ͨ�㷨����ʱ�仹Ҫ��,��ο�HString.h�ļ�����KMP�㷨Ч�ʵ�����\n\n");
	printf("�����������KMP�㷨�Ѻõ�������ģʽ��\n");
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
	printf("����KMP�㷨�Ѻó��� %zd ����:\n%s\n", StrLength(&t1), StrPointer(&t1));
	printf("����KMP�㷨�Ѻó��� %zd ģʽ��:\n%s\n", StrLength(&t2), StrPointer(&t2));
	ShowStringIndex(&t1, &t2);

	StrDestroy(&t1);
	StrDestroy(&t2);
	StrDestroy(&table);
}

const char *book_info[]=
{
	"005 Computer Data Structures",
	"010 Introduction to Data Structures",
	"023 Fundamentals of Data Structures",
	"034 The Design and Analysis of Computer Algorithms",
	"050 Introduction to Numberical Analysis",
	"067 Numberical Analysis"
};

const char *commonly_word[] =
{
	"an","a","of","the","to","and"
};

void ShowKeywordIndex()
{
	printf_color(FOREGROUND_GREEN, "�ؼ�����������ʾ:\n");
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