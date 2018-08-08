#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#ifdef cplusplus
extern "C"
{
#endif // cplusplus

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	BUFFER_TOO_SMALL	-3

	//һЩ����������

	typedef	int	Status;

	//ͨ�ýڵ�����
	typedef	struct _ELEMENTNODE
	{
		void *data;		//ָ��Ԫ�����ݵ�ָ��
		size_t size;	//Ԫ�����ݴ�С
	}ELEMENTNODE, *PELEMENTNODE;

	//ͨ����������
	typedef	struct _COORDINATE
	{
		int x;	//x����
		int y;	//y����
	}COORDINATE,*PCOORDINATE;

	typedef	int (__cdecl *PCOMPAREROUTINE)(ELEMENTNODE *firstelem, ELEMENTNODE *secondelem);
	typedef	void * (__cdecl *PALLOCATEROUTINE)(size_t bytesize);
	typedef	void(__cdecl *PFREEROUTINE)(void *buffer);

#define	CMEM_ALOC(size)				malloc(size)
#define	CMEM_FREE(block)			free(block)
#define	CMEM_REALOC(block, size)	realloc(block,size)

#include "list.h"
#include "Stack.h"

#ifdef cplusplus
}
#endif // cplusplus
