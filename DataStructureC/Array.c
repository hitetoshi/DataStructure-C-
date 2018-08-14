#include "Array.h"

Status ArrayInit(ARRAY *A, int dim, ...)
{
	int i;
	va_list ap;
	size_t elemtotal;

	if (dim < 1)
	{
		return ERROR;
	}
	A->dim = dim;
	A->bounds = CMEM_ALOC(dim * sizeof(int));
	if (!A->bounds)
	{
		return ERROR;
	}
	//若各维长度合法,则存入A->bounds,并求出A的元素总数
	elemtotal = 1;

	va_start(ap, dim);	//ap为va_list类型,是存放变长参数表信息的数组
	for (i = 0; i < dim; i++)
	{
		A->bounds[i] = va_arg(ap, int);
		if (A->bounds[i] < 0)
		{
			CMEM_FREE(A->bounds);
			return UNDERFLOW;
		}
		elemtotal *= A->bounds[i];
	}
	va_end(ap);
	A->base = CMEM_ALOC(sizeof(ELEMENT)*elemtotal);
	if (!A->base)
	{
		CMEM_FREE(A->bounds);
		return OVERFLOW;
	}
	//求映像函数Ci并存入A->constants[i-1],i=1,2...,dim
	A->constants = CMEM_ALOC(dim * sizeof(int));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,指针的增减以元素大小为单位(sizeof(ELEMENT))
	for (i = dim - 2; i >= 0; i--)
	{
		A->constants[i] = A->bounds[i + 1] * A->constants[i + 1];
	}
	return OK;
}

Status ArrayInit2(ARRAY *A, int dim, int *length)
{
	int i;
	size_t elemtotal;

	if (dim < 1)
	{
		return ERROR;
	}
	A->dim = dim;
	A->bounds = CMEM_ALOC(dim * sizeof(int));
	if (!A->bounds)
	{
		return ERROR;
	}
	//若各维长度合法,则存入A->bounds,并求出A的元素总数
	elemtotal = 1;

	for (i = 0; i < dim; i++)
	{
		A->bounds[i] = length[i];
		if (A->bounds[i] < 0)
		{
			CMEM_FREE(A->bounds);
			return UNDERFLOW;
		}
		elemtotal *= A->bounds[i];
	}
	A->base = CMEM_ALOC(sizeof(ELEMENT)*elemtotal);
	if (!A->base)
	{
		CMEM_FREE(A->bounds);
		return OVERFLOW;
	}
	//求映像函数Ci并存入A->constants[i-1],i=1,2...,dim
	A->constants = CMEM_ALOC(dim * sizeof(int));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,指针的增减以元素大小为单位(sizeof(ELEMENT))
	for (i = dim - 2; i >= 0; i--)
	{
		A->constants[i] = A->bounds[i + 1] * A->constants[i + 1];
	}
	return OK;
}

void ArrayDestroy(ARRAY *A)
{
	//教材在销毁数组时严格检查了各指针是否为NULL以避免对尚未初始化的数组作销毁操作这与
	//教材其它多处代码的思想相矛盾（如ArrayInit）,教材所有结构在初始化时基本上没有检查
	//给定结构是否已经初始化而直接分配内存,可以认为教材在初始化所有结构时都认为给定结构
	//尚没有初始化的。
	//根据这一思想,本例不检查给定数组的指针是否有效,始终认为将要销毁的数组已经初始化。
	//另:free函数当给定指针为NULL时不进行操作,不会出现释放空指针的异常。
	CMEM_FREE(A->base);
	A->base = NULL;
	CMEM_FREE(A->bounds);
	A->bounds = NULL;
	CMEM_FREE(A->constants);
	A->constants = NULL;
}

//若ap指示的各下标值合法,则求出该元素在A中的相对地址off
Status ArrayLocate(ARRAY *A, va_list ap, int *off)
{
	int i;
	int ind;

	*off = 0;
	for (i = 0; i < A->dim; i++)
	{
		ind = va_arg(ap, int);
		if (ind < 0 || ind >= A->bounds[i])
		{
			return OVERFLOW;
		}
		(*off) += A->constants[i] * ind;
	}
	return OK;
}

//若index指示的下标值合法,则求出该元素在A中的相对地址off
Status ArrayLocate2(ARRAY *A, int *index, int *off)
{
	int i = 0;

	*off = 0;
	for (i = 0; i < A->dim; i++)
	{
		if (index[i] < 0 || index[i] >= A->bounds[i])
		{
			return OVERFLOW;
		}
		(*off) += A->constants[i] * index[i];
	}
	return OK;
}

Status ArrayValue(ARRAY *A, ELEMENT *e, ...)
{
	Status result;
	int off;
	va_list ap;

	va_start(ap, e);
	if ((result = ArrayLocate(A, ap, &off)) != OK)
	{
		return result;
	}
	va_end(ap);
	e->data = (A->base + off)->data;
	e->size = (A->base + off)->size;
	return OK;
}

Status ArrayValue2(ARRAY *A, ELEMENT *e, int dim, int *index)
{
	Status result;
	int off;

	if (dim == A->dim)
	{
		if ((result = ArrayLocate2(A, index, &off)) != OK)
		{
			return result;
		}
		e->data = (A->base + off)->data;
		e->size = (A->base + off)->size;
		return OK;
	}
	else
	{
		return ERROR;
	}
}

Status ArrayAssign(ARRAY *A, ELEMENT *e, ...)
{
	Status result;
	int off;
	va_list ap;

	va_start(ap, e);
	if ((result = ArrayLocate(A, ap, &off)) != OK)
	{
		return result;
	}
	va_end(ap);
	(A->base + off)->data = CMEM_ALOC(e->size);
	if ((A->base + off)->data)
	{
		CMEM_COPY((A->base + off)->data, e->data, e->size);
		(A->base + off)->size = e->size;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}

Status ArrayAssign2(ARRAY *A, ELEMENT *e, int dim, int *index)
{
	Status result;
	int off;

	if (dim != A->dim)
	{
		return ERROR;
	}
	if ((result = ArrayLocate2(A, index, &off)) != OK)
	{
		return result;
	}
	(A->base + off)->data = CMEM_ALOC(e->size);
	if ((A->base + off)->data)
	{
		CMEM_COPY((A->base + off)->data, e->data, e->size);
		(A->base + off)->size = e->size;
		return OK;
	}
	else
	{
		return OVERFLOW;
	}
}