#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	MAX_ARRAY_DIM	8

	typedef	struct
	{
		ELEMENT *base;	//数组元素基址,由ArrayInit分配
		int dim;		//数组维数
		int *bounds;	//数组维界基址,有ArrayInit分配
		int *constants;	//数组映像函数常量基址,由ArrayInit分配
	}ARRAY;

	//若维数和随后的各维长度合法,则构造相应的数组A,并返回OK
	Status ArrayInit(ARRAY *A, int dim,...);
	//销毁数组A
	void ArrayDestroy(ARRAY *A);
	//A是n维数组,e为元素变量,随后是n个下标值
	//若各下标不越界,则e赋值为所指定的A的元素值,并返回OK
	Status ArrayValue(ARRAY *A, ELEMENT *e, ...);
	//A是n维数组,e为元素变量,随后是n个下标值
	//若各下标不越界,则将e赋给所指定的A的元素,并返回OK
	Status ArrayAssign(ARRAY *A, ELEMENT *e, ...);

	//为方便C语言编程的调用接口改进

	//按照dim维度构造数组A,各维长度存放于length中
	Status ArrayInit2(ARRAY *A, int dim, int *length);
	//A是n维数组,e为元素变量,dim为给定维数,index为下标
	//若给定维数与A的维数相同,则将index下标指定的A的元素赋值给e
	Status ArrayValue2(ARRAY *A, ELEMENT *e, int dim, int *index);
	//A是n维数组,e为元素变量,dim为给定维数,index为下标
	//若给定维数与A的维数相同,则将e的值赋给index下标指定的A的元素e
	Status ArrayAssign2(ARRAY *A, ELEMENT *e, int dim, int *index);

#ifdef __cplusplus
}
#endif // __cplusplus