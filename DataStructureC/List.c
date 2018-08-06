#include "DataStructureC.h"

Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	list->elem = MEM_ALOC(LIST_INIT_SIZE*sizeof(NODEELEMENT));
	if (list->elem)
	{
		memset(list->elem, 0, LIST_INIT_SIZE * sizeof(NODEELEMENT));
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
	NODEELEMENT *new_base;
	NODEELEMENT *p;
	void *new_node_data;

	if (pos<0 || pos > list->length)
	{
		return INFEASIBLE;
	}
	if (list->length >= list->listsize)
	{
		new_base = MEM_REALOC(list->elem, (list->length + LIST_INCREMENT) * sizeof(NODEELEMENT));
		if (new_base)
		{
			list->elem = new_base;
			memset(list->elem + list->listsize, 0, sizeof(NODEELEMENT) * LIST_INCREMENT);
			list->listsize += LIST_INCREMENT;
		}
		else
		{
			return OVERFLOW;
		}
	}
	new_node_data= list->allocateroutine(elem->size);
	if (new_node_data)
	{
		memcpy(new_node_data, elem->data, elem->size);

		////时间复杂度:T(n)=O(n)
		for (p = list->elem + list->length - 1; p >= list->elem + pos; p--)
		{
			*(p + 1) = *p;
		}

		list->elem[pos].data = new_node_data;
		list->elem[pos].size = elem->size;
		list->length++;
	}
	else
	{
		return OVERFLOW;
	}
	return OK;
}

void SqlistDestroy(SQLIST *list)
{
	//T(n)=O(n)
	//由SqlistClear决定
	SqlistClear(list);
	MEM_FREE(list->elem);
	list->elem = NULL;
	list->listsize = 0;
}

void SqlistClear(SQLIST *list)
{
	//T(n)=O(n)
	while(list->length)
	{
		if (list->elem[0].data)
		{
			MEM_FREE(list->elem[0].data);
		}
		list->elem[0].data = NULL;
		list->elem[0].size = 0;
		list->length--;
	}
}

Status SqlistEmpty(SQLIST *list)
{
	return list->length == 0 ? TRUE : FALSE;
}

size_t SqlistLength(SQLIST *list)
{
	return list->length;
}

