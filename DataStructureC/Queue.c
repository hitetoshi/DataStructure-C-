#include "Queue.h"

Status LinkQueueInit(LINKQUEUE *q, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	q->front = q->rear = CMEM_ALOC(sizeof(LINKNODE));
	if (q->front)
	{
		q->front->next = NULL;
		q->allocateroutine = allocateroutine;
		q->freeroutine = freeroutine;
		q->length = 0;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

Status LinkQueueDestroy(LINKQUEUE *q)
{
	LinkQueueClear(q);
	CMEM_FREE(q->front);
	q->front = q->rear = NULL;
	return OK;
}

Status LinkQueueClear(LINKQUEUE *q)
{
	PLINKNODE node;

	while (q->front!=q->rear)
	{
		node = q->front->next;
		if (q->rear==node)
		{
			q->rear = q->front;
		}
		else
		{
			q->front = node->next;
		}
		if (node->elem.data)
		{
			q->freeroutine(node->elem.data);
		}
		q->length--;
		CMEM_FREE(node);
	}
	return OK;
}

Status LinkQueueEmpty(LINKQUEUE *q)
{
	return (q->front == q->rear);
}

size_t LinkQueueLength(LINKQUEUE *q)
{
	return q->length;
}

Status LinkQueueGetHead(LINKQUEUE *q, ELEMENT *e)
{
	if (q->front != q->rear)
	{
		if (e->size >= q->front->next->elem.size)
		{
			memcpy(e->data, q->front->next->elem.data, q->front->next->elem.size);
			e->size = q->front->next->elem.size;
			return OK;
		}
		else
		{
			return BUFFER_TOO_SMALL;
		}
	}
	else
	{
		return ERROR;
	}
}

Status LinkQueueEn(LINKQUEUE *q, ELEMENT *e)
{
	PLINKNODE new_node;

	new_node = CMEM_ALOC(sizeof(LINKNODE));
	if (new_node)
	{
		memset(new_node, 0, sizeof(LINKNODE));
		new_node->elem.data = q->allocateroutine(e->size);
		if (new_node->elem.data)
		{
			memcpy(new_node->elem.data, e->data, e->size);
			new_node->elem.size = e->size;
			new_node->next = NULL;
			q->rear->next = new_node;
			q->rear = new_node;
			q->length++;
			return OK;
		}
		else
		{
			CMEM_FREE(new_node);
			return OVERFLOW;
		}
	}
	else
	{
		return OVERFLOW;
	}
}

Status LinkQueueDe(LINKQUEUE *q, ELEMENT *e)
{
	PLINKNODE node;

	if (q->front != q->rear)
	{
		node = q->front->next;
		if (e->size >= node->elem.size)
		{
			memcpy(e->data, node->elem.data, node->elem.size);
			e->size = node->elem.size;
			q->front->next = node->next;
			if (q->rear == node)
			{
				q->rear = q->front;
			}
			CMEM_FREE(node);
			q->length--;
			return OK;
		}
		else
		{
			return BUFFER_TOO_SMALL;
		}
	}
	else
	{
		return ERROR;
	}
}

Status LinkQueueTraverse(LINKQUEUE *q, Status(_cdecl *visit)(ELEMENT *elem, void *param), void *param)
{
	PLINKNODE node=q->front->next;

	if (q->front != q->rear)
	{
		node = q->front->next;
		while (node)
		{
			if (!visit(&node->elem, param))
			{
				return ERROR;
			}
			node = node->next;
		}
		return OK;
	}
	else
	{
		return INFEASIBLE;
	}
}

//离散事件模拟
int __cdecl BMCustomerCompare(ELEMENT *first,ELEMENT *second)
{
	return ((BM_CUSTOMER *)first->data)->ArrivalTime - ((BM_CUSTOMER *)second->data)->ArrivalTime;
}

Status OpenForDay(BANK_SIMULATION_DATA *bm_data)
{
	BM_EVENT en;
	ELEMENT e;
	int i,j;
	LINKLISTPOSITION h;

	bm_data->TotalTime = 0;
	bm_data->CustomerNum = 0;	//初始化累计时间和客户数为0
	e.data = &en;
	e.size = sizeof(en);
	en.NType = 0;
	en.OccurTime = 0;			//设定第一个客户达到事件
	if (!LinklistInit(&bm_data->ev, BMCustomerCompare, CommonAllocRotuine, CommonFreeRoutine))
	{
		return ERROR;
	}
	h = LinklistHeader(&bm_data->ev);
	h->elem.data = CMEM_ALOC(sizeof(en));
	if (!h->elem.data)
	{
		LinklistDestroy(&bm_data->ev);
		return ERROR;
	}
	h->elem.size = sizeof(en);
	((BM_EVENT *)h->elem.data)->NType = 0;
	((BM_EVENT *)h->elem.data)->OccurTime = -1;

	LinklistOrderInsert(&bm_data->ev, &e);	//插入事件列表
	//置空队列
	for (i = 0; i < QUEUE_COUNT; i++)
	{
		if (LinkQueueInit(&bm_data->q[i], CommonAllocRotuine, CommonFreeRoutine) != OK)
		{
			for (j = 0; j < i; j++)
			{
				LinkQueueDestroy(&bm_data->q[j]);
			}
			CMEM_FREE(bm_data->ev.header->elem.data);
			LinklistDestroy(&bm_data->ev);
			return ERROR;
		}
	}
	return OK;
}

void CloseForDay(BANK_SIMULATION_DATA *bm_data)
{
	int i;
	CMEM_FREE(bm_data->ev.header->elem.data);
	LinklistDestroy(&bm_data->ev);
	for (i = 0; i < QUEUE_COUNT; i++)
	{
		LinkQueueDestroy(&bm_data->q[i]);
	}
}

size_t MinimumQueue(LINKQUEUE *q, size_t count)
{
	size_t mini = 0;
	size_t i = 0;
	size_t min_len = LinkQueueLength(&q[0]);

	for (i = 0; i < count; i++)
	{
		if (LinkQueueLength(&q[i]) < min_len)
		{
			min_len = LinkQueueLength(&q[i]);
			mini = i;
		}
	}
	return mini;
}

void CustomerArrived(BANK_SIMULATION_DATA *bm_data, BM_EVENT *en)
{
	BM_EVENT event;
	BM_CUSTOMER customer;
	ELEMENT e_customer;
	ELEMENT e_event;
	int durtime, intertime;
	size_t i;

	bm_data->CustomerNum++;
	//生成随机数
	durtime = RangeRandom(1, MAX_DURTIME);
	intertime = RangeRandom(1, MAX_INTERTIME);

	event.OccurTime = en->OccurTime + intertime;	//下一个客户到达时间
	if (event.OccurTime < bm_data->CloseTime)		//银行尚未关门,插入事件表
	{
		event.NType = 0;
		e_event.data = &event;
		e_event.size = sizeof(event);
		LinklistOrderInsert(&bm_data->ev, &e_event);
	}
	i = MinimumQueue(bm_data->q, QUEUE_COUNT);		//求长度最短队列
	e_customer.data = &customer;
	e_customer.size = sizeof(customer);
	customer.ArrivalTime = en->OccurTime;
	customer.Duration = durtime;
	LinkQueueEn(&(bm_data->q[i]), &e_customer);
	if (LinkQueueLength(&bm_data->q[i]) == 1)
	{
		event.NType = (int)i+1;
		event.OccurTime = en->OccurTime + durtime;
		e_event.data = &event;
		e_event.size = sizeof(event);
		LinklistOrderInsert(&bm_data->ev, &e_event);	//设定第i个队列的离开事件并插入事件表
	}
}

void CustomerDeparture(BANK_SIMULATION_DATA *bm_data, BM_EVENT *en)
{
	BM_CUSTOMER customer;
	ELEMENT e_customer;
	BM_EVENT event;
	ELEMENT e_event;
	int i;

	i = en->NType - 1;
	e_customer.data = &customer;
	e_customer.size = sizeof(e_customer);
	LinkQueueDe(&bm_data->q[i], &e_customer);	//删除第i队列的排头客户
	bm_data->TotalTime += (en->OccurTime - customer.ArrivalTime);	//累计客户逗留时间
	if (!LinkQueueEmpty(&bm_data->q[i]))		//设定第i个队列的离开事件并插入事件表
	{
		LinkQueueGetHead(&bm_data->q[i], &e_customer);
		e_event.data = &event;
		e_event.size = sizeof(event);
		event.NType = i + 1;
		event.OccurTime = en->OccurTime + customer.Duration;
		LinklistOrderInsert(&bm_data->ev, &e_event);
	}
}

Status Bank_Simulation(int CloseTime, void(__cdecl *pEventNotify)(BM_EVENT *en), int *TotalTime, int *CustomerNum)
{
	BANK_SIMULATION_DATA bm_data;
	PLINKNODE q;
	BM_EVENT *en;

	srand((int)time(NULL));

	if (OpenForDay(&bm_data) != OK)	//初始化
	{
		return ERROR;
	}
	bm_data.CloseTime = CloseTime;

	while (!LinklistEmpty(&bm_data.ev))
	{
		LinklistDelFirst(LinklistHeader(&bm_data.ev), &q);
		bm_data.ev.len--;
		if (bm_data.ev.len == 0)
		{
			bm_data.ev.tail = bm_data.ev.header;
		}
		en = (BM_EVENT *)LinklistGetCurElem(q)->data;
		if (en->NType == 0)
		{
			CustomerArrived(&bm_data, en);		//处理客户到达事件
		}
		else
		{
			CustomerDeparture(&bm_data, en);	//处理客户离开事件
		}
		if (pEventNotify)
		{
			pEventNotify(en);
		}
	}
	CloseForDay(&bm_data);	//银行关门,释放开门时分配的资源

	if (TotalTime)
	{
		*TotalTime = bm_data.TotalTime;
	}
	if (CustomerNum)
	{
		*CustomerNum = bm_data.CustomerNum;
	}
	return OK;
}