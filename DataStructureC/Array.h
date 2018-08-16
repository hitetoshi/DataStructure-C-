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
		size_t dim;		//数组维数
		size_t *bounds;	//数组维界基址,由ArrayInit分配
		size_t *constants;	//数组映像函数常量基址,由ArrayInit分配
	}ARRAY;

	//若维数和随后的各维长度合法,则构造相应的数组A,并返回OK
	Status ArrayInit(ARRAY *A, size_t dim,...);
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
	Status ArrayInit2(ARRAY *A, size_t dim, size_t *length);
	//A是n维数组,e为元素变量,dim为给定维数,index为下标
	//若给定维数与A的维数相同,则将index下标指定的A的元素赋值给e
	Status ArrayValue2(ARRAY *A, ELEMENT *e, size_t dim, size_t *index);
	//A是n维数组,e为元素变量,dim为给定维数,index为下标
	//若给定维数与A的维数相同,则将e的值赋给index下标指定的A的元素e
	Status ArrayAssign2(ARRAY *A, ELEMENT *e, size_t dim, size_t *index);

	//由数组实现的普通矩阵

	//根据矩阵的定义,矩阵的元只能是数
	//本例只讨论实矩阵,因此,矩阵的数据类型定义为float
	typedef	struct _MATRIX
	{
		ARRAY data;
		size_t r;	//矩阵行数
		size_t c;	//矩阵列数
	}MATRIX,*PMATRIX;

	//创建m行n列的矩阵
	Status MatrixCreate(MATRIX *M, size_t m,size_t n);
#define	MatrixDestroy(M)	ArrayDestroy(&((MATRIX *)M)->data)
#define	MatrixRows(M)		((MATRIX *)M)->r
#define	MatrixCols(M)		((MATRIX *)M)->c
	//若下标(i,j)合法,将value赋值给矩阵M中下标(i,j)的元
	Status MatrixAssign(MATRIX *M, size_t i, size_t j, float value);
	//若下标(i,j)合法,将矩阵M中下标(i,j)元的值赋给value
	Status MatrixValue(MATRIX *M, size_t i, size_t j, float *value);
	//矩阵M,T已初始化,矩阵T的列数等于M的行数,T的行数等于M的列数
	//将M的转置矩阵存入T
	Status MatrixTranspose(MATRIX *M, MATRIX *T);

	//压缩存储的稀疏矩阵(行逻辑链接顺序表)
	typedef	struct _TRIPLE
	{
		size_t i, j;	//元下标
		float value;	//值
	}TRIPLE;

	//本例对教材的行逻辑链接稀疏矩阵结构定义进行了较大的修改
	//1.将data由类型为TRIPLE的定长数组(MAXSIZE)修改为数据类型为TRIPLE的顺序表
	//2.将rpos各行第一个非零元位置表由int类型定长数组(MAXRC)修改为数据类型为int的顺序表
	//这样做的目的一是可以灵活使用我们已经实现的数据结构,二是去掉了非零元最大个数和矩阵最大行数的限制
	typedef	struct _RLSMATRIX
	{
		SQLIST data;		//非零元三元组表(ELEMENT数据类型为TRIPLE)
		SQLIST rpos;		//各行第一个非零元的位置表(ELEMENT数据类型为size_t)
		size_t mu, nu;		//矩阵的行数,列数
		//size_t tu;		//矩阵的非零元个数,由于表data自带SQlistLength方法,这个辅助数据没有意义
	}RLSMATRIX;	//行逻辑链接稀疏矩阵结构

	//矩阵元的下标比较
	//比较(t1,j1)与(t2,j2)的下标大小
#define	MatrixIndexCompare(i1,j1,i2,j2)	i1 < i2 ? -1 : (i1 == i2 ? (int)(j1 - j2) : 1)

	//创建m*n稀疏矩阵M
	Status RLSMatrixCreate(RLSMATRIX *M, size_t m, size_t n);
	//销毁稀疏矩阵M
	void RLSMatrixDestroy(RLSMATRIX *M);
#define	RLSMatrixRows(M)		((RLSMATRIX *)M)->mu
#define	RLSMatrixCols(M)		((RLSMATRIX *)M)->nu
	//稀疏矩阵M已经存在,i,j为M中合法的行标和列标
	//将value的值赋到M中下标为(i,j)的元素
	Status RLSMatrixAssign(RLSMATRIX *M, size_t i, size_t j, float value);
	//稀疏矩阵M已经存在,i,j为M中合法的行标和列标
	//将M中下标为(i,j)的元素值赋给value
	Status RLSMatrixValue(RLSMATRIX *M, size_t i, size_t j, float *value);
	//稀疏矩阵M已经存在,按下标(i,j)定位M中的元,将位置存入pos,若下标非法返回ERROR
	//若定位失败pos赋值-1
	Status RLSMatrixLocate(RLSMATRIX *M, size_t i, size_t j, size_t *pos);
	//教材定义了一个打印稀疏矩阵的操作,但是在Windows下如果不使用Windows API很难打印
	//出漂亮的矩阵,因此本例不实现稀疏矩阵的打印操作,由使用者自行打印
	//T已经创建,由稀疏矩阵M复制得到T
	Status RLSMatrixCopy(RLSMATRIX *T, RLSMATRIX *M);
	//算法5.1稀疏矩阵M,T已建立,T的行数等于M的列数,T的列数等于M的行数
	//将M的转置矩阵存入T
	Status RLSMatrixTranspose(RLSMATRIX *M, RLSMATRIX *T);
	//算法5.2稀疏矩阵M,T已建立,T的行数等于M的列数,T的列数等于M的行数
	//将M的转置矩阵存入T
	Status RLSMatrixFastTranspose(RLSMATRIX *M, RLSMATRIX *T);

	//矩阵T已经初始化,行数和列数与稀疏矩阵M相同,将稀疏矩阵M转储到矩阵T
	Status RLSMatrixDump(MATRIX *T, RLSMATRIX *M);
	//稀疏矩阵M已初始化,清除M中存储的信息
	void RLSMatrixClear(RLSMATRIX *M);

#ifdef __cplusplus
}
#endif // __cplusplus