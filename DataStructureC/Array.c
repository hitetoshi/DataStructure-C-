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

	//ʱ�临�Ӷ�O(M->r*M->c)
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
	return OK;
}


void RLSMatrixDestroy(RLSMATRIX *M)
{
	SqlistDestroy(&M->data);
	SqlistDestroy(&M->rpos);
	M->mu = M->nu = 0;
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
	//�˴����ҵ�ʱ�临�Ӷ�ΪO(M->mu*M->nu),�������۰���ҷ�ʱ�临�Ӷ�ΪO(log(M->mu*M->nu))
	//��rops�������ҵ�ʱ�临�Ӷ�ΪO(M->nu),�����۰���ҷ�ʱ�临�Ӷ�ΪO(log(m->nu))
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

	if (T->mu != M->nu || T->nu != M->mu)
	{
		return ERROR;
	}
	//���T��ԭ��������
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
	//���T��ԭ��������
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