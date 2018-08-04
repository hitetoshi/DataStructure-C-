#pragma once

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2

#ifdef cplusplus
extern "C"
{
#endif // cplusplus

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

#ifdef cplusplus
}
#endif // cplusplus
