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
Status MatrixCreate(MATRIX *M)
{
	M->r = -1;
	M->c = -1;
	return OK;
}

Status MatrixReset(MATRIX *M, size_t m, size_t n)
{
	if (M->r != -1 || M->c != -1)
	{
		ArrayDestroy(&M->data);
	}
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
	if (MatrixReset(T, M->c, M->r) != OK)
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

Status MatrixMult(MATRIX *M, MATRIX *N, MATRIX *Q)
{
	size_t i, j, k;
	ELEMENT e1, e2;
	float v;
	ELEMENT e;

	if (M->c != N->r)
	{
		return ERROR;
	}
	e.data = &v;
	e.size = sizeof(v);
	if (MatrixReset(Q, M->r, N->c) != OK)
	{
		return ERROR;
	}
	for (i = 0; i < M->r; i++)
	{
		for (j = 0; j < N->c; j++)
		{
			v = 0;
			for (k = 0; k < M->c; k++)
			{
				if (ArrayValue(&M->data, &e1, i, k) == OK && ArrayValue(&N->data, &e2, k, j)==OK)
				{
					v += (*((float *)e1.data))*(*((float *)e2.data));
				}
			}
			ArrayAssign(&Q->data, &e, i, j);
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

Status RLSMatrixCreate(RLSMATRIX *M)
{
	if (SqlistInit(&M->data, SMatrixCompare, CommonAllocRotuine, CommonFreeRoutine) != OK)
	{
		return ERROR;
	}
	if (SqlistInit(&M->rpos, CommonIntCompareRoutine, CommonAllocRotuine, CommonFreeRoutine) != OK)
	{
		return ERROR;
	}
	M->mu = -1;
	M->nu = -1;
	return OK;
}

Status RLSMatrixReset(RLSMATRIX *M, size_t m, size_t n)
{
	size_t pos;
	ELEMENT e;
	size_t v;

	RLSMatrixClear(M);
	//初始化rpos表
	e.data = &v;
	e.size = sizeof(v);
	v = -1;
	for (pos = 0; pos < m; pos++)
	{
		if (SqlistOrderInsert(&M->rpos, &e) != OK)
		{
			SqlistClear(&M->data);
			SqlistClear(&M->rpos);
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
	M->mu = M->nu = -1;
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

	if (RLSMatrixReset(T, M->nu, M->mu) != OK)
	{
		return ERROR;
	}
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
	size_t *num;
	size_t *cpot;
	size_t t, col, p, q;
	ELEMENT e;

	if (RLSMatrixReset(T, M->nu, M->mu) != OK)
	{
		return ERROR;
	}
	if (SqlistLength(&M->data))
	{
		num = CMEM_ALOC(sizeof(size_t)*M->nu);
		if (!num)
		{
			return ERROR;
		}
		CMEM_SET(num, 0, sizeof(size_t)*M->nu);
		for (t = 0; t < SqlistLength(&M->data); t++)
		{
			if (SqlistGetElem(&M->data, t, &e) == OK)
			{
				num[((TRIPLE *)e.data)->j]++;		//求M中每一列含非零元个数
			}
		}
		cpot = CMEM_ALOC(sizeof(size_t)*SqlistLength(&M->data));
		if (!cpot)
		{
			CMEM_FREE(num);
			return ERROR;
		}
		//求第col列中第一个非零元在b.data中的序号
		cpot[0] = 0;
		for (col = 1; col < SqlistLength(&M->data); col++)
		{
			cpot[col] = cpot[col - 1] + num[col-1];
		}
		for (p = 0; p < SqlistLength(&M->data); p++)
		{
			if (SqlistGetElem(&M->data, p, &e) == OK)
			{
				col = ((TRIPLE *)e.data)->j;
				q = cpot[col];
				RLSMatrixAssign(T, ((TRIPLE *)e.data)->j, ((TRIPLE *)e.data)->i, ((TRIPLE *)e.data)->value);
				cpot[col]++;
			}
		}
		CMEM_FREE(num);
		CMEM_FREE(cpot);
	}

	return OK;
}

Status RLSMatrixFastTranspose2(RLSMATRIX *M, RLSMATRIX *T)
{
	size_t p;
	ELEMENT e;

	if (RLSMatrixReset(T, M->nu, M->mu) != OK)
	{
		return ERROR;
	}

	//因为RLSMATRIX的data已经实现了排序,且RLSMatrixValue在赋值时会自动更新rpos表
	//所以直接用M->data表交换行列标存入M即可
	for (p = 0; p < SqlistLength(&M->data); p++)
	{
		if (SqlistGetElem(&M->data, p, &e))
		{
			RLSMatrixAssign(T, ((TRIPLE *)e.data)->j, ((TRIPLE *)e.data)->i, ((TRIPLE *)e.data)->value);
		}
	}
	return OK;
}

Status RLSMatrixMult(RLSMATRIX *M, RLSMATRIX *N, RLSMATRIX *Q)
{
	size_t arow;
	ELEMENT e1, e2, e3, e4;
	size_t p, q, n;
	size_t brow;
	size_t t;
	size_t ccol;
	float *ctemp;

	if (M->nu != N->mu)
	{
		return ERROR;
	}
	if (RLSMatrixReset(Q, M->mu, N->nu) != OK)
	{
		return ERROR;
	}
	if (SqlistLength(&M->data) == 0 && SqlistLength(&N->data) == 0)
	{
		return OK;
	}
	ctemp = CMEM_ALOC(sizeof(float)*N->nu);
	if (!ctemp)
	{
		return ERROR;
	}
	for (arow = 0; arow < M->mu; arow++)	//处理M的每一行
	{
		//当前行各元素累加器清零
		CMEM_SET(ctemp, 0, sizeof(float)*N->nu);
		if (SqlistGetElem(&M->rpos, arow, &e1) == OK && *((size_t *)e1.data) != -1)
		{
			p = *((size_t *)e1.data);
			n = M->nu;
			//p为data表中当前行首个非零元位置,n为data表中当前行最后一个非零元位置+1(则当前行非零元数目为n-p)
			//n的求法是:如果rops中当前行的下一行有存储非零值,那么这个位置就是n
			//如果当前行的下一行没有存储非零值位置(*((size_t *)e1.data) == -1),则继续使用rops表
			//扫描下一行,直到遇到有非零元存储位置,这个位置就是n
			//如果rops表已经扫描到结束仍然没有其它非零元位置存储,那么data表中剩下的项全是当前行的非零元
			//此时n=Length(data)-p

			//教材中描述rpos[row+1]-1为row行最后一个非零元的位置,对于教材中的例子矩阵,每行都存在非零元
			//但是考虑当第row+1行没有非零元时,rops[row+1]的值应当等于row行首个非零元位置,此时rops[row+1]
			//有值,又应该认为第row+1行存在非零元,其存储位置rops[row+1]等于row行首个非零元位置,即:
			//如果第row+1行没有非零元,根据教材描述,会得出第row+1行有非零元,并且把对第row+1行非零元的所有访问
			//映射到第row行
			//此描述问题很严重本例不采用,本例中rops[row]==-1表示第row行不存在非零元
			if (SqlistGetElem(&M->rpos, arow + 1, &e1) == OK && *((size_t *)e1.data) != -1)
			{
				n = *((size_t *)e1.data);
			}
			else
			{
				for (t = p; t <SqlistLength(&M->rpos) - p; t++)
				{
					if (SqlistGetElem(&M->data, t, &e2) == OK && ((TRIPLE *)e2.data)->i != arow)
					{
						n = t;
						break;
					}
				}
			}

			for (p; p < n; p++)	//对当前行中每一个非零元
			{
				if (SqlistGetElem(&M->data, p, &e2) == OK)
				{
					brow = ((TRIPLE *)e2.data)->j;	//找到对应元在N中的行号
					if (SqlistGetElem(&N->rpos, brow, &e3) == OK && *((size_t *)e3.data) != -1)	//N中对应行中存在非零元
					{
						q = *((size_t *)e3.data);
						t = N->nu;
						if (SqlistGetElem(&N->rpos, brow + 1, &e3) == OK && *((size_t *)e3.data) != -1)
						{
							t = *((size_t *)e3.data);
						}
						for (q; q < t; q++)
						{
							if (SqlistGetElem(&N->data, q, &e4) == OK)
							{
								ccol = ((TRIPLE *)e4.data)->j;	//乘积元素在Q中的列号
								ctemp[ccol] += ((TRIPLE *)e2.data)->value * ((TRIPLE *)e4.data)->value;
							}
						}
					}
				}
			}
		}
		for (ccol = 0; ccol < Q->nu; ccol++)	//压缩存储该行非零元
		{
			if (ctemp[ccol])
			{
				RLSMatrixAssign(Q, arow, ccol, ctemp[ccol]);
			}
		}
	}
	CMEM_FREE(ctemp);
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

	if (MatrixReset(T, M->mu, M->nu) != OK)
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
	SqlistClear(&M->rpos);
	M->mu = -1;
	M->nu = -1;
}