#pragma once

#include "DataStructureC.h"
#include "List.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//矩阵最大维数的定义似乎没有必要
//#define	MAX_ARRAY_DIM	8

	typedef	struct
	{
		ELEMENT *base;	//数组元素基址,由ArrayInit分配
		int dim;		//数组维数
		int *bounds;	//数组维界基址,由ArrayInit分配
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


	//根据矩阵的定义,矩阵的元只能是数
	//本例只讨论实矩阵,因此,矩阵的数据类型定义为float
	typedef	struct _TRIPLE
	{
		size_t i, j;
		float value;
	}TRIPLE;

	//压缩存储的稀疏矩阵(行逻辑链接顺序表)

	//本例对教材的行逻辑链接稀疏矩阵结构定义进行了较大的修改
	//1.将data由类型为TRIPLE的定长数组(MAXSIZE)修改为数据类型为TRIPLE的顺序表
	//2.将rpos各行第一个非零元位置表由int类型定长数组(MAXRC)修改为数据类型为int的顺序表
	//这样做的目的一是可以灵活使用我们已经实现的数据结构,二是去掉了非零元最大个数和矩阵最大行数的限制
	typedef	struct _RLSMATRIX
	{
		SQLIST data;		//非零元三元组表(ELEMENT数据类型为TRIPLE)
		SQLIST rpos;		//各行第一个非零元的位置表(ELEMENT数据类型为size_t)
		size_t mu, nu, tu;	//矩阵的行数,列数和非零元个数
	}RLSMATRIX;	//行逻辑链接稀疏矩阵结构

	//矩阵元的下标比较
	//比较(t1,j1)与(t2,j2)的下标大小
#define	MatrixIndexCompare(i1,j1,i2,j2)	i1 < i2 ? -1 : (i1 == i2 ? (int)(j1 - j2) : 1)

	//创建m*n稀疏矩阵M
	Status RLSMatrixCreate(RLSMATRIX *M, size_t m, size_t n);
	//销毁稀疏矩阵M
	void RLSMatrixDestroy(RLSMATRIX *M);
	//稀疏矩阵M已经存在,i,j为M中合法的行标和列标
	//将value的值赋到M中下标为(i,j)的元素
	Status RLSMatrixAssign(RLSMATRIX *M, size_t i, size_t j, float value);
	//稀疏矩阵M已经存在,i,j为M中合法的行标和列标
	//将M中下标为(i,j)的元素值赋给value
	Status RLSMatrixValue(RLSMATRIX *M, size_t i, size_t j, float *value);
	//打印稀疏矩阵M
	void RLSMatrixPrint(RLSMATRIX *M);

	void RLSMatrixPrintNoneZero(RLSMATRIX *M);
	//T已经创建,由稀疏矩阵M复制得到T
	Status RLSMatrixCopy(RLSMATRIX *T, RLSMATRIX *M);

#ifdef __cplusplus
}
#endif // __cplusplus