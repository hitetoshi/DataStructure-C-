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