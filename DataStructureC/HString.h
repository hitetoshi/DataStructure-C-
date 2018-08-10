#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//�����Խ̲ĵĶѷ���ṹ������һЩ�޸�
	//�ڷ��䴮�洢�ռ�ʱ,�����1���ַ��ĳ���
	//�ڴ���ĩβ�洢'\0',����������C/C++����
	//��ʹ�����ǵĴ��ṹ
	
	//Unicode String
	//typedef	wchar_t HSElem;
	//#define	HSElem_Len(s)	wcslen(s)

	typedef	char	HSElem;
#define	HSElem_Len(s)	CSTRING_LENGTH(s)

	typedef	struct _HString
	{
		HSElem *ch;	//���Ƿǿմ�,�򰴴����ȷ���洢��,����chΪNULL
		size_t length;
	}HString;

	void StrInit(HString *t);
	Status StrDestroy(HString *s);
	Status StrClear(HString *s);
	Status StrAssign(HString *t, HSElem *chars);
	Status StrCopy(HString *t, HString *s);
	Status StrEmpty(HString *s);
	int StrCompare(HString *s, HString *t);
	size_t StrLength(HString *s);
	//��s1��s2���Ӻ�����´�t
	Status StrConcat(HString *t, HString *s1, HString *s2);
	//�㷨4.3���ش�sλ��pos�𳤶�Ϊlen���Ӵ�
	Status StrSubString(HString *sub, HString *s, size_t pos, size_t len);
	//�㷨4.5�ĸĽ�,�ɶ���˳��ṹ���Ľ����ѷ���ṹ��
	//���Ӵ�������,����-1
	size_t StrIndex(HString *s, HString *t, size_t pos);
	//�㷨4.6ģʽƥ��KMP�㷨
	size_t StrIndexKMP(HString *s, HString *t, size_t pos);
	//�㷨4.7��ģʽ��t��next����
	void KMP_Next(HString *t, int next[]);
	//�㷨��4.8��ģʽ��t��next��������
	void KMP_NextVal(HString *t, int next[]);
	Status StrReplace(HString *s, HString *t, HString *v);
	//�㷨4.4
	Status StrInsert(HString *s, size_t pos, HString *t);
	Status StrDelete(HString *s, size_t pos, size_t len);

	//���书��
	//����s���ӵ�t֮��
	Status StrCat(HString *t, HString *s);
	//���ַ�c���ӵ�t֮��
	Status StrCatc(HString *t, HSElem c);
	//����HSElemָ��
	HSElem *StrPointer(HString *t);
	//����posλ�õ��ַ�,��λ�÷Ƿ�,����0
	HSElem StrElem(HString *t, size_t pos);

#ifdef __cplusplus
}
#endif // __cplusplus

