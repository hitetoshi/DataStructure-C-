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
	//���㷨4.6����΢�Ľ�����̬�������ȫ��������Ϊnext��,�Ż���next���������
	size_t StrIndexKMPOpt(HString *s, HString *t, size_t pos);
	//�㷨4.7��ģʽ��t��next����
	void KMP_Next(HString *t, size_t next[]);
	//�㷨��4.8��ģʽ��t��next��������
	void KMP_NextVal(HString *t, size_t next[]);
	Status StrReplace(HString *s, HString *t, HString *v);
	//�㷨4.4
	Status StrInsert(HString *s, size_t pos, HString *t);
	Status StrDelete(HString *s, size_t pos, size_t len);

	//���书��
	//����s���ӵ�t֮��
	Status StrCat(HString *t, HString *s);
	//���ַ�c���ӵ�t֮��
	Status StrCatc(HString *t, HSElem c);
	//����HSElemָ��(����C����ʹ��)
	HSElem *StrPointer(HString *t);
	//����posλ�õ��ַ�,��λ�÷Ƿ�,����0
	HSElem StrElem(HString *t, size_t pos);

	/*****************************************************************************
							����KMP�㷨Ч�ʵ�����
	��������ͨģʽƥ���㷨�����ʱ�临�ӶȻ�ﵽO(n*m)��KMP�㷨��֤ʱ�临�Ӷ�ΪO(n+m)
	����ʵ��Ӧ���лᷢ��KMP�㷨��ִ��ʱ��Զ������ͨ�㷨(���ɴ�10��������)
	��ģʽƥ���㷨��ʱ�临�Ӷȶ���ѡȡ"�Ƚ�"������Ϊ��������

	���������㷨��"ָ�����"�Ĳ���:
	��ͨ�㷨:
	else
	{
		//ָ��������¿�ʼƥ��
		i = i - j + 1;
		j = 0;
	}
	�������Ż���Ļ�����Ϊ:
	mov         ecx,1		//����Ѱַ
	sub         ecx,esi		//�Ĵ���Ѱַ
	add         eax,ecx		//�Ĵ���Ѱַ		i=i-j+1;
	xor         esi,esi		//�Ĵ���Ѱַ		j=0;

	KMP�㷨:
	else
	{
		//ָ�����
		j = next[j];
	}
	�������Ż���Ļ�����Ϊ:
	mov         eax,dword ptr [edi+eax*4]	//��ַ��ַ	j=next[j];

	�ɼ�,��������ͨ�㷨������Ѱַ�ͼĴ���Ѱַ���ٶ�Ҫ��Ķ�,KMP�㷨����ʹ��һ���ڴ���
	��next��,ֻ���Ի�ַ��ַ��Ѱַ��ʽ�������ںܴ�̶���Ӱ����KMP�㷨��ʵ��Ч�ʡ�
	
	ͨ�������ɼ�,�����㷨���ӵ�ָ�겻Ӧ������ʱ�临�ӶȺͿռ临�Ӷ�;��Ӧ����ѡ����ȷ��
	��������,�̲���ѡ��"�Ƚ�"��Ϊ��������,����������������"�ڴ�Ƚ�"�����ڴ�Ƚϲ�����,
	Ӱ��Ч�ʵĹؼ�����Ӧ��"�ڴ����"����"�Ƚ�"(cmpָ��Ŀ���Զ���ڻ�ַ��ַѰַ�Ŀ���),
	��ȻKMP�㷨��ʱ�临�Ӷ�ΪO(n+m)������ÿ��ʧ�䶼����ģʽ��,����������,KMP�㷨
	�Ļ�������Ƶ��Ϊ2n+m��

	�������Ϸ���,�����Խ̲��㷨4.6����һ����С���޸�:��ʹ�öѷ��䶯̬����next����,������
	ȫ�ֱ���size_t g_next[1024],���п��Թ۲�KMP�㷨Ч�����˺ܴ�����,��ʱ��Ϊ��ȫ������
	�ķ���ʹ�����Ѱַ,�ٶȱȻ�ַ��ַ���졣
	*****************************************************************************/

#ifdef __cplusplus
}
#endif // __cplusplus

