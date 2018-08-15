#pragma once

#include "DataStructureC.h"
#include "List.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//�������ά���Ķ����ƺ�û�б�Ҫ
//#define	MAX_ARRAY_DIM	8

	typedef	struct
	{
		ELEMENT *base;	//����Ԫ�ػ�ַ,��ArrayInit����
		int dim;		//����ά��
		int *bounds;	//����ά���ַ,��ArrayInit����
		int *constants;	//����ӳ����������ַ,��ArrayInit����
	}ARRAY;

	//��ά�������ĸ�ά���ȺϷ�,������Ӧ������A,������OK
	Status ArrayInit(ARRAY *A, int dim,...);
	//��������A
	void ArrayDestroy(ARRAY *A);
	//A��nά����,eΪԪ�ر���,�����n���±�ֵ
	//�����±겻Խ��,��e��ֵΪ��ָ����A��Ԫ��ֵ,������OK
	Status ArrayValue(ARRAY *A, ELEMENT *e, ...);
	//A��nά����,eΪԪ�ر���,�����n���±�ֵ
	//�����±겻Խ��,��e������ָ����A��Ԫ��,������OK
	Status ArrayAssign(ARRAY *A, ELEMENT *e, ...);

	//Ϊ����C���Ա�̵ĵ��ýӿڸĽ�

	//����dimά�ȹ�������A,��ά���ȴ����length��
	Status ArrayInit2(ARRAY *A, int dim, int *length);
	//A��nά����,eΪԪ�ر���,dimΪ����ά��,indexΪ�±�
	//������ά����A��ά����ͬ,��index�±�ָ����A��Ԫ�ظ�ֵ��e
	Status ArrayValue2(ARRAY *A, ELEMENT *e, int dim, int *index);
	//A��nά����,eΪԪ�ر���,dimΪ����ά��,indexΪ�±�
	//������ά����A��ά����ͬ,��e��ֵ����index�±�ָ����A��Ԫ��e
	Status ArrayAssign2(ARRAY *A, ELEMENT *e, int dim, int *index);


	//���ݾ���Ķ���,�����Ԫֻ������
	//����ֻ����ʵ����,���,������������Ͷ���Ϊfloat
	typedef	struct _TRIPLE
	{
		size_t i, j;
		float value;
	}TRIPLE;

	//ѹ���洢��ϡ�����(���߼�����˳���)

	//�����Խ̲ĵ����߼�����ϡ�����ṹ��������˽ϴ���޸�
	//1.��data������ΪTRIPLE�Ķ�������(MAXSIZE)�޸�Ϊ��������ΪTRIPLE��˳���
	//2.��rpos���е�һ������Ԫλ�ñ���int���Ͷ�������(MAXRC)�޸�Ϊ��������Ϊint��˳���
	//��������Ŀ��һ�ǿ������ʹ�������Ѿ�ʵ�ֵ����ݽṹ,����ȥ���˷���Ԫ�������;����������������
	typedef	struct _RLSMATRIX
	{
		SQLIST data;		//����Ԫ��Ԫ���(ELEMENT��������ΪTRIPLE)
		SQLIST rpos;		//���е�һ������Ԫ��λ�ñ�(ELEMENT��������Ϊsize_t)
		size_t mu, nu, tu;	//���������,�����ͷ���Ԫ����
	}RLSMATRIX;	//���߼�����ϡ�����ṹ

	//����Ԫ���±�Ƚ�
	//�Ƚ�(t1,j1)��(t2,j2)���±��С
#define	MatrixIndexCompare(i1,j1,i2,j2)	i1 < i2 ? -1 : (i1 == i2 ? (int)(j1 - j2) : 1)

	//����m*nϡ�����M
	Status RLSMatrixCreate(RLSMATRIX *M, size_t m, size_t n);
	//����ϡ�����M
	void RLSMatrixDestroy(RLSMATRIX *M);
	//ϡ�����M�Ѿ�����,i,jΪM�кϷ����б���б�
	//��value��ֵ����M���±�Ϊ(i,j)��Ԫ��
	Status RLSMatrixAssign(RLSMATRIX *M, size_t i, size_t j, float value);
	//ϡ�����M�Ѿ�����,i,jΪM�кϷ����б���б�
	//��M���±�Ϊ(i,j)��Ԫ��ֵ����value
	Status RLSMatrixValue(RLSMATRIX *M, size_t i, size_t j, float *value);
	//��ӡϡ�����M
	void RLSMatrixPrint(RLSMATRIX *M);

	void RLSMatrixPrintNoneZero(RLSMATRIX *M);
	//T�Ѿ�����,��ϡ�����M���Ƶõ�T
	Status RLSMatrixCopy(RLSMATRIX *T, RLSMATRIX *M);

#ifdef __cplusplus
}
#endif // __cplusplus