Status SqlistGetElem(SQLIST *list, size_t pos, NODEELEMENT *elem)
{
	//T(n)=O(1)
	if (pos >= 0 && pos < list->length)
	{
		elem->data = list->elem[pos].data;
		elem->size = list->elem[pos].size;
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

	//T(n)=O(n)
	for (pos = 0; pos < list->length; pos++)
	{
		if (list->compareroutine(elem, list->elem + pos) == 0)
		{
			return pos;
		}
	}
	return -1;
}

Status SqlistPriorElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem)
{
	//T(n)=O(n)
	size_t pos = SqlistLocate(list, elem);
	if (pos != -1 && pos > 0)
	{
		pre_elem->data = list->elem[pos-1].data;
		pre_elem->size = list->elem[pos-1].size;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

Status SqlistNextElem(SQLIST *list, NODEELEMENT *elem, NODEELEMENT *pre_elem)
{
	//T(n)=O(n)
	size_t pos = SqlistLocate(list, elem);

	if (pos >= 0 && pos < list->length - 1)
	{
		pre_elem->data = list->elem[pos + 1].data;
		pre_elem->size = list->elem[pos + 1].size;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

Status SqlistDelete(SQLIST *list, size_t pos, NODEELEMENT *elem)
{
	NODEELEMENT *p;
	if (pos >= 0 && pos < list->length)
	{
		if (elem->size >= list->elem[pos].size)
		{
			memcpy(elem->data, list->elem[pos].data, list->elem[pos].size);
			elem->size = list->elem[pos].size;
			list->freeroutine(list->elem[pos].data);

			//T(n)=O(n)
			for (p = list->elem + pos; p < list->elem + list->length - 1; p++)
			{
				*p = *(p + 1);
			}
			list->length--;
			return OK;
		}
		else
		{
			return BUFFER_TOO_SMALL;
		}
	}
	else
	{
		return INFEASIBLE;
	}
}

Status SqlistTraverse(SQLIST *list, Status(_cdecl *visit)(NODEELEMENT *elem))
{
	for (size_t pos = 0; pos < list->length; pos++)
	{
		if (visit(list->elem + pos) != OK)
		{
			return ERROR;
		}
	}
	return OK;
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

//Linklist

Status LinklistMakeNode(PALLOCATEROUTINE allocateroutine, PLINKLISTNODE *p, NODEELEMENT *elem)
{
	*p = MEM_ALOC(sizeof(LINKLISTNODE));
	if (*p)
	{
		(*p)->elem.data = allocateroutine(elem->size);
		if ((*p)->elem.data)
		{
			memcpy((*p)->elem.data, elem->data, elem->size);
			(*p)->elem.size = elem->size;
			(*p)->next = NULL;
			return OK;
		}
		else
		{
			MEM_FREE(*p);
			*p = NULL;
			return ERROR;
		}
	}
	else
	{
		return ERROR;
	}
}

Status LinklistFreeNode(PFREEROUTINE freeroutine, PLINKLISTNODE *p)
{
	freeroutine((*p)->elem.data);
	(*p)->elem.data = NULL;
	(*p)->elem.size = 0;
	MEM_FREE(*p);
	*p = NULL;
	return OK;
}

Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	list->header = MEM_ALOC(sizeof(LINKLISTNODE));
	if (list->header)
	{
		list->compareroutine=compareroutine;
		list->allocateroutine=allocateroutine;
		list->freeroutine=freeroutine;
		list->header->elem.data = NULL;
		list->header->elem.size = 0;
		list->header->next = NULL;
		list->tail = NULL;
		list->len = 0;
		return OK;
	}
	else
	{
		return ERROR;
	}
}

void LinklistClear(LINKLIST *list)
{
	LINKLISTNODE *header = LinklistHeader(list);
	LINKLISTNODE *node = header->next;
	LINKLISTNODE *next;
	//T(n)=O(n)
	while (node)
	{
		next = node->next;
		LinklistFreeNode(list->freeroutine, &node);
		list->len--;
		node = next;
	}
	list->header->next = NULL;;
	list->tail = NULL;
}

void LinklistDestroy(LINKLIST *list)
{
	LinklistClear(list);
	MEM_FREE(list->header);
	list->header = NULL;
}

Status LinklistInsFirst(LINKLISTNODE *h, PLINKLISTNODE s)
{
	s->next = h->next;
	h->next = s;
	return OK;
}

Status LinklistDelFirst(LINKLISTNODE *h, PLINKLISTNODE *q)
{
	if (h->next)
	{
		*q = h->next;
		h->next = (*(q))->next;
		return OK;
	}
	else
	{
		return ERROR;
	}
}

Status LinklistAppend(LINKLIST *list, PLINKLISTNODE s)
{
	PLINKLISTNODE node;
	PLINKLISTNODE next;

	list->tail->next = s;
	node = s;
	if (s)
	{
		next = node->next;
		list->len++;
		while (next)
		{
			node = node->next;
			next = node->next;
			list->len++;
		}
		list->tail = node;
	}
	return OK;
}

Status LinklistRemove(LINKLIST *list, PLINKLISTNODE *q)
{
	PLINKLISTNODE node = list->header;

	while (node)
	{
		if (node->next == list->tail)
		{
			*q = list->tail;
			node->next = NULL;
			list->tail = node;
			return OK;
		}
		node = node->next;
	}
	return INFEASIBLE;
}

Status LinklistInsBefor(LINKLIST *list, PLINKLISTNODE *p, PLINKLISTNODE s)
{
	PLINKLISTNODE node = list->header;

	while (node)
	{
		if (node->next == *p)
		{
			s->next = node->next;
			node->next = s;
			*p = s;
			return OK;
		}
		node = node->next;
	}
	return INFEASIBLE;
}

Status LinklistInsAfter(LINKLIST *list, PLINKLISTNODE *p, PLINKLISTNODE s)
{
	PLINKLISTNODE node = list->header;

	while (node)
	{
		if (node == *p)
		{
			s->next = node->next;
			node->next = s;
			*p = s;
			return OK;
		}
		node = node->next;
	}

	return INFEASIBLE;
}

Status LinklistDel(LINKLIST *list, PLINKLISTNODE p, PLINKLISTNODE *q)
{
	PLINKLISTNODE node = list->header;

	while(node)
	{
		if (node->next == p)
		{
			*q = p;
			node->next = p->next;
			if (node->next == NULL)
			{
				list->tail = node;
			}
			return OK;
		}
		node = node->next;
	}
	return INFEASIBLE;
}

Status LinklistSetCurElem(PLINKLISTNODE *p, NODEELEMENT *elem)
{
	if ((*p)->elem.size >= elem->size)
	{
		memcpy((*p)->elem.data, elem->data, elem->size);
		(*p)->elem.size = elem->size;
		return OK;
	}
	else
	{
		return ERROR;
	}
}

NODEELEMENT *LinklistGetCurElem(PLINKLISTNODE p)
{
	return &(p->elem);
}

Status LinklistEmpty(LINKLIST *list)
{
	return list->len == 0 ? TRUE : FALSE;
}

size_t LinklistLength(LINKLIST *list)
{
	return list->len;
}

LINKLISTPOSITION LinklistHeader(LINKLIST *list)
{
	return list->header;
}

LINKLISTPOSITION LinklistGetLast(LINKLIST *list)
{
	return list->tail;
}

LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKLISTNODE p)
{
	LINKLISTNODE *node = list->header;

	while (node)
	{
		if (node->next == p)
		{
			return node;
		}
		node = node->next;
	}
	return NULL;
}

LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKLISTNODE p)
{
	LINKLISTNODE *node = list->header;

	while (node)
	{
		if (p == node)
		{
			return node->next;
		}
		node = node->next;
	}
	return NULL;
}

Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKLISTNODE *p)
{
	LINKLISTNODE *node = list->header;
	size_t count = 0;

	while (node)
	{
		if (i == count)
		{
			*p = node;
			return OK;
		}
		count++;
		node = node->next;
	}
	return INFEASIBLE;
}

Status LinklistInsert(LINKLIST *list, size_t pos, NODEELEMENT *elem)
{
	LINKLISTNODE *node;
	LINKLISTNODE *new_node;

	if (LinklistLocatePos(list, pos, &node)==OK)
	{
		if (LinklistMakeNode(list->allocateroutine, &new_node, elem) == OK)
		{
			if (LinklistInsFirst(node, new_node) == OK)
			{
				if (new_node->next == NULL)
				{
					list->tail = new_node;
				}
				list->len++;
				return OK;
			}
			else
			{
				LinklistFreeNode(list->freeroutine, &new_node);
			}
		}
	}
	return ERROR;
}

Status LinklistDelete(LINKLIST *list, size_t pos, NODEELEMENT *elem)
{
	LINKLISTNODE *node;

	if (LinklistLocatePos(list, pos, &node) == OK)
	{
		if (LinklistDel(list, node, &node)==OK)
		{
			LinklistFreeNode(list->freeroutine, &node);
			list->len--;
			return OK;
		}
	}
	return ERROR;
}

Status LinklistDeletePos(LINKLIST *list, LINKLISTPOSITION pos)
{
	LINKLISTNODE *node;

	if (LinklistDel(list, pos, &node) == OK)
	{
		LinklistFreeNode(list->freeroutine, &node);
		list->len--;
		return OK;
	}
	return ERROR;
}

Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(NODEELEMENT *elem))
{
	LINKLISTNODE *node = list->header->next;
	Status status;

	while (node && (status=visit(&node->elem) != ERROR))
	{
		node = node->next;
	}
	return status;
}

