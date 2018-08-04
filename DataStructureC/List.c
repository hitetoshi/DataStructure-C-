#include "list.h"

Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	list->elem = MEM_ALOC(LIST_INIT_SIZE*sizeof(NODEELEMENT **));
	if (list->elem)
	{
		memset(list->elem, 0, LIST_INIT_SIZE * sizeof(NODEELEMENT **));
		list->length = 0;
		list->listsize = LIST_INIT_SIZE;
		list->compareroutine = compareroutine;
		list->allocateroutine = allocateroutine;
		list->freeroutine = freeroutine;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

Status SqlistInsert(SQLIST *list, size_t pos, NODEELEMENT *elem)
{
	NODEELEMENT **new_base;
	NODEELEMENT **p;
	NODEELEMENT *new_elem;

	if (pos<0 || pos>list->length)
	{
		return ERROR;
	}
	if (list->length >= list->listsize)
	{
		new_base = MEM_REALOC(list->elem, (list->length + LIST_INCREMENT) * sizeof(NODEELEMENT **));
		if (new_base)
		{
			list->elem = new_base;
			memset(list->elem + list->listsize, 0, sizeof(NODEELEMENT **) * LIST_INCREMENT);
			list->listsize += LIST_INCREMENT;
		}
		else
		{
			return OVERFLOW;
		}
	}
	new_elem = MEM_ALOC(sizeof(NODEELEMENT));
	if (new_elem)
	{
		memset(new_elem, 0, sizeof(NODEELEMENT));
		new_elem->data = list->allocateroutine(elem->size);
		if (new_elem)
		{
			memcpy(new_elem->data, elem->data, elem->size);
			new_elem->size = elem->size;
			//时间复杂度T(n)=O(list->length)
			for (p = list->elem + list->length - 1; p >= list->elem + pos; p--)
			{
				*(p + 1) = *p;
			}
			*(list->elem + pos) = new_elem;
			list->length++;
			return OK;
		}
		else
		{
			MEM_FREE(new_elem);
			return OVERFLOW;
		}
	}
	else
	{
		return OVERFLOW;
	}
}

void SqlistDestroy(SQLIST *list)
{
	//T(n)=O(list->length)
	//由SqlistClear决定
	SqlistClear(list);
	MEM_FREE(list->elem);
	list->elem = NULL;
	list->listsize = 0;
}

void SqlistClear(SQLIST *list)
{
	size_t i;

	//时间复杂度T(n)=O(list->length)
	for (i = 0; i < list->length; i++)
	{
		MEM_FREE((*(list->elem + i))->data);
		(*(list->elem + i))->data = NULL;
		(*(list->elem + i))->size = 0;
		MEM_FREE(*(list->elem + i));
		*(list->elem + i) = NULL;
		list->length--;
	}
}

bool SqlistEmpty(SQLIST *list)
{
	return (bool)(list->length==0);
}

size_t SqlistLength(SQLIST *list)
{
	return list->length;
}

Status SqlistGetElem(SQLIST *list, size_t pos, NODEELEMENT *elem)
{
	//时间复杂度T(n)=O(1)
	if (pos >= 0 && pos < list->length)
	{
		elem->data = list->elem[pos]->data;
		elem->size = list->elem[pos]->size;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

size_t SqlistLocate(SQLIST *list, NODEELEMENT *elem)
{
	size_t pos;

	//T(n)=O(list->length)
	for (pos = 0; pos < list->length; pos++)
	{
		if (list->compareroutine(elem, list->elem[pos]) == 0)
		{
			return pos;
		}
	}
	return -1;
}

Status SqlistPriorElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem)
{
	//T(n)=O(list->length)
	size_t pos = SqlistLocate(list, elem);
	if (pos != -1 && pos > 0)
	{
		pre_elem->data = list->elem[pos-1]->data;
		pre_elem->size = list->elem[pos-1]->size;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

Status SqlistNextElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem)
{
	//T(n)=O(list->length)
	size_t pos = SqlistLocate(list, elem);

	if (pos >= 0 && pos < list->length - 1)
	{
		pre_elem->data = list->elem[pos + 1]->data;
		pre_elem->size = list->elem[pos + 1]->size;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

Status SqlistDelete(SQLIST *list, size_t pos, NODEELEMENT *elem)
{
	NODEELEMENT **p;
	if (pos >= 0 && pos < list->length)
	{
		if (elem->size >= list->elem[pos]->size)
		{
			memcpy(elem->data, list->elem[pos]->data, list->elem[pos]->size);
			elem->size = list->elem[pos]->size;
		}
		list->freeroutine(list->elem[pos]->data);
		MEM_FREE(list->elem[pos]);

		//T(n)=O(list->length)
		for (p = list->elem + pos; p < list->elem + list->length - 1; p++)
		{
			*p = *(p + 1);
		}
		list->length--;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

void SqlistUnio(SQLIST *list1, SQLIST *list2)
{
	NODEELEMENT elem;

	//T(n)=O(list2->length*list1->length)
	for (size_t pos = 0; pos < list2->length; pos++)
	{
		if (SqlistGetElem(list2, pos, &elem) == OK)
		{
			if (SqlistLocate(list1, &elem) == -1)
			{
				SqlistInsert(list1, list1->length, &elem);
			}
		}
	}
}

/*void LinklistInit(LINKLIST *pLinklist, PLINKLIST_COMPARE_ROUTINE CompareRoutine,
	PLINKLIST_ALLOCATE_ROUTINE AllocateRoutine, PLINKLIST_FREE_ROUTINE FreeRoutine)
{
	pLinklist->AllocateRoutine = AllocateRoutine;
	pLinklist->CompareRoutine = CompareRoutine;
	pLinklist->FreeRoutine = FreeRoutine;
	LinklistFirstNode(pLinklist) = NULL;
}

void LinklistClear(LINKLIST *pLinklist)
{
	PLINKLIST_NODE pNode = LinklistFirstNode(pLinklist);
	PLINKLIST_NODE pNextNode;
	while (pNode)
	{
		pLinklist->FreeRoutine(pLinklist, pNode);
		pNextNode = pNode->pNext;
		free(pNode);
		pNode = pNextNode;
	}
}

bool LinklistEmpty(LINKLIST *pLinklist)
{
	return (bool)(LinklistFirstNode(pLinklist));
}

int LinklistLength(LINKLIST *pLinklist)
{
	int count = 0;
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	while (pNode)
	{
		count++;
		pNode = pNode->pNext;
	}
	return count;
}

void *LinklistGetElement(LINKLIST *pLinklist, int i)
{
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	int index = 0;

	while (pNode && index++!=i)
	{
		pNode = pNode->pNext;
	}
	return pNode?pNode->Element.data:NULL;
}

void *LinklistLookupElement(LINKLIST *pLinklist, void *pElement)
{
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	void *pLinklistElement = NULL;

	while (pNode && !pLinklistElement)
	{
		if (pLinklist->CompareRoutine(pLinklist, pElement, pNode->Element.data) == 0)
		{
			pLinklistElement = pNode->Element.data;
		}
		pNode = pNode->pNext;
	}

	return pLinklistElement;
}

void *LinklistPriorElement(LINKLIST *pLinklist, void *pElement)
{
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	void *pLinklistElement=NULL;
	
	if (pNode)
	{
		while (pNode->pNext && !pLinklistElement)
		{
			if (pLinklist->CompareRoutine(pLinklist, pElement, pNode->pNext->Element.data) == 0)
			{
				pLinklistElement = pNode->Element.data;
			}
			pNode = pNode->pNext;
		}
	}
	return pLinklistElement;
}

void *LinklistNextElement(LINKLIST *pLinklist, void *pElement)
{
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	void *pLinklistElement = NULL;

	while (pNode && pNode->pNext && !pLinklistElement)
	{
		if (pLinklist->CompareRoutine(pLinklist, pElement, pNode->Element.data) == 0)
		{
			pLinklistElement = pNode->pNext->Element.data;
		}
	}
	return pLinklistElement;
}

void *LinklistInsertElement(LINKLIST *pLinklist, int i, void *pElement, unsigned int nElementSize)
{
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	int index = 0;
	LINKLIST_NODE *pNewNode = NULL;

	if (i != 0)
	{
		while (pNode && index++ != i-1)
		{
			pNode = pNode->pNext;
		}
	}
	if (pNode || i==0)
	{
		pNewNode = malloc(sizeof(LINKLIST_NODE));
		if (pNewNode)
		{
			pNewNode->Element.data = pLinklist->AllocateRoutine(pLinklist, nElementSize);
			if (pNewNode->Element.data)
			{
				memcpy(pNewNode->Element.data, pElement, nElementSize);
				pNewNode->Element.size = nElementSize;
				pNewNode->pNext = (i==0? LinklistFirstNode(pLinklist):pNode->pNext);
				(i == 0) ? (LinklistFirstNode(pLinklist) = pNewNode) : (pNode->pNext = pNewNode);
			}
			else
			{
				free(pNewNode);
				pNewNode = NULL;
			}
		}
	}
	return pNewNode;
}

bool LinklistDeleteElement(LINKLIST *pLinklist, int i, void *pElement, unsigned int nElementSize)
{
	LINKLIST_NODE *pNode = LinklistFirstNode(pLinklist);
	int index = 0;
	bool ret = false;

	if (i != 0)
	{
		while (pNode && index++ != i - 1)
		{
			pNode = pNode->pNext;
		}
	}
	if (pNode && pNode->pNext)
	{
		if (nElementSize >= pNode->pNext->Element.size)
		{
			memcpy(pElement, pNode->pNext->Element.data, pNode->pNext->Element.size);
		}
		pLinklist->FreeRoutine(pLinklist, pNode->pNext->Element.data);
		pNode->pNext = pNode->pNext->pNext;
		free(pNode->pNext);
		ret = true;
	}
	return ret;
}*/