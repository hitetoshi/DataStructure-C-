#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

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

	typedef	int	Status;

	typedef	struct _NODEELEMENT
	{
		void *data;
		size_t size;
	}NODEELEMENT, *PNODEELEMENT;

	typedef	int (__cdecl *PCOMPAREROUTINE)(NODEELEMENT *firstelem, NODEELEMENT *secondelem);
	typedef	void * (__cdecl *PALLOCATEROUTINE)(size_t bytesize);
	typedef	void(__cdecl *PFREEROUTINE)(void *buffer);

#define	MEM_ALOC(size)			malloc(size)
#define	MEM_FREE(block)			free(block)
#define	MEM_REALOC(block, size)	realloc(block,size)

#include "list.h"

#ifdef cplusplus
}
#endif // cplusplus
