#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	TRUE	1
#define	FALSE	0
#define	OK		1
#define	ERROR	0
#define	INFEASIBLE	-1
#define	OVERFLOW	-2
#define	BUFFER_TOO_SMALL	-3

	//一些基本的类型

	typedef	int	Status;

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

	typedef	int (__cdecl *PCOMPAREROUTINE)(ELEMENT *firstelem, ELEMENT *secondelem);
	typedef	void * (__cdecl *PALLOCATEROUTINE)(size_t bytesize);
	typedef	void(__cdecl *PFREEROUTINE)(void *buffer);

#define	CMEM_ALOC(size)				malloc(size)
#define	CMEM_FREE(block)			free(block)
#define	CMEM_REALOC(block, size)	realloc(block,size)

	//一些公用函数
	void * __cdecl CommonAllocRotuine(size_t bytes);
	void __cdecl CommonFreeRoutine(void *block);
	int RangeRandom(int range_min, int range_max);

#ifdef __cplusplus
}
#endif // __cplusplus
