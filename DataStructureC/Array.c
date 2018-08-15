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

//稀疏矩阵
int __cdecl SMatrixCompare(ELEMENT *first, ELEMENT *second)
{
	//稀疏矩阵的元按下标定位,因此比较函数比较给定下标
	return MatrixIndexCompare(((TRIPLE *)(first->data))->i, ((TRIPLE *)(first->data))->j, 
		((TRIPLE *)(second->data))->i, ((TRIPLE *)(second->data))->j);
}

Status RLSMatrixCreate(RLSMATRIX *M, size_t m, size_t n)
{
	size_t pos;
	ELEMENT e;
	size_t v;

	if (SqlistInit(&M->data, SMatrixCompare, CommonAllocRotuine, CommonFreeRoutine) != OK)
	{
		return ERROR;
	}
	if (SqlistInit(&M->rpos, CommonIntCompareRoutine, CommonAllocRotuine, CommonFreeRoutine) != OK)
	{
		return ERROR;
	}
	//初始化rpos表
	e.data = &v;
	e.size = sizeof(v);
	v = -1;
	for (pos = 0; pos < m; pos++)
	{
		if (SqlistOrderInsert(&M->rpos, &e) != OK)
		{
			SqlistDestroy(&M->data);
			SqlistDestroy(&M->rpos);
			return ERROR;
		}
	}
	M->mu = m;
	M->nu = n;
	M->tu = 0;
	return OK;
}


void RLSMatrixDestroy(RLSMATRIX *M)
{
	SqlistDestroy(&M->data);
	SqlistDestroy(&M->rpos);
	M->mu = M->nu = M->tu = 0;
}

//当从稀疏矩阵M位置pos摘除下标(i,j)的非零元后调用此函数更新rpos表
void RLSMatrixUpdateRopsDelete(RLSMATRIX *M, size_t pos, size_t i, size_t j)
{
	ELEMENT e;
	size_t p;

	if (SqlistGetElem(&M->rpos, i, &e) != OK)
	{
		return;
	}

	for (p = pos + 1; p < SqlistLength(&M->rpos); p++)
	{
		if (SqlistGetElem(&M->rpos, p, &e) == OK)
		{
			*((size_t *)e.data)--;
		}
	}
}

Status RLSMatrixAssign(RLSMATRIX *M, size_t i, size_t j, float value)
{
	TRIPLE trip;
	ELEMENT e;
	size_t pos;
	ELEMENT *b;

	if ((i >= 0 && i < M->mu) && (j>=0 && j<M->nu))
	{
		trip.i = i;
		trip.j = j;
		trip.value = value;
		e.data = &trip;
		e.size = sizeof(trip);
		b = NULL;
		pos = SqlistOrderLocate(&M->data, &e, &b);
		if (b)	//指定下标在非零元表存在
		{
			if (value == 0)
			{
				//给定值为0,从非零元表中摘除此元
				if (SqlistDelete(&M->data, pos, &e) == OK)
				{
					M->tu--;
					//更新rpos表
					RLSMatrixUpdateRopsDelete(M, pos, i, j);
					return OK;
				}
			}
			else
			{
				//更新此非零元值
				return SqlistSetElem(&M->data, pos, &e);
			}
		}
		else
		{
			//若value非零创建新的非零元项
			if (value != 0)
			{
				if (SqlistInsert(&M->data, pos, &e) == OK)
				{
					M->tu++;
					//更新rpos表
					return OK;
				}
			}
			else
			{
				return OK;
			}
		}
	}
	return ERROR;
}

Status RLSMatrixValue(RLSMATRIX *M, size_t i, size_t j, float *value)
{
	TRIPLE trip;
	ELEMENT e;
	size_t pos;
	ELEMENT *b;

	if ((i >= 0 && i < M->mu) && (j >= 0 && j < M->nu))
	{
		trip.i = i;
		trip.j = j;
		e.data = &trip;
		e.size = sizeof(trip);
		b = NULL;
		pos = SqlistOrderLocate(&M->data, &e, &b);
		if (b)
		{
			*value = ((TRIPLE *)b->data)->value;
			return OK;
		}
		else
		{
			*value = 0;
			return OK;
		}
	}
	return ERROR;
}

void MatrixPrintZero(size_t colnum)
{
	size_t i;

	for (i = 0; i < colnum; i++)
	{
		printf("0  ");
	}
}

void RLSMatrixPrint(RLSMATRIX *M)
{
	size_t p;
	ELEMENT e;
	size_t pos;
	TRIPLE trip;
	size_t last_j;

	for (p = 0; p < M->mu; p++)
	{
		last_j = 0;

		if (SqlistGetElem(&M->rpos, p, &e) == OK)
		{
			if (*((size_t *)e.data) == -1)
			{
				MatrixPrintZero(M->nu);
			}
			else
			{
				if (SqlistGetElem(&M->data, *((size_t *)e.data), &e))
				{
					do
					{
						MatrixPrintZero(((TRIPLE *)e.data)->j - last_j);
						last_j = ((TRIPLE *)e.data)->j;
						printf("%d ", (int)(((TRIPLE *)e.data)->value));
						SqlistNextElem(&M->data, &e, &e);
					} while (((TRIPLE *)e.data)->i==p);
					MatrixPrintZero(M->nu - last_j);
				}
			}
		}
		printf("\n");
	}
}

Status __cdecl print_nonezero(ELEMENT *e)
{
	printf("%d(%zd,%zd)\n",(int)((TRIPLE *)(e->data))->value, ((TRIPLE *)(e->data))->i, ((TRIPLE *)(e->data))->j);
	return OK;
}

Status __cdecl print_rpos(ELEMENT *e)
{
	printf("%zd ", *((size_t *)e->data));
	return OK;
}

void RLSMatrixPrintNoneZero(RLSMATRIX *M)
{
	SqlistTraverse(&M->data, print_nonezero);
	SqlistTraverse(&M->rpos, print_rpos);
}

Status RLSMatrixCopy(RLSMATRIX *T, RLSMATRIX *M)
{
	size_t i;
	ELEMENT e;
	ELEMENT new_elem;
	TRIPLE t;

	//清除T中原来的数据
	SqlistClear(&T->data);
	SqlistClear(&T->rpos);
	//复制M->data
	new_elem.data = &t;
	new_elem.size = sizeof(t);
	for (i = 0; i < SqlistLength(&M->data);i++)
	{
		if (SqlistGetElem(&M->data, i, &e) != OK)
		{
			SqlistClear(&T->data);
			return ERROR;
		}
		if (SqlistInsert(&T->data, i, &e) != OK)
		{
			SqlistClear(&T->data);
			return ERROR;
		}
	}
	//复制M->rpos
	for (i = 0; i < SqlistLength(&M->rpos); i++)
	{
		if (SqlistGetElem(&M->rpos, i, &e) != OK)
		{
			SqlistClear(&T->data);
			SqlistClear(&T->rpos);
			return ERROR;
		}
		if (SqlistInsert(&T->rpos, i, &e) != OK)
		{
			SqlistClear(&T->data);
			SqlistClear(&T->rpos);
			return ERROR;
		}
	}
	T->mu = M->mu;
	T->nu = M->nu;
	T->tu = M->tu;
	return OK;
}