Status LinklistGetElem(LINKLIST *list, size_t pos, NODEELEMENT *elem)
{
	PLINKLISTNODE node;

	if (LinklistLocatePos(list, pos, &node) == OK)
	{
		if (elem->size >= node->elem.size)
		{
			memcpy(elem->data, node->elem.data, node->elem.size);
			elem->size = node->elem.size;
			return OK;
		}
	}
	return ERROR;
}

LINKLISTPOSITION LinklistLocateElem(LINKLIST *list, NODEELEMENT *elem)
{
	PLINKLISTNODE node = list->header->next;

	while (node)
	{
		if (list->compareroutine(&node->elem, elem) == 0)
		{
			return node;
		}
		node = node->next;
	}
	return NULL;
}

void LinklistUnio(LINKLIST *list1, LINKLIST *list2)
{
	LINKLISTNODE *list2_node = list2->header->next;
	LINKLISTNODE *node;
	LINKLISTPOSITION tail;

	while (list2_node)
	{
		if (!LinklistLocateElem(list1, &list2_node->elem))
		{
			if (LinklistMakeNode(list1->allocateroutine, &node, &list2_node->elem) == OK)
			{
				tail = LinklistGetLast(list1);
				if (tail)
				{
					if (LinklistInsAfter(list1, &tail, node) == OK)
					{
						list1->tail = node;
						list1->len++;
					}
				}
			}
		}
		list2_node = list2_node->next;
	}
}

