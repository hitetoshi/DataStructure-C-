#pragma once

#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//					《数据结构（C语言版）》所有数据结构和算法的C语言实现					        //
	//																									//
	//DataStructureC静态库基本上实现了《数据结构（C语言版）》的所有数据结构和算法。						//
	//																									//
	//部分数据结构的不同类型（如链栈、双向队列、循环队列等）因为已经实现能反应结构特的主要部分且该类型的//
	//原理在其它结构已经有所展示所以不再用代码重复实现。												//
	//																									//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//从编程的简便和算法为实际应用的原则出发，DataStructureC的代码与教材存在一些差异：					//
	//																									//
	//1.教材认为C语言下标以0开始使用自然语言描述不甚方便，因此教材中所有下标均从1开始（如：各种结构元素 //
	//定位函数返回的元素位置）；但以1作为下标起始在实际C语言编程中更加不方便（如：C语言访问数组均从0下标//
	//开始）。因此，DataStructureC对所有下标的使用均从0开始，这可能导致某些算法的代码与教材的语言描述不 //
	//一致甚至有较大差别（如：求KMP算法的next数组函数）。												//
	//																									//
	//2.教材对数据结构操作的定义使用了大量了C++中的“引用（&）”传递参数，这是C语言不支持的，			//
	//DataStructureC将这些引用传参改为了C语言支持的“按地址传参（*）”。								//
	//																									//
	//3.教材对某些结构和函数的命名不是很统一,命名没有特殊性,容易与C标准库或第三方库形成冲突。			//
	//DataStructureC统一结构和类型定义为全大写（如LINKLIST，STACK，HSTRING）；操作方法（函数）为“结构名//
	//+操作名”（如SqlistInit，StrIndex）；参数全小写（特殊情况除外）。									//
	//																									//
	//4.一些其它的，作者认为在实际编程中不合理或者不方便的地方，如：									//
	//1）大量使用全局变量在函数之间传递数据：离散事件模式。DataStructureC通过增加函数调用参数，以参数传 //
	//递数据。																							//
	//2）在算法内部分配了内存但是不释放的：链表归并。根据“谁分配谁释放”的原则，如果该内存需要交给调用	//
	//者使用，则DataStructureC不在算法中分配该内存，而是要求调用者自行分配后通过参数传递给算法。		//
	//3）函数接口封装不友好的：数组结构中使用可变参数传递各维长度和各维下标，如果调用者需要初始化一个维 //
	//度很大的数组在函数调用时需要传递的参数太多；多项式求值和表达式求值要求用户在算法内部输入数据，算法//
	//基本上无法移植。DataStructureC在不改变原算法的基础上将函数接口封装的更加友好更加有利于移植。		//
	//4）某些描述不详尽或者可能产生Bug的，参见：StrInsert函数注释，LinklistOrderLocalElem定义注释，		//
	//PolynAdd函数注释，RLSMatrixMult函数注释。DataStructureC对其稍作了修改。							//
	//5）算法无法实现具体应用的：关键词索引表。DataStructureC增加了调用者接口，使算法能够被调用者具体应 //
	//用。																								//
	//																									//
	//5.为了增强代码健壮性和可移植性作的修改和补充（参考后续的注释部分）。								//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//																									//
	//另外，DataStructureC对实现的某些算法作了作了一些讨论，如离散事件模拟的作业调度算法，KMP算法的效率 //
	//等，可参见相关算法的注释。																		//
	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//文件组织结构：																					//
	//DataStructureC.h	//公用头文件,包含文件、公用数据结构定义、公用函数声明							//
	//DataStructureC.c	//公用函数的实现																//
	//List.h			//线性顺序表、线性链表、一元多项式的数据结构定义和函数声明						//
	//list.c			//线性顺序表、线性链表、一元多项式的实现										//
	//Stack.h			//顺序栈、进制转换、括号匹配、行编辑、迷宫求解、表达式求值数据结构定义和函数声明//
	//Stack.c			//顺序栈、进制转换、括号匹配、行编辑、迷宫求解、表达式求值的实现				//
	//Queue.h			//顺序队列、离散事件模拟的数据结构定义和函数声明								//
	//Queue.c			//顺序队列、离散事件模拟的实现													//
	//HString.h			//堆分配存储结构串、词索引表的数据结构定义和函数声明							//
	//HString.c			//堆分配存储结构串、词索引表的实现												//
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//一些基本的类型

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	UNDERFLOW	-3
#define	BUFFER_TOO_SMALL	-4

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

	例如线性链表中多项式运算ElemType为:
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

	队列离散事件模拟中客户端队列的ElementType为:
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
	//C++中可使用new/delete分配释放内存；WIN 32环境下可使用HeapAlloc/HeapFree
	//分配/释放内存；WDK环境下用ExAllocatePoolWithTag/ExFreePoolWithTag分配/
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
