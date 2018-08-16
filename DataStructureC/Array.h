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
		size_t dim;		//����ά��
		size_t *bounds;	//����ά���ַ,��ArrayInit����
		size_t *constants;	//����ӳ����������ַ,��ArrayInit����
	}ARRAY;

	//��ά�������ĸ�ά���ȺϷ�,������Ӧ������A,������OK
	Status ArrayInit(ARRAY *A, size_t dim,...);
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
	Status ArrayInit2(ARRAY *A, size_t dim, size_t *length);
	//A��nά����,eΪԪ�ر���,dimΪ����ά��,indexΪ�±�
	//������ά����A��ά����ͬ,��index�±�ָ����A��Ԫ�ظ�ֵ��e
	Status ArrayValue2(ARRAY *A, ELEMENT *e, size_t dim, size_t *index);
	//A��nά����,eΪԪ�ر���,dimΪ����ά��,indexΪ�±�
	//������ά����A��ά����ͬ,��e��ֵ����index�±�ָ����A��Ԫ��e
	Status ArrayAssign2(ARRAY *A, ELEMENT *e, size_t dim, size_t *index);

	//������ʵ�ֵ���ͨ����

	//���ݾ���Ķ���,�����Ԫֻ������
	//����ֻ����ʵ����,���,������������Ͷ���Ϊfloat
	typedef	struct _MATRIX
	{
		ARRAY data;
		size_t r;	//��������
		size_t c;	//��������
	}MATRIX,*PMATRIX;

	//����m��n�еľ���
	Status MatrixCreate(MATRIX *M, size_t m,size_t n);
#define	MatrixDestroy(M)	ArrayDestroy(&((MATRIX *)M)->data)
#define	MatrixRows(M)		((MATRIX *)M)->r
#define	MatrixCols(M)		((MATRIX *)M)->c
	//���±�(i,j)�Ϸ�,��value��ֵ������M���±�(i,j)��Ԫ
	Status MatrixAssign(MATRIX *M, size_t i, size_t j, float value);
	//���±�(i,j)�Ϸ�,������M���±�(i,j)Ԫ��ֵ����value
	Status MatrixValue(MATRIX *M, size_t i, size_t j, float *value);
	//����M,T�ѳ�ʼ��,����T����������M������,T����������M������
	//��M��ת�þ������T
	Status MatrixTranspose(MATRIX *M, MATRIX *T);

	//ѹ���洢��ϡ�����(���߼�����˳���)
	typedef	struct _TRIPLE
	{
		size_t i, j;	//Ԫ�±�
		float value;	//ֵ
	}TRIPLE;

	//�����Խ̲ĵ����߼�����ϡ�����ṹ��������˽ϴ���޸�
	//1.��data������ΪTRIPLE�Ķ�������(MAXSIZE)�޸�Ϊ��������ΪTRIPLE��˳���
	//2.��rpos���е�һ������Ԫλ�ñ���int���Ͷ�������(MAXRC)�޸�Ϊ��������Ϊint��˳���
	//��������Ŀ��һ�ǿ������ʹ�������Ѿ�ʵ�ֵ����ݽṹ,����ȥ���˷���Ԫ�������;����������������
	typedef	struct _RLSMATRIX
	{
		SQLIST data;		//����Ԫ��Ԫ���(ELEMENT��������ΪTRIPLE)
		SQLIST rpos;		//���е�һ������Ԫ��λ�ñ�(ELEMENT��������Ϊsize_t)
		size_t mu, nu;		//���������,����
		//size_t tu;		//����ķ���Ԫ����,���ڱ�data�Դ�SQlistLength����,�����������û������
	}RLSMATRIX;	//���߼�����ϡ�����ṹ

	//����Ԫ���±�Ƚ�
	//�Ƚ�(t1,j1)��(t2,j2)���±��С
#define	MatrixIndexCompare(i1,j1,i2,j2)	i1 < i2 ? -1 : (i1 == i2 ? (int)(j1 - j2) : 1)

	//����m*nϡ�����M
	Status RLSMatrixCreate(RLSMATRIX *M, size_t m, size_t n);
	//����ϡ�����M
	void RLSMatrixDestroy(RLSMATRIX *M);
#define	RLSMatrixRows(M)		((RLSMATRIX *)M)->mu
#define	RLSMatrixCols(M)		((RLSMATRIX *)M)->nu
	//ϡ�����M�Ѿ�����,i,jΪM�кϷ����б���б�
	//��value��ֵ����M���±�Ϊ(i,j)��Ԫ��
	Status RLSMatrixAssign(RLSMATRIX *M, size_t i, size_t j, float value);
	//ϡ�����M�Ѿ�����,i,jΪM�кϷ����б���б�
	//��M���±�Ϊ(i,j)��Ԫ��ֵ����value
	Status RLSMatrixValue(RLSMATRIX *M, size_t i, size_t j, float *value);
	//ϡ�����M�Ѿ�����,���±�(i,j)��λM�е�Ԫ,��λ�ô���pos,���±�Ƿ�����ERROR
	//����λʧ��pos��ֵ-1
	Status RLSMatrixLocate(RLSMATRIX *M, size_t i, size_t j, size_t *pos);
	//�̲Ķ�����һ����ӡϡ�����Ĳ���,������Windows�������ʹ��Windows API���Ѵ�ӡ
	//��Ư���ľ���,��˱�����ʵ��ϡ�����Ĵ�ӡ����,��ʹ�������д�ӡ
	//T�Ѿ�����,��ϡ�����M���Ƶõ�T
	Status RLSMatrixCopy(RLSMATRIX *T, RLSMATRIX *M);
	//�㷨5.1ϡ�����M,T�ѽ���,T����������M������,T����������M������
	//��M��ת�þ������T
	Status RLSMatrixTranspose(RLSMATRIX *M, RLSMATRIX *T);
	//�㷨5.2ϡ�����M,T�ѽ���,T����������M������,T����������M������
	//��M��ת�þ������T
	Status RLSMatrixFastTranspose(RLSMATRIX *M, RLSMATRIX *T);

	//����T�Ѿ���ʼ��,������������ϡ�����M��ͬ,��ϡ�����Mת��������T
	Status RLSMatrixDump(MATRIX *T, RLSMATRIX *M);
	//ϡ�����M�ѳ�ʼ��,���M�д洢����Ϣ
	void RLSMatrixClear(RLSMATRIX *M);

#ifdef __cplusplus
}
#endif // __cplusplus