//一元多项式

int __cdecl polyn_compare_routine(NODEELEMENT *firstelem, NODEELEMENT *secondelem)
{
	if (firstelem->size == sizeof(POLYN_ELEMENT_DATA) && secondelem->size == sizeof(POLYN_ELEMENT_DATA))
	{
		//一元多项式链表按指数从小到大排序
		//因此两个元素的比较即是指数的比较
		return ((PPOLYN_ELEMENT_DATA)firstelem->data)->expn - ((PPOLYN_ELEMENT_DATA)secondelem->data)->expn;
	}
	else
	{
		return -1;
	}
}

Status LinklistOrderLocalElem(LINKLIST *list, NODEELEMENT *elem, LINKLISTPOSITION *q)
{
	LINKLISTNODE *node = list->header;
	LINKLISTNODE *prior_node = NULL;

	while (node)
	{
		if (!prior_node && ((node->next && list->compareroutine(&node->next->elem, elem) >= 0) || node->next==NULL))
		{
			prior_node = node;
		}
		if (list->compareroutine(&node->elem, elem) == 0)
		{
			*q = node;
			return TRUE;
		}
		node = node->next;
	}
	*q = prior_node;
	return FALSE;
}

Status PolynCreate(POLYNOMAIL *polynmail, int m, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	LINKLISTNODE *h;
	NODEELEMENT elem;
	LINKLISTNODE *q;
	LINKLISTNODE *new_node;
	POLYN_ELEMENT_DATA elem_data;
	int i;
	int r;
	char ch[100];

	if (LinklistInit(polynmail, polyn_compare_routine, allocateroutine, freeroutine) == OK)
	{
		h = LinklistHeader(polynmail);
		h->elem.size = sizeof(POLYN_ELEMENT_DATA);
		h->elem.data = allocateroutine(sizeof(POLYN_ELEMENT_DATA));
		if (h->elem.data)
		{
			elem.size = sizeof(POLYN_ELEMENT_DATA);
			elem.data = &elem_data;
			elem_data.coef = 0.0;
			elem_data.expn = -1;
			if (LinklistSetCurElem(&h, &elem) == OK)
			{
				for (i = 0; i < m; i++)
				{
					printf("输入第 %d 项系数和指数(逗号分隔): ", i);
					elem_data.coef = 0;
					elem_data.expn = -1;
					r=scanf_s("%f,%d", &elem_data.coef,&elem_data.expn);
					if (r != 2)
					{
						printf("输入的数据有误,请重新输入\n");
						scanf_s("%s", ch,100);
						i--;
					}
					else
					{
						q = NULL;
						if (LinklistOrderLocalElem(polynmail, &elem, &q))
						{
							printf("具有 %d 次系数的项已经存在,请重新输入\n",elem_data.expn);
							
							i--;
						}
						else if(q)
						{
							if (LinklistMakeNode(polynmail->allocateroutine, &new_node, &elem) == OK)
							{
								if (LinklistInsFirst(q, new_node) == OK)
								{
									if (new_node->next == NULL)
									{
										polynmail->tail = new_node;
									}
									polynmail->len++;
								}
							}
						}
					}
				}
				return OK;
			}
		}
		LinklistDestroy(polynmail);
	}
	return ERROR;
}

void PolynPrint(POLYNOMAIL *polynmail)
{
	LINKLISTNODE *node = polynmail->header->next;

	while (node)
	{
		printf("%.2fx^%d + ",((POLYN_ELEMENT_DATA *)(node->elem.data))->coef, ((POLYN_ELEMENT_DATA *)(node->elem.data))->expn);
		node = node->next;
	}
	if (polynmail->header->next)
	{
		printf("\b\b  ");
	}
}