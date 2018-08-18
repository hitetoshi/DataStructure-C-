#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//					�����ݽṹ��C���԰棩���������ݽṹ���㷨��C����ʵ��					        //
	//																									//
	//DataStructureC��̬�������ʵ���ˡ����ݽṹ��C���԰棩�����������ݽṹ���㷨��						//
	//																									//
	//�������ݽṹ�Ĳ�ͬ���ͣ�����ջ��˫����С�ѭ�����еȣ���Ϊ�Ѿ�ʵ���ܷ�Ӧ�ṹ�ص���Ҫ�����Ҹ����͵�//
	//ԭ���������ṹ�Ѿ�����չʾ���Բ����ô����ظ�ʵ�֡�												//
	//																									//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ӱ�̵ļ����㷨Ϊʵ��Ӧ�õ�ԭ�������DataStructureC�Ĵ�����̲Ĵ���һЩ���죺					//
	//																									//
	//1.�̲���ΪC�����±���0��ʼʹ����Ȼ���������������㣬��˽̲��������±����1��ʼ���磺���ֽṹԪ�� //
	//��λ�������ص�Ԫ��λ�ã�������1��Ϊ�±���ʼ��ʵ��C���Ա���и��Ӳ����㣨�磺C���Է����������0�±�//
	//��ʼ������ˣ�DataStructureC�������±��ʹ�þ���0��ʼ������ܵ���ĳЩ�㷨�Ĵ�����̲ĵ����������� //
	//һ�������нϴ����磺��KMP�㷨��next���麯������												//
	//																									//
	//2.�̲Ķ����ݽṹ�����Ķ���ʹ���˴�����C++�еġ����ã�&�������ݲ���������C���Բ�֧�ֵģ�			//
	//DataStructureC����Щ���ô��θ�Ϊ��C����֧�ֵġ�����ַ���Σ�*������								//
	//																									//
	//3.�̲Ķ�ĳЩ�ṹ�ͺ������������Ǻ�ͳһ,����û��������,������C��׼�����������γɳ�ͻ��			//
	//DataStructureCͳһ�ṹ�����Ͷ���Ϊȫ��д����LINKLIST��STACK��HSTRING��������������������Ϊ���ṹ��//
	//+������������SqlistInit��StrIndex��������ȫСд������������⣩��									//
	//																									//
	//4.һЩ�����ģ�������Ϊ��ʵ�ʱ���в�������߲�����ĵط����磺									//
	//1������ʹ��ȫ�ֱ����ں���֮�䴫�����ݣ���ɢ�¼�ģʽ��DataStructureCͨ�����Ӻ������ò������Բ����� //
	//�����ݡ�																							//
	//2�����㷨�ڲ��������ڴ浫�ǲ��ͷŵģ�����鲢�����ݡ�˭����˭�ͷš���ԭ��������ڴ���Ҫ��������	//
	//��ʹ�ã���DataStructureC�����㷨�з�����ڴ棬����Ҫ����������з����ͨ���������ݸ��㷨��		//
	//3�������ӿڷ�װ���Ѻõģ�����ṹ��ʹ�ÿɱ�������ݸ�ά���Ⱥ͸�ά�±꣬�����������Ҫ��ʼ��һ��ά //
	//�Ⱥܴ�������ں�������ʱ��Ҫ���ݵĲ���̫�ࣻ����ʽ��ֵ�ͱ��ʽ��ֵҪ���û����㷨�ڲ��������ݣ��㷨//
	//�������޷���ֲ��DataStructureC�ڲ��ı�ԭ�㷨�Ļ����Ͻ������ӿڷ�װ�ĸ����Ѻø�����������ֲ��		//
	//4��ĳЩ�������꾡���߿��ܲ���Bug�ģ��μ���StrInsert����ע�ͣ�LinklistOrderLocalElem����ע�ͣ�		//
	//PolynAdd����ע�ͣ�RLSMatrixMult����ע�͡�DataStructureC�����������޸ġ�							//
	//5���㷨�޷�ʵ�־���Ӧ�õģ��ؼ���������DataStructureC�����˵����߽ӿڣ�ʹ�㷨�ܹ��������߾���Ӧ //
	//�á�																								//
	//																									//
	//5.Ϊ����ǿ���뽡׳�ԺͿ���ֲ�������޸ĺͲ��䣨�ο�������ע�Ͳ��֣���								//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//																									//
	//���⣬DataStructureC��ʵ�ֵ�ĳЩ�㷨��������һЩ���ۣ�����ɢ�¼�ģ�����ҵ�����㷨��KMP�㷨��Ч�� //
	//�ȣ��ɲμ�����㷨��ע�͡�																		//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//�ļ���֯�ṹ��																					//
	//DataStructureC.h	//����ͷ�ļ�,�����ļ����������ݽṹ���塢���ú�������							//
	//DataStructureC.c	//���ú�����ʵ��																//
	//List.h			//����˳�����������һԪ����ʽ�����ݽṹ����ͺ�������						//
	//list.c			//����˳�����������һԪ����ʽ��ʵ��										//
	//Stack.h			//˳��ջ������ת��������ƥ�䡢�б༭���Թ���⡢���ʽ��ֵ���ݽṹ����ͺ�������//
	//Stack.c			//˳��ջ������ת��������ƥ�䡢�б༭���Թ���⡢���ʽ��ֵ��ʵ��				//
	//Queue.h			//˳����С���ɢ�¼�ģ������ݽṹ����ͺ�������								//
	//Queue.c			//˳����С���ɢ�¼�ģ���ʵ��													//
	//HString.h			//�ѷ���洢�ṹ����������������ݽṹ����ͺ�������							//
	//HString.c			//�ѷ���洢�ṹ�������������ʵ��												//
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//һЩ����������

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	UNDERFLOW	-3
#define	BUFFER_TOO_SMALL	-4

	typedef	int	Status;

