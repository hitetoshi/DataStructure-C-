#pragma once

#include "DataStructureC.h"
#include "List.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

	typedef	struct _LINKQUEUE
	{
		PLINKNODE front;
		PLINKNODE rear;
		PALLOCATEROUTINE allocateroutine;
		PFREEROUTINE freeroutine;
		size_t length;
	}LINKQUEUE;

	Status LinkQueueInit(LINKQUEUE *q, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	Status LinkQueueDestroy(LINKQUEUE *q);
	Status LinkQueueClear(LINKQUEUE *q);
	Status LinkQueueEmpty(LINKQUEUE *q);
	size_t LinkQueueLength(LINKQUEUE *q);
	Status LinkQueueGetHead(LINKQUEUE *q, ELEMENT *e);
	Status LinkQueueEn(LINKQUEUE *q, ELEMENT *e);
	Status LinkQueueDe(LINKQUEUE *q, ELEMENT *e);
	Status LinkQueueTraverse(LINKQUEUE *q, Status(_cdecl *visit)(ELEMENT *elem, void *param), void *param);

	//离散事件模拟

#define	MAX_INTERTIME	5	//两个相邻到达银行的客户的时间间隔不超过5分钟
#define	MAX_DURTIME		30	//为每个客户办理业务不超过30分钟
#define	QUEUE_COUNT		4	//4个窗口

	typedef	struct _BM_EVENT
	{
		int OccurTime;	//事件发生时刻
		int NType;		//事件类型,0表示达到事件,1-4表示4个窗口的离开事件
	}BM_EVENT;

	typedef	LINKLIST	EVENTLIST;

	typedef	struct _BM_CUSTOMER
	{
		int ArrivalTime;	//到达时间
		int Duration;		//办理事务所需时间
	}BM_CUSTOMER;

	typedef	struct _BANK_SIMULATION_DATA	//模拟函数中用到的全部数据
	{
		EVENTLIST ev;	//事件表
		LINKQUEUE q[QUEUE_COUNT];	//4个客户队列
		int TotalTime, CustomerNum;	//累计客户逗留时间,客户数
		int CloseTime;
	}BANK_SIMULATION_DATA;

	//算法3.7离散事件模拟
	//本例对教材作了一些修改,过多使用全局变量有诸多缺点,例如:
	//1.降低代码可移植性
	//2.易造成命名冲突
	//3.不支持多线程
	//4.全局变量不易管理,其他代码中对其意外赋值的情况很难排查
	//5.全局变量生命周期太长,以本例为例,Bank_Simulation函数执行完毕就应该释放
	//它执行过程中使用到的所有资源,不应该给系统造成额外开销
	//因此本例将教材中所用到的全部全局变量定义到结构体struct _BANK_SIMULATION_DATA中
	//在主函数Bank_Simulation中定义为局部变量,以参数方式传递给其它函数以供使用
	//增加pEventNotify回调函数参数,当新客户达到时或客户离开事件产生时,Bank_Simulation将调用pEventNotify通知主调函数
	//方便主调函数输出相关信息
	//增加TotalTime和CustomerNum,将模拟完成时候总耗时和总客户数通过参数返回给主调函数,方便主调函数处理相关信息
	Status Bank_Simulation(int CloseTime, void(__cdecl *pEventNotify)(BM_EVENT *en), int *TotalTime, int *CustomerNum);

	//关于离散事件模拟算法的讨论
	//1.先来先服务算法(FCFS)和短作业优先算法(SJF)
	//算法3.7中设置两个数据结构：有序链表来储存事件,队列来存储客户
	//在同一个客户队列中,依队列的先进先出性质,不可能出现同一队列中等待时间较短的客人比等待时间更长的客人的业务优先处理
	//这是典型的先来先服务算法
	//在事件链表中,事件发生时刻最短的事件总是被置于链表头优先处理,这是典型的短作业优先算法

	//2.对客户队列的改进
	//1)先来先服务算法那对长作业有力，短作业不利,根据相邻客户到达的间隔时间和每个客户的平均业务时间,可以确定FCFS和SJF的
	//最佳选择，在本例中,可以通过修改MAX_DURTIME(处理每位客户业务的最大时间),MAX_INTERTIME(相邻两个客户达到银行的最大时间间隔)
	//和QUEUE_COUNT(银行柜台数)来观察平均每用户在银行的停留时间的差别
	//2)即使使用FCFS算法,由于算法3.7设置了4个队列,任然可能出现在A队列中先达到银行的客户比B队列中后达到银行的客户的业务后处理
	//比较自然的想法是只使用一个用户队列,这样即可保证所有用户都是先来先服务。
	//但是算法3.7采用同步处理方式,即:每次只从事件列表中取出一个事件(可以交给不同的柜台)处理,待此事件处理完毕后再取下一个
	//事件,在此算法上,如果只设置一个客户队列相当于只有一个柜台可以处理业务,解决这个问题就要使用异步处理方式(多线程)
	//有关队列的异步操作,是比较典型的生产者消费者问题,需要借助操作系统的临界区(CriticalSection)、信号量(Semaphore)等同步对象
	//来保证多线程安全,在此不作多的讨论.

#ifdef __cplusplus
}
#endif // __cplusplus