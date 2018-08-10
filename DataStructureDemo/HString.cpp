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

void ShowHString()
{
	HString t1, t2, table;
	int str_len;
	size_t pos;
	size_t substr_len;
	LARGE_INTEGER end_time;
	LARGE_INTEGER begin_time;
	LARGE_INTEGER freq;
	double us_time;

	printf("����ʾ(�ѷ���洢�ṹ):\n\n");
	srand((int)time(NULL));
	StrInit(&t2);
	StrInit(&t1);
	StrInit(&table);

	QueryPerformanceFrequency(&freq);

	StrAssign(&table, string_table);

	str_len = RangeRandom(200, 300);	//����200-300������������
	RandomString(&table, str_len, &t1);
	printf("������ɳ��� %d ��:\n%s\n", str_len, StrPointer(&t1));

	pos = RangeRandom(0, str_len);
	substr_len = RangeRandom(1,(int)StrLength(&t1) - (int)pos);
	StrSubString(&t2, &t1, pos, substr_len);
	printf("\n�����ȡ���� %zd λ�� %zd �Ӵ�:\n%s\n", substr_len, pos, StrPointer(&t2));

	QueryPerformanceCounter(&begin_time);
	pos = StrIndex(&t1, &t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("\nģʽƥ�� λ��: %zd ʱ��: %f΢��\n", pos, us_time);

	QueryPerformanceCounter(&begin_time);
	pos = StrIndexKMP(&t1, &t2, 0);
	QueryPerformanceCounter(&end_time);
	us_time = (end_time.QuadPart - begin_time.QuadPart) * 1000000.0 / freq.QuadPart;
	printf("ģʽƥ��(KMP�㷨) λ��: %zd ʱ��: %f΢��\n", pos, us_time);
	printf("\n����������㷢��KMP�㷨����ʱ��Ƚ���ͨ�㷨����ʱ�仹Ҫ��,��ο�HString.h�ļ�������KMP�㷨Ч�ʵ�����\n");

	StrDestroy(&t1);
	StrDestroy(&t2);
	StrDestroy(&table);
}