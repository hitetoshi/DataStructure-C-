#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	BUFFER_TOO_SMALL	-3

	//һЩ����������

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
	�������������ж���ʽ����ElemTypeΪ

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

	������ɢ�¼�ģ���пͻ��˶��е�ElementTypeΪ

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

	typedef	int (__cdecl *PCOMPAREROUTINE)(ELEMENT *firstelem, ELEMENT *secondelem);
	typedef	void * (__cdecl *PALLOCATEROUTINE)(size_t bytesize);
	typedef	void(__cdecl *PFREEROUTINE)(void *buffer);

#define	CMEM_ALOC(size)				malloc(size)
#define	CMEM_FREE(block)			free(block)
#define	CMEM_REALOC(block, size)	realloc(block,size)
#define	CMEM_SET(block, v, size)	memset(block,v,size)
#define	CMEM_COPY(dest,src,size)	memcpy(dest,src,size)

#define	CSTRING_LENGTH(str)	_strlen(str)

#define	CISDIGIT(c)		_isdigit(c)
#define	CTIME(_Time)	time(_Time)

	//һЩ���ú���
	void * __cdecl CommonAllocRotuine(size_t bytes);
	void __cdecl CommonFreeRoutine(void *block);
	int RangeRandom(int range_min, int range_max);
	size_t _strlen(char *s);
	Status _isdigit(int c);

#ifdef __cplusplus
}
#endif // __cplusplus
