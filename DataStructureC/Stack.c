#include "Stack.h"
#include "List.h"

Status StackInit(SQSTACK *stack, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	stack->base = CMEM_ALOC(STACK_INIT_SIZE*sizeof(ELEMENT));
	if (stack->base)
	{
		CMEM_SET(stack->base, 0, STACK_INIT_SIZE*sizeof(ELEMENT));
		stack->top = stack->base;
		stack->stacksize = STACK_INIT_SIZE;
		stack->allocateroutine = allocateroutine;
		stack->freeroutine = freeroutine;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

Status StackDestroy(SQSTACK *stack)
{
	StackClear(stack);
	CMEM_FREE(stack->base);
	memset(stack, 0, sizeof(SQSTACK));
	return OK;
}

Status StackClear(SQSTACK *stack)
{
	while (stack->top != stack->base)
	{
		stack->top--;
		CMEM_FREE(stack->top->data);
		stack->top->data = NULL;
		stack->top->size = 0;
	}
	return OK;
}

Status StackEmpty(SQSTACK *stack)
{
	return (stack->top == stack->base);
}

size_t StackLength(SQSTACK *stack)
{
	return stack->top - stack->base;
}

Status StackGetTop(SQSTACK *stack, ELEMENT *e)
{
	if (stack->top == stack->base)
	{
		return ERROR;
	}

	if (e->data && e->size >= (stack->top-1)->size)
	{
		CMEM_COPY(e->data, (stack->top-1)->data, (stack->top-1)->size);
		e->size = (stack->top-1)->size;
		return OK;
	}
	else
	{
		return BUFFER_TOO_SMALL;
	}
}

Status StackPush(SQSTACK *stack, ELEMENT *e)
{
	ELEMENT *new_base;

	if (stack->top - stack->base >= stack->stacksize)
	{
		new_base = CMEM_REALLOC(stack->base, (stack->stacksize + STACKINCREMENT)*sizeof(ELEMENT));
		if (!new_base)
		{
			return OVERFLOW;
		}
		else
		{
			memset(new_base + stack->stacksize, 0, STACKINCREMENT*sizeof(ELEMENT));
			stack->base = new_base;
			stack->top = stack->base + stack->stacksize;
			stack->stacksize += STACKINCREMENT;
		}
	}
	stack->top->data = stack->allocateroutine(e->size);
	if (stack->top->data)
	{
		memcpy(stack->top->data, e->data, e->size);
		stack->top->size = e->size;
		stack->top++;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

Status StackPop(SQSTACK *stack, ELEMENT *e)
{
	if (stack->top == stack->base)
	{
		return ERROR;
	}
	if (e->size >= (stack->top - 1)->size)
	{
		stack->top--;
		memcpy(e->data, stack->top->data, stack->top->size);
		e->size = stack->top->size;
		CMEM_FREE(stack->top->data);
		stack->top->data = NULL;
		stack->top->size = 0;
		return OK;
	}
	else
	{
		return BUFFER_TOO_SMALL;
	}
}

Status StackTraverse(SQSTACK *stack, Status(*visit)(ELEMENT *e, void *param), void *param)
{
	ELEMENT *node = stack->top;

	while (node != stack->base)
	{
		node--;
		if (!visit(node, param))
		{
			return ERROR;
		}
	}
	return OK;
}

void conversion(int num, int base)
{
	ELEMENT e;
	int N1, N2;
	SQSTACK s;

	if (StackInit(&s, CommonAllocRotuine, CommonFreeRoutine) == OK)
	{
		N1 = num;
		e.data = &N2;
		e.size = sizeof(N2);
		while (N1)
		{
			N2 = N1 % base;
			StackPush(&s, &e);
			N1 = N1 / base;
		}
		printf("%d进制数:", base);
		while (!StackEmpty(&s))
		{
			StackPop(&s, &e);
			if (N2 <= 9)
			{
				printf("%d", N2);
			}
			else
			{
				printf("%c", N2 - 10 + 'A');
			}
		}
		printf("\n");
		StackDestroy(&s);
	}
}

void brackets(char *ch)
{
	SQSTACK s;
	ELEMENT e;
	char v;
	Status failure = FALSE;
	int i;

	e.data = &v;
	e.size = sizeof(v);

	if (StackInit(&s, CommonAllocRotuine, CommonFreeRoutine))
	{
		for (i = 0; i < (int)CSTRING_LENGTH(ch) && !failure; i++)
		{
			if (ch[i] == '(' || ch[i] == '[' || ch[i] == '{')
			{
				v = ch[i];
				StackPush(&s, &e);
			}
			else
			{
				if (StackGetTop(&s, &e) == OK)
				{
					if (ch[i] == ')')
					{
						if (v == '(')
						{
							StackPop(&s, &e);
						}
						else
						{
							failure = TRUE;
						}
					}
					else if (ch[i] == ']')
					{
						if (v == '[')
						{
							StackPop(&s, &e);
						}
						else
						{
							failure = TRUE;
						}
					}
					else if (ch[i] == '}')
					{
						if (v == '{')
						{
							StackPop(&s, &e);
						}
						else
						{
							failure = TRUE;
						}
					}
					else
					{
						failure = TRUE;
					}
				}
				else
				{
					failure = TRUE;
				}
			}
		}
		if (failure)
		{
			printf("括号匹配失败,在第 %d 个字符 '%c' \n", i, ch[i - 1]);
		}
		else
		{
			printf(StackEmpty(&s) ? "括号匹配成功\n" : "括号匹配失败\n");
		}
		StackDestroy(&s);
	}
}

Status LineEdit(char *buf, size_t buf_size)
{
	SQSTACK s;
	char ch;
	ELEMENT e,r;
	char v;
	size_t bytes_tranfered = 0;

	e.data = &v;
	e.size = sizeof(v);
	if (StackInit(&s, CommonAllocRotuine, CommonFreeRoutine) == OK)	//构造空栈s
	{
		ch = getchar();
		while (ch != EOF)
		{
			while (ch != EOF && ch != '\n')
			{
				switch (ch)
				{
				case '#':	//仅当栈非空时退栈
					StackPop(&s, &e);
					break;
				case '@':	//重置s为空栈
					StackClear(&s);
					break;
				default:	//有效字符进栈,栈可以动态增长
					v = ch;
					StackPush(&s, &e);
					break;
				}
				ch = getchar();
			}
			if (buf_size - bytes_tranfered > StackLength(&s))
			{
				if (ch == '\n')
				{
					v = ch;
					StackPush(&s, &e);
				}

				r.data = buf + bytes_tranfered + StackLength(&s) - 1;
				while (!StackEmpty(&s))
				{
					StackPop(&s, &r);
					bytes_tranfered++;
					(((char *)r.data))--;
				}
				if (ch != EOF)
				{
					ch = getchar();
				}
			}
			else
			{
				return BUFFER_TOO_SMALL;
			}
		}
		StackDestroy(&s);
	}
	else
	{
		return ERROR;
	}
	return OK;
}

//当前位置是否可以通过(未曾走到过的通道块)
Status MazePass(int **point, int x, int y, COORDINATE *pos, SQSTACK *s, SQLIST *blocked, SQLIST *foot)
{
	ELEMENT e;

	//位置合法且是通道
	if (pos->x >= 0 && pos->x < x && pos->y >= 0 && pos->y < y && point[pos->x][pos->y] == 0)
	{
		//不在不通路径中,不在已走过的路径中
		e.data = pos;
		e.size = sizeof(COORDINATE);
		if (SqlistLocate(blocked, &e) == -1 && SqlistLocate(foot,&e)==-1)
		{
			return TRUE;
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}

void MazeNextPos(COORDINATE *curpos, int di, COORDINATE *nextpos)
{
	nextpos->x = curpos->x;
	nextpos->y = curpos->y;
	switch (di)
	{
	case 1:
		nextpos->x++;
		break;
	case 2:
		nextpos->y++;
		break;
	case 3:
		nextpos->x--;
		break;
	case 4:
		nextpos->y--;
		break;
	}
}

int __cdecl MazeCompareRoutine(ELEMENT *first, ELEMENT *second)
{
	if (((COORDINATE *)first->data)->x == ((COORDINATE *)second->data)->x && ((COORDINATE *)first->data)->y == ((COORDINATE *)second->data)->y)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

Status MazePath(int **point, int x, int y, COORDINATE *start, COORDINATE *end, SQSTACK *path)
{
	SQSTACK s;
	COORDINATE curpos;
	int curstep;
	MAZEELEMENT elem;
	ELEMENT e;
	SQLIST blocked;	//不能通过的位置
	SQLIST foot;	//足迹
	ELEMENT e_list;

	e.data = &elem;
	e.size = sizeof(elem);

	if (SqlistInit(&foot, MazeCompareRoutine, CommonAllocRotuine, CommonFreeRoutine) == OK)
	{
		if (SqlistInit(&blocked, MazeCompareRoutine, CommonAllocRotuine, CommonFreeRoutine) == OK)
		{
			if (StackInit(&s, CommonAllocRotuine, CommonFreeRoutine) == OK)		//设定"当前位置"为"入口位置"
			{
				curpos.x = start->x;
				curpos.y = start->y;
				curstep = 1;		//探索第一步
				do
				{
					if (MazePass(point, x, y, &curpos, &s, &blocked, &foot))	//当前位置可通,即是未曾走到过的通道块
					{
						e_list.data = &curpos;
						e_list.size = sizeof(COORDINATE);
						SqlistInsert(&blocked, 0, &e_list);	//留下足迹

						elem.ord = curstep;
						elem.seat.x = curpos.x;
						elem.seat.y = curpos.y;
						elem.di = 1;
						StackPush(&s, &e);		//加入路径
						if (curpos.x == end->x && curpos.y == end->y)	//达到终点(出口)
						{
							while (!StackEmpty(&s))
							{
								StackPop(&s, &e);
								e_list.data = &(((MAZEELEMENT *)e.data)->seat);
								e_list.size = sizeof(COORDINATE);
								StackPush(path, &e_list);
							}
							StackDestroy(&s);
							SqlistDestroy(&foot);
							SqlistDestroy(&blocked);
							return TRUE;
						}
						MazeNextPos(&curpos, 1, &curpos);	//取下一位置
						curstep++;	//探索下一步
					}
					else
					{	//当前位置不能通过
						if (!StackEmpty(&s))
						{
							StackPop(&s, &e);
							while (elem.di == 4 && !StackEmpty(&s))
							{
								e_list.data = &(((MAZEELEMENT *)e.data)->seat);
								e_list.size = sizeof(COORDINATE);
								SqlistInsert(&blocked, 0, &e_list);	//此路不通,退回一步
								StackPop(&s, &e);
							}
							if (elem.di < 4)
							{
								elem.di++;
								StackPush(&s, &e);		//换下一个方向探索
								MazeNextPos(&elem.seat, elem.di, &curpos);	//设定'当前位置'为新位置
							}
						}
					}
				} while (!StackEmpty(&s));
				StackDestroy(&s);
			}
			SqlistDestroy(&blocked);
		}
		SqlistDestroy(&foot);
	}

	return FALSE;
}

struct _OPTRPRIOR_SUB
{
	char optr;
	int prior;
};

struct _OPTRPRIOR
{
	char optr;
	struct _OPTRPRIOR_SUB optrsub[7];
};

const struct _OPTRPRIOR OptrPriorTable[] = {
	{ '+',{ { '+',1 },{ '-',1 },{ '*',-1 },{ '/',-1 },{ '(',-1 },{ ')',1 },{ '#',1 } } },
	{ '-',{ { '+',1 },{ '-',1 },{ '*',-1 },{ '/',-1 },{ '(',-1 },{ ')',1 },{ '#',1 } } },
	{ '*',{ { '+',1 },{ '-',1 },{ '*',1 },{ '/',1 },{ '(',-1 },{ ')',1 },{ '#',1 } } },
	{ '/',{ { '+',1 },{ '-',1 },{ '*',1 },{ '/',1 },{ '(',-1 },{ ')',1 },{ '#',1 } } },
	{ '(',{ { '+',-1 },{ '-',-1 },{ '*',-1 },{ '/',-1 },{ '(',-1 },{ ')',0 },{ '#',-2 } } },
	{ ')',{ { '+',1 },{ '-',1 },{ '*',1 },{ '/',1 },{ '(',-2 },{ ')',1 },{ '#',1 } } },
	{ '#',{ { '+',-1 },{ '-',-1 },{ '*',-1 },{ '/',-1 },{ '(',-1 },{ ')',-2 },{ '#',0 } } },
};

const char OP[] = { '+','-','*','/','(',')','#',0 };

Status EEOptrStackPush(SQSTACK *s, char ch)
{
	ELEMENT e;
	e.data = &ch;
	e.size = sizeof(ch);
	return StackPush(s, &e);
}

char EEOptrStackGetTop(SQSTACK *s)
{
	ELEMENT e;
	char ch;

	e.data = &ch;
	e.size = sizeof(ch);
	return (StackGetTop(s, &e) == OK)?ch:0;
}

char EEOptrStackPop(SQSTACK *s)
{
	ELEMENT e;
	char ch;

	e.data = &ch;
	e.size = sizeof(ch);
	return StackPop(s, &e) == OK ? ch : 0;
}

Status EEOpndStackPush(SQSTACK *s, int opnd)
{
	ELEMENT e;
	e.data = &opnd;
	e.size = sizeof(int);
	return StackPush(s, &e);
}

int EEOpndStackPop(SQSTACK *s)
{
	ELEMENT e;
	int v;
	
	e.data = &v;
	e.size = sizeof(v);
	return (StackPop(s, &e) == OK) ? v : 0;
}

Status EEIn(char c, const char *p)
{
	while (*p)
	{
		if (c == *p)
		{
			return TRUE;
		}
		p++;
	}
	return FALSE;
}

int EEPrecede(char c1, char c2)
{
	for (size_t i = 0; i < sizeof(OptrPriorTable) / sizeof(struct _OPTRPRIOR); i++)
	{
		if (c1 == OptrPriorTable[i].optr)
		{
			for (size_t j = 0; j < sizeof(OptrPriorTable[i].optrsub) / sizeof(struct _OPTRPRIOR_SUB); j++)
			{
				if (c2 == OptrPriorTable[i].optrsub[j].optr)
				{
					return OptrPriorTable[i].optrsub[j].prior;
				}
			}
		}
	}
	return -2;
}

int EEOperate(int a, char optr, int b)
{
	switch (optr)
	{
	case '+':
		return a + b;
		break;
	case '-':
		return a - b;
		break;
	case '*':
		return a*b;
		break;
	case '/':
		return a / b;
		break;
	default:
		break;
	}
	return 0;
}

Status EvaluateExpression(char *expression, int *result)
{
	SQSTACK OPTR, OPND;
	char *c;
	char theta;
	int a, b;
	char opnd[100];
	int opnd_size;

	if (!StackInit(&OPTR, CommonAllocRotuine, CommonFreeRoutine))
	{
		return ERROR;
	}
	if (!StackInit(&OPND, CommonAllocRotuine, CommonFreeRoutine))
	{
		StackDestroy(&OPTR);
		return ERROR;
	}
	EEOptrStackPush(&OPTR, '#');
	c = expression;
	while (*c)
	{
		memset(opnd, 0, sizeof(opnd));
		opnd_size = 0;

		//对教材代码稍作改进,可以处理多位数运算
		//如果不是操作符,则储存起来,直到遇到操作符
		//将存储的操作数字符串整体转换为数字(atoi函数)
		while (*c && !EEIn(*c, OP))
		{
			if (CISDIGIT(*c))
			{
				opnd[opnd_size++] = *c;
			}
			else
			{
				StackDestroy(&OPTR);
				StackDestroy(&OPND);
				return ERROR;
			}
			c++;
		}
		if (opnd_size)
		{
			EEOpndStackPush(&OPND, atoi(opnd));	//进操作数栈
		}

		switch (EEPrecede(EEOptrStackGetTop(&OPTR), *c))	//教材Precede函数返回字符'>','<','='表示比较结果,这里返回-1,0,1表示比较结果
		{
		case -1:	//栈定元素优先权低
			EEOptrStackPush(&OPTR, *c);
			c++;
			break;
		case 0:		//脱括号并接收下一字符
			EEOptrStackPop(&OPTR);
			c++;
			break;
		case 1:		//退栈并将运算结果入栈
			theta = EEOptrStackPop(&OPTR);
			b = EEOpndStackPop(&OPND);
			a = EEOpndStackPop(&OPND);
			EEOpndStackPush(&OPND, EEOperate(a, theta, b));
			break;
		default:
			StackDestroy(&OPTR);
			StackDestroy(&OPND);
			return ERROR;
		}
	
	}
	*result = EEOpndStackPop(&OPND);
	StackDestroy(&OPTR);
	StackDestroy(&OPND);
	return OK;
}