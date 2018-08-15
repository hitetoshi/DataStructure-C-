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
	//����ά���ȺϷ�,�����A->bounds,�����A��Ԫ������
	elemtotal = 1;

	va_start(ap, dim);	//apΪva_list����,�Ǵ�ű䳤��������Ϣ������
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
	//��ӳ����Ci������A->constants[i-1],i=1,2...,dim
	A->constants = CMEM_ALOC(dim * sizeof(int));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,ָ���������Ԫ�ش�СΪ��λ(sizeof(ELEMENT))
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
	//����ά���ȺϷ�,�����A->bounds,�����A��Ԫ������
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
	//��ӳ����Ci������A->constants[i-1],i=1,2...,dim
	A->constants = CMEM_ALOC(dim * sizeof(int));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,ָ���������Ԫ�ش�СΪ��λ(sizeof(ELEMENT))
	for (i = dim - 2; i >= 0; i--)
	{
		A->constants[i] = A->bounds[i + 1] * A->constants[i + 1];
	}
	return OK;
}

void ArrayDestroy(ARRAY *A)
{
	//�̲�����������ʱ�ϸ����˸�ָ���Ƿ�ΪNULL�Ա������δ��ʼ�������������ٲ�������
	//�̲������ദ�����˼����ì�ܣ���ArrayInit��,�̲����нṹ�ڳ�ʼ��ʱ������û�м��
	//�����ṹ�Ƿ��Ѿ���ʼ����ֱ�ӷ����ڴ�,������Ϊ�̲��ڳ�ʼ�����нṹʱ����Ϊ�����ṹ
	//��û�г�ʼ���ġ�
	//������һ˼��,�����������������ָ���Ƿ���Ч,ʼ����Ϊ��Ҫ���ٵ������Ѿ���ʼ����
	//��:free����������ָ��ΪNULLʱ�����в���,��������ͷſ�ָ����쳣��
	CMEM_FREE(A->base);
	A->base = NULL;
	CMEM_FREE(A->bounds);
	A->bounds = NULL;
	CMEM_FREE(A->constants);
	A->constants = NULL;
}

//��apָʾ�ĸ��±�ֵ�Ϸ�,�������Ԫ����A�е���Ե�ַoff
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

//��indexָʾ���±�ֵ�Ϸ�,�������Ԫ����A�е���Ե�ַoff
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

//ϡ�����
int __cdecl SMatrixCompare(ELEMENT *first, ELEMENT *second)
{
	//ϡ������Ԫ���±궨λ,��˱ȽϺ����Ƚϸ����±�
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
	//��ʼ��rpos��
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

//����ϡ�����Mλ��posժ���±�(i,j)�ķ���Ԫ����ô˺�������rpos��
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
		if (b)	//ָ���±��ڷ���Ԫ�����
		{
			if (value == 0)
			{
				//����ֵΪ0,�ӷ���Ԫ����ժ����Ԫ
				if (SqlistDelete(&M->data, pos, &e) == OK)
				{
					M->tu--;
					//����rpos��
					RLSMatrixUpdateRopsDelete(M, pos, i, j);
					return OK;
				}
			}
			else
			{
				//���´˷���Ԫֵ
				return SqlistSetElem(&M->data, pos, &e);
			}
		}
		else
		{
			//��value���㴴���µķ���Ԫ��
			if (value != 0)
			{
				if (SqlistInsert(&M->data, pos, &e) == OK)
				{
					M->tu++;
					//����rpos��
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

	//���T��ԭ��������
	SqlistClear(&T->data);
	SqlistClear(&T->rpos);
	//����M->data
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
	//����M->rpos
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