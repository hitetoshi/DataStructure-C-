#include "DataStructureC.h"

Status StackInit(SQSTACK *stack, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine)
{
	stack->base = MEM_ALOC(STACK_INIT_SIZE*sizeof(ELEMENTNODE));
	if (stack->base)
	{
		memset(stack->base, 0, STACK_INIT_SIZE*sizeof(ELEMENTNODE));
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
	MEM_FREE(stack->base);
	memset(stack, 0, sizeof(SQSTACK));
	return OK;
}

Status StackClear(SQSTACK *stack)
{
	while (stack->top != stack->base)
	{
		stack->top--;
		MEM_FREE(stack->top->data);
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
	return (stack->top - stack->base) / sizeof(ELEMENTNODE);
}

Status StackGetTop(SQSTACK *stack, ELEMENTNODE *e)
{
	if (stack->top == stack->base)
	{
		return ERROR;
	}

	if (e->data && e->size >= (stack->top-1)->size)
	{
		memcpy(e->data, (stack->top-1)->data, (stack->top-1)->size);
		e->size = (stack->top-1)->size;
		return OK;
	}
	else
	{
		return BUFFER_TOO_SMALL;
	}
}

Status StackPush(SQSTACK *stack, ELEMENTNODE *e)
{
	ELEMENTNODE *new_base;

	if (stack->top - stack->base >= stack->stacksize)
	{
		new_base = MEM_REALOC(stack->base, (stack->stacksize + STACKINCREMENT)*sizeof(ELEMENTNODE));
		if (!new_base)
		{
			return OVERFLOW;
		}
		else
		{
			memset(new_base + stack->stacksize*sizeof(ELEMENTNODE), 0, STACKINCREMENT*sizeof(ELEMENTNODE));
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

Status StackPop(SQSTACK *stack, ELEMENTNODE *e)
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
		MEM_FREE(stack->top->data);
		stack->top->data = NULL;
		stack->top->size = 0;
		return OK;
	}
	else
	{
		return BUFFER_TOO_SMALL;
	}
}

Status StackTraverse(SQSTACK *stack, Status(*visit)(ELEMENTNODE *e))
{
	ELEMENTNODE *node = stack->top;

	while (node != stack->base)
	{
		node--;
		if (!visit(node))
		{
			return ERROR;
		}
	}
	return OK;
}