#ifndef _VCRUNTIME_H
#ifdef _WIN64
	typedef	unsigned __int64 size_t;
#else
	typedef	unsigned int size_t;
#endif // _WIN64

#endif // !_VCRUNTIME_H


	//ͨ�ýڵ�Ԫ��
	typedef	struct _ELEMENT
	{
		void *data;		//ָ��Ԫ�����ݵ�ָ��
		size_t size;	//Ԫ�����ݴ�С
	}ELEMENT, *PELEMENT;

	//ͨ����ʽ�ڵ�
	/*�̲ĶԽڵ�Ԫ�ص����Ͷ���ΪElemType,��Բ�ͬ��Ӧ������,ElemType���в��

	�������������ж���ʽ����ElemTypeΪ:
	typedef	struct _POLYN_ELEMENT_DATA
	{
		float coef;		//ϵ��
		int expn;		//ָ��
	}POLYN_ELEMENT_DATA, *PPOLYN_ELEMENT_DATA;

	ջ�н���ת��ElemTypeΪint,�б༭ElemTypeΪchar,�Թ������ElementTypeΪ
	typedef	struct _MAZEELEMENT
	{
		int ord;			//ͨ������·���ϵ����
		COORDINATE seat;	//ͨ�������Թ��е�����λ��
		int di;				//�Ӵ�ͨ����������һͨ����ķ���
	}MAZEELEMENT;

	������ɢ�¼�ģ���пͻ��˶��е�ElementTypeΪ:
	typedef	struct _BM_CUSTOM
	{
		int ArrivalTime;	//����ʱ��
		int Duration;		//������������ʱ��
	}BM_CUSTOM;

	�����Բ�ͬ��Ӧ��,��ͬ�Ľڵ�Ԫ���������Ͷ�Ҫȥ��д���ݽṹ����ش���
	(�ڴ����,Ԫ��ֵ�Ƚ�,Ԫ��ֵ��ֵ)��ô���������൱��,���Ҵ���Ŀ���ֲ��
	Ҳ�ܵ͡�Ϊ�˷��������,�����������ݽṹ�����������ͨ�ýڵ�����,���ص�
	��,���ݽṹ�������Ľڵ�Ԫ������,ֻ���������Ĳ���,����ELEMENT.size
	�����ڴ�,Ԫ�ظ�ֵ�Ĳ��������ڴ濽��memcpy,Ԫ�رȽ��ɵ������ṩ�ȽϺ���
	(��PCOMPAREROUTINE���͵�ע��)����Ԫ���������͵Ľ����ɵ�����������ɡ�
	����,��ͬ�ľ���Ӧ�ÿ��Ը����Լ���ʵ����Ҫ�����Լ���Ԫ���������Ͳ�������
	����,���ݽṹ����Ҫ�޸��κδ���*/
	typedef	struct _LINKNODE
	{
		struct _ELEMENT elem;
		struct _LINKNODE *next;
	}LINKNODE, *PLINKNODE;

	//ͨ������
	typedef	struct _COORDINATE
	{
		int x;	//x����
		int y;	//y����
	}COORDINATE,*PCOORDINATE;

	//�����д󲿷����ݽṹ�ĳ�ʼ����������Ҫ�������ṩ�����ص�������Ϊ����

	//1.PCOMPAREROUTINE:������ȽϺ���,�������ݽṹʹ�õ���������Ϊ��������
	//��������Ҫ���бȽϲ���(�綨λ������)ʱ,����Ҫ�������ṩ�ȽϺ���,�����
	//ÿ�������Ĳ����ж��ɵ����ߴ���ȽϺ���,��ȽϷ������������ݽṹ��ʼ��
	//ʱ,�ɵ����ߴ���,���ݽṹ������ص��������ڽṹ�ڲ�,��Ҫʹ��ʱֱ�ӵ���

	//2.PALLOCATEROUTINE/PFREEROUTINE�ڴ������ͷź���:���������ڲ�ͬ�ı��
	//�����е������ݽṹʱ,���ܻ����ʵ������Զ����ڴ�ķ�����ͷŲ���(��
	//C++�п�ʹ��new/delete�����ͷ��ڴ棻WIN 32�����¿�ʹ��HeapAlloc/HeapFree
	//����/�ͷ��ڴ棻WDK��������ExAllocatePoolWithTag/ExFreePoolWithTag����/
	//�ͷ��ڴ�)�����߸����Լ�ʵ��ʹ������ڳ�ʼ�����ݽṹʱ�����ڴ���䡢�ͷ�
	//�Ļص�����,����Ҫʱ,�ṹ�ᰴ�յ������ṩ�ķ������в���

	//ͬ��,�������ݽṹ������ֲʱ,���ݽṹ���������ڴ�����������Ŀ⺯������Ҳ
	//���в�ͬ,��ֱ���޸����涨���CMEM_ALOC��CMEM_FREE�Ⱥ��������

	typedef	int (__cdecl *PCOMPAREROUTINE)(ELEMENT *firstelem, ELEMENT *secondelem);
	typedef	void * (__cdecl *PALLOCATEROUTINE)(size_t bytesize);
	typedef	void(__cdecl *PFREEROUTINE)(void *buffer);

#define	CMEM_ALOC(size)				malloc(size)
#define	CMEM_FREE(block)			free(block)
#define	CMEM_REALLOC(block, size)	realloc(block,size)
#define	CMEM_SET(block, v, size)	memset(block,v,size)
#define	CMEM_COPY(dest,src,size)	memcpy(dest,src,size)
#define	CMEM_MOVE(dest,src,size)	memmove(dest,src,size)

#define	CSTRING_LENGTH(str)	strlen(str)
#define	CSTRING_LOWER(str)	_strlwr_s(str,CSTRING_LENGTH(str)+1)
#define	CSTRING_ATOI(str)	atoi(str)

#define	CISDIGIT(c)		isdigit(c)
#define	CTIME(_Time)	time(_Time)

	//һЩ���ú���
	void * __cdecl CommonAllocRotuine(size_t bytes);
	void __cdecl CommonFreeRoutine(void *block);
	int RangeRandom(int range_min, int range_max);
	//ͨ�������������ͱȽϺ���
	int __cdecl CommonIntCompareRoutine(ELEMENT *first, ELEMENT *second);

#ifdef __cplusplus
}
#endif // __cplusplus
