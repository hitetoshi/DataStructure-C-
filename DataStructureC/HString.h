#pragma once

#include "DataStructureC.h"
#include "List.h"

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

	typedef	struct _HSTRING
	{
		HSElem *ch;	//�̲�:���Ƿǿմ�,�򰴴����ȷ���洢��,����chΪNULL
					//����:���ǿմ�,��ch[0]==0, length==0
		size_t length;
	}HSTRING;

	void StrInit(HSTRING *t);
	Status StrDestroy(HSTRING *s);
	Status StrClear(HSTRING *s);
	Status StrAssign(HSTRING *t, HSElem *chars);
	Status StrCopy(HSTRING *t, HSTRING *s);
	Status StrEmpty(HSTRING *s);
	int StrCompare(HSTRING *s, HSTRING *t);
	size_t StrLength(HSTRING *s);
	//��s1��s2���Ӻ�����´�t
	Status StrConcat(HSTRING *t, HSTRING *s1, HSTRING *s2);
	//�㷨4.3���ش�sλ��pos�𳤶�Ϊlen���Ӵ�
	Status StrSubString(HSTRING *sub, HSTRING *s, size_t pos, size_t len);
	//�㷨4.5�ĸĽ�,�ɶ���˳��ṹ���Ľ����ѷ���ṹ��
	//���Ӵ�������,����-1
	size_t StrIndex(HSTRING *s, HSTRING *t, size_t pos);
	//�㷨4.6ģʽƥ��KMP�㷨
	size_t StrIndexKMP(HSTRING *s, HSTRING *t, size_t pos);
	//���㷨4.6����΢�Ľ�����̬�������ȫ��������Ϊnext��,�Ż���next���������
	size_t StrIndexKMPOpt(HSTRING *s, HSTRING *t, size_t pos);
	//�㷨4.7��ģʽ��t��next����
	void KMP_Next(HSTRING *t, size_t next[]);
	//�㷨��4.8��ģʽ��t��next��������
	void KMP_NextVal(HSTRING *t, size_t next[]);
	//��v�滻s��������t��ȵĲ��ص����ִ�
	Status StrReplace(HSTRING *s, HSTRING *t, HSTRING *v);
	//�㷨4.4
	//�ڴ���pos���ַ�֮ǰ���봮t
	Status StrInsert(HSTRING *s, size_t pos, HSTRING *t);
	//�Ӵ�s��ɾ��pos���ַ��𳤶�Ϊlen���Ӵ�
	Status StrDelete(HSTRING *s, size_t pos, size_t len);

	//���书��
	//����s���ӵ�t֮��
	Status StrCat(HSTRING *t, HSTRING *s);
	//���ַ�c���ӵ�t֮��
	Status StrCatc(HSTRING *t, HSElem c);
	//����HSElemָ��(����C����ʹ��)
	HSElem *StrPointer(HSTRING *t);
	//����posλ�õ��ַ�,��λ�÷Ƿ�,����0
	HSElem StrElem(HSTRING *t, size_t pos);

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

	�������Ϸ���,�����Խ̲��㷨4.6����һ����С���޸�:��ʹ�öѷ��䶯̬����next����,������
	ȫ�ֱ���size_t g_next[1024],���п��Թ۲�KMP�㷨Ч�����˺ܴ�����,��ʱ��Ϊ��ȫ������
	�ķ���ʹ�����Ѱַ,�ٶȱȻ�ַ��ַ���졣
	*****************************************************************************/


	//�ؼ���������

/*�̲Ķ���ĸ����,�������ò���,��Ϊ����ʹ�õĶѷ���ṹ����˳�������
���Զ�̬�����ڴ�,����Ҫ�趨��󳤶�

#define	MAX_BOOK_NUM	1000	//����ֻ��1000���齨������
#define	MAX_KEY_NUM		2500	//�������������
#define	MAX_LIN_LEN		500		//��Ŀ������󳤶�
#define	MAX_WORD_NUM	10		//�ʱ���������
*/

	/*�̲��϶Դʱ��������͵Ķ���
	����û�в�����һ�Ķ���,���ǽ��ʱ���������Ͷ���ΪHString:
	һ��Ϊ�˱������ݻ�ѧ����
	����˳����Ԫ�����ͱ�����Ϊvoid *,ֻ��Ҫ�ڱ����н������ΪHString *����,�������ⶨ��
	���ݽ̲Ķ���,�ʱ�ֻ���Ƕ�����
	typedef	struct
	{
		char *item[];
		int last;
	}WordListType;*/

	//������������Ͷ���Ϊint,û�б�Ҫ����,ֻ��Ҫ�ڱ�Ӧ���н�
	//����ELEMENT.data����Ϊint *����
	//typedef	int ElemType;

	//����������
	typedef	struct
	{
		HSTRING key;		//�ؼ���
		LINKLIST bnolist;	//����������������
	}IDXTERMTYPE;

	/*�̲��϶����������͵Ķ���
	����û�в�����һ����,ʹ��SQLIST��Ϊ������ṹ
	��ELEMENT.data���ͽ���ΪIDXTERMTYPE *
	�������õ�˳���Ϊ��̬��,����û�б�����
	typedef	struct
	{
		IDXTERMTYPE item[MAX_KEY_NUM];
		int last;
	}IdxListType;*/

	//����������,�൱�ڽ̲��㷨4.9main����idxlistΪ����˳���
	Status CreateIdxList(SQLIST *idxlist);
	void DestroyIdxList(SQLIST *idxlist);

	//��ʼ������,��������idxlistΪ�ձ�
	//ע��:��̲Ĳ�ͬ,�����Ѿ���װ�õ����Ա�ṹ,û�б�Ҫ��idxlist.item[0]��Ϊһ�մ�
	Status InitIdxList(SQLIST *idxlist);
	//�㷨4.10�����Ϊbno�������ؼ��ʰ��ʵ�˳�����������idxlist
	Status InsertIdxList(SQLIST *idxlist, int bno, SQLIST *wdlist);
	//�㷨4.11��wd���شʱ�wdlist�е�i���ؼ���
	Status GetWord(SQLIST *wdlist, size_t i, HSTRING **wd);

	//�㷨4.13��������idxlist��i���ϲ����¹ؼ���wd����ʼ���������������Ϊ�ձ�
	//��Ϊ�ദ�ڴ�������,�����޸ķ���ֵ����ΪStatusΪ��
	Status InsertNewKey(SQLIST *idxlist, size_t i, HSTRING *wd);
	//�㷨4.14��������idxlist�ĵ�i���в������Ϊbno������
	Status InsertBook(SQLIST *idxlist, size_t i, int bno);

	//������ıȽϺ���
	//�㷨4.12ʹ��Locate����������������ҹؼ����Ƿ����
	//�������������ʼ��ʱ,ʹ��IdxListCompare��Ϊcompare����
	//�����ʹ�����Ա��SqlistOrderLocate��������ѯ�ؼ���
	int __cdecl IdxListCompare(ELEMENT *first, ELEMENT *second);

	/*****************************************************************************
								�ؼ��������������
	
	ͨ������������ᵽ,��ǰ�������ݽṹ���������Ͷ���Ϊ�����void *�����ڽṹ�ĳ�
	ʼ��������������Compare����,ʹ������ʹ����Щ���ݽṹ��ʱ����Ҫ�����¶�������
	����,����ض��������͵Ĳ���(���ҡ���λ�������ƶ���)������Ҫ��д���롣�����
	�������Ǵ���ı�д��,���������ݽṹ���õ�Ч�ʡ�
	*****************************************************************************/
	

#ifdef __cplusplus
}
#endif // __cplusplus

