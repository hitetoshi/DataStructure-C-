#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	MAX_ARRAY_DIM	8

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

#ifdef __cplusplus
}
#endif // __cplusplus