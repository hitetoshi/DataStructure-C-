#pragma once

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	STACK_INIT_SIZE	100
#define	STACKINCREMENT	10

	typedef	struct _SQSTACK
	{
		ELEMENTNODE *base;
		ELEMENTNODE *top;
		int stacksize;
		PALLOCATEROUTINE allocateroutine;
		PFREEROUTINE freeroutine;
	}SQSTACK;

	Status StackInit(SQSTACK *stack, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status StackDestroy(SQSTACK *stack);
	Status StackClear(SQSTACK *stack);
	Status StackEmpty(SQSTACK *stack);
	size_t StackLength(SQSTACK *stack);
	Status StackGetTop(SQSTACK *stack, ELEMENTNODE *e);
	Status StackPush(SQSTACK *stack, ELEMENTNODE *e);
	Status StackPop(SQSTACK *stack, ELEMENTNODE *e);
	Status StackTraverse(SQSTACK *stack, Status(*visit)(ELEMENTNODE *e));

#ifdef __cplusplus
}
#endif // __cplusplus