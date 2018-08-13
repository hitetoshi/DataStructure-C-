#pragma once

#include "DataStructureC.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define	LIST_INIT_SIZE	100
#define	LIST_INCREMENT	10

	//线性顺序表
	typedef	struct _SQLIST
	{
		ELEMENT *elem;					//存储空间基址
		size_t length;						//当前长度
		size_t listsize;					//当前分配的存储容量
		PCOMPAREROUTINE compareroutine;		//比较函数
		PALLOCATEROUTINE allocateroutine;	//内存分配函数
		PFREEROUTINE freeroutine;			//内存释放函数
	}SQLIST, *PSQLIST;

	//算法2.3
	Status SqlistInit(SQLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	//算法2.4
	Status SqlistInsert(SQLIST *list, size_t pos, ELEMENT *elem);
	//有序插入
	Status SqlistOrderInsert(SQLIST *list, ELEMENT *elem);
	void SqlistDestroy(SQLIST *list);
	void SqlistClear(SQLIST *list);
	Status SqlistEmpty(SQLIST *list);
	size_t SqlistLength(SQLIST *list);
	Status SqlistGetElem(SQLIST *list, size_t pos, ELEMENT *elem);
	//算法2.6
	size_t SqlistLocate(SQLIST *list, ELEMENT *elem);
	//为实现有序顺序表增加的函数返回大于elem的第一个元素的位置
	//若有相等的元素存在,则b中返回该元素
	size_t SqlistOrderLocate(SQLIST *list, ELEMENT *elem, ELEMENT **b);
	Status SqlistPriorElem(SQLIST *list, ELEMENT *elem, ELEMENT *pre_elem);
	Status SqlistNextElem(SQLIST *list, ELEMENT *elem, ELEMENT *pre_elem);
	//算法2.5
	Status SqlistDelete(SQLIST *list, size_t pos, ELEMENT *elem);
	Status SqlistTraverse(SQLIST *list, Status(_cdecl *visit)(ELEMENT *elem));
	//算法2.1
	void SqlistUnio(SQLIST *list1, SQLIST *list2);
	//算法2.7
	Status SqlistMerge(SQLIST *la, SQLIST *lb, SQLIST *lc);

	//线性链表
	typedef	PLINKNODE	LINKLISTPOSITION;

	typedef struct _LINKLIST
	{
		LINKNODE *header, *tail;		//分别指向头尾节点
		size_t len;							//元素个数
		PCOMPAREROUTINE compareroutine;		//比较函数
		PALLOCATEROUTINE allocateroutine;	//内存分配函数
		PFREEROUTINE freeroutine;			//内存释放函数
	}LINKLIST,*PLINKLIST;

	Status LinklistMakeNode(PALLOCATEROUTINE allocateroutine, PLINKNODE *p, ELEMENT *elem);
	Status LinklistFreeNode(PFREEROUTINE freeroutine, PLINKNODE *p);
	Status LinklistInit(LINKLIST *list, PCOMPAREROUTINE compareroutine, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void LinklistClear(LINKLIST *list);
	void LinklistDestroy(LINKLIST *list);

	//链表节点操作的一组函数
	//根据上下文和教材算法2.20理解,以下函数似乎只对链表节点做插入删除操作,并不
	//实际分配和释放节点内存
	//LinklistInsFirst,LinklistDelFirst这两个函数设计很不好,参数只有头节点,没有
	//链表结构,在这两个函数内部不能对链表的长度和可能改变的尾指针进行修正,因此调用
	//者必须自行修改链表长度(list->len),检查操作后链表长度是否为0,若为0,应修正链表
	//尾指针
	Status LinklistInsFirst(LINKNODE *h, PLINKNODE s);
	Status LinklistDelFirst(LINKNODE *h, PLINKNODE *q);
	Status LinklistAppend(LINKLIST *list, PLINKNODE s);
	Status LinklistRemove(LINKLIST *list, PLINKNODE *q);
	Status LinklistInsBefor(LINKLIST *list, PLINKNODE *p, PLINKNODE s);
	Status LinklistInsAfter(LINKLIST *list, PLINKNODE *p, PLINKNODE s);
	Status LinklistDel(LINKLIST *list, PLINKNODE p, PLINKNODE *q);

	//与Sqlist类似的一组操作,对节点元素实际分配和释放节点内存,再调用上面一组函数做节点的插入删除操作
	//算法2.9
	Status LinklistInsert(LINKLIST *list, size_t pos, ELEMENT *elem);
	//算法2.10
	Status LinklistDelete(LINKLIST *list, size_t pos, ELEMENT *elem);
	Status LinklistDeletePos(LINKLIST *list, LINKLISTPOSITION pos);

	Status LinklistSetCurElem(PLINKNODE *p, ELEMENT *elem);
	//算法2.8类似
	ELEMENT *LinklistGetCurElem(PLINKNODE p);
	Status LinklistEmpty(LINKLIST *list);
	size_t LinklistLength(LINKLIST *list);
	LINKLISTPOSITION LinklistHeader(LINKLIST *list);
	LINKLISTPOSITION LinklistGetLast(LINKLIST *list);
	LINKLISTPOSITION LinklistPriorPos(LINKLIST *list, PLINKNODE p);
	LINKLISTPOSITION LinklistNextPos(LINKLIST *list, PLINKNODE p);
	Status LinklistLocatePos(LINKLIST *list, size_t i, PLINKNODE *p);
	LINKLISTPOSITION LinklistLocateElem(LINKLIST *list, ELEMENT *elem);
	Status LinklistTraverse(LINKLIST *list, Status(_cdecl *visit)(ELEMENT *elem));
	void LinklistUnio(LINKLIST *list1, LINKLIST *list2);
	//算法2.21
	//本例对教材算法2.21作了一些修改
	//首先若在LinklistMerge函数内部初始化lc(分配内存),则需要主调函数调用LinklistDestroy(lc)将其释放
	//这不符合"谁分配谁释放"原则,因此本例要求传入的lc是已经调用LinklistInit的链表
	//同时也不释放la和lb的头结点,由调用者自行销毁
	void LinklistMerge(LINKLIST *la, LINKLIST *lb, LINKLIST *lc);

	//一元多项式
	typedef	LINKLIST	POLYNOMAIL;
	typedef	PLINKLIST	PPOLYNOMAIL;

	/*一元多项式的元素数据类型
	在通用节点类型中
	ELEMENT.data设置为指向POLYN_ELEMENT_DATA结构的指针
	ELEMENT.size=sizeof(POLYN_ELEMENT_DATA)即可*/
	typedef	struct _POLYN_ELEMENT_DATA
	{
		float coef;		//系数
		int expn;		//指数
	}POLYN_ELEMENT_DATA,*PPOLYN_ELEMENT_DATA;

	//一元多项式链表元素值比较函数
	int __cdecl polyn_compare_routine(ELEMENT *firstelem, ELEMENT *secondelem);

	//本例对教材作了修改,没有采用一项一项手工输入的方式初始化多项式而采用PolynAddMonos添加项
	//调用者可以让用户手工输入各项次数和系数后调用PolynAddMonos产生多项式,也可以随机生成或通过其他方式
	//产生多项式的项
	//算法2.22
	Status PolynCreate(POLYNOMAIL *polynmail, PALLOCATEROUTINE allocateroutine, PFREEROUTINE freeroutine);
	void PolynDestroy(POLYNOMAIL *polynmail);
	//已知一元多项式polynmail存在,items中存放item_count个单项式,将多项式polynmail依次与items中的单项式相加,
	//结果存入polynmail
	Status PolynAddMonos(POLYNOMAIL *polynmail, POLYN_ELEMENT_DATA *items, int item_count);
	//算法2.23根据"谁分配谁释放"的原则,本例PolynAdd函数不销毁pb,主调函数负责销毁
	void PolynAdd(POLYNOMAIL *pa, POLYNOMAIL *pb);
	void PolynSubtract(POLYNOMAIL *pa, POLYNOMAIL *pb);
	//一元多项式p存在,将p乘以常数constant
	void PolynMultiConstant(POLYNOMAIL *p, float constant);
	//一元多项式p存在,将p乘以单项式mono
	void PolynMultiMono(POLYNOMAIL *p, POLYN_ELEMENT_DATA *mono);
	//一元多项式pa和pb存在,将pa和pb相乘,结果存放于pc
	void PolynMulti(POLYNOMAIL *pa, POLYNOMAIL *pb, POLYNOMAIL *pc);
	void PolynPrint(POLYNOMAIL *polynmail);

	//教材特别说明的有序链表与基本链表不同职能的LocalElem和增加的OrderInsert
	Status LinklistOrderLocalElem(LINKLIST *list, ELEMENT *elem, LINKLISTPOSITION *q);
	//教材中对本操作的描述为满足compare()==0时q指示第一个值为elem的节点位置并返回TRUE;
	//否则q指示第一个与elem满足判定函数compare()>0的元素的前驱的位置并返回FALSE
	//但根据教材算法2.22的描述,考虑计划插入的元素大于有序链表中所有元素,应补充若上述条件都不满足时,q指示尾节点并返回FALSE
	Status LinklistOrderInsert(LINKLIST *list, ELEMENT *elem);


#ifdef __cplusplus
}
#endif // __cplusplus
