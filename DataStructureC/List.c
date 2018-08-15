#include "List.h"

Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	list->elem = CMEM_ALOC(LIST_INIT_SIZE*sizeof(ELEMENT));
	if (list->elem)
	{
		CMEM_SET(list->elem, 0, LIST_INIT_SIZE * sizeof(ELEMENT));
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

Status SqlistInsert(SQLIST *list, size_t pos, ELEMENT *elem)
{
	ELEMENT *new_base;
	ELEMENT *p;
	void *new_node_data;

	if (pos<0 || pos > list->length)
	{
		return INFEASIBLE;
	}
	if (list->length >= list->listsize)
	{
		new_base = CMEM_REALLOC(list->elem, (list->length + LIST_INCREMENT) * sizeof(ELEMENT));
		if (new_base)
		{
			list->elem = new_base;
			memset(list->elem + list->listsize, 0, sizeof(ELEMENT) * LIST_INCREMENT);
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
		CMEM_COPY(new_node_data, elem->data, elem->size);

		////时间复杂度:O(list->length)
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

Status SqlistOrderInsert(SQLIST *list, ELEMENT *elem)
{
	size_t pos = SqlistOrderLocate(list, elem, NULL);

	return SqlistInsert(list, pos, elem);
}

void SqlistDestroy(SQLIST *list)
{
	//O(list->length)
	//由SqlistClear决定
	SqlistClear(list);
	CMEM_FREE(list->elem);
	list->elem = NULL;
	list->listsize = 0;
}

void SqlistClear(SQLIST *list)
{
	//O(list->length)
	while(list->length)
	{
		if (list->elem[0].data)
		{
			CMEM_FREE(list->elem[0].data);
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

Status SqlistGetElem(SQLIST *list, size_t pos, ELEMENT *elem)
{
	//O(1)
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

Status SqlistSetElem(SQLIST *list, size_t pos, ELEMENT *elem)
{
	if (pos >= 0 && pos < list->length)
	{
		if (list->elem[pos].size < elem->size)
		{
			list->freeroutine(list->elem[pos].data);
			list->elem[pos].data = list->allocateroutine(elem->size);
		}
		if (list->elem[pos].data)
		{
			CMEM_COPY(list->elem[pos].data, elem->data, elem->size);
			list->elem[pos].size = elem->size;
			return OK;
		}
		else
		{
			return OVERFLOW;
		}
	}
	else
	{
		return INFEASIBLE;
	}
}

size_t SqlistLocate(SQLIST *list, ELEMENT *elem)
{
	size_t pos;

	//O(list->length)
	for (pos = 0; pos < list->length; pos++)
	{
		if (list->compareroutine(elem, list->elem + pos) == 0)
		{
			return pos;
		}
	}
	return -1;
}

size_t SqlistOrderLocate(SQLIST *list, ELEMENT *elem, ELEMENT **b)
{
	size_t pos;
	int result;

	if (b)
	{
		*b = NULL;
	}
	//O(list->length)
	for (pos = 0; pos < list->length; pos++)
	{
		result = list->compareroutine(elem, list->elem + pos);
		if (result <= 0)
		{
			if (result == 0)
			{
				if (b)
				{
					*b = list->elem + pos;
				}
			}
			return pos;
		}
	}
	return pos;
}

Status SqlistPriorElem(SQLIST *list, ELEMENT *elem, ELEMENT *pre_elem)
{
	//O(list->length)
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

Status SqlistNextElem(SQLIST *list, ELEMENT *elem, ELEMENT *next_elem)
{
	//O(list->length)
	size_t pos = SqlistLocate(list, elem);

	if (pos >= 0 && pos < list->length - 1)
	{
		next_elem->data = list->elem[pos + 1].data;
		next_elem->size = list->elem[pos + 1].size;
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

Status SqlistDelete(SQLIST *list, size_t pos, ELEMENT *elem)
{
	ELEMENT *p;
	if (pos >= 0 && pos < list->length)
	{
		if (elem->size >= list->elem[pos].size)
		{
			memcpy(elem->data, list->elem[pos].data, list->elem[pos].size);
			elem->size = list->elem[pos].size;
			list->freeroutine(list->elem[pos].data);

			//O(list->length)
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

Status SqlistTraverse(SQLIST *list, Status(_cdecl *visit)(ELEMENT *elem))
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
	ELEMENT elem;

	//O(list2->length*list1->length)
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

Status SqlistMerge(SQLIST *la, SQLIST *lb, SQLIST *lc)
{
	ELEMENT *pa, *pb;
	ELEMENT *new_node_base;

	SqlistClear(lc);
	lc->listsize = la->length + lb->length;
	lc->length = 0;
	new_node_base = CMEM_REALLOC(lc->elem, lc->listsize * sizeof(ELEMENT));
	if (new_node_base)
	{
		lc->elem = new_node_base;
		pa = la->elem;
		pb = lb->elem;
		//O(la->length+lb->length）
		while (pa < la->elem + la->length && pb < lb->elem + lb->length)
		{
			//与教材算法2.7不同,因本例线性表的元素类型与教材不同
			//不能采用直接赋值的方式插入元素
			SqlistInsert(lc, lc->length, (lc->compareroutine(pa, pb) <= 0) ? pa++ : pb++);
		}
		while (pa < la->elem + la->length)
		{
			SqlistInsert(lc, lc->length, pa++);
		}
		while (pb < lb->elem + lb->length)
		{
			SqlistInsert(lc, lc->length, pb++);
		}
		return OK;
	}
	return ERROR;
}

//Linklist

Status LinklistMakeNode(PALLOCATEROUTINE allocateroutine, PLINKNODE *p, ELEMENT *elem)
{
	*p = CMEM_ALOC(sizeof(LINKNODE));
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
			CMEM_FREE(*p);
			*p = NULL;
			return ERROR;
		}
	}
	else
	{
		return ERROR;
	}
}

Status LinklistFreeNode(PFREEROUTINE freeroutine, PLINKNODE *p)
{
	freeroutine((*p)->elem.data);
	(*p)->elem.data = NULL;
	(*p)->elem.size = 0;
	CMEM_FREE(*p);
	*p = NULL;
	return OK;
}

Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	list->header = CMEM_ALOC(sizeof(LINKNODE));
	if (list->header)
	{
		list->compareroutine=compareroutine;
		list->allocateroutine=allocateroutine;
		list->freeroutine=freeroutine;
		list->header->elem.data = NULL;
		list->header->elem.size = 0;
		list->header->next = NULL;
		list->tail = list->header;
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
	LINKNODE *header = LinklistHeader(list);
	LINKNODE *node = header->next;
	LINKNODE *next;
	//O(list->length)
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
	CMEM_FREE(list->header);
	list->header = NULL;
}

Status LinklistInsFirst(LINKNODE *h, PLINKNODE s)
{
	s->next = h->next;
	h->next = s;
	return OK;
}

Status LinklistDelFirst(LINKNODE *h, PLINKNODE *q)
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

Status LinklistAppend(LINKLIST *list, PLINKNODE s)
{
	PLINKNODE node;
	PLINKNODE next;

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

Status LinklistRemove(LINKLIST *list, PLINKNODE *q)
{
	PLINKNODE node = list->header;

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

Status LinklistInsBefor(LINKLIST *list, PLINKNODE *p, PLINKNODE s)
{
	PLINKNODE node = list->header;

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

Status LinklistInsAfter(LINKLIST *list, PLINKNODE *p, PLINKNODE s)
{
	PLINKNODE node = list->header;

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

Status LinklistDel(LINKLIST *list, PLINKNODE p, PLINKNODE *q)
{
	PLINKNODE node = list->header;

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

Status LinklistSetCurElem(PLINKNODE *p, ELEMENT *elem)
{
	//这里本应为elem.data重新分配内存
	//但教材给的本函数的定义没有SQLIST结构,找不到内存分配函数
	//因此只能使用原来已分配的内存
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

ELEMENT *LinklistGetCurElem(PLINKNODE p)
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

LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKNODE p)
{
	LINKNODE *node = list->header;

	//O(list->length)
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

LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKNODE p)
{
	/*LINKNODE *node = list->header;

	while (node)
	{
		if (p == node)
		{
			return node->next;
		}
		node = node->next;
	}
	return NULL;*/

	//O(1)
	return p->next;
}

Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKNODE *p)
{
	LINKNODE *node = list->header;
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

Status LinklistInsert(LINKLIST *list, size_t pos, ELEMENT *elem)
{
	LINKNODE *node;
	LINKNODE *new_node;

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

Status LinklistDelete(LINKLIST *list, size_t pos, ELEMENT *elem)
{
	LINKNODE *node;

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
	LINKNODE *node;

	if (LinklistDel(list, pos, &node) == OK)
	{
		LinklistFreeNode(list->freeroutine, &node);
		list->len--;
		return OK;
	}
	return ERROR;
}

Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(ELEMENT *elem))
{
	LINKNODE *node = list->header->next;
	Status status;

	while (node && (status=visit(&node->elem) != ERROR))
	{
		node = node->next;
	}
	return status;
}

Status LinklistGetElem(LINKLIST *list, size_t pos, ELEMENT *elem)
{
	PLINKNODE node;

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

LINKLISTPOSITION LinklistLocateElem(LINKLIST *list, ELEMENT *elem)
{
	PLINKNODE node = list->header->next;

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
	LINKNODE *list2_node = list2->header->next;
	LINKNODE *node;
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

void LinklistMerge(LINKLIST *la, LINKLIST *lb, LINKLIST *lc)
{
	LINKNODE *ha, *hb, *pa, *pb;
	ELEMENT *ea, *eb;
	PLINKNODE q;
	
	ha = LinklistHeader(la);
	hb = LinklistHeader(lb);
	pa = LinklistNextPos(la, ha);
	pb = LinklistNextPos(lb, hb);

	while (!LinklistEmpty(la) && !LinklistEmpty(lb))
	{
		ea = LinklistGetCurElem(pa);
		eb = LinklistGetCurElem(pb);
		if (lc->compareroutine(ea, eb) <= 0)
		{
			pa = LinklistNextPos(la, pa);
			LinklistDelFirst(ha, &q);
			q->next = NULL;
			la->len--;
			if (la->len == 0)
			{
				la->tail = la->header;
			}
		}
		else
		{
			pb = LinklistNextPos(lb, pb);
			LinklistDelFirst(hb, &q);
			q->next = NULL;
			lb->len--;
			if (lb->len == 0)
			{
				lb->tail = lb->header;
			}
		}
		LinklistAppend(lc, q);
	}
	if (!LinklistEmpty(la))
	{
		LinklistAppend(lc, pa);
		//la的节点全部Append到lc后,la清空
		la->header->next = NULL;
		la->tail = la->header;
		la->len = 0;
	}
	if (!LinklistEmpty(lb))
	{
		LinklistAppend(lc, pb);
		lb->header->next = NULL;
		la->tail = la->header;
		lb->len = 0;
	}
}

//一元多项式

int __cdecl polyn_compare_routine(ELEMENT *firstelem, ELEMENT *secondelem)
{
	return (firstelem->size == sizeof(POLYN_ELEMENT_DATA) && secondelem->size == sizeof(POLYN_ELEMENT_DATA)) ?
		((PPOLYN_ELEMENT_DATA)firstelem->data)->expn - ((PPOLYN_ELEMENT_DATA)secondelem->data)->expn : -1;
}

Status LinklistOrderLocalElem(LINKLIST *list, ELEMENT *elem, LINKLISTPOSITION *q)
{
	LINKNODE *node = list->header;
	LINKNODE *prior_node = NULL;

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

Status LinklistOrderInsert(LINKLIST *list, ELEMENT *elem)
{
	LINKNODE *q;
	LINKNODE *new_node;

	q = NULL;
	LinklistOrderLocalElem(list, elem, &q);
	if (q)
	{
		if (LinklistMakeNode(list->allocateroutine, &new_node, elem) == OK)
		{
			if (LinklistInsFirst(q, new_node) == OK)
			{
				if (new_node->next == NULL)
				{
					list->tail = new_node;
				}
				list->len++;
				return OK;
			}
		}
	}
	return ERROR;
}

Status PolynCreate(POLYNOMAIL *polynmail, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	ELEMENT elem;
	POLYN_ELEMENT_DATA elem_data;

	if (LinklistInit(polynmail, polyn_compare_routine, allocateroutine, freeroutine) == OK)
	{
		elem.data = &elem_data;
		elem.size = sizeof(POLYN_ELEMENT_DATA);
		elem_data.coef = 0.0;
		elem_data.expn = -1;
		polynmail->header->elem.data = CMEM_ALOC(sizeof(POLYN_ELEMENT_DATA));
		if (polynmail->header)
		{
			polynmail->header->elem.size = sizeof(POLYN_ELEMENT_DATA);
			LinklistSetCurElem(&polynmail->header, &elem);
			return OK;
		}
		else
		{
			LinklistDestroy(polynmail);
		}
	}
	return ERROR;
}

void PolynDestroy(POLYNOMAIL *polynmail)
{
	CMEM_FREE(polynmail->header->elem.data);
	LinklistDestroy(polynmail);
}

Status PolynAddMonos(POLYNOMAIL *polynmail, POLYN_ELEMENT_DATA *items, int item_count)
{
	ELEMENT elem;
	PLINKNODE new_node;
	LINKLISTPOSITION q;

	for (int i = 0; i < item_count; i++)
	{
		elem.data = items + i;
		elem.size = sizeof(POLYN_ELEMENT_DATA);

		q = NULL;
		if (LinklistOrderLocalElem(polynmail, &elem, &q))
		{
			(((POLYN_ELEMENT_DATA *)q->elem.data)->coef) += items[i].coef;
		}
		else if (q)
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
	return OK;
}

void PolynAdd(POLYNOMAIL *pa, POLYNOMAIL *pb)
{
	LINKNODE *ha, *hb;
	LINKLISTPOSITION qa, qb, next_qa,next_qb;
	ELEMENT *a, *b;
	int res;
	POLYN_ELEMENT_DATA sum;

	ha = LinklistHeader(pa);
	hb = LinklistHeader(pb);
	qa = LinklistNextPos(pa, ha);
	qb = LinklistNextPos(pb, hb);

	//时间复杂度O(pa->length+pb->length)
	while (!LinklistEmpty(pa) && !LinklistEmpty(pb) && qa && qb)	//教材:pa,pb均非空
																	//补充条件:pa,pb均没有扫描到表尾(qa,qb不为NULL)
	{
		a = LinklistGetCurElem(qa);
		b = LinklistGetCurElem(qb);
		res = pa->compareroutine(a, b);
		if (res < 0)
		{
			ha = qa;
			qa = LinklistNextPos(pa,qa);
		}
		else if (res > 0)
		{
			next_qb = LinklistNextPos(pb, qb);
			LinklistDelFirst(hb, &qb);
			qb->next = NULL;
			pb->len--;
			if (pb->len == 0)
			{
				pb->tail = pb->header;
			}
			LinklistInsFirst(ha, qb);
			if (qb->next == NULL)
			{
				pa->tail = qb->next;
			}
			ha = qb;
			pa->len++;
			qb = next_qb;
		}
		else
		{
			next_qa = LinklistNextPos(pa, qa);
			next_qb = LinklistNextPos(pb, qb);
			sum.coef = ((POLYN_ELEMENT_DATA *)a->data)->coef + ((POLYN_ELEMENT_DATA *)b->data)->coef;
			if (sum.coef != 0)
			{
				((POLYN_ELEMENT_DATA *)a->data)->coef = sum.coef;
				ha = qa;
			}
			else
			{
				LinklistDelFirst(ha, &qa);
				qa->next = NULL;
				pa->len--;
				if (pa->len == 0)
				{
					pa->tail = pa->header;
				}
				LinklistFreeNode(pa->freeroutine, &qa);
			}
			LinklistDelFirst(hb, &qb);
			qb->next = NULL;
			pb->len--;
			if (pb->len == 0)
			{
				pb->tail = pb->header;
			}
			LinklistFreeNode(pa->freeroutine, &qb);
			qb = next_qb;
			qa = next_qa;
		}
	}
	if (!LinklistEmpty(pb))
	{
		LinklistAppend(pa, qb);
		pb->len = 0;
		pb->header->next = NULL;
		pb->tail = pb->header;
	}
}

void PolynSubtract(POLYNOMAIL *pa, POLYNOMAIL *pb)
{
	//减去一个数,等于加上这个数的相反数
	//从性能上看,增加了一个以乘法为基本操作,时间复杂度为O(pb->length)的操作
	//但代码简洁
	PolynMultiConstant(pb, -1);
	PolynAdd(pa, pb);
}

void PolynMultiConstant(POLYNOMAIL *p, float constant)
{
	LINKNODE *h;
	LINKLISTPOSITION q;
	h = LinklistHeader(p);
	q = LinklistNextPos(p, h);
	//时间复杂度O(p->length)
	while (q)
	{
		((POLYN_ELEMENT_DATA *)q->elem.data)->coef *= constant;
		q = LinklistNextPos(p, q);
	}
}

void PolynMultiMono(POLYNOMAIL *p, POLYN_ELEMENT_DATA *mono)
{
	LINKNODE *h;
	LINKLISTPOSITION q;
	h = LinklistHeader(p);
	q = LinklistNextPos(p, h);
	//O(p->length)
	while (q)
	{
		((POLYN_ELEMENT_DATA *)q->elem.data)->coef *= mono->coef;
		((POLYN_ELEMENT_DATA *)q->elem.data)->expn += mono->expn;
		q = LinklistNextPos(p, q);
	}
}

void PolynMulti(POLYNOMAIL *pa, POLYNOMAIL *pb, POLYNOMAIL *pc)
{
	LINKLISTPOSITION qa = LinklistNextPos(pa, LinklistHeader(pa));
	LINKLISTPOSITION qb;
	ELEMENT *a, *b;
	POLYN_ELEMENT_DATA v;
	
	LinklistClear(pc);
	//以单项式乘法为基本操作,时间复杂度O(pa->length*pb->length)
	//也可以用PolynMultiMono做,时间复杂度同样为O(pa->length*pb->length)
	while (qa)
	{
		a = LinklistGetCurElem(qa);
		qb = LinklistNextPos(pb, LinklistHeader(pb));

		while (qb)
		{
			b = LinklistGetCurElem(qb);
			v.coef = ((POLYN_ELEMENT_DATA *)a->data)->coef * ((POLYN_ELEMENT_DATA *)b->data)->coef;
			v.expn= ((POLYN_ELEMENT_DATA *)a->data)->expn + ((POLYN_ELEMENT_DATA *)b->data)->expn;
			PolynAddMonos(pc, &v, 1);
			qb = qb->next;
		}
		qa = qa->next;
	}
}

void PolynPrint(POLYNOMAIL *polynmail)
{
	LINKNODE *node = polynmail->header->next;

	while (node)
	{
		printf(((POLYN_ELEMENT_DATA *)(node->elem.data))->coef>0?((node== polynmail->header->next)?"":"+"):"");
		printf("%.2fx^%d",((POLYN_ELEMENT_DATA *)(node->elem.data))->coef, ((POLYN_ELEMENT_DATA *)(node->elem.data))->expn);
		node = node->next;
	}
}