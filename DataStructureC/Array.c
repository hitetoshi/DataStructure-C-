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
	//����ά���ȺϷ�,�����A->bounds,�����A��Ԫ������
	elemtotal = 1;

	va_start(ap, dim);	//apΪva_list����,�Ǵ�ű䳤��������Ϣ������
	for (i = 0; i < dim; i++)
	{
		A->bounds[i] = va_arg(ap, size_t);
		/*if (A->bounds[i] < 0)		//A->bonds����Ϊsize_t����Ļ�,������С��0
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
	//��ӳ����Ci������A->constants[i-1],i=1,2...,dim
	A->constants = CMEM_ALOC(dim * sizeof(size_t));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,ָ���������Ԫ�ش�СΪ��λ(sizeof(ELEMENT))
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
	A->constants = CMEM_ALOC(dim * sizeof(size_t));
	if (!A->constants)
	{
		CMEM_FREE(A->bounds);
		CMEM_FREE(A->base);
		return OVERFLOW;
	}
	A->constants[dim - 1] = 1;	//L=1,ָ���������Ԫ�ش�СΪ��λ(sizeof(ELEMENT))
	for (i = dim - 1; i > 0; i--)
	{
		A->constants[i - 1] = A->bounds[i] * A->constants[i];
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

//��indexָʾ���±�ֵ�Ϸ�,�������Ԫ����A�е���Ե�ַoff
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

//��ͨ����
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

	//ʱ�临�Ӷ�O(M->r*M->c)
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

//ϡ�����
int __cdecl SMatrixCompare(ELEMENT *first, ELEMENT *second)
{
	//ϡ������Ԫ���±궨λ,��˱ȽϺ����Ƚϸ����±�
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
	//��ʼ��rpos��
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

//����ϡ�����Mλ��posժ���±�(i,j)�ķ���Ԫ����ô˺�������rpos��
void RLSMatrixUpdateRopsDelete(RLSMATRIX *M, size_t pos, size_t i, size_t j)
{
	ELEMENT e;
	size_t p;

	if ((SqlistGetElem(&M->rpos, i, &e) == OK) && (*((size_t *)e.data) == pos))	//ժ����Ԫ���Ǹ����׸�����Ԫ
	{
		if ((SqlistGetElem(&M->data, pos, &e) == OK))
		{
			if (((TRIPLE *)e.data)->i != i)		//�����Ѿ�û�з���Ԫ
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

	for (p = i + 1; p < SqlistLength(&M->rpos); p++)	//����rops���к�����׸�����Ԫλ��
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

//����ϡ�����Mλ��pos�����±�(i,j)�ķ���Ԫ����ô˺�������rpos��
void RLSMatrixUpdateRopsInsert(RLSMATRIX *M, size_t pos, size_t i, size_t j)
{
	ELEMENT e;
	size_t p;

	if (SqlistGetElem(&M->rpos, i, &e) == OK)
	{
		if (*((size_t *)e.data) == -1)	//����ķ���Ԫ�Ǹ����׸�����Ԫ
		{
			*((size_t *)e.data) = pos;
		}
		else
		{
			if (SqlistGetElem(&M->data, *((size_t *)e.data), &e) == OK)
			{
				if (((TRIPLE *)e.data)->j > j)	//�����Ԫ��Ϊ�����׸�����Ԫ
				{
					*((size_t *)e.data) = pos;
				}
			}
		}
	}

	for (p = i + 1; p < SqlistLength(&M->rpos); p++)	//����rops���к�����׸�����Ԫλ��
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

	//�˲�������M->data����ɨ����㷨,Ҳ����ʹ��M->rops������λ,��M->data���ɨ�跶Χ��С��
	//����һ�еķ�Χ�������Ǵ˴���λʧ�ܻ���Ҫ�õ�Ԫ�ز���λ��,�����ʹ������ɨ���Ϊ����
	//�����RLSMatrixValue��ʾ�˵���RLSMatrixLocatec�Ӷ�rops����ɨ����㷨
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
					//����rpos��
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

	//���T��ԭ��������
	RLSMatrixClear(T);
	//����M->data
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
	//����M->rpos
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
				num[((TRIPLE *)e.data)->j]++;		//��M��ÿһ�к�����Ԫ����
			}
		}
		cpot = CMEM_ALOC(sizeof(size_t)*SqlistLength(&M->data));
		if (!cpot)
		{
			CMEM_FREE(num);
			return ERROR;
		}
		//���col���е�һ������Ԫ��b.data�е����
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

	//��ΪRLSMATRIX��data�Ѿ�ʵ��������,��RLSMatrixValue�ڸ�ֵʱ���Զ�����rpos��
	//����ֱ����M->data�������б����M����
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
	for (arow = 0; arow < M->mu; arow++)	//����M��ÿһ��
	{
		//��ǰ�и�Ԫ���ۼ�������
		CMEM_SET(ctemp, 0, sizeof(float)*N->nu);
		if (SqlistGetElem(&M->rpos, arow, &e1) == OK && *((size_t *)e1.data) != -1)
		{
			p = *((size_t *)e1.data);
			n = M->nu;
			//pΪdata���е�ǰ���׸�����Ԫλ��,nΪdata���е�ǰ�����һ������Ԫλ��+1(��ǰ�з���Ԫ��ĿΪn-p)
			//n������:���rops�е�ǰ�е���һ���д洢����ֵ,��ô���λ�þ���n
			//�����ǰ�е���һ��û�д洢����ֵλ��(*((size_t *)e1.data) == -1),�����ʹ��rops��
			//ɨ����һ��,ֱ�������з���Ԫ�洢λ��,���λ�þ���n
			//���rops���Ѿ�ɨ�赽������Ȼû����������Ԫλ�ô洢,��ôdata����ʣ�µ���ȫ�ǵ�ǰ�еķ���Ԫ
			//��ʱn=Length(data)-p

			//�̲�������rpos[row+1]-1Ϊrow�����һ������Ԫ��λ��,���ڽ̲��е����Ӿ���,ÿ�ж����ڷ���Ԫ
			//���ǿ��ǵ���row+1��û�з���Ԫʱ,rops[row+1]��ֵӦ������row���׸�����Ԫλ��,��ʱrops[row+1]
			//��ֵ,��Ӧ����Ϊ��row+1�д��ڷ���Ԫ,��洢λ��rops[row+1]����row���׸�����Ԫλ��,��:
			//�����row+1��û�з���Ԫ,���ݽ̲�����,��ó���row+1���з���Ԫ,���ҰѶԵ�row+1�з���Ԫ�����з���
			//ӳ�䵽��row��
			//��������������ر���������,������rops[row]==-1��ʾ��row�в����ڷ���Ԫ
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

			for (p; p < n; p++)	//�Ե�ǰ����ÿһ������Ԫ
			{
				if (SqlistGetElem(&M->data, p, &e2) == OK)
				{
					brow = ((TRIPLE *)e2.data)->j;	//�ҵ���ӦԪ��N�е��к�
					if (SqlistGetElem(&N->rpos, brow, &e3) == OK && *((size_t *)e3.data) != -1)	//N�ж�Ӧ���д��ڷ���Ԫ
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
								ccol = ((TRIPLE *)e4.data)->j;	//�˻�Ԫ����Q�е��к�
								ctemp[ccol] += ((TRIPLE *)e2.data)->value * ((TRIPLE *)e4.data)->value;
							}
						}
					}
				}
			}
		}
		for (ccol = 0; ccol < Q->nu; ccol++)	//ѹ���洢���з���Ԫ
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