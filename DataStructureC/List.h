#pragma once

#include <stdlib.h>
#include <memory.h>
#include <stdbool.h>

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	LIST_INIT_SIZE	100
#define	LIST_INCREMENT	10

	typedef	struct _SQLIST
	{
		NODEELEMENT **elem;					//�洢�ռ��ַ
		size_t length;						//��ǰ����
		size_t listsize;					//��ǰ����Ĵ洢����
		PCOMPAREROUTINE compareroutine;		//�ȽϺ���
		PALLOCATEROUTINE allocateroutine;	//�ڴ���亯��
		PFREEROUTINE freeroutine;			//�ڴ��ͷź���

	}SQLIST, *PSQLIST;

	Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status SqlistInsert(SQLIST *list, size_t pos, NODEELEMENT *elem);
	void SqlistDestroy(SQLIST *list);
	void SqlistClear(SQLIST *list);
	bool SqlistEmpty(SQLIST *list);
	size_t SqlistLength(SQLIST *list);
	Status SqlistGetElem(SQLIST *list, size_t pos, NODEELEMENT *elem);
	size_t SqlistLocate(SQLIST *list, NODEELEMENT *elem);
	Status SqlistPriorElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem);
	Status SqlistNextElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem);
	Status SqlistDelete(SQLIST *list, size_t pos, NODEELEMENT *elem);
	void SqlistUnio(SQLIST *list1, SQLIST *list2);

	typedef	struct _LINKLIST_NODE
	{
		NODEELEMENT *elem;
		struct _LINKLIST_NODE *next;
	}LINKLISTNODE, *PLINKLISTNODE;

	typedef struct _LINKLIST
	{
		LINKLISTNODE *header, *tail;		//�ֱ�ָ��ͷβ�ڵ�
		size_t len;							//Ԫ�ظ���
		PCOMPAREROUTINE compareroutine;		//�ȽϺ���
		PALLOCATEROUTINE allocateroutine;	//�ڴ���亯��
		PFREEROUTINE freeroutine;			//�ڴ��ͷź���
	}LINKLIST,*PLINKLIST;

	Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);

/*typedef	int(*PLINKLIST_COMPARE_ROUTINE)(struct _LINKLIST *pLinkList, void *FirstStruct, void *SecondStruct);
typedef	void *(*PLINKLIST_ALLOCATE_ROUTINE)(struct _LINKLIST *pLinkList, unsigned int ByteSize);
typedef	void(*PLINKLIST_FREE_ROUTINE)(struct _LINKLIST *pLinkList, void *Buffer);

typedef	struct _LINKLIST_ELEMENT
{
	void *data;
	unsigned int size;
}LINKLIST_ELEMENT;

typedef	struct _LINKLIST_NODE
{
	LINKLIST_ELEMENT Element;
	struct _LINKLIST_NODE *pNext;
}LINKLIST_NODE, *PLINKLIST_NODE;

typedef	struct _LINKLIST
{
	PLINKLIST_COMPARE_ROUTINE CompareRoutine;
	PLINKLIST_ALLOCATE_ROUTINE AllocateRoutine;
	PLINKLIST_FREE_ROUTINE FreeRoutine;
	PLINKLIST_NODE pFirstNode;
}LINKLIST, *PLINKLIST;

void LinklistInit(LINKLIST *pLinklist, PLINKLIST_COMPARE_ROUTINE CompareRoutine,
	PLINKLIST_ALLOCATE_ROUTINE AllocateRoutine, PLINKLIST_FREE_ROUTINE FreeRoutine);
void LinklistClear(LINKLIST *pLinklist);
#define	LinklistFirstNode(p)	((LINKLIST *)p)->pFirstNode
bool LinklistEmpty(LINKLIST *pLinklist);
int LinklistLength(LINKLIST *pLinklist);
void *LinklistGetElement(LINKLIST *pLinklist, int i);
void *LinklistLookupElement(LINKLIST *pLinklist, void *pElement);
void *LinklistPriorElement(LINKLIST *pLinklist, void *pElement);
void *LinklistNextElement(LINKLIST *pLinklist, void *pElement);
void *LinklistInsertElement(LINKLIST *pLinklist, int i, void *pElement, unsigned int nElementSize);
bool LinklistDeleteElement(LINKLIST *pLinklist, int i, void *pElement, unsigned int nElementSize);*/

#ifdef __cplusplus
}
#endif // __cplusplus
