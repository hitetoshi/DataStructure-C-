#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

/*《数据机构（C语言
*/

	//一些基本的类型

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	BUFFER_TOO_SMALL	-3

	typedef	int	Status;

#ifndef _VCRUNTIME_H
#ifdef _WIN64
	typedef	unsigned __int64 size_t;
#else
	typedef	unsigned int size_t;
#endif // _WIN64

#endif // !_VCRUNTIME_H


	//通用节点元素
	typedef	struct _ELEMENT
	{
		void *data;		//指向元素数据的指针
		size_t size;	//元素数据大小
	}ELEMENT, *PELEMENT;

	//通用链式节点
	/*教材对节点元素的类型定义为ElemType,针对不同的应用类型,ElemType会有差别
	例如线性链表中多项式运算ElemType为

	typedef	struct _POLYN_ELEMENT_DATA
	{
		float coef;		//系数
		int expn;		//指数
	}POLYN_ELEMENT_DATA, *PPOLYN_ELEMENT_DATA;

	栈中进制转换ElemType为int,行编辑ElemType为char,迷宫求解中ElementType为

	typedef	struct _MAZEELEMENT
	{
		int ord;			//通道块在路径上的序号
		COORDINATE seat;	//通道块在迷宫中的坐标位置
		int di;				//从此通道块走向下一通道块的方向
	}MAZEELEMENT;

	队列离散事件模拟中客户端队列的ElementType为

	typedef	struct _BM_CUSTOM
	{
		int ArrivalTime;	//到达时间
		int Duration;		//办理事务所需时间
	}BM_CUSTOM;

	如果针对不同的应用,不同的节点元素数据类型都要去重写数据结构的相关代码
	(内存分配,元素值比较,元素值赋值)那么工作量将相当大,而且代码的可移植性
	也很低。为克服这个困难,本例所有数据结构都采用下面的通用节点类型,其特点
	是,数据结构本身不关心节点元素类型,只做抽象层面的操作,按照ELEMENT.size
	分配内存,元素赋值的操作采用内存拷贝memcpy,元素比较由调用者提供比较函数
	(见PCOMPAREROUTINE类型的注释)。对元素数据类型的解释由调用者自行完成。
	这样,不同的具体应用可以根据自己的实际需要定义自己的元素数据类型并解释其
	含义,数据结构不需要修改任何代码*/
	typedef	struct _LINKNODE
	{
		struct _ELEMENT elem;
		struct _LINKNODE *next;
	}LINKNODE, *PLINKNODE;

	//通用坐标
	typedef	struct _COORDINATE
	{
		int x;	//x坐标
		int y;	//y坐标
	}COORDINATE,*PCOORDINATE;

	//本例中大部分数据结构的初始化操作都需要调用者提供三个回调函数作为参数

	//1.PCOMPAREROUTINE:数据项比较函数,由于数据结构使用的数据类型为抽象类型
	//当数据需要进行比较操作(如定位、排序)时,就需要调用者提供比较函数,如果在
	//每个这样的操作中都由调用者传入比较函数,则比较繁琐。故在数据结构初始化
	//时,由调用者传入,数据结构将这个回调函数存在结构内部,需要使用时直接调用

	//2.PALLOCATEROUTINE/PFREEROUTINE内存分配和释放函数:当调用者于不同的编程
	//环境中调用数据结构时,可能会根据实际情况自定义内存的分配和释放操作(如
	//C++中可使用new/delete分配、释放内存；WIN 32环境下可使用HeapAlloc/HeapFree
	//分配、释放内存；WDK环境下用ExAllocatePoolWithTag/ExFreePoolWithTag分配、
	//释放内存)调用者根据自己实际使用情况在初始化数据结构时传入内存分配、释放
	//的回调函数,当需要时,结构会按照调用者提供的方法进行操作

	//同理,当对数据结构进行移植时,数据结构管理自身内存和其他操作的库函数可能也
	//会有不同,可直接修改下面定义的CMEM_ALOC、CMEM_FREE等宏进行适配

	typedef	int (__cdecl *PCOMPAREROUTINE)(ELEMENT *firstelem, ELEMENT *secondelem);
	typedef	void * (__cdecl *PALLOCATEROUTINE)(size_t bytesize);
	typedef	void(__cdecl *PFREEROUTINE)(void *buffer);

#define	CMEM_ALOC(size)				malloc(size)
#define	CMEM_FREE(block)			free(block)
#define	CMEM_REALLOC(block, size)	realloc(block,size)
#define	CMEM_SET(block, v, size)	memset(block,v,size)
#define	CMEM_COPY(dest,src,size)	memcpy(dest,src,size)
#define	CMEM_MOVE(dest,src,size)	memmove(dest,src,size)

#define	CSTRING_LENGTH(str)	strlen(str)
#define	CSTRING_LOWER(str)	_strlwr_s(str,CSTRING_LENGTH(str)+1)
#define	CSTRING_ATOI(str)	atoi(str)

#define	CISDIGIT(c)		isdigit(c)
#define	CTIME(_Time)	time(_Time)

	//一些公用函数
	void * __cdecl CommonAllocRotuine(size_t bytes);
	void __cdecl CommonFreeRoutine(void *block);
	int RangeRandom(int range_min, int range_max);
	//通用整型数据类型比较函数
	int __cdecl CommonIntCompareRoutine(ELEMENT *first, ELEMENT *second);

#ifdef __cplusplus
}
#endif // __cplusplus
