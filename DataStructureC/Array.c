#include "Array.h"

Status ArrayInit(ARRAY *A, size_t dim, ...)
{
	size_t i;
	va_list ap;
	size_t elemtotal;

	if (dim < 1)
	{
		return ERROR;
	}
	A->dim = dim;
	A->bounds = CMEM_ALOC(dim * sizeof(size_t));
	if (!A->bounds)
	{
		return ERROR;
	}
	//若各维长度合法,则存入A->bounds,并求出A的元素总数
	elemtotal = 1;

	va_start(ap, dim);	//ap为va_list类型,是存放变长参数表信息的数组
	for (i = 0; i < dim; i++)
	{
		A->bounds[i] = va_arg(ap, size_t);
		/*if (A->bounds[i] < 0)		//A->bonds定义为size_t数组的话,不可能小于0
		{
			CMEM_FREE(A->bounds);
			return UNDERFLOW;
		}*/
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
	A->constants = CMEM_ALOC(dim * sizeof(size_t));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,指针的增减以元素大小为单位(sizeof(ELEMENT))
	for (i = dim - 1; i > 0; i--)
	{
		A->constants[i - 1] = A->bounds[i] * A->constants[i];
	}
	return OK;
}

Status ArrayInit2(ARRAY *A, size_t dim, size_t *length)
{
	size_t i;
	size_t elemtotal;

	if (dim < 1)
	{
		return ERROR;
	}
	A->dim = dim;
	A->bounds = CMEM_ALOC(dim * sizeof(size_t));
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
	A->constants = CMEM_ALOC(dim * sizeof(size_t));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,指针的增减以元素大小为单位(sizeof(ELEMENT))
	for (i = dim - 1; i > 0; i--)
	{
		A->constants[i - 1] = A->bounds[i] * A->constants[i];
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
Status ArrayLocate(ARRAY *A, va_list ap, size_t *off)
{
	size_t i;
	size_t ind;

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
Status ArrayLocate2(ARRAY *A, size_t *index, size_t *off)
{
	size_t i = 0;

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
	size_t off;
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

Status ArrayValue2(ARRAY *A, ELEMENT *e, size_t dim, size_t *index)
{
	Status result;
	size_t off;

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
	size_t off;
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

Status ArrayAssign2(ARRAY *A, ELEMENT *e, size_t dim, size_t *index)
{
	Status result;
	size_t off;

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

//普通矩阵

Status MatrixCreate(MATRIX *M, size_t m, size_t n)
{
	if (ArrayInit(&M->data, 2, m, n) == OK)
	{
		M->r = m;
		M->c = n;
		return OK;
	}
	return ERROR;
}

Status MatrixAssign(MATRIX *M, size_t i, size_t j, float value)
{
	ELEMENT e;

	if (i >= M->r || j >= M->c)
	{
		return ERROR;
	}
	e.data = &value;
	e.size = sizeof(value);
	return ArrayAssign(&M->data, &e, i, j);
}

Status MatrixValue(MATRIX *M, size_t i, size_t j, float *value)
{
	ELEMENT e;

	if (i >= M->r || j >= M->c)
	{
		return ERROR;
	}

	if (ArrayValue(&M->data, &e, i, j) == OK)
	{
		*value = *((float *)e.data);
		return OK;
	}
	return ERROR;
}

Status MatrixTranspose(MATRIX *M, MATRIX *T)
{
	size_t i, j;
	float v;

	//时间复杂度O(M->r*M->c)
	if (T->r != M->c || T->c != M->r)
	{
		return ERROR;
	}

	for (i = 0; i < M->c; i++)
	{
		for (j = 0; j < M->r; j++)
		{
			MatrixValue(M, j, i, &v);
			MatrixAssign(T, i, j, v);
		}
	}
	return OK;
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
	return OK;
}


void RLSMatrixDestroy(RLSMATRIX *M)
{
	SqlistDestroy(&M->data);
	SqlistDestroy(&M->rpos);
	M->mu = M->nu = 0;
}

//当从稀疏矩阵M位置pos摘除下标(i,j)的非零元后调用此函数更新rpos表
void RLSMatrixUpdateRopsDelete(RLSMATRIX *M, size_t pos, size_t i, size_t j)
{
	ELEMENT e;
	size_t p;

	if ((SqlistGetElem(&M->rpos, i, &e) == OK) && (*((size_t *)e.data) == pos))	//摘除的元素是该行首个非零元
	{
		if ((SqlistGetElem(&M->data, pos, &e) == OK))
		{
			if (((TRIPLE *)e.data)->i != i)		//该行已经没有非零元
			{
				p = -1;
				e.data = &p;
				e.size = sizeof(p);
				SqlistSetElem(&M->rpos, i, &e);
			}
		}
		else
		{
			p = -1;
			e.data = &p;
			e.size = sizeof(p);
			SqlistSetElem(&M->rpos, i, &e);
		}
	}

	for (p = i + 1; p < SqlistLength(&M->rpos); p++)	//修正rops表中后继行首个非零元位置
	{
		if (SqlistGetElem(&M->rpos, p, &e) == OK)
		{
			if (*((size_t *)e.data) != -1)
			{
				(*((size_t *)e.data))--;
			}
		}
	}
}

//当从稀疏矩阵M位置pos插入下标(i,j)的非零元后调用此函数更新rpos表
void RLSMatrixUpdateRopsInsert(RLSMATRIX *M, size_t pos, size_t i, size_t j)
{
	ELEMENT e;
	size_t p;

	if (SqlistGetElem(&M->rpos, i, &e) == OK)
	{
		if (*((size_t *)e.data) == -1)	//插入的非零元是该行首个非零元
		{
			*((size_t *)e.data) = pos;
		}
		else
		{
			if (SqlistGetElem(&M->data, *((size_t *)e.data), &e) == OK)
			{
				if (((TRIPLE *)e.data)->j > j)	//插入的元成为该行首个非零元
				{
					*((size_t *)e.data) = pos;
				}
			}
		}
	}

	for (p = i + 1; p < SqlistLength(&M->rpos); p++)	//修正rops表中后继行首个非零元位置
	{
		if (SqlistGetElem(&M->rpos, p, &e) == OK)
		{
			if (*((size_t *)e.data) != -1)
			{
				(*((size_t *)e.data))++;
			}
		}
	}
}

Status RLSMatrixAssign(RLSMATRIX *M, size_t i, size_t j, float value)
{
	TRIPLE trip;
	ELEMENT e;
	size_t pos;
	ELEMENT *b;

	//此操作采用M->data整表扫描的算法,也可以使用M->rops辅助定位,将M->data表的扫描范围缩小到
	//至多一行的范围。但考虑此处定位失败还需要得到元素插入位置,因此仍使用整表扫描较为方便
	//后面的RLSMatrixValue演示了调用RLSMatrixLocatec从而rops辅助扫描的算法
	//此处查找的时间复杂度为O(M->mu*M->nu),若采用折半查找法时间复杂度为O(log(M->mu*M->nu))
	//以rops辅助查找的时间复杂度为O(M->nu),采用折半查找法时间复杂度为O(log(m->nu))
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
					//更新rpos表
					RLSMatrixUpdateRopsInsert(M, pos, i, j);
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
	ELEMENT e;
	size_t pos;

	if (RLSMatrixLocate(M, i, j, &pos) == OK)
	{
		*value = (pos == -1) ? 0 : ((SqlistGetElem(&M->data, pos, &e) == OK) ? ((TRIPLE *)e.data)->value : 0);
		return OK;
	}
	return ERROR;
}

Status RLSMatrixLocate(RLSMATRIX *M, size_t i, size_t j, size_t *pos)
{
	ELEMENT e;
	size_t locate;
	size_t pos_start;
	size_t pos_end;

	if ((i <0 || i >= M->mu) || (j < 0 || j >= M->nu))
	{
		return ERROR;
	}

	*pos = -1;
	if ((SqlistGetElem(&M->rpos, i, &e) == OK) && ((*(size_t *)e.data) != -1))
	{
		pos_start = *(size_t *)e.data;
		pos_end = pos_start + M->nu;
		if ((SqlistGetElem(&M->rpos, i + 1, &e) == OK) && ((*(size_t *)e.data) != -1))
		{
			pos_end = (*(size_t *)e.data);
		}
		for (locate = pos_start; locate < pos_end && *pos == -1; locate++)
		{
			if ((SqlistGetElem(&M->data, locate, &e) == OK) && (MatrixIndexCompare(((TRIPLE *)e.data)->i, ((TRIPLE *)e.data)->j, i, j) == 0))
			{
				*pos = locate;
			}
		}
	}
	return OK;
}

Status RLSMatrixCopy(RLSMATRIX *T, RLSMATRIX *M)
{
	size_t i;
	ELEMENT e;
	ELEMENT new_elem;
	TRIPLE t;

	//清除T中原来的数据
	RLSMatrixClear(T);
	//复制M->data
	new_elem.data = &t;
	new_elem.size = sizeof(t);
	for (i = 0; i < SqlistLength(&M->data);i++)
	{
		if (SqlistGetElem(&M->data, i, &e) != OK)
		{
			RLSMatrixClear(T);
			return ERROR;
		}
		if (SqlistInsert(&T->data, i, &e) != OK)
		{
			RLSMatrixClear(T);
			return ERROR;
		}
	}
	//复制M->rpos
	for (i = 0; i < SqlistLength(&M->rpos); i++)
	{
		if (SqlistGetElem(&M->rpos, i, &e) != OK)
		{
			RLSMatrixClear(T);
			return ERROR;
		}
		if (SqlistInsert(&T->rpos, i, &e) != OK)
		{
			RLSMatrixClear(T);
			return ERROR;
		}
	}
	T->mu = M->mu;
	T->nu = M->nu;
	return OK;
}

Status RLSMatrixTranspose(RLSMATRIX *M, RLSMATRIX *T)
{
	size_t col, p;
	ELEMENT e;

	if (T->mu != M->nu || T->nu != M->mu)
	{
		return ERROR;
	}
	//清除T中原来的数据
	RLSMatrixClear(T);
	if (SqlistLength(&M->data))
	{
		for (col = 0; col < M->nu; col++)
		{
			for (p = 0; p < SqlistLength(&M->data); p++)
			{
				if (SqlistGetElem(&M->data, p, &e) == OK && ((TRIPLE *)e.data)->j == col)
				{
					RLSMatrixAssign(T, ((TRIPLE *)e.data)->j, ((TRIPLE *)e.data)->i, ((TRIPLE *)e.data)->value);
				}
			}
		}
	}
	return OK;
}

Status RLSMatrixFastTranspose(RLSMATRIX *M, RLSMATRIX *T)
{
	if (T->mu != M->nu || T->nu != M->mu)
	{
		return ERROR;
	}
	//清除T中原来的数据
	RLSMatrixClear(T);
	if (SqlistLength(&M->data))
	{

	}

	return OK;
}

void RLSMatrixDumpValue(MATRIX *T, size_t i, size_t c, float value, size_t count)
{
	size_t p;

	for (p = 0; p < count; p++)
	{
		MatrixAssign(T, i, p + c, value);
	}
}

Status RLSMatrixDump(MATRIX *T, RLSMATRIX *M)
{
	size_t pos_r, pos_c, pos;
	size_t last_c;
	ELEMENT e,ev;

	if (T->r != M->mu || T->c != M->nu)
	{
		return ERROR;
	}

	for (pos_r = 0; pos_r < M->mu; pos_r++)
	{
		last_c = 0;

		if (SqlistGetElem(&M->rpos, pos_r, &e) == OK && *((size_t *)e.data) != -1)
		{
			pos = *((size_t *)e.data);
			while(SqlistGetElem(&M->data, pos++, &ev) == OK && ((TRIPLE *)ev.data)->i == pos_r)
			{
				pos_c = ((TRIPLE *)(ev.data))->j;
				RLSMatrixDumpValue(T, pos_r, last_c, 0, pos_c - last_c);
				RLSMatrixDumpValue(T, pos_r, pos_c, ((TRIPLE *)ev.data)->value, 1);
				last_c = pos_c + 1;
			}
			RLSMatrixDumpValue(T, pos_r, last_c, 0, M->nu - last_c);
		}
		else
		{
			RLSMatrixDumpValue(T, pos_r, 0, 0, M->nu);
		}
	}
	return OK;
}

Status __cdecl reset_rpos(ELEMENT *e)
{
	*((size_t *)e->data) = -1;
	return OK;
}

void RLSMatrixClear(RLSMATRIX *M)
{
	SqlistClear(&M->data);
	SqlistTraverse(&M->rpos, reset_rpos);
}